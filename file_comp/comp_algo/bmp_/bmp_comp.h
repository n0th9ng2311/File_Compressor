#ifndef BMP_COMP_H
#define BMP_COMP_H

#include "../../hdr/common.h"
enum class BMPCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    INVALID_CONFIG,
    ENCODING_F,
    OUTPUT_F,

};

BMPCompError bmpToPng(const std::string& input_path,
              const std::string& output_path,
              int compression = 6);

#endif