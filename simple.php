<?php

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
//error = -1, close = 0, get = 1, getr = 2, readfile = 4, retfile = 5
$HEADER_SIZE = 4;
$COMMAND = 4;
//this variable will be replaced with $_GET[] when implemented in my website
//$filename = "simple.php";
$filename = "testtext.txt";
$filenamelen = strlen($filename);
//put command into first byte then the payload size into the next 3
$str = pack("C", $COMMAND). substr(pack("N",$filenamelen), 1 ); 
//send header, filename to be retrived
socket_write($socket, $str, $HEADER_SIZE);
socket_write($socket, $filename);
//get the header
$reply = socket_read($socket, $HEADER_SIZE);
//use last 3 bytes for the size of the payload, store into reply_size
$reply_size = unpack("Nelem", "\00" .substr($reply, 1));
//read $reply_size many bytes and echo
echo socket_read($socket, $reply_size["elem"]);

socket_close($socket);
//jkdshfkjasdhfkjasjhdfklash dklfahsdk fhasldf jasdkf
?>

