#ifndef AIFF_COMP_H
#define AIFF_COMP_H

#include"../../hdr/common.h"
#include <../../external/audioFile/AudioFile.h>
#include <lame.h>


enum class AudioCompError_AIFF {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_DATA,
    INVALID_CONFIG,
    LAME_INIT_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

AudioCompError_AIFF compressAifftoMp3(const std::string& input_path,
                                      const std::string& output_path,
                                      int bitrate,
                                      int quality,
                                      void (*progressBar)(int) = nullptr);

void progressBar (int percentage);

#endif
