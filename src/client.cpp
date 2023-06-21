#include "include/util.h"
#include "include/Socket.h"
#include "include/mes_process.hpp"
#include <string.h>

#define BUFFER_SIZE 1024 

void blocking_loop(int sockfd)
{
    printf("Client working in blocking mode ...\n");
    while(true){
        char buf[BUFFER_SIZE];  
        bzero(&buf, sizeof(buf));
        fgets(buf, BUFFER_SIZE, stdin);
        mes_process s_message(buf, 0);
        
        bzero(&buf, sizeof(buf));
        memcpy(buf, (char *)s_message.get_message(), sizeof(*(s_message.get_message())));
        //printf("The message sent is %s\n", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0){
            rep_process r_message(buf);
            printf("Reply from server: %d %s\n", r_message.rec_code(), r_message.rec_val());
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            close(sockfd);
            errif(true, "socket read error");
        }
    }
}

void nonblocking_loop(int sockfd, mes_process *message)
{
    char buf[BUFFER_SIZE];  
    bzero(&buf, sizeof(buf));
    memcpy(buf, message->get_message(), BUF_SIZE);
    ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
    if(write_bytes == -1){
        printf("socket already disconnected, can't write any more!\n");
        delete message;
        exit(1);
    }

    bzero(&buf, sizeof(buf));
    ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
    if(read_bytes > 0){
        rep_process r_message = rep_process(buf);
        traceEvent(TRACE_LEVEL, TRACE_LEVEL_NORMAL, INFO, "Reply from server: %d %s", r_message.rec_code(), r_message.rec_val());
    }
}

int main(int argc, char *argv[]) {

    int blocking = 0;
    mes_process *i_message;
   try
   {
      i_message = new mes_process(argc, argv, &blocking);
   }
   catch(const RC &e)
   {
      traceEvent(TRACE_LEVEL, TRACE_LEVEL_ERROR, INFO, log_msg[(int)e]/*"Missing arguments, exit..."*/);
      exit(RC_MISS_ARGS);
   }

    Socket client;
    client.connect();
    int sockfd = client.getFd();
    
    if(blocking)
        blocking_loop(sockfd);
    else
        nonblocking_loop(sockfd, i_message);
    
    delete i_message;
    close(sockfd);
    return 0;
}
