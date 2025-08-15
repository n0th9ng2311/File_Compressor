//
// Created by n0th9ng on 16-07-2025.
//
#include "txt_comp.h"
#include "../../hdr/common.h"
#include "../log_/log_comp.h"

namespace fs = std::filesystem;

TXTCompError compressFile(const std::string& input_path_str,
                         const std::string& output_dir_str) {
    fs::path input_path(input_path_str);
    fs::path output_dir(output_dir_str);

    // Validate input file
    if (!fs::exists(fs::path(input_path))) {
        std::cerr << "Input file does not exist: " << input_path << std::endl;
        return TXTCompError::FILE_OPEN_F;
    }

    if (input_path.extension() != ".txt" && input_path.extension() != ".TXT") {
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
    if (!input_file) {                                           //memory corruption(zlib guy said so)
        std::cerr << "Failed to open input file: " << input_path << std::endl;
        return TXTCompError::FILE_OPEN_F;
    }

    // Read file into memory
    std::vector<char> inputData(
        (std::istreambuf_iterator<char>(input_file)),
        std::istreambuf_iterator<char>()
    );
    input_file.close();

    if (inputData.empty()) {
        std::cerr << "Input file is empty: " << input_path << std::endl;
        return TXTCompError::FILE_READING_F;
    }

    // Prepare buffer for compression
    uLongf compressed_size = compressBound(inputData.size());
    std::vector<Bytef> compressed_data(compressed_size);

    int result = compress(
        compressed_data.data(),
        &compressed_size,
        reinterpret_cast<const Bytef*>(inputData.data()),
        inputData.size()
    );

    if (result != Z_OK) {
        std::cerr << "Compression failed. Zlib error code: " << result << std::endl;
        return TXTCompError::ENCODING_F;
    }

    // Construct output file path: output_dir/compressed_<filename>.z
    std::string compressed_file_name = "compressed_" + input_path.filename().string() + ".z";
    fs::path output_path = output_dir / compressed_file_name;

    // Open output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file) {
        std::cerr << "Failed to create output file: " << output_path << std::endl;
        return TXTCompError::OUTPUT_F;
    }

    if (!output_file.write(reinterpret_cast<char*>(compressed_data.data()), compressed_size)) {
        std::cerr<< "Error writing to output file\n";
        return TXTCompError::FILE_WRITING_F;
    }
    output_file.close();

    std::cout << "Compression successful.\n";
    std::cout << "Input: " << input_path << "\n";
    std::cout << "Output: " << output_path << "\n";
    std::cout << "Original size: " << inputData.size()
              << " bytes, Compressed size: " << compressed_size << " bytes\n";

    return TXTCompError::SUCCESS;
}

// Function to decompress a file using zlib
bool decompressFile(const std::string& input_path, const std::string& output_path) {
    // Open compressed file
    std::ifstream input_file(input_path, std::ios::binary);
    if (!input_file) {
        std::cerr << "Error opening input file: " << input_path << std::endl;
        return false;
    }

    // Read entire compressed file into memory
    std::vector<char> compressed_data(
        (std::istreambuf_iterator<char>(input_file)),
        std::istreambuf_iterator<char>()
    );
    input_file.close();

    uLongf decompressed_size = compressed_data.size() * 20; //Assuming the compressed file is 20x smaller
    std::vector<char> decompressed_data(decompressed_size);

    // Decompress the data
    int result = uncompress(
        reinterpret_cast<Bytef*>(decompressed_data.data()), // Destination buffer
        &decompressed_size,                                // Pointer to decompressed size
        reinterpret_cast<Bytef*>(compressed_data.data()),  // Source data
        compressed_data.size()                             // Source data size
    );

    if (result != Z_OK) {
        std::cerr << "Decompression failed with error: " << result << std::endl;
        return false;
    }

    // Write decompressed data to output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file) {
        std::cerr << "Error opening output file: " << output_path << std::endl;
        return false;
    }

    output_file.write(decompressed_data.data(), decompressed_size);
    output_file.close();

    std::cout << "Decompression successful. Compressed size: " << compressed_data.size()
              << ", Decompressed size: " << decompressed_size << std::endl;
    return true;
}


