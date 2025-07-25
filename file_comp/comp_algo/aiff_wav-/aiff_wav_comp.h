//
// Created by n0th9ng on 16-07-2025.
//

#ifndef AIFF_COMP_H
#define AIFF_COMP_H

#include"../../hdr/common.h"
#include<cstring>
#include"AudioFile.h"
#include"lame.h"

enum class AudioCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_DATA,
    INVALID_CONFIG,
    LAME_INIT_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

namespace audio_headers {
    // WAV header struct(THIS IS A GENERALISED HEADER OF A WAV FILE)
    struct WavHeader {   //it is needed to read the header correctly
        std::array<char,4> riff;    // "RIFF"
        uint32_t file_size;         // File size - 8
        std::array<char,4> wave;    // "WAVE"
        std::array<char,4> fmt;     // "fmt "
        uint32_t fmt_size;          // Format chunk size (16 for PCM)
        uint16_t audio_format;      // 1 for PCM
        uint16_t num_channels;      // 1 for mono, 2 for stereo
        uint32_t sample_rate;       // 44100, 48000 etc.
        uint32_t byte_rate;         // sample_rate * num_channels * bits_per_sample/8
        uint16_t block_align;       // numChannels * bitsPerSample/8
        uint16_t bits_per_sample;   // 16, 24, 32
        std::array<char,4> data;    // "data"
        uint32_t data_size;         // Audio data size
    };


    // AIFF header struct(THIS IS A GENERAL HEADER OF AN AIFF FILE)
#pragma pack(push,1)
    struct AiffHeader {
        std::array<char,4> form;            // "FORM"
        uint32_t file_size;                 // File size - 8
        std::array<char,4> aiff;            // "AIFF"
        std::array<char,4> comm;            // "COMM"
        uint32_t comm_size;                 // COMM chunk size (18 for standard)
        uint16_t num_channels;              // 1 for mono, 2 for stereo
        uint32_t num_sample_frames;
        uint16_t sample_size;               // 16, 24, 32
        std::array<unsigned char,10> sample_rate;  // 80-bit extended float
        std::array<char,4> ssnd;            // "SSND"
        uint32_t ssnd_size;                 // Sound data size
        uint32_t offset;
        uint32_t block_size;
    };
#pragma pack(pop)

    // Compile-time known header markers
    constexpr std::array RIFF_MARKER{'R','I','F','F'};
    constexpr std::array WAVE_MARKER{'W','A','V','E'};
    constexpr std::array FMT_MARKER {'f','m','t',' '};
    constexpr std::array FORM_MARKER{'F','O','R','M'};
    constexpr std::array AIFF_MARKER{'A','I','F','F'};
    constexpr std::array COMM_MARKER{'C','O','M','M'};
}


/*
*    COMPRESSING WAV/AIFF WITH LAME TO MP3
*    @param input_path          Input path of WAV/AIFF
*    @param output_path         Output path of MP3
*    @param bitrate             MP3 bitrate(default : 192kb (works for most files))
*    @param quality             LAME compression quality (0-9, 0 = best, 9 = fastest)
*    @param progress_bar        Progress callback(0-100%)
*    @return                    Error code return
*/

AudioCompError compressToMp3(const std::string& input_path, const std::string& output_path,
                            int bitrate = 192, int quality = 5, void(*progress_bar)(int)=nullptr);

void printProgress(int percent);



#endif //AIFF_COMP_H
