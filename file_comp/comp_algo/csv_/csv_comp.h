#ifndef csv_comp_h
#define csv_comp_h


#include"../../hdr/common.h"

#include"zlib.h"

bool compressCSV(const std::string& input_path,
                const std::string& output_path);

#endif
