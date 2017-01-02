include "rpc_shared.thrift"

namespace cpp thrift_test

service Rpc2 {
  // start/stop processing
  oneway void ack_start(1:i32 success),
  oneway void ack_stop(1:i32 success),
}
