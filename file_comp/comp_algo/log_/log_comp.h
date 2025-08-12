#ifndef log_comp_h
#define log_comp_h

#include"..\..\hdr\common.h"
#include"zlib.h"

bool compressLOG(const std::string& input_path,
                const std::string& output_path);
#endif