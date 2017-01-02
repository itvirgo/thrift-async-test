#ifndef RPC_SERVER_H
#define RPC_SERVER_H

#include <cstdlib>
#include <mutex>

#include "RpcConfig.h"
#include "gen-cpp-rpc1/Rpc1.h"
#include "gen-cpp-rpc2/Rpc2.h"

namespace thrift_test {

class ServerClient : public Rpc2Client {
public:
  ServerClient(::apache::thrift::protocol::TProtocol *iprot,
               ::apache::thrift::protocol::TProtocol *oprot)
      : Rpc2Client(nullptr) {
    // these are protected members of Rpc2Cllient
    // override the values from Rpc2Client ctor
    // (this is an ugly makeshift solution for the moment)
    iprot_ = iprot;
    oprot_ = oprot;
  }
};

class Server : public Rpc1If {
public:
  Server(const ::apache::thrift::TConnectionInfo &conn)
      : iprot(conn.input), oprot(conn.output),
        client(iprot.get(), oprot.get()) {}
  ~Server() = default;

  //
  // Rpc1If
  //
  void start();
  void stop();
private:
  boost::shared_ptr<::apache::thrift::protocol::TProtocol> iprot;
  boost::shared_ptr<::apache::thrift::protocol::TProtocol> oprot;
  ServerClient client;
};

class ServerHandlerFactory : virtual public Rpc1IfFactory {
 public:
  virtual Rpc1If* getHandler(const ::apache::thrift::TConnectionInfo& conn) {
    return new Server(conn);
  }
  virtual void releaseHandler(Rpc1If *handler) { delete handler; }
};

class ServerProcessorFactory : public ::apache::thrift::TProcessorFactory {
public:
  ServerProcessorFactory(const ::boost::shared_ptr<Rpc1IfFactory> &handlerFactory)
      : handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr<::apache::thrift::TProcessor>
  getProcessor(const ::apache::thrift::TConnectionInfo &connInfo) {
    ::apache::thrift::ReleaseHandler<Rpc1IfFactory> cleanup(handlerFactory_);
    ::boost::shared_ptr<Rpc1If> handler(handlerFactory_->getHandler(connInfo),
                                        cleanup);
    ::boost::shared_ptr<::apache::thrift::TProcessor> processor(
        new Rpc1Processor(handler));
    return processor;
  }

protected:
  ::boost::shared_ptr<Rpc1IfFactory> handlerFactory_;
};
} // namespace
#endif
