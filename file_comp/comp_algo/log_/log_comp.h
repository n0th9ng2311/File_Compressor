#ifndef LOG_COMP_H
#define LOG_COMP_H

#include"..\..\hdr\common.h"
#include"zlib.h"

enum class LOGCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    FILE_READING_F,
    FILE_WRITING_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

LOGCompError
compressLOG(
    const std::string& input_path,
    const std::string& output_path);

#endif