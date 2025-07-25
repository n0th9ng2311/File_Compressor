//
// Created by n0th9ng on 16-07-2025.
//
#include "txt_comp.h"
#include "../../hdr/common.h"

namespace fs = std::filesystem;

bool compressFile(const std::string& input_path_str, const std::string& output_dir_str) {
    fs::path input_path(input_path_str);
    fs::path output_dir(output_dir_str);

    // Validate input file
    if (!fs::exists(input_path)) {
        std::cerr << "Input file does not exist: " << input_path << std::endl;
        return false;
    }

    // Check if input is a file
    if (!fs::is_regular_file(input_path)) {
        std::cerr << "Input path is not a regular file: " << input_path << std::endl;
        return false;
    }

    // Check if output directory exists
    if (!fs::exists(output_dir)) {
        std::cerr << "Output directory does not exist: " << output_dir << std::endl;
        return false;
    }

    // Open input file
    std::ifstream input_file(input_path, std::ios::binary); //opening in binary to avoid
    if (!input_file) {                                           //memory corruption(zlib guy said so)
        std::cerr << "Failed to open input file: " << input_path << std::endl;
        return false;
    }

    // Read file into memory
    std::vector<char> inputData(
        (std::istreambuf_iterator<char>(input_file)),
        std::istreambuf_iterator<char>()
    );
    input_file.close();

    if (inputData.empty()) {
        std::cerr << "Input file is empty: " << input_path << std::endl;
        return false;
    }

    // Prepare buffer for compression
    uLongf compressedSize = compressBound(inputData.size());
    std::vector<Bytef> compressedData(compressedSize);

    int result = compress(
        compressedData.data(),
        &compressedSize,
        reinterpret_cast<const Bytef*>(inputData.data()),
        inputData.size()
    );

    if (result != Z_OK) {
        std::cerr << "Compression failed. Zlib error code: " << result << std::endl;
        return false;
    }

    // Construct output file path: output_dir/compressed_<filename>.z
    std::string compressedFileName = "compressed_" + input_path.filename().string() + ".z";
    fs::path output_path = output_dir / compressedFileName;

    // Open output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file) {
        std::cerr << "Failed to create output file: " << output_path << std::endl;
        return false;
    }

    output_file.write(reinterpret_cast<char*>(compressedData.data()), compressedSize);
    output_file.close();

    std::cout << "Compression successful.\n";
    std::cout << "Input: " << input_path << "\n";
    std::cout << "Output: " << output_path << "\n";
    std::cout << "Original size: " << inputData.size()
              << " bytes, Compressed size: " << compressedSize << " bytes\n";

    return true;
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
    std::vector<char> compressedData(
        (std::istreambuf_iterator<char>(input_file)),
        std::istreambuf_iterator<char>()
    );
    input_file.close();

    uLongf decompressedSize = compressedData.size() * 20; //Assuming the compressed file is 20x smaller
    std::vector<char> decompressedData(decompressedSize);

    // Decompress the data
    int result = uncompress(
        reinterpret_cast<Bytef*>(decompressedData.data()), // Destination buffer
        &decompressedSize,                                // Pointer to decompressed size
        reinterpret_cast<Bytef*>(compressedData.data()),  // Source data
        compressedData.size()                             // Source data size
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

    output_file.write(decompressedData.data(), decompressedSize);
    output_file.close();

    std::cout << "Decompression successful. Compressed size: " << compressedData.size()
              << ", Decompressed size: " << decompressedSize << std::endl;
    return true;
}


