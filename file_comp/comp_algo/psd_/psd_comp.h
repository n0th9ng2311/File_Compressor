#ifndef PSD_COMP_H
#define PSD_COMP_H

#include <string>

#include"..\..\hdr\common.h"
#include "zlib.h"
#include "libpsd.h"


enum class PSDCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    FILE_READING_F,
    FILE_WRITING_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

PSDCompError compressPSD(const std::string& input_path,
                         const std::string& output_path);


#endif