#ifndef XML_COMP_H
#define XML_COMP_H

#include"../../hdr/common.h"
#include"zlib.h"


enum class XMLCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    FILE_READING_F,
    FILE_WRITING_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

XMLCompError
compressXML(
    const std::string& input_path,
    const std::string& output_path);

#endif