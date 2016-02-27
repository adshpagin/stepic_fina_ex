#include <string>
#include "tcp.hxx"

namespace HTTP {

  enum class Method {
    UNSUPPORTED = 0,
    GET,
    POST
  };

  enum class StatusCode {
    OK_200 = 0,
    NOT_FOUND_404
  };

  struct Request {
    Request(const std::string &str);
    Method      m_method;
    std::string m_path;
    std::string m_params;
  };
  
  struct Response {
    Response();
    std::stringstream m_body;
    StatusCode m_code;
  };


  class Server : public TCP::Server::RequestHandler {
  public:
    virtual void onRequest(const TCP::MsgData &req, TCP::MsgData &resp);

    class RequestHandler {
    public:
      virtual void onRequest(const Request &req, Response &resp) {};
    };
    typedef std::shared_ptr<RequestHandler> RequestHandlerPtr;
    
    Server(RequestHandlerPtr handler);
  private:
    RequestHandlerPtr m_handler;
  };
}