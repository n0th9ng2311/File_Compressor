
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

  File_compressor file_compressor;

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

  file_compressor.choice();

  compress_switch(file_compressor);

  return 0;
}




