#include "txt_comp.h"

namespace fs = std::filesystem;

TXTCompError
compressFile(
    const std::string& input_path_str,
    const std::string& output_dir_str) {
    //temp path variables to check difference between raw and compressed file later
    fs::path input_path(input_path_str);
    fs::path output_dir(output_dir_str);

    // Validate input file
    if (!fs::exists(fs::path(input_path))) {
        std::cerr << "Input file does not exist: " << input_path << std::endl;
        return TXTCompError::FILE_OPEN_F;
    }

    // Checking if the input file was txt or not
    if (input_path.extension() != ".txt"
        && input_path.extension() != ".TXT") {
        std::cerr<< "Input file is not txt file\n";
        return TXTCompError::FILE_OPEN_F;
    }

    // Check if output directory exists
    if (!fs::exists(output_dir)) {
        std::cerr << "Output directory does not exist: " << output_dir << std::endl;
        return TXTCompError::OUTPUT_F;
    }

    // Open input file
    std::ifstream input_file(input_path, std::ios::binary); //opening in binary to avoid
    if (!input_file) {                                           //memory corruption
        std::cerr << "Failed to open input file: " << input_path << std::endl;
        return TXTCompError::FILE_OPEN_F;
    }

    // Read the whole txt in memory(will chunk encode it later)
    std::vector<char> input_data(
        (std::istreambuf_iterator<char>(input_file)),
        std::istreambuf_iterator<char>()
    );
    input_file.close();

    //Checking if the file was read successfully
    if (input_data.empty()) {
        std::cerr << "Input file is empty: " << input_path << std::endl;
        return TXTCompError::FILE_READING_F;
    }

    // Preparing the buffer for compression
    uLongf compressed_size = compressBound(input_data.size());
    std::vector<Bytef> compressed_data(compressed_size);

    int result =
            compress(
                compressed_data.data(), &compressed_size,
                reinterpret_cast<const Bytef*>(input_data.data()),input_data.size()
    );

    //Checking if the result is valid
    if (result != Z_OK) {
        std::cerr << "Compression failed. Zlib error code: " << result << std::endl;
        return TXTCompError::ENCODING_F;
    }

    // Constructing output file path: output_dir/compressed_<filename>.z
    std::string compressed_file_name = "compressed_" + input_path.filename().string() + ".z";
    fs::path output_path = output_dir / compressed_file_name;

    // Opening output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file) {
        std::cerr << "Failed to create output file: " << output_path << std::endl;
        return TXTCompError::OUTPUT_F;
    }

    //Writing the output to the output file
    if (!output_file.write(reinterpret_cast<char*>(compressed_data.data()), compressed_size)) {
        std::cerr<< "Error writing to output file\n";
        return TXTCompError::FILE_WRITING_F;
    }
    output_file.close();

    // Printing the size of original and compressed file
    printCompSize(input_path, output_path);


    return TXTCompError::SUCCESS;
}



