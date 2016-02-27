#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include "log.hxx"
#include "tcp.hxx"
#include "http.hxx"
#include "logic.hxx"

int main(int argc, char **argv) {

  std::string ip;
  unsigned port = 0;
  std::string filesFolder;
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
      filesFolder = std::string(optarg);
      break;
    case '?': std::cout << "ERROR!!" << std::endl; 
      break;
    };
  };
  
  g_log.write("Server has been started: ");
  g_log.write("Host: " + ip);
  g_log.write("Port: " + std::to_string(port));
  g_log.write("Static dir: " + filesFolder);

  HTTP::Server::RequestHandlerPtr logic = HTTP::Server::RequestHandlerPtr(new Logic::RequestHandler);
  TCP::Server::RequestHandlerPtr httpServer = TCP::Server::RequestHandlerPtr(new HTTP::Server(filesFolder, logic));
  TCP::Server tcpServer(ip, port, httpServer);

  return 0;
}
