#include <fstream>
#include <iostream>

#include "image_ppm.hpp"
#include "seam_carver.hpp"

void PrintUsage( const std::string& command ) {
  std::cout << "Usage: " << command << " INPUT_FILE [options] -o OUTPUT_FILE\n"
            << "Options:\n"
            << "\t-h times\tcarve times horizontal seams\n"
            << "\t-v times\tcarve times vertical seams\n";
}

int main( int argc, char* argv[] ) {
  std::string out_option = "-o";
  if ( argc < 4 || out_option != argv[argc - 2] ) {
    PrintUsage( argv[0] );
    return 1;
  }

  std::ifstream infile( argv[1] );
  if ( !infile.good() ) {
    std::cout << "file '" << argv[1] << "' not found" << std::endl;
    return 1;
  }

}
