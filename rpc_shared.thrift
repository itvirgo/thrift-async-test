namespace cpp thrift_test

//
// Exception
//
enum ExceptionReason {
  CONN_RPC1_FAIL = 1,
  CONN_RPC2_FAIL = 2,
  RUNNING = 3,
  STOPPED = 4,
  LOADED = 5,
  UNLOADED = 6,
  LOAD_FAIL = 7,
  UNLOAD_FAIL = 8,
  FILE_TRANSFER = 9,
  INVALID_PARAM = 10,
  INTERNAL = 11,
}

exception InvalidOperation {
  1: ExceptionReason reason,
  2: string descr
}
