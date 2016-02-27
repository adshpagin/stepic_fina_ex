#include <iostream>
#include "log.hxx"
#include "logic.hxx"

void Logic::RequestHandler::onRequest(const HTTP::Request &req, HTTP::Response &resp) {

  g_log.write("Logic: request received, id: " + std::to_string(req.m_id));

  resp.m_code = HTTP::StatusCode::OK_200;

  std::string filePath = "/tmp" + req.m_path;
  std::ifstream f(filePath);

  if (f.good()) {
    g_log.write("Logic: file exists, retrun it in response: " + filePath);
    resp.m_code = HTTP::StatusCode::OK_200;
    resp.m_body << f.rdbuf();
  } else {
    g_log.write("Logic: file not found, retrun 404 in response: " + filePath);
    resp.m_code = HTTP::StatusCode::NOT_FOUND_404;

    resp.m_body << "<html>" << std::endl;
    resp.m_body << "<head>" << std::endl;
    resp.m_body << "<title>Not Found</title>" << std::endl;
    resp.m_body << "</head>" << std::endl;
    resp.m_body << "<body>" << std::endl;
    resp.m_body << "<p>404 Request file not found.</p>" << std::endl;
    resp.m_body << "</body>" << std::endl;
    resp.m_body << "</html>" << std::endl;
  }

}
