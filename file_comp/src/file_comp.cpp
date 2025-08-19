#include "../hdr/file_comp.h"
#include "../hdr/common.h"
#include "../hdr/all_comp_algo.h"

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
        {".txt",  Type::TXT},    {".jpg",  Type::JPG},    {".jpeg", Type::JPG},
        {".png",  Type::PNG},    {".bmp",  Type::BMP},    {".tiff", Type::TIFF},
        {".psd",  Type::PSD},    {".log",  Type::LOG},    {".csv",  Type::CSV},
        {".wav",  Type::WAV},    {".xml",  Type::XML},    {".aiff", Type::AIFF},
        {".json", Type::JSON},   {".tar",  Type::TAR},    {".vhd",  Type::VHD}
    };

    auto it = ext_to_type.find(ext);
    if (it != ext_to_type.end()) {
        return static_cast<int>(it->second);
    }

    return static_cast<int>(Type::NO_EXIST);
}

void compress_switch(File_compressor& cr) {

    auto src_path_str = cr.getSrc_path().string();
    auto src_path = cr.getSrc_path();

    switch (static_cast<Type>(cr.checkType())) {

    case Type::TXT :
      std::cout<<".txt detected... starting compression!\n";
      compressFile(src_path_str,
                   src_path.parent_path().string());
      std::cout<<"Ending compression!\n";
      break;

    case Type::PNG :
      std::cout<<".png detected... starting compression!\n";
      //handel the case
        break;

    case Type::JPG :
      std::cout<<".jpg detected... starting compression!\n";
      //handel the case
        break;

    case Type::BMP :
      std::cout<<".bmp detected... starting compression!\n";
      bmpToPng(src_path_str,
               src_path.replace_extension("png").string());
        break;

      case Type::TIFF :
      std::cout<<".tiff detected... starting compression!\n";
      compressTiff(src_path_str,
                   src_path.parent_path().string() +
                   "/" + (src_path.stem().string() + "_DEF.tif"));
        break;

    case Type::PSD :
      std::cout<<".psd detected... starting compression!\n";
        compressPSD(src_path_str,
                    src_path.parent_path().string() +
                    "/" + (src_path.stem().string() + "_DEF.psd"));
        break;

    case Type::LOG :
      std::cout<<".log detected... starting compression!\n";
      compressLOG(src_path_str,
                  src_path.replace_extension(".log.gz").string());
        break;

    case Type::CSV :
      std::cout<<".csv detected... starting compression!\n";
      compressCSV(src_path_str,
                  src_path.replace_extension(".csv.gz").string());
        break;

    case Type::WAV :
      std::cout<<".wav detected... starting compression!\n";
      compressWavToMp3(src_path_str,
                       src_path.replace_extension(".mp3").string(),
                       192,
                       2,
                       printProgress
        );
    std::cout<<"\nEnding compression!\n";
        break;

    case Type::XML :
      std::cout<<".xml detected... starting compression!\n";
      compressXML(src_path_str,
                 src_path.replace_extension(".xml.gz").string());
        break;
    case Type::AIFF :
      std::cout<<".aiff detected... starting compression!\n";
      compressAifftoMp3(src_path_str,
                        src_path.replace_extension(".mp3").string(),
                        192,
                        2,
                        printProgress
        );
      std::cout<<"\nEnding compression!\n";
        break;

    case Type::JSON :
      std::cout<<".json detected... starting compression!\n";
      compressLOG(src_path_str,
                  src_path.replace_extension(".json.gz").string());
        break;

    case Type::TAR :
      std::cout<<".tar detected... starting compression!\n";
      //handel the case
        break;

    case Type::VHD :
      std::cout<<".vhd detected... starting compression!\n";
      //handel the case
        break;

    case Type::NO_EXIST :
      std::cout<<"File does not exist\n";
      //handel the case
        break;

    default:
      std::cout<<"Unknown file_format\n";
        break;
  }

}
