//Hooshing Schaefer 2018
#include <string>
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "protocol.h"
using namespace std;

void sigchld_handler(int ){
   while (wait(0) > 0);
}

void reply_read_file(int fd, const char* filename){
   ifstream ifs{filename};
   if (!ifs.good()){
      //do error handling
      cout << "invalid file " << endl;
      return;
   }
   //read file if valid
   string result{};
   string buffer;
   if( getline(ifs,buffer)) result += buffer;
   while (getline(ifs, buffer)){
      result += "\n" + buffer;
   }
   int filesize = result.size();
   elem_header header;
   header.command = command_type::RETFILE;
   uint32_t converted = htonl(filesize);//now in big-endian form
   //copy the 3 bytes into header
   memcpy(&header.size[0], reinterpret_cast<char*>(&converted) + 1, 3);
   //convert string into c string
   char retstr[filesize+1];
   strcpy(retstr, result.c_str());
   //send header, buffer containing file contents
   send_packets(fd, &header, sizeof header);
   send_packets(fd, retstr,  filesize);
}


void reply_write_file(int fd, const char*filen){
   //get header for file contents size
   elem_header header;
   recv_packets(fd, &header, sizeof header);
   int sum = (header.size[0] << 16) + (header.size[1] << 8) + header.size[2];
   char filec[sum];
   recv_packets(fd, filec, sum);
   //make ostream for the file
   ofstream ofs{filen};
   if (ofs.good()){
      ofs.write(filec, sum);
      ofs.close();
   }else{
      cout << "invalid file" << endl;
   }
}



void serve_client(int fd){
   elem_header header;
   recv_packets(fd,  &header, sizeof header); 
   //convert the 3 char array into an int
   int sum = (header.size[0] << 16) + (header.size[1] << 8) + header.size[2];
   switch(header.command) {
      case (command_type::READFILE) : {
         cout << "request: read file " ;
         char buff[sum+1];
         recv_packets(fd, buff, sum);
         buff[sum] = 0;
         cout << buff << endl;
         reply_read_file(fd, buff);
         break;
      } 
      case (command_type::WRITEFILE) : {
         //get filename
         cout << "request: write file " ;
         char filen[sum+1];
         recv_packets(fd, &filen, sum);
         filen[sum] = 0;//null terminate the string
         cout <<filen << endl;
         //dispatch 
         reply_write_file(fd, filen);
         break;
      }   
      default:
            break;
   }
}


int main(int , char** ){
      char port[] = "6969";
      char MAX_CLIENTS = 5;
      int sock_fd, new_fd, yes = 1;
      struct sockaddr_in cli_addr;
      struct sigaction sa;
      struct addrinfo hints, *server_info, *p;

      memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;

      if ((getaddrinfo(0, port, &hints, &server_info))!= 0){
         cout << "error getting address info" << endl;
         exit(1);
      }
      //cycle through possible connections
      for(p = server_info; p != NULL; p = p->ai_next) {
          if ((sock_fd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
              perror("server: socket");
              continue;
          }

          if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                                sizeof(int)) == -1) {
              perror("setsockopt");
              exit(1);
          }

          if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
              close(sock_fd);
              perror("server: bind");
              continue;
          }
          break;
       }
       freeaddrinfo(server_info);

       if (p == nullptr){
          cout << "failed to bind";
       }
      //start listening for connections
       if (listen(sock_fd, MAX_CLIENTS) == -1){
          cout << "listening error" << endl;
          exit(1);
       }
      //setup zombie reaper
      sa.sa_handler = sigchld_handler;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = SA_RESTART;
      if( sigaction(SIGCHLD, &sa, 0) == -1){
         cout << "sigchld error" << endl;
         exit(1);
      }

       while(1) {  // main accept() loop
           socklen_t sin_size = sizeof cli_addr;
           new_fd = accept(sock_fd, reinterpret_cast<struct sockaddr *>(&cli_addr), &sin_size);
           if (new_fd == -1) {
               perror("accept");
              continue;
           }

           if (0 == fork()) { // indicates child process
               //close listener
               close(sock_fd); 
               //respond appropriately to the request
               serve_client(new_fd);
               close(new_fd);
               //end child
               exit(0);         
           }
           close(new_fd);  // if parent, close child's fd
       }

}

