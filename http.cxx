#include <iostream>
#include <ctime>

#include "log.hxx"
#include "http.hxx"

HTTP::Request::Request(const std::string &str, unsigned id) 
  : m_id(id)
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
    m_path = firstLine.substr(pathPos+1, protoVersPos - pathPos - 1);
  } else { 
    m_path = firstLine.substr(pathPos+1, paramsPos - pathPos - 1);
    m_params = firstLine.substr(paramsPos + 1, protoVersPos - paramsPos - 1);
  }
}

HTTP::Response::Response()
{}

HTTP::Server::Server(RequestHandlerPtr handler)
  : m_handler(handler)
{}

void HTTP::Server::onRequest(const TCP::MsgData &req, unsigned reqID, TCP::MsgData &resp)
{
  HTTP::Request request(req.str(), reqID);

  g_log.write("HTTP: request id " + std::to_string(reqID) + " received and parsed: ");
  if (request.m_method == HTTP::Method::GET)
    g_log.write("Method: GET");
  else if (request.m_method == HTTP::Method::POST)
    g_log.write("Method: POST");

  g_log.write("Path: " + request.m_path);
  g_log.write("Params: " + request.m_params);

  HTTP::Response response;
  m_handler->onRequest(request, response);
  
  resp << "HTTP/1.0 ";

  if (response.m_code == HTTP::StatusCode::OK_200)
    resp << "200 OK"<< std::endl;
  else if (response.m_code == HTTP::StatusCode::NOT_FOUND_404)
    resp << "404 Not Found"<< std::endl;


  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  char buffer[256];

  std::strftime (buffer, 256,"Date: %a, %d %b %G %T %Z", &tm);

  resp << buffer << std::endl;

  resp << "Content-Type: text/html" << std::endl;

  response.m_body.seekg(0, std::ios::end);
  int len = response.m_body.tellg();
  response.m_body.seekg(0, std::ios::beg);

  resp << "Content-Length: " << len << std::endl << std::endl;

  g_log.write("HTTP: Response id " + std::to_string(reqID) + ": ");
  g_log.write(resp.str());
  g_log.write("< body >");

  resp << response.m_body.str() << std::endl << std::endl;
};
