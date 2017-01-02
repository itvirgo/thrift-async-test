#include "client.h"

using Client = ::thrift_test::Client;
using ClientServer = ::thrift_test::ClientServer;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

static void usage() {
    std::cerr << "Usage : client {start|stop}\n";
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage();
    return -1;
  }
  Client client{RPC1_PORT};
  client.open();
  if (!strcmp("start", argv[1]))
    client.start();
  else if (!strcmp("stop", argv[1]))
    client.stop();
  else {
    usage();
    client.close();
    return -1;
  }

  shared_ptr<ClientServer> handler{new ClientServer()};
  shared_ptr<TProcessor> processor{new ::thrift_test::Rpc2Processor(handler)};
  processor->process(client.getInputProtocol(), client.getOutputProtocol(),
                    nullptr); // wait for start ack
  client.close();
  return 0;
}

namespace thrift_test {

//using TException = ::apache::thrift::TException;

void Client::start() {
  std::cout << "Client::start\n";
  client.start();
}

void Client::stop() {
  std::cout << "Client::stop\n";
  client.stop();
}

} // namespace
