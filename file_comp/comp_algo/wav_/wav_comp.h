

#ifndef WAV_COMP_H
#define WAV_COMP_H
#include "../../hdr/common.h"
#include <../../external/audioFile/AudioFile.h>
#include <lame.h>

enum class AudioCompError_WAV {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_DATA,
    INVALID_CONFIG,
    LAME_INIT_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

AudioCompError_WAV
compressWavToMp3(
    const std::string &input_path,
    const std::string &output_path,
    int bitrate,
    int quality,
    void (*progress_bar)(int) = nullptr);

void printProgress(int percent);

#endif // WAV_COMP_H


