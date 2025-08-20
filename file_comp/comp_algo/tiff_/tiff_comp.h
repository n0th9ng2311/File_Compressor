#ifndef  TIFF_COMP_H
#define TIFF_COMP_H

#include"../../hdr/common.h"
#include"tiffio.h"


enum class TIFFCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    TAG_READ_ERR,
    TAG_WRITE_ERR,
    FILE_READING_F,
    FILE_WRITING_F,
    MEM_ALLOC_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

TIFFCompError
compressTiff(
    const std::string& input_path,
    const std::string& output_path,
    int compression_scheme = COMPRESSION_ADOBE_DEFLATE);


#endif