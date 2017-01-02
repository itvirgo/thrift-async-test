cmake_minimum_required(VERSION 3.0)
project(thrift-test)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.5")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  endif()
endif()
if(NOT CMAKE_CXX_FLAGS_DEBUG MATCHES "-O")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif()
set(CMAKE_CXX_FLAGS_RELEASE
  "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native")
if(NOT CMAKE_CXX_FLAGS_RELEASE MATCHES "-O")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# the files in gen-cpp folder has too many warnings
# maybe due to being automatically generated
# with these many disabled there are still some warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-extra-semi")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-documentation")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-covered-switch-default")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unreachable-code")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undef")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-disabled-macro-expansion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-variable-declarations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum")

find_package(Boost 1.55.0 REQUIRED
  COMPONENTS system thread program_options)
find_package(Threads)
find_library(THRIFT_LIB NAMES thrift)
find_program(THRIFT_COMPILER NAMES thrift)
#find_library(EDITLINE_LIB NAMES edit)

include_directories(SYSTEM ${CMAKE_INSTALL_PREFIX}/include)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${LUA_INCLUDE_DIR})
include_directories(${CMAKE_BINARY_DIR})

# Compile time options for some config parameters
set(RPC1_PORT 9090)
set(RPC2_PORT 9091)
configure_file(
  "${CMAKE_SOURCE_DIR}/RpcConfig.h.in"
  "${CMAKE_BINARY_DIR}/RpcConfig.h"
  )

set(RPC1_GENCPP_SOURCES
  gen-cpp-rpc1/Rpc1.cpp
  gen-cpp-rpc1/rpc1_constants.cpp
  gen-cpp-rpc1/rpc1_types.cpp
  gen-cpp-rpc1/rpc_shared_constants.cpp
  gen-cpp-rpc1/rpc_shared_types.cpp
)
add_library(rpc1_gencpp STATIC ${RPC1_GENCPP_SOURCES})

set(RPC2_GENCPP_SOURCES
  gen-cpp-rpc2/Rpc2.cpp
  gen-cpp-rpc2/rpc2_constants.cpp
  gen-cpp-rpc2/rpc2_types.cpp
  gen-cpp-rpc2/rpc_shared_constants.cpp
  gen-cpp-rpc2/rpc_shared_types.cpp
)
add_library(rpc2_gencpp STATIC ${RPC2_GENCPP_SOURCES})

add_custom_command(OUTPUT
  gen-cpp-rpc1/Rpc1.cpp gen-cpp-rpc1/rpc1_constants.cpp
  gen-cpp-rpc1/rpc1_types.cpp gen-cpp-rpc1/rpc_shared_constants.cpp
  gen-cpp-rpc1/rpc_shared_types.cpp
  COMMAND mkdir -p gen-cpp-rpc1
  COMMAND ${THRIFT_COMPILER} --gen cpp -r -out gen-cpp-rpc1 ${CMAKE_SOURCE_DIR}/rpc1.thrift
  DEPENDS ${CMAKE_SOURCE_DIR}/rpc1.thrift ${CMAKE_SOURCE_DIR}/rpc_shared.thrift
)

add_custom_command(OUTPUT
  gen-cpp-rpc2/Rpc2.cpp gen-cpp-rpc2/rpc2_constants.cpp
  gen-cpp-rpc2/rpc2_types.cpp gen-cpp-rpc2/rpc_shared_constants.cpp
  gen-cpp-rpc2/rpc_shared_types.cpp
  COMMAND mkdir -p gen-cpp-rpc2
  COMMAND ${THRIFT_COMPILER} --gen cpp -r -out gen-cpp-rpc2 ${CMAKE_SOURCE_DIR}/rpc2.thrift
  DEPENDS ${CMAKE_SOURCE_DIR}/rpc2.thrift ${CMAKE_SOURCE_DIR}/rpc_shared.thrift
)

# client (Rpc1 client + Rpc2 server?)
add_executable(client client.cpp)
target_link_libraries(client rpc1_gencpp rpc2_gencpp
  ${THRIFT_LIB} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

# server (Rpc1 server + Rpc2 client?)
add_executable(server server.cpp)
target_link_libraries(server rpc1_gencpp rpc2_gencpp
  ${THRIFT_LIB} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
