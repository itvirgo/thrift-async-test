#include "server.h"

#include <iostream>
#include <algorithm>
#include <mutex>
#include <atomic>
#include <signal.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/file.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

TThreadedServer *pServer = nullptr;
static void sighandler(int sig) {
  switch (sig) {
  case SIGINT:
  case SIGTERM:
    if (pServer)
      pServer->stop();
    else {
      signal(SIGINT, SIG_DFL);
      signal(SIGTERM, SIG_DFL);
    }
    break;
  case SIGHUP:
    // TODO
    break;
  default:
    break;
  }
}

int main() {
  // set up signal handler (TODO : revisit)
  struct sigaction sa;
  memset(&sa, 0, sizeof(struct sigaction));
  sa.sa_handler = sighandler;
  sa.sa_flags = 0; /* clear SA_RESTART flag */
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGHUP, &sa, NULL);

  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  sigaddset(&sigset, SIGTERM);
  sigaddset(&sigset, SIGHUP);
  // Every threads will inherit the following signal mask
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  int port = RPC1_PORT;
  try {
    /*
    shared_ptr<::thrift_test::Server> handler(new ::thrift_test::Server());
    shared_ptr<TProcessor> processor(new ::thrift_test::Rpc1Processor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(
        new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TThreadedServer server(processor, serverTransport, transportFactory,
                           protocolFactory);
    */
    shared_ptr<::thrift_test::Rpc1IfFactory> handlerFactory(
        new ::thrift_test::ServerHandlerFactory());
    shared_ptr<TProcessorFactory> processorFactory(
        new ::thrift_test::ServerProcessorFactory(handlerFactory));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(
        new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TThreadedServer server(processorFactory, serverTransport, transportFactory,
                           protocolFactory);
    std::cout << "Starting rpc server...\n";
    pServer = &server;
    pthread_sigmask(SIG_UNBLOCK, &sigset, NULL); // only main thread gets signal
    server.serve();
    std::cout << "Done.\n";
  } catch (std::exception &ex) {
    std::cerr << "Server launch fail\n";
    std::cerr << ex.what() << "\n";
  }
  return 0;
}

namespace thrift_test {

void Server::start() {
  static int cnt = 0;
  client.ack_start(++cnt);
  std::cout << "Server::start\n";
}

void Server::stop() {
  static int cnt = 0;
  client.ack_stop(++cnt);
  std::cout << "Server::stop\n";
}

} // namespace
