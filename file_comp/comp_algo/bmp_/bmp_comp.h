#ifndef BMP_COMP_H
#define BMP_COMP_H

#include "../../hdr/common.h"


bool bmpToPng(const std::string& input_path,
              const std::string& output_path,
              int compression = 6);

#endif