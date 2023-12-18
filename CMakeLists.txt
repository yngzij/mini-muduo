cmake_minimum_required(VERSION 3.21)
project(untitled8)

set(CMAKE_CXX_STANDARD 23)

include_directories(/usr/local/include)

# add base  directory
include_directories(base)
include_directories(net)


add_executable(untitled8 main.cpp net/EventLoop.cpp net/EventLoop.h base/nocopyable.h base/CurrentThread.cpp base/CurrentThread.h base/Mutex.h net/Channel.cpp net/Channel.h base/Timestamp.cpp base/Timestamp.h base/copyable.h net/Poller.cpp net/Poller.h net/poller/kqueue.cpp net/poller/kqueue.h base/Types.h net/TcpServer.cpp net/TcpServer.h net/Callback.h net/TcpConnection.cpp net/TcpConnection.h net/Buffer.cpp net/Buffer.h base/SingPiece.cpp base/SingPiece.h net/Callbacks.cpp net/Callbacks.h)
