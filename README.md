# A simple Apache Thrift asynchronous two-way communication example in C++



This repository contains simple code example for proof of concept
implementation of two-way asynchronous communication between two ends.

As far as I know Apache Thrift doesn't support this concept out of the box, i.e. in the basic set up that Thrift compiler provides there is no way for server side to send notification asynchronously to the client side.

In this repository of code example, I investigated how we can support such functionalities by utilizing some inner (and somewhat hidden) features of Apache Thrift.

## How to build

In the terminal, follow the steps below

```
$ cd path-to-src
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Afther following the above steps, you should see binaries `client` and `server` created under the `build` directory.

## How to execute sample client & server

Change directory to the above `build` directory and run the following first to execute server

```
$ ./server
```

In the separate terminal run the client commands as follows.
You should see the outputs something like this.

```
$ ./client start
Client::start
ClientServer ack_start 1
$ ./client stop
Client::stop
ClientServer ack_stop 1
$ ./client start
Client::start
ClientServer ack_start 2
$ ./client stop
Client::stop
ClientServer ack_stop 2
```