#ifndef UTILS_H_
#define UTILS_H_

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <cstring>
#include <getopt.h>

#define INFO __FILE__, __LINE__

inline char log_file_path[] = ""; 
inline int TRACE_LEVEL = 2;
enum{

TRACE_LEVEL_ERROR = 0,
TRACE_LEVEL_WARNING,
TRACE_LEVEL_NORMAL,
TRACE_LEVEL_DEBUG
};

enum RC {
  RC_SUCCESS = 0,
  RC_MISS_ARGS,
  RC_SOCKET_ERROR,
  RC_CONNECTION_ERROR,
  RC_ACCEPT_ERROR,
  RC_RECV_ERROR,
  RC_SENT_ERROR,
  RC_GENERIC = 127
};

inline const char *log_msg[] = {
        "Success\n",
        "Missing arguments\n",
        "Error opening socket\n",
        "Error in connecting...\n",
        "Error in accepting...\n",
        "ERROR receiving from socket\n",
        "ERROR writing to socket\n"
};

void traceEvent(int traceLevel, int eventTraceLevel,
		       const char* file, const int line, const char * format, ...);


const char* const short_opts = "l:g:s:bhv";
const option long_opts[] = {
        {"load", required_argument, nullptr, 'l'},
        {"get", required_argument, nullptr, 'g'},
        {"set", required_argument, nullptr, 's'},
        {"blocking", no_argument, nullptr, 'b'},
        {"help", no_argument, nullptr, 'h'},
        {"verbose", no_argument, nullptr, 'v'},
        {nullptr, no_argument, nullptr, 0}
};

void inline PrintHelp()
{
    puts(
            "--load <path>:        Load a file into the filesystem\n"
            "--get <key>:          Retrieve the value of the specific key in the file\n"
            "--set <key> <val>:    Set value to a specific key\n"
            "-b, --blocking:       Run client in blocking mode\n"
            "--help:               Show help\n"
            "--verbose:            Verbose mode\n");
    
}

#endif