#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

int main(int argc, char **argv) {

  std::string ip;
  unsigned port = 0;
  std::string files_folder;
  int rez = 0;

  while ( (rez = getopt(argc,argv,"h:p:d:") ) != -1) {
    switch (rez){
    case 'h': 
      ip = std::string(optarg);
      break;
    case 'p': 
      port = std::stoi(optarg);
      break;
    case 'd': 
      files_folder = std::string(optarg);
      break;
    case '?': std::cout << "ERROR!!" << std::endl; 
      break;
    };
  };

  std::cout << "Host: " << ip << ":" << port << ", files folder: " << files_folder << std::endl;

  return 0;
}
