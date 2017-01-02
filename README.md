# A simple Apache Thrift asynchronous two-way communication example in C++



This repository contains simple code example for proof of concept
implementation of two-way asynchronous communication between two ends.

As far as I know Apache Thrift doesn't support this concept out of the box, i.e. in the basic set up that Thrift compiler provides there is no way for server side to send notification asynchronously to the client side.

In this repository of code example, I investigated how we can support such functionalities by utilizing some inner (and somewhat hidden) features of Apache Thrift.




