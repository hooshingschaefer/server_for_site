//Hooshing Schaefer 2018
#include <cstring>
#include <vector>
#include <sys/wait.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>

using namespace std;

//#include "protocol.h"


int main(int argc, char** argv){
   string port {"6969"};
   //if (argc  < 2 ) cout << "must specify host" << endl;
   if (argc == 3 ) port = (argv[2]);
   //string str = argv[1];
   int sock_fd;
   //if (sock_fd = socket(AF_INET, SOCK_STREAM, 0) == -1){
     // exit(1);
  // }
   string hostn = "localhost";
   struct addrinfo hints, *servinfo, *p;
   memset(&hints, 0 , sizeof hints);
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
    
   if ((getaddrinfo(hostn.c_str(), port.c_str() , &hints, &servinfo)) != 0) {
      return 1;
   }


   for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sock_fd = socket(p->ai_family, p->ai_socktype,
                           p->ai_protocol)) == -1) {
         perror("client: socket");
         continue;
      }
      if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sock_fd);
          perror("client: connect");
          continue;
       }
  
      break;
   }
   cout << "connected" << endl;
   freeaddrinfo(servinfo); // all done with this structure
   if (p == NULL) {
      exit(1);
   }

   char buf [16];
   recv(sock_fd, buf, 15, 0);
   buf[15] = '\0';
   //cout << buf << endl;
   printf("%s\n", buf);
   close(sock_fd);
/*
   header_s header; 
   header.len = 0;
   header.command = command_type::GET;
   cout << "sending header" <<endl;
   send_packets(sock_fd, &header, sizeof header);
   recv_packets(sock_fd, &header, sizeof header);
   cout << "received reply" << header.len << endl;
   char buf[header.len];
   recv_packets(sock_fd, &buf, header.len);
   buf[header.len-1] = '\0';
   //string s (&buf[0]);
   for (int i =0; i < 16; i++){
       cout << +buf[i] << endl;
   } 
   cout << "eh" << endl;
   //delete[] buf;*/
}






