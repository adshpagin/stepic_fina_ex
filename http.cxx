#include <iostream>
#include "http.hxx"


HTTP::Request::Request(const std::string &str) 
{
  std::string firstLine = str.substr(0, str.find_first_of("\n"));

  std::size_t pathPos = firstLine.find_first_of(" ");
  std::size_t protoVersPos = firstLine.find_first_of(" ", pathPos + 1);
  std::size_t paramsPos = firstLine.find_first_of("?", pathPos + 1);

  std::string methStr = firstLine.substr(0, pathPos);
  if (methStr == "GET")
    m_method = HTTP::Method::GET;
  else if (methStr == "POST") 
    m_method = HTTP::Method::POST;
  else
    m_method = HTTP::Method::UNSUPPORTED;

  if (paramsPos == std::string::npos) {
    m_path = firstLine.substr(pathPos, protoVersPos - pathPos);
  } else { 
    m_path = firstLine.substr(pathPos, paramsPos - pathPos);
    m_params = firstLine.substr(paramsPos + 1, protoVersPos - paramsPos - 1);
  }
}
