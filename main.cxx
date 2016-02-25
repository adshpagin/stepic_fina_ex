#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include "tcp.hxx"
#include "http.hxx"

class Logic : public HTTP::Server::RequestHandler {
public:
  virtual void onRequest(const HTTP::Request &req, HTTP::Response &resp);
};

void Logic::onRequest(const HTTP::Request &req, HTTP::Response &resp) {

  resp.m_code = HTTP::StatusCode::OK_200;

  resp.m_body << "<html>" << std::endl; 
  resp.m_body << "<body>" << std::endl; 
  resp.m_body << "<h1>Test web page!</h1>" << std::endl; 
  resp.m_body << "</body>" << std::endl; 
  resp.m_body << "</html>" << std::endl; 
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

  HTTP::Server::RequestHandlerPtr logic = HTTP::Server::RequestHandlerPtr(new Logic);
  TCP::Server::RequestHandlerPtr httpServer = TCP::Server::RequestHandlerPtr(new HTTP::Server(logic));
  TCP::Server tcpServer(httpServer);
  
  // EchoHandler *h = new EchoHandler();
  // TCP::TCPServer::RequestHandlerPtr h_ptr = TCP::TCPServer::RequestHandlerPtr(h);

  // TCP::TCPServer tcpServer(h_ptr);
  
  // std::stringstream ss;
  // ss << "GET /path/file.html?param1=123&param2=456 HTTP/1.0" << std::endl;
  // // ss << "GET /path/file.html HTTP/1.0" << std::endl;
  // ss << "From: someuser@jmarshall.com" << std::endl;
  // ss << "User-Agent: HTTPTool/1.0" << std::endl << std::endl;

  // HTTP::Request req1(ss.str());

  // std::cout << "Meth: " << (int)req1.m_method << "|" << std::endl;
  // std::cout << "Path: " << req1.m_path << "|" << std::endl;
  // std::cout << "Params: " << req1.m_params << "|" << std::endl;

  return 0;
}
