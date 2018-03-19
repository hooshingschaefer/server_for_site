//Hooshing Schaefer 2018

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

enum class command_type : char{
   ERROR = -1, CLOSE = 0, GET = 1, GETR = 2, READFILE = 4, RETFILE = 5, WRITEFILE = 6, ACK = 7,
};

struct elem_header{
   command_type command{command_type::ERROR};
   char size[3];

};

void send_packets(int, void*, int);
void recv_packets(int, void*, int);





















