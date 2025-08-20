//These macros had to be included here to work in this cpp file,
//defining them again in .h was troublesome
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../external/stb/stb_image_write.h"

#include "bmp_comp.h"

namespace fs = std::filesystem;

BMPCompError
bmpToPng(
    const std::string &input_path,
    const std::string &output_path,
    int compression) {
    //temp path variables to check difference between raw and compressed file later
    fs::path temp_input_path = input_path;
    fs::path temp_output_path = output_path;

    //Checking if the input file exists
    if (!fs::exists(input_path)) {
        std::cerr << "File not found\n";
        return BMPCompError::FILE_OPEN_F;
    }

    //Verifying if the format is bmp or not
    if (fs::path(input_path).extension() != ".bmp"
        && fs::path(input_path).extension() != ".BMP") {
        std::cerr << "File is not a bmp file\n";
        return BMPCompError::INVALID_FILE_TYPE;
    }

    //Initializing the BMP parameters
    int width{};
    int height{};
    int channels{};

    //Making a char array and loading it with data through stbi
    std::unique_ptr<unsigned char[], void(*)(void *)>
            data(stbi_load(
                     input_path.c_str(),
                     &width, &height, &channels,
                     STBI_rgb),
                 stbi_image_free);

    //Checking if data was successfully initialized
    if (!data) {
        std::cerr << "Error loading BMP: " << stbi_failure_reason() << "\n";
        return BMPCompError::FILE_OPEN_F;
    }

    //Verifying if the format is supported by stbi encoder
    if (width <= 0 || height <= 0 || (channels != 3 && channels != 4)) {
        std::cerr << "Loaded BMP has invalid dimensions or unsupported channel count\n";
        return BMPCompError::INVALID_CONFIG;
    }


    //Checking if output path exists(creates a directory if it does not)
    auto out_parent = std::filesystem::path(output_path).parent_path();
    if (!out_parent.empty() && !fs::exists(out_parent)) {
        if (!fs::create_directories(out_parent)) {
            std::cerr << "Could not create output directory\n";
            return BMPCompError::OUTPUT_F;
        }
    }

    //Writing to the output file
    const int stride = width * channels;
    if (!stbi_write_png(
        output_path.c_str(),
        width, height, channels,
        data.get(), stride)) {
        std::cerr << "Error saving PNG\n";
        return BMPCompError::ENCODING_F;
    }

    // Printing the size of original and compressed file
    printCompSize(temp_input_path, temp_output_path);

    return BMPCompError::SUCCESS;
}
