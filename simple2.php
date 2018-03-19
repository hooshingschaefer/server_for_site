<?php
//simple template for writing to a file. once again since I have no access to a linux machine to run the server on, localhost is used and there are not GET/POST variables
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    echo "socket was not created". "\n";
} 
//use localhost
$address = "127.0.0.1";

$port = 6969;
$result = socket_connect($socket, $address, $port);
if ($result === false) {
    echo "socket did not connect \n";
}

//COMMAND MACROS
//error = -1, close = 0, get = 1, getr = 2, readfile = 4, retfile = 5, writefile =6, ack = 7
$HEADER_SIZE = 4;
$COMMAND = 6;
//this variable will be replaced with $_GET[] when implemented in my website
$filename = "output.txt"; // change filename here
$filenamelen = strlen($filename);

//put command into first byte then the payload size into the next 3
$str = pack("C", $COMMAND). substr(pack("N",$filenamelen), 1 ); 

//send header, filename to be written in
socket_write($socket, $str, $HEADER_SIZE);
socket_write($socket, $filename);


//file contents and size of file
$towrite = "blahblahblah testing hello\n dshfladhfjkdfksdhf dkashdf kadf	\n hfldkfs end";
$filenamelen = strlen($towrite);

//build header
$str = pack("C", $COMMAND). substr(pack("N",$filenamelen), 1 ); 

//send header
socket_write($socket, $str, $HEADER_SIZE);

//send file contents
socket_write($socket, $towrite);

//close socket
socket_close($socket);
?>

