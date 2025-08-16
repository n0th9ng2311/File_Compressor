#include "tiff_comp.h"

namespace fs = std::filesystem;

TIFFCompError compressTiff(const std::string& input_path,
                           const std::string& output_path,
                           int compression_scheme) {

    //Opening input TIFF
    TIFF* input_file = TIFFOpen(input_path.c_str(), "r");
    if (!input_file) {
        std::cerr << "Could not open input TIFF file" << std::endl;
        return TIFFCompError::FILE_OPEN_F;
    }

    fs::path temp_input_path = input_path;
    fs::path temp_output_path = output_path;
    
    if (temp_input_path.extension() != ".tiff" && temp_input_path.extension() != ".TIFF"
        && temp_input_path.extension() != ".tif" && temp_input_path.extension() != ".TIF") {
        std::cerr<< "Input file is not a tiff file\n";
        return TIFFCompError::INVALID_FILE_TYPE;
    }
    
    //Opening output TIFF with compression
    TIFF* output_file = TIFFOpen(output_path.c_str(), "w");
    if (!output_file) {
        TIFFClose(input_file);
        std::cerr << "Could not create output TIFF file" << std::endl;
        return TIFFCompError::OUTPUT_F;
    }

    //Copying tags and set compression
    std::uint32_t width;
    std::uint32_t height;
    std::uint16_t samples;
    std::uint16_t bits_per_sample;
    std::uint16_t photometric;

    //Setting input tags
    if (!TIFFGetField(input_file, TIFFTAG_IMAGEWIDTH, &width)               ||
        !TIFFGetField(input_file, TIFFTAG_IMAGELENGTH, &height)             ||
        !TIFFGetField(input_file, TIFFTAG_SAMPLESPERPIXEL, &samples)        ||
        !TIFFGetField(input_file, TIFFTAG_BITSPERSAMPLE, &bits_per_sample)  ||
        !TIFFGetField(input_file, TIFFTAG_PHOTOMETRIC, &photometric)) {

        TIFFClose(input_file);
        TIFFClose(output_file);
        std::cerr<< "Failed to read one or more required TIFF tags\n";

        return TIFFCompError::TAG_READ_ERR;

        }

    //Setting output tags
    if (!TIFFSetField(output_file, TIFFTAG_IMAGEWIDTH, width)               ||
        !TIFFSetField(output_file, TIFFTAG_IMAGELENGTH, height)             ||
        !TIFFSetField(output_file, TIFFTAG_SAMPLESPERPIXEL, samples)        ||
        !TIFFSetField(output_file, TIFFTAG_BITSPERSAMPLE, bits_per_sample)  ||
        !TIFFSetField(output_file, TIFFTAG_PHOTOMETRIC, photometric)        ||
        !TIFFSetField(output_file, TIFFTAG_COMPRESSION, compression_scheme) ||
        !TIFFSetField(output_file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) {

        TIFFClose(input_file);
        TIFFClose(output_file);
        std::cerr<< "Failed to set output TIFF tags\n";
        return TIFFCompError::TAG_WRITE_ERR;
    }

    // Allocate buffer and process image data
    tmsize_t scanline_size = TIFFScanlineSize(input_file);
    std::uint8_t* buf = (std::uint8_t*)_TIFFmalloc(scanline_size);
    if (!buf) {
        TIFFClose(input_file);
        TIFFClose(output_file);
        std::cerr<< "Failed to allocate memory for buffer\n";
        return TIFFCompError::MEM_ALLOC_F;
    }

    for (std::uint32_t row = 0; row < height; ++row) {
        if (TIFFReadScanline(input_file, buf, row) == -1) {
            TIFFClose(input_file);
            TIFFClose(output_file);
            std::cerr << "Error reading scanline " << row << "\n";
            return TIFFCompError::FILE_READING_F;
        }
        if (TIFFWriteScanline(output_file, buf, row) == -1) {
            TIFFClose(input_file);
            TIFFClose(output_file);
            std::cerr << "Error writing scanline " << row << "\n";
            return TIFFCompError::FILE_WRITING_F;
        }
    }

    _TIFFfree(buf);
    TIFFClose(input_file);
    TIFFClose(output_file);

    printCompSize(temp_input_path, temp_output_path);

    return TIFFCompError::SUCCESS;
}
