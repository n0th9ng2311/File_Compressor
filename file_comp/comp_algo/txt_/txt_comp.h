//
// Created by n0th9ng on 16-07-2025.
//

#ifndef TXT_COMP_H
#define TXT_COMP_H

#include "../../hdr/common.h"
#include<stdio.h>
#include<assert.h>
#include<zlib.h>

enum class TXTCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    FILE_READING_F,
    FILE_WRITING_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};
TXTCompError compressFile(const std::string& inputPath,
                         const std::string& outputPath);

bool decompressFile(const std::string& inputPath,
                    const std::string& outputPath);


#endif //TXT_COMP_H
