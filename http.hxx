#include <string>

namespace HTTP {

  enum class Method {
    UNSUPPORTED = 0,
    GET,
    POST
  };

  //class Server : public TCP::

  struct Request {
    Request(const std::string &str);
    Method      m_method;
    std::string m_path;
    std::string m_params;
  };

}
