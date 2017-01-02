#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H

#include <cstdlib>
#include "RpcConfig.h"
#include "gen-cpp-rpc1/Rpc1.h"
#include "gen-cpp-rpc2/Rpc2.h"
#include "gen-cpp-rpc1/rpc_shared_constants.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>

namespace thrift_test {

class Client : public Rpc1If {
  using TTransport = ::apache::thrift::transport::TTransport;
  using TProtocol = ::apache::thrift::protocol::TProtocol;
  using TSocket = ::apache::thrift::transport::TSocket;
  using TBufferedTransport = ::apache::thrift::transport::TBufferedTransport;
  using TBinaryProtocol = ::apache::thrift::protocol::TBinaryProtocol;
public:
  Client(int p)
    : socket(new TSocket("localhost", p)),
      transport(new TBufferedTransport(socket)),
      protocol(new TBinaryProtocol(transport)), client(protocol) {}
  ~Client() = default;


  void open() { // throws TException
    transport->open();
  }
  void close() { // throws TException
    transport->close();
  }

  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return client.getInputProtocol();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return client.getOutputProtocol();
  }

  //
  // thrift_test::Rpc1If
  //
  virtual void start();
  virtual void stop();
private:
  boost::shared_ptr<TTransport> socket;
  boost::shared_ptr<TTransport> transport;
  boost::shared_ptr<TProtocol> protocol;
  Rpc1Client client;
};

class ClientServer : public Rpc2If {
public:
  ClientServer() = default;
  ~ClientServer() = default;

  //
  // Rpc2If
  //
  void ack_start(int32_t success) {
    std::cout << "ClientServer ack_start " << success << "\n";
  }
  void ack_stop(int32_t success) {
    std::cout << "ClientServer ack_stop " << success << "\n";
  }
};

} // namespace
#endif
