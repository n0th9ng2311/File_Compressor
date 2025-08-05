//
// Created by n0th9ng  on 16-07-2025.
//
#include "../hdr/file_comp.h"
#include "../hdr/common.h"

//adding an enum class here to assist with the return of checkType() fcn, probably the result from that
//will be fed into a switch statement that will handel the cases
enum class Type;

void fixCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

File_compressor::File_compressor(){
}

void  File_compressor::choice() {
    getSrc_user();
    //getDst();

}

void File_compressor::getSrc_user(){
   std::string src;
   std::cout<<"Enter the file name including the complete path and extension (Ex: D:\\text.txt)\n"
            <<"Enter here: ";

   while(true){
      std::getline(std::cin>>std::ws, src);

      fs::path temp_src {src};
      if(exists(temp_src) && fs::is_regular_file((temp_src))){
        setSrc(temp_src);
          break;
      }
      else{
        std::cerr<<"File does not exist\n";
        std::cout<<"Please enter a valid filename: \n";
      }
   }
}

//Dont really need a destination path for now, will just output the compressed file in the same dir
/*void File_compressor::getDst_user(){
   std::string dst;
   std::cout<<"Enter the destination: ";

   while(true){
      std::getline(std::cin>>std::ws, dst);

      fs::path temp_dst{dst};
      if(exists(temp_dst) && fs::is_regular_file((temp_dst))){
        setDst(temp_dst);
          break;
      }
      else{
        std::cerr<<"Destination does not exist\n";
        std::cout<<"Please enter a valid destination: \n";
      }
   }
}*/

int File_compressor::checkType() const {
    fs::path check_type = File_compressor::getSrc_path();

    std::string ext = check_type.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    static const std::unordered_map<std::string, Type> ext_to_type = {
        {".txt", Type::TXT}, {".jpg",  Type::JPG}, {".jpeg", Type::JPG},
        {".png", Type::PNG}, {".bmp",  Type::BMP}, {".tiff", Type::TIFF},
        {".psd", Type::PSD}, {".log",  Type::LOG}, {".csv",  Type::CSV},
        {".wav", Type::WAV}, {".flac", Type::FLAC},{".aiff", Type::AIFF},
        {".iso", Type::ISO}, {".tar",  Type::TAR}, {".vhd",  Type::VHD}
    };

    auto it = ext_to_type.find(ext);
    if (it != ext_to_type.end()) {
        return static_cast<int>(it->second);
    }

    return static_cast<int>(Type::NO_EXIST);
}