#include "include/util.h"
#include "include/Socket.h"
#include "include/mes_process.hpp"
#include <string.h>

#define BUFFER_SIZE 1024 

int main(/*int argc, char *argv[]*/) {
    Socket client;
    client.connect();
    int sockfd = client.getFd();
    
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
    close(sockfd);
    return 0;
}
