
//
// Created by n0th9ng on 16-07-2025.
//

#include "aiff_wav_comp.h"

// Add this helper function near the top of your .cpp file
void hexDump(const char* desc, const void* addr, int len) {
    printf("%s:\n", desc);
    const unsigned char* pc = (const unsigned char*)addr;
    for (int i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0) printf("\n");
            printf("  %04x ", i);
        }
        printf(" %02x", pc[i]);
    }
    printf("\n");
}

uint16_t read_be16(const uint8_t* data) {
    return (static_cast<uint16_t>(data[0]) << 8 |
           static_cast<uint16_t>(data[1]));
}

uint32_t read_be32(const uint8_t* data) {
    return (static_cast<uint32_t>(data[0]) << 24) |
           (static_cast<uint32_t>(data[1]) << 16) |
           (static_cast<uint32_t>(data[2]) << 8) |
           static_cast<uint32_t>(data[3]);
}

namespace fs = std::filesystem;

// WAV header parsing
std::optional<audio_headers::WavHeader> parse_wav_header(std::ifstream& file) {
    audio_headers::WavHeader header;
    if (file.read(reinterpret_cast<char*>(&header), sizeof(header))) {
        if (header.riff == audio_headers::RIFF_MARKER &&
            header.wave == audio_headers::WAVE_MARKER &&
            header.fmt  == audio_headers::FMT_MARKER &&
            header.audio_format == 1) {
            return header;
            }
    }
    return std::nullopt;
}

// AIFF header parsing
std::optional<audio_headers::AiffHeader> parse_aiff_header(std::ifstream& file) {
    file.seekg(0);  // Always start from beginning

    // Read just the FORM header first
    struct {
        std::array<char,4> form;
        uint32_t file_size;
        std::array<char,4> aiff;
    } form_header;

    if (!file.read(reinterpret_cast<char*>(&form_header), sizeof(form_header))) {
        std::cerr << "Failed to read FORM header\n";
        return std::nullopt;
    }

    // Debug dump
    hexDump("FORM header", &form_header, sizeof(form_header));

    // Verify FORM header
    if (form_header.form != audio_headers::FORM_MARKER ||
        form_header.aiff != audio_headers::AIFF_MARKER) {
        std::cerr << "Invalid FORM/AIFF markers\n";
        return std::nullopt;
    }

    // Replace the current COMM chunk struct with:
    struct {
        std::array<char,4> comm;
        uint32_t comm_size_be;  // Big-endian storage
        uint16_t num_channels_be;
        uint16_t sample_size_be;  // Now properly aligned
        uint32_t num_sample_frames_be;
        std::array<unsigned char,10> sample_rate;
    } comm_chunk;

    if (!file.read(reinterpret_cast<char*>(&comm_chunk), sizeof(comm_chunk))) {
        std::cerr << "Failed to read COMM chunk\n";
        return std::nullopt;
    }

    hexDump("COMM chunk", &comm_chunk, sizeof(comm_chunk));

    // Verify COMM chunk
    if (comm_chunk.comm != audio_headers::COMM_MARKER) {
        std::cerr << "Invalid COMM marker\n";
        return std::nullopt;
    }

    // Convert all big-endian values
    audio_headers::AiffHeader header;
    header.form = form_header.form;
    header.file_size = read_be32(reinterpret_cast<const uint8_t*>(&form_header.file_size));
    header.aiff = form_header.aiff;
    header.comm = comm_chunk.comm;
    header.comm_size = read_be32(reinterpret_cast<const uint8_t*>(&comm_chunk.comm_size_be));
    header.num_channels = read_be16(reinterpret_cast<const uint8_t*>(&comm_chunk.num_channels_be));
    header.num_sample_frames = read_be32(reinterpret_cast<const uint8_t*>(&comm_chunk.num_sample_frames_be));
    header.sample_size = read_be16(reinterpret_cast<const uint8_t*>(&comm_chunk.sample_size_be));
    header.sample_rate = comm_chunk.sample_rate;

    // Skip to SSND chunk (COMM chunk size is usually 18 bytes)
    file.seekg(12 + 8 + header.comm_size, std::ios::beg);

    // Read SSND chunk header
    struct {
        std::array<char,4> ssnd;
        uint32_t ssnd_size;
        uint32_t offset;
        uint32_t block_size;
    } ssnd_chunk;

    if (!file.read(reinterpret_cast<char*>(&ssnd_chunk), sizeof(ssnd_chunk))) {
        std::cerr << "Failed to read SSND chunk\n";
        return std::nullopt;
    }

    hexDump("SSND chunk", &ssnd_chunk, sizeof(ssnd_chunk));

    if (ssnd_chunk.ssnd != std::array{'S','S','N','D'}) {
        std::cerr << "Invalid SSND marker\n";
        return std::nullopt;
    }

    header.ssnd = ssnd_chunk.ssnd;
    header.ssnd_size = read_be32(reinterpret_cast<const uint8_t*>(&ssnd_chunk.ssnd_size));
    header.offset = read_be32(reinterpret_cast<const uint8_t*>(&ssnd_chunk.offset));
    header.block_size = read_be32(reinterpret_cast<const uint8_t*>(&ssnd_chunk.block_size));

    return header;
}

// Replace your current sample rate extraction with:
uint32_t extract_aiff_sample_rate(const std::array<unsigned char,10>& rate) {
    // AIFF sample rate is 80-bit extended float (IEEE 754)
    // First 2 bytes: sign + exponent
    // Remaining 8 bytes: fraction

    // For standard rates (44.1kHz, 48kHz etc.), we can simplify:
    uint32_t rate_value = (rate[2] << 24) | (rate[3] << 16) | (rate[4] << 8) | rate[5];

    // Common sample rates in AIFF files
    switch(rate_value) {
        case 0x40AC44C0: return 44100;  // 44.1kHz
        case 0x40AF89C0: return 48000;  // 48kHz
        case 0x40B38800: return 88200;  // 88.2kHz
        case 0x40B77000: return 96000;  // 96kHz
        default: return rate_value;     // Fallback
    }
}


AudioCompError compressToMp3(const std::string &input_path, const std::string &output_path,
                             int bitrate, int quality, void (*progress_bar)(int)) {

    //Reading the input file
    std::ifstream input_file(input_path, std::ios::binary );
        if (!input_file.is_open()) {
            return AudioCompError::FILE_OPEN_F;
    }


    //detecting the file type
    std::array<char, 4> file_type;
    if (!input_file.read(file_type.data(), file_type.size())) {
        return AudioCompError::INVALID_DATA;
    }
    input_file.seekg(0);


    //parsing the header
    std::uint32_t sample_rate_arb = 44100;      //these are arbitrary constants that will be overwritten
    std::uint16_t num_channels_arb = 2;
    std::uint16_t bits_per_sample_arb = 16;
    std::size_t data_offset_arb = 0;
    std::size_t data_size_arb = 0;


    if (file_type == audio_headers::RIFF_MARKER) {
        if (auto header = parse_wav_header(input_file)) {
            sample_rate_arb = header->sample_rate;
            num_channels_arb = header->num_channels;
            bits_per_sample_arb = header->bits_per_sample;
            data_offset_arb = sizeof(audio_headers::WavHeader);
            data_size_arb = header->data_size;
        }else {
            return  AudioCompError::INVALID_DATA;
        }
    }

    else if (file_type == audio_headers::FORM_MARKER) {
        if (auto header = parse_aiff_header(input_file)) {
            // Extract sample rate (simplified: assumes IEEE 80-bit float exponent is standard rate)
            const uint8_t* rate_bytes = header->sample_rate.data();
            uint32_t sample_rate = read_be32(rate_bytes + 2); // Skip sign + exponent (first 2 bytes)

            sample_rate_arb = extract_aiff_sample_rate(header->sample_rate);
            num_channels_arb = header->num_channels;
            bits_per_sample_arb = header->sample_size;
            data_offset_arb = sizeof(audio_headers::AiffHeader);
            data_size_arb = header->ssnd_size;

            // After getting the header, add these validations:
            if (header->num_channels != 1 && header->num_channels != 2) {
                std::cerr << "Unsupported channel count: " << header->num_channels << "\n";
                return AudioCompError::INVALID_DATA;
            }

            if (header->sample_size != 16) {
                std::cerr << "Unsupported bit depth: " << header->sample_size << "\n";
                return AudioCompError::INVALID_DATA;
            }

            if (header->ssnd_size == 0) {
                std::cerr << "Invalid audio data size\n";
                return AudioCompError::INVALID_DATA;
            }
        } else {
            return AudioCompError::INVALID_DATA;
        }
    }

     else{
         return AudioCompError::UNSUPPORTED_FORMAT;
    }

    //verifying the format
    if (bits_per_sample_arb != 16) {
        return AudioCompError::INVALID_DATA;
    }

    // Read PCM data
    input_file.seekg(data_offset_arb);
    std::vector<short> pcm_data(data_size_arb / sizeof(short));
    if (!input_file.read(reinterpret_cast<char*>(pcm_data.data()), data_size_arb)) {
        input_file.close();
        return AudioCompError::INVALID_DATA;
    }

    if (pcm_data.empty()) {
        input_file.close();
        return AudioCompError::INVALID_DATA;
    }
    input_file.close();


    //initializing LAME with dynamic setting
    lame_global_flags* lame = lame_init();
    if (!lame) {
        return AudioCompError::LAME_INIT_F;
    }

    //configuring LAME
    lame_set_in_samplerate(lame, sample_rate_arb);  // Replace with actual sample rate if known(44.1kHz is CD quality)
    lame_set_num_channels(lame, num_channels_arb);  // Auto-detect mono/stereo
    lame_set_quality(lame, quality);                // Quality control
    lame_set_brate(lame, bitrate);                  // Bitrate (kbps)
    lame_set_VBR(lame, vbr_default);                // Enable VBR for better quality(might increase the mp3 size)
    lame_init_params(lame);


    //writing to output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file.is_open()) {
        lame_close(lame);
        return AudioCompError::OUTPUT_F;
    }

    //encoding in chunks for progress tracking
    constexpr std::size_t CHUNK_S = 8192; //chunk size
    std::vector<unsigned char> mp3_buffer(CHUNK_S*1.25 + 7200); //LAME's suggested size
    const std::size_t total_samples = pcm_data.size()/num_channels_arb; //for stereo samples
    std::size_t processed_samples = 0;

    for (std::size_t i{0}; i < pcm_data.size(); i+= CHUNK_S*num_channels_arb ) {
        std::size_t remaining = std::min(CHUNK_S, (pcm_data.size()-i)/num_channels_arb);

        int mp3_bytes = lame_encode_buffer_interleaved(
            lame,
            pcm_data.data() + i,
            remaining,
            mp3_buffer.data(),
            mp3_buffer.size()
            );

        if (mp3_bytes < 0) {
            lame_close(lame);
            output_file.close();
            return AudioCompError::ENCODING_F;
        }

        output_file.write(reinterpret_cast<char*>(mp3_buffer.data()), mp3_bytes);
        processed_samples += remaining;

        //progress the callback
        if (progress_bar) {
            const int progress = static_cast<int>((processed_samples*100) / total_samples);
            progress_bar(progress);
        }
    }

    //flushing the remaining data
    int mp3_bytes = lame_encode_flush(lame, mp3_buffer.data(), mp3_buffer.size());
    if (mp3_bytes > 0) {
        output_file.write(reinterpret_cast<char*>(mp3_buffer.data()), mp3_bytes);
    }


    //finally the cleanup
    lame_close(lame);
    output_file.close();
    return AudioCompError::SUCCESS;
}

void printProgress(int percent) {
    static int last_printed = -5;

    if (percent >= last_printed + 5 || percent == 100) {
        std::cout<<"\rProgress: " << percent<<"%"<< std::flush;
        last_printed = percent;
    }
}
