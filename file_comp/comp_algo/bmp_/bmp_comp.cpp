//These macros had to be included here to work in this cpp file,
//defining them again in .h was troublesome
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../external/stb/stb_image_write.h"

#include "bmp_comp.h"

bool bmpToPng(const std::string &input_path,
              const std::string &output_path,
              int compression) {

    //Loading in the BMP
    int width{};
    int height{};
    int channels{};

    std::unique_ptr<unsigned char[], void(*)(void*)>
    data(stbi_load(input_path.c_str(),
        &width,
        &height,
        &channels,
        STBI_rgb),
        stbi_image_free);

    if (!data) {
        std::cerr<< "Error loading BMP: "<< stbi_failure_reason() << "\n";
        return false;
    }

    //converting to PNG
    const int stride = width * channels;
    if (!stbi_write_png(output_path.c_str(),
                        width,
                        height,
                    channels,
                    data.get(),
                        stride)) {

        std::cerr<< "Error saving PNG\n";
        return false;
    }
    return true;

}

