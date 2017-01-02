include "rpc_shared.thrift"

namespace cpp thrift_test

service Rpc1 {
  // start/stop processing
  oneway void start(),
  oneway void stop(),
}
