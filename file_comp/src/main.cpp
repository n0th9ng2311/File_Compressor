
#include "../hdr/all_comp_algo.h"
#include "../hdr/file_comp.h"


enum class Type;

int main(int argc, char* argv[]) {

  std::cout << "================================================" << "\n";
  std::cout << "||        FILE COMPRESSOR v1.0              ||" << "\n";
  std::cout << "================================================" << "\n";
  std::cout << "||  Maximize Storage * Minimize File Size   ||" << "\n";
  std::cout << "||     Lightning-Fast Compression Tool      ||" << "\n";
  std::cout << "================================================" << "\n\n";
  std::cout << ">>> Ready to compress your files efficiently! <<<" << "\n\n";

  File_compressor cr;

  //Working on this feature
  // if (argc>1) {
  //   //Command line mode
  //   std::cout<<"value of parameter at 1: "<<argv[1];
  //
  //   std::string file_path = argv[1];
  //   cr.setSrc(file_path);
  // }
  // else {
  //   //Interactive mode
  //   cr.choice();
  // }

  cr.choice();
  switch (static_cast<Type>(cr.checkType())) {

    case Type::TXT :
      std::cout<<".txt detected... starting compression!\n";
      compressFile(cr.getSrc_path().string(),
          cr.getSrc_path().parent_path().string());
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
      bmpToPng(cr.getSrc_path().string(),
                cr.getSrc_path().replace_extension("png").string());
        break;

      case Type::TIFF :
      std::cout<<".tiff detected... starting compression!\n";
      compressTiff(cr.getSrc_path().string(),
                   cr.getSrc_path().parent_path().string()
                    +"/"+ (cr.getSrc_path().stem().string() + "_DEF.tif"));
        break;

    case Type::PSD :
      std::cout<<".psd detected... starting compression!\n";
      //handel the case
        break;

    case Type::LOG :
      std::cout<<".log detected... starting compression!\n";
      compressLOG(cr.getSrc_path().string(),
                cr.getSrc_path().replace_extension(".log.gz").string());
        break;

    case Type::CSV :
      std::cout<<".csv detected... starting compression!\n";
      compressCSV(cr.getSrc_path().string(),
                  cr.getSrc_path().replace_extension(".csv.gz").string());
        break;

    case Type::WAV :
      std::cout<<".wav detected... starting compression!\n";
      compressWavToMp3(
            cr.getSrc_path().string(),
            cr.getSrc_path().replace_extension(".mp3").string(),
            192,
            2,
            printProgress
        );
    std::cout<<"\nEnding compression!\n";
        break;

    case Type::XML :
      std::cout<<".xml detected... starting compression!\n";
      compressXML(cr.getSrc_path().string(),
                cr.getSrc_path().replace_extension(".xml.gz").string());
        break;
    case Type::AIFF :
      std::cout<<".aiff detected... starting compression!\n";
      compressAifftoMp3(
            cr.getSrc_path().string(),
            cr.getSrc_path().replace_extension(".mp3").string(),
            192,
            2,
            printProgress
        );
      std::cout<<"\nEnding compression!\n";
        break;

    case Type::JSON :
      std::cout<<".json detected... starting compression!\n";
      compressLOG(cr.getSrc_path().string(),
                 cr.getSrc_path().replace_extension(".json.gz").string());
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




