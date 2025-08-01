#include "wav_comp.h"


AudioCompError_WAV compressWavToMp3(const std::string &input_path,
                                    const std::string &output_path,
                                    int bitrate,
                                    int quality,
                                    void (*progress_bar)(int)) {

    // Load WAV file using AudioFile
    AudioFile<float> audioFile;
    if (!audioFile.load(input_path)) {
        return AudioCompError_WAV::FILE_OPEN_F;
    }

    // Verify format is supported
    if (audioFile.getBitDepth() != 16) {
        return AudioCompError_WAV::UNSUPPORTED_FORMAT;
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
        return AudioCompError_WAV::LAME_INIT_F;
    }

    lame_set_in_samplerate(lame, sample_rate);
    lame_set_out_samplerate(lame, sample_rate);
    lame_set_num_channels(lame, num_channels);
    lame_set_quality(lame, quality);
    lame_set_brate(lame, bitrate);
    lame_set_VBR(lame, vbr_default);

    if (lame_init_params(lame) < 0) {
        lame_close(lame);
        return AudioCompError_WAV::LAME_INIT_F;
    }

    // 4. Open output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file.is_open()) {
        lame_close(lame);
        return AudioCompError_WAV::OUTPUT_F;
    }

    // 5. Encode in chunks
    constexpr size_t CHUNK_SAMPLES = 8192;
    std::vector<unsigned char> mp3_buffer(1.25 * CHUNK_SAMPLES + 7200);
    size_t processed_samples = 0;

    for (size_t i = 0; i < pcm_data.size(); i += CHUNK_SAMPLES * num_channels) {
        size_t remaining = std::min(CHUNK_SAMPLES,
                                  (pcm_data.size() - i) / num_channels);

        int mp3_bytes;
        if (num_channels == 2) {
            mp3_bytes = lame_encode_buffer_interleaved(
                                        lame,
                                        pcm_data.data() + i,
                                        remaining,
                                        mp3_buffer.data(),
                                        mp3_buffer.size());
        } else {
            mp3_bytes = lame_encode_buffer(
                                        lame,
                                        pcm_data.data() + i,
                                        nullptr,
                                        remaining,
                                        mp3_buffer.data(),
                                        mp3_buffer.size());
        }

        if (mp3_bytes < 0) {
            lame_close(lame);
            output_file.close();
            return AudioCompError_WAV::ENCODING_F;
        }

        output_file.write(reinterpret_cast<char*>(mp3_buffer.data()), mp3_bytes);
        processed_samples += remaining;

        if (progress_bar) {
            int progress = static_cast<int>((processed_samples * 100) / total_samples);
            progress_bar(progress);
        }
    }

    // 6. Flush encoder
    int mp3_bytes = lame_encode_flush(lame, mp3_buffer.data(), mp3_buffer.size());
    if (mp3_bytes > 0) {
        output_file.write(reinterpret_cast<char*>(mp3_buffer.data()), mp3_bytes);
    }

    // 7. Cleanup
    lame_close(lame);
    output_file.close();

    // Verify output
    std::ifstream check(output_path, std::ios::binary | std::ios::ate);
    if (check.tellg() <= 0) {
        return AudioCompError_WAV::ENCODING_F;
    }

    return AudioCompError_WAV::SUCCESS;
}

void printProgress(int percent) {
    static int last_printed = -5;
    if (percent >= last_printed + 5 || percent == 100) {
        std::cout << "\rProgress: " << percent << "%" << std::flush;
        last_printed = percent;
    }
}