#include"aiff_comp.h"


AudioCompError_AIFF
compressAifftoMp3(
    const std::string &input_path,
    const std::string &output_path,
    int bitrate,
    int quality,
    void (*progress_bar)(int)) {
    //temp path variables to check difference between raw and compressed file later
    fs::path temp_input_path = input_path;
    fs::path temp_output_path = output_path;

    //Loading the AIFF file
    AudioFile<float> audioFile;
    if (!audioFile.load(input_path)) {
        std::cerr << "Error: could not open or parse AIFF file\n";
        return AudioCompError_AIFF::FILE_OPEN_F;
    }

    //Verify the format is supported(file should be of 16bit depth)
    if (audioFile.getBitDepth() != 16) {
        std::cerr << "Error: Only 16-bit AIFF files are supported\n";
        return AudioCompError_AIFF::UNSUPPORTED_FORMAT;
    }

    // Prepare PCM data for LAME
    const std::uint32_t sample_rate = audioFile.getSampleRate();
    const std::uint16_t num_channels = audioFile.getNumChannels();
    const std::size_t total_samples = audioFile.getNumSamplesPerChannel();


    // Convert float samples to 16-bit PCM (AudioFile loads as float [-1, 1])
    std::vector<short> pcm_data(total_samples * num_channels);
    for (std::size_t i = 0; i < total_samples; i++) {
        for (int ch = 0; ch < num_channels; ch++) {
            double sample = audioFile.samples[ch][i];
            pcm_data[i * num_channels + ch] = static_cast<short>(sample * 32767.0f);
        }
    }


    // Initialize LAME encoder
    lame_global_flags *lame = lame_init();
    if (!lame) {
        return AudioCompError_AIFF::LAME_INIT_F;
    }

    lame_set_in_samplerate(lame, sample_rate);  //setting in sample rates from metadata
    lame_set_out_samplerate(lame, sample_rate); //setting out sample rated(same as in sample rate)
    lame_set_num_channels(lame, num_channels);  //setting number of channels
    lame_set_quality(lame, quality);            //setting quality(by default = 2)
    lame_set_brate(lame, bitrate);              //setting bitrate(by default = 192kbps)
    lame_set_VBR(lame, vbr_default);            //enabling variable bitrate

    //Checking if lame was initialized
    if (lame_init_params(lame) < 0) {
        lame_close(lame);
        std::cerr << "Error: Could not initialize LAME parameters\n";
        return AudioCompError_AIFF::LAME_INIT_F;
    }

    // Open output file
    std::ofstream out_file(output_path, std::ios::binary);
    if (!out_file.is_open()) {
        lame_close(lame);
        std::cerr << "Error: Could not open output file\n";
        return AudioCompError_AIFF::OUTPUT_F;
    }

    // Encoding in chunks
    constexpr size_t CHUNK_SAMPLES = 8192;
    std::vector<unsigned char> mp3_buffer(CHUNK_SAMPLES * 1.25 + 7200); //recommended buffer size
    std::size_t processed_samples = 0;

    for (int i = 0; i < pcm_data.size(); i += CHUNK_SAMPLES * num_channels) {
        std::size_t remaining =
                        std::min(
                            CHUNK_SAMPLES,
                            (pcm_data.size() - i) / num_channels
                        );

        int mp3_bytes{};

        if (num_channels == 2) {
            //handles case where both right and left channels are present
            mp3_bytes =
                    lame_encode_buffer_interleaved(
                        lame, pcm_data.data() + i,
                        remaining,
                        mp3_buffer.data(), mp3_buffer.size()
                    );
        } else {
            //handel case where only one channel is present
            mp3_bytes =
                    lame_encode_buffer(
                        lame, pcm_data.data() + i,
                        nullptr, remaining,
                        mp3_buffer.data(), mp3_buffer.size()
                    );
        }

        //Checking if any bytes were read
        if (mp3_bytes < 0) {
            lame_close(lame);
            out_file.close();
            return AudioCompError_AIFF::ENCODING_F;
        }
        //Writing the bytes to output file
        out_file.write(reinterpret_cast<char *>(mp3_buffer.data()), mp3_bytes);
        processed_samples += remaining;

        if (progress_bar) {
            int progress = static_cast<int>((processed_samples * 100) / total_samples);
            progress_bar(progress);
        }
    }

    // Flushing encoder
    int mp3_bytes = lame_encode_flush(lame, mp3_buffer.data(), mp3_buffer.size());
    // Writing the last bit of data to output file incase there was some remaining
    if (mp3_bytes > 0) {
        out_file.write(reinterpret_cast<char *>(mp3_buffer.data()), mp3_bytes);
    }

    // Cleaning up lame
    lame_close(lame);
    out_file.close();

    // Verifying if output was written to the output file
    std::ifstream check(output_path, std::ios::binary | std::ios::ate);
    if (check.tellg() <= 0) {
        return AudioCompError_AIFF::ENCODING_F;
    }

    // Printing the size of original and compressed file
    printCompSize(temp_input_path, temp_output_path);

    return AudioCompError_AIFF::SUCCESS;
}
