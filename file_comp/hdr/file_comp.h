#ifndef FILE_COMP_H
#define FILE_COMP_H

#include "common.h"

//enum to decide upon the type of file entered by the user based on the extension
enum class Type : int
{
    NO_EXIST = -1,
    TXT = 1,
    // PNG = 2,
    // JPG = 3,
    BMP = 4,
    TIFF = 5,
    PSD = 6,
    LOG = 7,
    CSV = 8,
    WAV = 9,
    XML = 10,
    AIFF = 11,
    JSON = 12,
    // TAR = 13,
    // VHD = 14,
  };

namespace fs = std::filesystem;

class File_compressor{

public:
    File_compressor(); //default constructor

    void setSrc(fs::path path){m_src_path = std::move(path);} //helper fcn to set source file location
    void setDst(fs::path path){m_dst_path = std::move(path);} //helper fcn to set the destination of file

    fs::path getSrc_path() const {return m_src_path;} //helper fcn to get the source location of file
    fs::path getDst_path() const {return m_dst_path;} //helper fcn to get the destination location of file

    void getSrc_user();    //Fcn to take the src from user

    void choice();         //Fcn to get src path from user

    int checkType() const; //Fcn to check the type of file (txt, bmp,...)

private:
    fs::path m_src_path{};
    fs::path m_dst_path{};
};

void compress_switch(File_compressor& cr); //Switch that calls the relevant compression fcn according
                                           //to the extension of given file
#endif //FILE_COMP_H