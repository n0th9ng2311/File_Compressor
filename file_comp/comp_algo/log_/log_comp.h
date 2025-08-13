#ifndef LOG_COMP_H
#define LOG_COMP_H

#include"..\..\hdr\common.h"
#include"zlib.h"

bool compressLOG(const std::string& input_path,
                const std::string& output_path);
#endif