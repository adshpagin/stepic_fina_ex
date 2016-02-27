#ifndef LOGIC_HXX
#define LOGIC_HXX

#include "http.hxx"

namespace Logic {

class RequestHandler : public HTTP::Server::RequestHandler {
public:
  virtual void onRequest(const HTTP::Request &req, HTTP::Response &resp);
};

}

#endif
