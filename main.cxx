#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include "tcp.hxx"

class EchoHandler : public TCP::TCPServer::RequestHandler {
public:
  virtual void onRequest(const TCP::MsgData &req, TCP::MsgData &resp);
};

void EchoHandler::onRequest(const TCP::MsgData &req, TCP::MsgData &resp) {
  std::cout << "EchoHandler::onRequest. Msg: " << req.str() << std::endl;
  resp << "Test message :)" << std::endl;
}

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

  EchoHandler *h = new EchoHandler();
  TCP::TCPServer::RequestHandlerPtr h_ptr = TCP::TCPServer::RequestHandlerPtr(h);

  TCP::TCPServer tcpServer(h_ptr);

  return 0;
}
