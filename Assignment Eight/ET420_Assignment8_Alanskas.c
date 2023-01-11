/* 	
	ET420 Assignment #8
	Programmer: Matt Alanskas
	Program: Temp Sensor
	Created: 08 April 2021
	Modified: 16 April 2021
*/

#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
   DCB dcb={0};
   HANDLE hCom;
   BOOL fSuccess;
   char *pcCommPort = "COM6";
   
   short szBuff[1] = {0}; // reading two int + 1 extra
   DWORD BytesRead = 0 ;
   
   char buffer[256];
   struct tm *loctime;
   time_t curtime;
   
	char header[81];		// output filename and then header to data file
	FILE *data_file;

/***************************************CommTimeouts******************************************/
COMMTIMEOUTS timeouts={0};
timeouts.ReadIntervalTimeout=50;
timeouts.ReadTotalTimeoutConstant=50;
timeouts.ReadTotalTimeoutMultiplier=10;
timeouts.WriteTotalTimeoutConstant=50;
timeouts.WriteTotalTimeoutMultiplier=10;

/*******************************************Handle*******************************************/
   hCom = CreateFile( pcCommPort,
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ,    // must be opened with exclusive-access
                    NULL, // no security attributes
                    OPEN_EXISTING, // must use OPEN_EXISTING
                    FILE_ATTRIBUTE_NORMAL,    // not overlapped I/O
                    NULL  // hTemplate must be NULL for comm devices
                    );

/***************************************SET*UP*COM*PORT**************************************/
   if (hCom == INVALID_HANDLE_VALUE)
   {
       printf ("CreateFile failed with error %d.\n", GetLastError());
       return (1);
   }

   if(!SetCommTimeouts(hCom, &timeouts))
    {
        /*Well, then an error occurred*/
   }

   fSuccess = GetCommState(hCom, &dcb);

   if (!fSuccess)
   {
     /*More Error Handling*/
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }


   dcb.BaudRate = 9600;     			// set the baud rate
   dcb.ByteSize = 8;             		// data size, xmit, and rcv
   dcb.Parity = NOPARITY;        		// no parity bit
   dcb.StopBits = ONESTOPBIT;    		// one stop bit
   fSuccess = SetCommState(hCom, &dcb);

   if (!fSuccess)
   {
      printf ("SetCommState failed. Error: %d.\n", GetLastError());
      return (3);
   }

   printf ("Serial port %s successfully configured.\n", pcCommPort);
   
/********************************************************************************************/   
     
	fputs("Input name of data file -  ",stdout);		// gets name of data file
    gets(header);
              
    if((data_file = fopen(header,"w")) == NULL)		// opens data file
    fprintf(stderr,"\nCan't open data file - %s. \n",header); 	// checks file opened ok
    
    curtime = time (NULL); // get current time in seconds from PC
    /* Convert it to local time representation. */
    loctime = gmtime (&curtime);
       
    fputs ("File started ", data_file);
	fputs (asctime (loctime), data_file);

	 
/**************************Writing data to the file******************************************/
	   
   while(1){
   	
   	WriteFile(hCom, "r",1, &BytesRead,NULL);	// 	prompts arduino for temps
   	Sleep(200);
   	if(!ReadFile(hCom, szBuff, 2, &BytesRead, NULL)){
   	printf("ReadFile Failed");
	exit(4);
   }
	if (BytesRead != 2)
	{
		printf("\n BytesRead = %d \n\n",BytesRead);
		break;
	}
	curtime = time (NULL); // get current time in seconds from PC
	
  		
   		printf(" A/D Conversion Data =  %d, Temperature = %d \n",szBuff[0], szBuff[1]);
   		
   		fprintf(data_file,"%ld,%d,%d\n",curtime,szBuff[0],szBuff[1]);
   		
   		
   		Sleep(6000);		// wait 6 second
   	
   		
   }
   
   
   fclose(data_file);
   CloseHandle(hCom);
   printf("Program terminated normally");
   return (0);
   
/********************************************************************************************/
}



