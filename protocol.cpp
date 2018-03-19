#include "protocol.h"
void send_packets(int fd,  void* buffer, int len){
   char* buf = static_cast<char*>( buffer);
   int stat;
   int sent = 0;
   while (sent < len ){
      stat = send(fd, buf+sent, len-sent,0);
      if (stat == -1 ) exit(1);
      else{
         sent+=stat;
      }
   }

} 

void recv_packets(int fd, void* buffer, int len){
   char* buf = static_cast<char*>(buffer); 
   int stat;
   int recvd = 0;
   while (recvd < len  ){
      stat = recv(fd, buf+recvd, len-recvd, 0);
      if (stat == -1 ){
         exit(1);
      }else{
         recvd+=stat;
      }
   }
}


