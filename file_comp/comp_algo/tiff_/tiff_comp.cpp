#include "tiff_comp.h"

namespace fs = std::filesystem;

TIFFCompError
compressTiff(
    const std::string& input_path,
    const std::string& output_path,
    int compression_scheme) {

    //Opening input TIFF file
    TIFF* input_file = TIFFOpen(input_path.c_str(), "r");
    if (!input_file) {
        std::cerr << "Could not open input TIFF file" << std::endl;
        return TIFFCompError::FILE_OPEN_F;
    }
    //temp path variables to check difference between raw and compressed file later
    fs::path temp_input_path = input_path;
    fs::path temp_output_path = output_path;

    //Verifying if the file is a TIFF
    if (temp_input_path.extension() != ".tiff"
        && temp_input_path.extension() != ".TIFF"
        && temp_input_path.extension() != ".tif"
        && temp_input_path.extension() != ".TIF") {
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

    //Copying tags and compression parameters
    std::uint32_t width;
    std::uint32_t height;
    std::uint16_t samples;
    std::uint16_t bits_per_sample;
    std::uint16_t photometric;

    //Setting input tags
    if (!TIFFGetField(input_file, TIFFTAG_IMAGEWIDTH, &width)               || //setting the width tag
        !TIFFGetField(input_file, TIFFTAG_IMAGELENGTH, &height)             || //setting the height tag
        !TIFFGetField(input_file, TIFFTAG_SAMPLESPERPIXEL, &samples)        || //setting the samples tag
        !TIFFGetField(input_file, TIFFTAG_BITSPERSAMPLE, &bits_per_sample)  || //setting the bits per sample tag
        !TIFFGetField(input_file, TIFFTAG_PHOTOMETRIC, &photometric)) {

        TIFFClose(input_file);  //closing input and output file if the tag reading fails
        TIFFClose(output_file);
        std::cerr<< "Failed to read one or more required TIFF tags\n";

        return TIFFCompError::TAG_READ_ERR;

        }

    //Setting output tags
    if (!TIFFSetField(output_file, TIFFTAG_IMAGEWIDTH, width)               || //setting the width tag
        !TIFFSetField(output_file, TIFFTAG_IMAGELENGTH, height)             || //setting the height tag
        !TIFFSetField(output_file, TIFFTAG_SAMPLESPERPIXEL, samples)        || //setting the samples tag
        !TIFFSetField(output_file, TIFFTAG_BITSPERSAMPLE, bits_per_sample)  || //setting the bits per sample tag
        !TIFFSetField(output_file, TIFFTAG_PHOTOMETRIC, photometric)        ||
        !TIFFSetField(output_file, TIFFTAG_COMPRESSION, compression_scheme) || //setting the compression tag
        !TIFFSetField(output_file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) {

        TIFFClose(input_file);  //closing the input and output file if the tag writing fails
        TIFFClose(output_file);
        std::cerr<< "Failed to set output TIFF tags\n";
        return TIFFCompError::TAG_WRITE_ERR;
    }

    // Allocate buffer and process image data
    tmsize_t scanline_size = TIFFScanlineSize(input_file);
    std::uint8_t* buf = (std::uint8_t*)_TIFFmalloc(scanline_size);

    //Verifying that the buffer was initializd
    if (!buf) {
        TIFFClose(input_file);
        TIFFClose(output_file);
        std::cerr<< "Failed to allocate memory for buffer\n";
        return TIFFCompError::MEM_ALLOC_F;
    }

    //Reading data from the input file
    for (std::uint32_t row = 0; row < height; ++row) {
        if (TIFFReadScanline(input_file, buf, row) == -1) {
            TIFFClose(input_file);
            TIFFClose(output_file);
            std::cerr << "Error reading scanline " << row << "\n";
            return TIFFCompError::FILE_READING_F;
        }
    //Writing data to the output file
        if (TIFFWriteScanline(output_file, buf, row) == -1) {
            TIFFClose(input_file);
            TIFFClose(output_file);
            std::cerr << "Error writing scanline " << row << "\n";
            return TIFFCompError::FILE_WRITING_F;
        }
    }

    //Freeing the resources after compression is done
    _TIFFfree(buf);
    TIFFClose(input_file);
    TIFFClose(output_file);

    // Printing the size of original and compressed file
    printCompSize(temp_input_path, temp_output_path);

    return TIFFCompError::SUCCESS;
}
