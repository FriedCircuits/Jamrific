#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <bluetooth.h>
#include <hci.h>
#include <hci_lib.h>
#include <signal.h>

using namespace std;
volatile sig_atomic_t flag = 0;
FILE *gatt;

#include <wiringSerial.h>

void finish(int sig){
	flag = 1;
	//printf("Break1");
	//fwrite("\n\nexit\n", sizeof(char), strlen("\n\nexit\n"), gatt);
	//pclose(gatt); printf("\nBreak\n");
	exit(0);
}

int main(int argc, char* argv[])
{
	//signal(SIGINT, finish);
	if(argc < 2){cout << "Missing MAC addrss of Bulb" << endl; return 1;}
	//if (wiringPiSetup() == -1)
	//	exit (1);
	
	int fd;
	if((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	{
		 fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		 return 1 ;
	}

	//char buffer[17];
	//FILE *gatt;	
	//gatt = popen("gatttool -I", "w");		
	//sleep(1);
	//fwrite("connect B4:99:4C:64:80:3E\n", sizeof(char), strlen("connect B4:99:4C:64:80:3E\n"), gatt); 
	//sleep(1);
	
	char in = '0';
	int r,g,b;
	int i;
		stringstream line;

	for(;;)
	{
	   	//stringstream rs;
		//stringstream gs;
		//stringstream bs;


	   // putchar(serialGetchar(fd));
	   // fflush (stdout);
	  in = serialGetchar(fd);
	  if (in == '.') {
		r = serialGetchar(fd);
		g = serialGetchar(fd);
		b = serialGetchar(fd);
		//cout << r << g << b << endl;
		//rs << std::hex << r;
		//gs << std::hex << g;
		//bs << std::hex << b;
		//printf(rs.str().c_str());
		
  	  
	//fwrite("\nchar-write-cmd 0x0028 58010301ff00ff00ff\n", sizeof(char), strlen("\nchar-write-cmd 0x0028 58010301ff00ff00ff\n"), gatt);
	//cmd += 
	line << "gatttool -i hci0 -b B4:99:4C:64:80:3E --char-write-req -a 0x0028 -n 58010301ff00" << std::hex << r << std::hex << g << std::hex << b;	
	//printf(line.str().c_str());
	//i = system(("gatttool -i hci0 -b B4:99:4C:64:80:3E --char-write-req -a 0x0028 -n 58010301ff00"+rs+gs+bs).c_str());
	i = system(line.str().c_str());
	//printf("SYS: %d\n", i);
	//sleep(.5);

	}
	else {		  serialFlush(fd);}

	/*if(flag){
		printf("\nTest\n");
		//fwrite("\nexit\n", sizeof(char), strlen("\nexit\n"), gatt);
		//pclose(gatt); printf("\nBreak\n"); return 0;
		exit(0);
	}*/
    }

    return 0;
}
