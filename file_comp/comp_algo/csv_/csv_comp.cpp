
#include "csv_comp.h"

namespace fs = std::filesystem;
bool compressCSV(const std::string& input_path,
                const std::string& output_path) {
    //Verifying if the file exists and if it is a csv
    if (!fs::exists(input_path)) {
        std::cerr << "Input file not exists\n";
        return false;
    }

    fs::path temp_input_path = input_path;
    fs::path temp_output_path = output_path;
    if ( temp_input_path.extension()!= ".csv" && temp_input_path.extension() != ".CSV") {
        std::cerr << "Input file is not a csv file\n";
        return false;
    }

    //Reading the csv file
    std::ifstream input_file(input_path, std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Input file is not open\n";
        return false;
    }

    auto file_size = fs::file_size(input_path);
    std::vector<char> input_data(file_size);
    if (!input_file.read(input_data.data(), file_size)) {
        std::cerr<< "Failed to read input file\n";
        return false;
    }

    //Preparing for compression
    uLongf compressed_size = compressBound(file_size);
    std::vector<Bytef> compressed_data(compressed_size);

    //compressing the data
    auto result = compress2(compressed_data.data(),
                            &compressed_size,
                            reinterpret_cast<const Bytef*>(input_data.data()),file_size,
                            Z_BEST_COMPRESSION);

    if (result != Z_OK) {
        std::cerr<< "Compression failed, error: "<< result<< "\n";
        return false;
    }

    //Writing compressed data
    fs::create_directories(temp_output_path.parent_path()); //creating output dir if needed

    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr<< "Error opening the output file\n";
        return false;
    }

    if (!output_file.write(reinterpret_cast<char*>(compressed_data.data()), compressed_size)) {
        std::cerr<< "Error writing to the output file\n";
        return false;
    }

    output_file.close();

    auto inputF_size = fs::file_size(input_path);
    auto outputF_size = fs::file_size(output_path);
    double reduction_percent = (static_cast<double>(outputF_size) / inputF_size) * 100.0;

    std::cout<< "\nOriginal Size: "<<inputF_size << " bytes"
             << "\nCompressed Size: "<<outputF_size << " bytes"
             << "\nSize reduced to: "<< reduction_percent <<"% of original ";

    return true;
}


