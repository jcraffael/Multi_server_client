# Multi_server_client

This is a new project based on my previous Unix server client model https://github.com/jcraffael/unix_socket_ser_cli
and on the open source project https://github.com/yuesong-feng/30dayMakeCppServer
The server is a multiplexing one using epoll to manage multi client connections. At the moment the client process works ONLY in blocking mode waiting for user input (pls refer to my previous project for user input rules).

Next steps:
○ Uniformat logging messages
○ Add non-blocking mode client feature
○ Continuous code refactoring
○ Use of CMake
○ Integrate clang-format, clang-tidy and other tools
