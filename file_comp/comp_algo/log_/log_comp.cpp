#include "log_comp.h"

#include "../csv_/csv_comp.h"

namespace fs = std::filesystem;

LOGCompError
compressLOG(
    const std::string &input_path,
    const std::string &output_path) {
    //temp path variables to check difference between raw and compressed file later
    fs::path temp_input_path = input_path;
    fs::path temp_output_path = output_path;

    //Verifying if the file exists and if it is a log
    if (!fs::exists(input_path)) {
        std::cerr << "Input file not exists\n";
        return LOGCompError::FILE_READING_F;
    }
    if (temp_input_path.extension() != ".log"
        && temp_input_path.extension() != ".LOG") {
        std::cerr << "Input file is not a log file\n";
        return LOGCompError::INVALID_FILE_TYPE;
    }

    //Checking if the file is open
    std::ifstream input_file(input_path, std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Input file is not open\n";
        return LOGCompError::FILE_OPEN_F;
    }

    //Reading the whole log file into memory(for now, will chunk encode it later)
    auto file_size = fs::file_size(input_path);
    std::vector<char> input_data(file_size);
    if (!input_file.read(input_data.data(), file_size)) {
        std::cerr << "Failed to read input file\n";
        return LOGCompError::FILE_READING_F;
    }

    //Preparing for compression
    uLongf compressed_size = compressBound(file_size);
    std::vector<Bytef> compressed_data(compressed_size);

    //compressing the data
    auto result =
            compress2(
                compressed_data.data(), &compressed_size,
                reinterpret_cast<const Bytef *>(input_data.data()), file_size,
                Z_BEST_COMPRESSION
            );

    //Checking if the result is valid
    if (result != Z_OK) {
        std::cerr << "Compression failed, error: " << result << "\n";
        return LOGCompError::ENCODING_F;
    }

    //Checking if output path exists(creates a directory if it does not)
    fs::create_directories(temp_output_path.parent_path());
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Error opening the output file\n";
        return LOGCompError::OUTPUT_F;
    }

    //Writing compressed data to output file
    if (!output_file.write(reinterpret_cast<char *>(compressed_data.data()), compressed_size)) {
        std::cerr << "Error writing to the output file\n";
        return LOGCompError::FILE_WRITING_F;
    }

    output_file.close();

    // Printing the size of original and compressed file
    printCompSize(temp_input_path, temp_output_path);

    return LOGCompError::SUCCESS;
}
