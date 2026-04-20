# TCP BroadCasting Room

A simple multi-client TCP chat room in C++ using POSIX sockets and threads.

## Project Structure

- src/server.cpp: TCP server that accepts clients and broadcasts messages.
- src/client.cpp: TCP client with send and receive threads.
- CMakeLists.txt: Build configuration for server and client targets.

## Requirements

- Linux
- CMake 3.10+
- GCC/G++ with C++17 support
- pthread support

## Build (CMake)

From the project root:

1. cmake -S . -B build
2. cmake --build build

This creates:

- build/server
- build/client

## Run

Open one terminal for server:

1. ./build/server

Open one or more terminals for clients:

1. ./build/client

## How To Use

1. Start the server first.
2. Start a client.
3. The first message sent by each client is treated as the username by the server.
4. After that, each message is broadcast to other connected clients in this format:
   [username]: message

## Common Commands

Rebuild:

1. cmake --build build

Clean build folder:

1. rm -rf build
2. cmake -S . -B build
3. cmake --build build

## Notes

- Server listens on port 8080.
- The project includes a .gitignore configured for CMake and local build artifacts.
