#include <iostream>
#include <memory>
#include <thread>
#include <uv.h>
#include "tcp.hxx"

void uvConnectionCB(uv_stream_t *, int);
void readCB(uv_stream_t *, ssize_t, const uv_buf_t *);
void after_work(uv_work_t *, int);
void worker_proc(uv_work_t *);
void writeCB(uv_write_t*, int);

namespace TCP {

  class ServerImpl {
  public:
    ServerImpl(Server::RequestHandlerPtr handler);
    void log(const std::string &msg);
  private:

    class Request {
    public:
      Request(const char *buf,
              unsigned len,
              uv_stream_t *stream,
              Server::RequestHandlerPtr handler);
      void callHandler();
    private:
      MsgData      m_requestData;
      MsgData      m_responseData;
      uv_stream_t *m_stream;
      char        *m_bufBase;

      Server::RequestHandlerPtr m_handler;
      
      friend void ::after_work(uv_work_t *, int);
      friend void ::writeCB(uv_write_t*, int);
    };

    uv_loop_t *m_loop;
    uv_tcp_t   m_server;

    Server::RequestHandlerPtr m_handler;

    friend void ::uvConnectionCB(uv_stream_t *, int);
    friend void ::readCB(uv_stream_t *, ssize_t, const uv_buf_t *);
    friend void ::worker_proc(uv_work_t *);
    friend void ::after_work(uv_work_t *, int);
    friend void ::writeCB(uv_write_t*, int);
  };

}

void worker_proc(uv_work_t *req) {
  TCP::ServerImpl::Request *reqObj = static_cast<TCP::ServerImpl::Request *>(req->data);
  reqObj->callHandler();
}

void after_work(uv_work_t *req, int status) {
  TCP::ServerImpl::Request *reqObj = static_cast<TCP::ServerImpl::Request *>(req->data);

  reqObj->m_responseData.seekg(0, std::ios::end);
  int size = reqObj->m_responseData.tellg();
  reqObj->m_responseData.seekg(0, std::ios::beg);
  if (size <= 0)
    return;

  reqObj->m_bufBase = (char*) malloc(size);
  reqObj->m_responseData.read(reqObj->m_bufBase, size);

  uv_buf_t buf[1];
  buf[0] = uv_buf_init(reqObj->m_bufBase, size);

  uv_write_t *w_req = (uv_write_t*) malloc(sizeof(uv_write_t));
  w_req->data = (void*)reqObj;

  uv_write(w_req, reqObj->m_stream, buf, 1, writeCB);
}

void writeCB(uv_write_t* req, int status) {
  // TODO: check status
  TCP::ServerImpl::Request *reqObj = static_cast<TCP::ServerImpl::Request *>(req->data);
  std::cout << "closing stream" << std::endl;
  uv_close((uv_handle_t*)(reqObj->m_stream), NULL);
  free(reqObj->m_bufBase);
  delete reqObj;
  free(req);
}

void readCB(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {

  if (nread == UV_EOF) {
    std::cout << "readCB: EOF reached" << std::endl;
    return;
  }

  if (nread == 0) {
    std::cout << "readCB: nread == 0, it means EAGAIN" << std::endl;
    free(buf->base);
    return;
  }
    
  if (nread < 0) {
    std::cout << "readCB: error has occured, nread == " << nread << std::endl;
    free(buf->base);
    return;
  }  

  TCP::ServerImpl* srv = static_cast<TCP::ServerImpl *>(stream->data);

  // std::cout << "Buf len: " << buf->len << std::endl;
  // std::cout << "Actually read: " << nread << std::endl;

  TCP::ServerImpl::Request *reqObj = new TCP::ServerImpl::Request(buf->base, nread, stream, srv->m_handler);
  free(buf->base);
  
  uv_work_t req;
  req.data = (void*)reqObj;

  uv_queue_work(srv->m_loop, &req, worker_proc, after_work);
}

void allocBuffer(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  *buf = uv_buf_init((char*) malloc(size), size);
}

void uvConnectionCB(uv_stream_t *server, int status)
{
  if (status < 0) {
    std::cerr << "New connection error: " << uv_strerror(status) << std::endl;
    return;
  }

  TCP::ServerImpl* srv = static_cast<TCP::ServerImpl *>(server->data);

  uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  uv_tcp_init(srv->m_loop, client);
  if (uv_accept(server, (uv_stream_t*) client) == 0) {
    client->data = static_cast<void *>(srv);
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "new connection is established. Thr_id: " << this_id << std::endl;
    uv_read_start((uv_stream_t*) client, allocBuffer, readCB);
  }
  else {
    uv_close((uv_handle_t*) client, NULL);
  }
}

TCP::Server::Server(TCP::Server::RequestHandlerPtr handler)
  : m_tcpServerImpl(new ServerImpl(handler))
{}

TCP::Server::~Server()
{}


TCP::ServerImpl::Request::Request(const char *buf, unsigned len, uv_stream_t *stream, Server::RequestHandlerPtr handler) 
  : m_stream(stream)
  , m_handler(handler)
{
  m_requestData.write(buf, len);
}

void TCP::ServerImpl::Request::callHandler()
{
  this->m_handler->onRequest(this->m_requestData, this->m_responseData);
}

TCP::ServerImpl::ServerImpl(TCP::Server::RequestHandlerPtr handler)
  : m_loop(nullptr)
  ,  m_handler(handler)
{
  m_loop = uv_default_loop();
  uv_tcp_init(m_loop, &m_server);

  m_server.data = static_cast<void *>(this);

  struct sockaddr_in addr;
  uv_ip4_addr("127.0.0.1", 8081, &addr);

  uv_tcp_bind(&m_server, (const struct sockaddr*)&addr, 0);

  int r = uv_listen((uv_stream_t*) &m_server, 128, uvConnectionCB);
  if (r) {
    std::cerr << "Listen error: " << uv_strerror(r) << std::endl;
  }

  uv_run(m_loop, UV_RUN_DEFAULT);
}

void TCP::ServerImpl::log(const std::string &msg) {
  std::cout << "ServerImpl: " << msg << std::endl;
}
