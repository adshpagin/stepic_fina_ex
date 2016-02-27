#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <unistd.h>

#include "log.hxx"
#include "tcp.hxx"
#include "http.hxx"
#include "logic.hxx"

void networkLoop(const std::string &ip, unsigned port, const std::string &filesFolder)
{
  HTTP::Server::RequestHandlerPtr logic = HTTP::Server::RequestHandlerPtr(new Logic::RequestHandler);
  TCP::Server::RequestHandlerPtr httpServer = TCP::Server::RequestHandlerPtr(new HTTP::Server(filesFolder, logic));
  TCP::Server tcpServer(ip, port, httpServer);
}

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

  daemon(1, 0);
  
  g_log.write("Server has been started: ");
  g_log.write("Host: " + ip);
  g_log.write("Port: " + std::to_string(port));
  g_log.write("Static dir: " + filesFolder);

  std::thread networkThread(networkLoop, ip, port, filesFolder);
  networkThread.join();

  return 0;
}
