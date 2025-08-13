#ifndef XML_COMP_H
#define XML_COMP_H

#include"../../hdr/common.h"
#include"zlib.h"

bool compressXML(const std::string& input_path, const std::string& output_path);

#endif