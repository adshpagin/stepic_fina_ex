#ifndef TCP_HXX
#define TCP_HXX

#include <memory>
#include <sstream>
#include <vector>

namespace TCP {

  typedef std::stringstream MsgData;

  class ServerImpl;

  class Server {
  public:
    class RequestHandler {
    public:
      virtual void onRequest(const MsgData &req, unsigned reqID, MsgData &resp) {};
    };
    typedef std::shared_ptr<RequestHandler> RequestHandlerPtr; 

    Server(const std::string &ip, unsigned port, RequestHandlerPtr handler);
    ~Server();
  private:
    typedef std::unique_ptr<ServerImpl> ServerImplPtr;
    ServerImplPtr m_tcpServerImpl;
  };

}

#endif
