#include"aiff_comp.h"


AudioCompError_AIFF compressAifftoMp3(const std::string& input_path,
                                      const std::string& output_path,
                                      int bitrate,
                                      int quality,
                                      void (*progress_bar)(int)){

    //Loading the AIFF file
    AudioFile<float> audioFile;
    if (!audioFile.load(input_path)) {
        std::cerr<< "Error: could not open or parse AIFF file\n";
        return AudioCompError_AIFF::FILE_OPEN_F;
    }

    //Verify the format is supported
    if (audioFile.getBitDepth() != 16) {
        std::cerr<< "Error: Only 16-bit AIFF files are supported\n";
        return AudioCompError_AIFF::UNSUPPORTED_FORMAT;
    }

    // Prepare PCM data for LAME
    const uint32_t sample_rate = audioFile.getSampleRate();
    const uint16_t num_channels = audioFile.getNumChannels();
    const size_t total_samples = audioFile.getNumSamplesPerChannel();


    // Convert float samples to 16-bit PCM (AudioFile loads as float [-1, 1])
    std::vector<short> pcm_data(total_samples * num_channels);
    for (size_t i = 0; i < total_samples; i++) {
        for (int ch = 0; ch < num_channels; ch++) {
            float sample = audioFile.samples[ch][i];
            pcm_data[i * num_channels + ch] = static_cast<short>(sample * 32767.0f);
        }
    }


    // Initialize LAME encoder
    lame_global_flags* lame = lame_init();
    if (!lame) {
        return AudioCompError_AIFF::LAME_INIT_F;
    }

    lame_set_in_samplerate(lame, sample_rate);
    lame_set_out_samplerate(lame, sample_rate);
    lame_set_num_channels(lame, num_channels);
    lame_set_quality(lame, quality);
    lame_set_brate(lame, bitrate);
    lame_set_VBR(lame, vbr_default);

    if (lame_init_params(lame) < 0) {
        lame_close(lame);
        std::cerr<< "Error: Could not initialize LAME parameters\n";
        return AudioCompError_AIFF::LAME_INIT_F;
    }

    // Open output file
    std::ofstream out_file(output_path, std::ios::binary);
    if (!out_file.is_open()) {
        lame_close(lame);
        std::cerr<< "Error: Could not open output file\n";
        return AudioCompError_AIFF::OUTPUT_F;
    }

    // Encoding in chunks
    constexpr size_t CHUNK_SAMPLES = 8192;
    std::vector<unsigned char> mp3_buffer(CHUNK_SAMPLES * 1.25 + 7200);
    size_t processed_samples = 0;

    for (int i = 0; i < pcm_data.size(); i += CHUNK_SAMPLES * num_channels ) {
        size_t remaining = std::min(CHUNK_SAMPLES,
                                    (pcm_data.size() - i) / num_channels);

        int mp3_bytes{};

        if (num_channels == 2) {
            mp3_bytes = lame_encode_buffer_interleaved(lame,
                                                        pcm_data.data() + i,
                                                        remaining,
                                                        mp3_buffer.data(),
                                                        mp3_buffer.size()
                                                        );

        } else {
            mp3_bytes = lame_encode_buffer(lame,
                                            pcm_data.data() + i,
                                            nullptr,
                                            remaining,
                                            mp3_buffer.data(),
                                            mp3_buffer.size()
                                            );
        }

        if (mp3_bytes < 0) {
            lame_close(lame);
            out_file.close();
            return AudioCompError_AIFF::ENCODING_F;

        }
        out_file.write(reinterpret_cast<char*>(mp3_buffer.data()), mp3_bytes);
        processed_samples += remaining;

        if (progress_bar) {
            int progress = static_cast<int>((processed_samples * 100) / total_samples);
            progress_bar(progress);
        }
    }

    // Flush encoder
    int mp3_bytes = lame_encode_flush(lame, mp3_buffer.data(), mp3_buffer.size());
    if (mp3_bytes > 0) {
        out_file.write(reinterpret_cast<char*>(mp3_buffer.data()), mp3_bytes);
    }

    // Cleanup
    lame_close(lame);
    out_file.close();

    // Verify output
    std::ifstream check(output_path, std::ios::binary | std::ios::ate);
    if (check.tellg() <= 0) {
        return AudioCompError_AIFF::ENCODING_F;
    }


    auto inputF_size = std::filesystem::file_size(input_path);
    auto outputF_size = std::filesystem::file_size(output_path);
    double reduction_percent = (static_cast<double>(outputF_size) / inputF_size) * 100.0;

    std::cout<< "\nOriginal Size: "<<inputF_size << " bytes"
             << "\nCompressed Size: "<<outputF_size << " bytes"
             << "\nSize reduced to: "<< reduction_percent <<"% of original ";

    return AudioCompError_AIFF::SUCCESS;
}


    
