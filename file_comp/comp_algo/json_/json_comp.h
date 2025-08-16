#ifndef JSON_COMP_H
#define JSON_COMP_H

#include"../../hdr/common.h"
#include "zlib.h"


enum class JSONCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    FILE_READING_F,
    FILE_WRITING_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

JSONCompError compressJSON(const std::string& input_path,
                        const std::string& output_path);



#endif