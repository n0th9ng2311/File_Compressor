//
// Created by n0th9ng on 16-07-2025.
//

#ifndef TXT_COMP_H
#define TXT_COMP_H

#include "../../hdr/common.h"
#include<stdio.h>
#include<assert.h>
#include<zlib.h>

bool compressFile(const std::string& inputPath, const std::string& outputPath);
bool decompressFile(const std::string& inputPath, const std::string& outputPath);


#endif //TXT_COMP_H
