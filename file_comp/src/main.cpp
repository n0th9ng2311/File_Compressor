
#include "../hdr/all_comp_algo.h"
#include "../hdr/file_comp.h"


enum class Type;

int main() {

  std::cout << "================================================" << "\n";
  std::cout << "||        FILE COMPRESSOR v1.0              ||" << "\n";
  std::cout << "================================================" << "\n";
  std::cout << "||  Maximize Storage * Minimize File Size   ||" << "\n";
  std::cout << "||     Lightning-Fast Compression Tool      ||" << "\n";
  std::cout << "================================================" << "\n\n";
  std::cout << ">>> Ready to compress your files efficiently! <<<" << "\n\n";

  File_compressor cr{};
  cr.choice();

  switch (static_cast<Type>(cr.checkType())) {

    case Type::TXT :
      std::cout<<".txt detected... starting compression!\n";
      compressFile(cr.getSrc_path().string(), "D:/"); //arbitrary outfile location for now
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
      bmpToPng(cr.getSrc_path().string(), "D:/sample.png");
        break;

    case Type::TIFF :
      std::cout<<".tiff detected... starting compression!\n";
      //handel the case
        break;

    case Type::PSD :
      std::cout<<".psd detected... starting compression!\n";
      //handel the case
        break;

    case Type::LOG :
    std::cout<<".log detected... starting compression!\n";
      //handel the case
        break;

    case Type::CSV :
      std::cout<<".csv detected... starting compression!\n";
      //handel the case
        break;

    case Type::WAV :
      std::cout<<".wav detected... starting compression!\n";
      compressWavToMp3(
            cr.getSrc_path().string(),
            "D:/output.mp3",
            192,
            2,
            printProgress
        );
    std::cout<<"\nEnding compression!\n";
        break;

    case Type::FLAC :
      std::cout<<".flac detected... starting compression!\n";
      //handel the case
        break;
    case Type::AIFF :
      std::cout<<".aiff detected... starting compression!\n";
    compressAifftoMp3(
            cr.getSrc_path().string(),
            "D:/output.mp3",
            192,
            2,
            printProgress
        );
      std::cout<<"\nEnding compression!\n";
        break;

    case Type::ISO :
      std::cout<<".iso detected... starting compression!\n";
      //handel the case
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
