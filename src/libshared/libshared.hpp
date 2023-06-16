#ifndef LIB_SHARED_
#define LIB_SHARED_
#include <string>
#include <iostream>
#include <fstream>
#include <cstring> 
#include <map>
//include <sys/ipc.h>
//include <sys/shm.h>
#include <memory.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>


unsigned short load_resource(const std::string path);
unsigned short get_value(const std::string& key,std::string& value);
unsigned short set_value(const std::string& key, const std::string &value);

#endif