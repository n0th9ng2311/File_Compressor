#ifndef csv_comp_h
#define csv_comp_h


#include"../../hdr/common.h"

#include"zlib.h"


enum class CSVCompError {
    SUCCESS,
    FILE_OPEN_F,
    INVALID_FILE_TYPE,
    FILE_READING_F,
    FILE_WRITING_F,
    ENCODING_F,
    OUTPUT_F,
    UNSUPPORTED_FORMAT
};

CSVCompError
compressCSV(
    const std::string& input_path,
    const std::string& output_path);

#endif
