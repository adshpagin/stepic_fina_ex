#ifndef TCP_HXX
#define TCP_HXX

#include <memory>
#include <sstream>
#include <vector>

namespace TCP {

  typedef std::stringstream MsgData;

  class TCPServerImpl;

  class TCPServer {
  public:
    class RequestHandler {
    public:
      virtual void onRequest(const MsgData &req, MsgData &resp) {};
    };
    typedef std::shared_ptr<RequestHandler> RequestHandlerPtr; 

    TCPServer(RequestHandlerPtr handler);
    ~TCPServer();
  private:
    typedef std::unique_ptr<TCPServerImpl> TCPServerImplPtr;
    TCPServerImplPtr m_tcpServerImpl;
  };

}

#endif
