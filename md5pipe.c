
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include "md5.h"
#include <iostream>
#define  MSGSIZE 20 
using namespace std;
 
 int pid =-1;
 char inbuf[32]; 
 bool bLen32=false;
void WritePipe(int pipeP2P,const char* msg,int sizeMsg)
{
 write(pipeP2P, msg, sizeMsg);    
}
  
int ReadPipe(int pipeP2P, char* inbuf_,int sizeMsg)
{
   int nbytes = read(pipeP2P, inbuf_, sizeMsg);   
   return nbytes;
}

void sigCatch(int pid_)
{
   int sizeOfArray = sizeof(inbuf)/sizeof(inbuf[0]);

   if(sizeOfArray<=32)
   {
      bLen32 = true;
   }
   sleep(1);
};

void sigquit(int pid) 
{ 
    printf("My DADDY has Killed me!!!\n"); 
    exit(0); 
}

int main() 
{ 
    char* MsgUser=new char[MSGSIZE];
    printf("Get Massege from the Username:\n");
    char charUser;
    string resMsg;
    for(int i=0; i<MSGSIZE; i++)
    {
        scanf("%c",&charUser);
        
        int CharInt=(int)charUser;
        if(CharInt<=19)
        {
        break;
        }
         MsgUser[i]=charUser;
    } 
    
    char* result;
    int pr2chPipe[2],ch2prPipe[2] ,pRoll, nbytes; 
    

    if (pipe(pr2chPipe) < 0) 
        exit(1); 
     if (pipe(ch2prPipe) < 0) 
        exit(1);     
  
    /* continued */
     if ((pRoll = fork()) < 0) 
     { 
        perror("fork"); 
        exit(1); 
    } 
    if (pRoll==0) {
        
        close(pr2chPipe[1]);
        while(nbytes=ReadPipe(pr2chPipe[0],inbuf,MSGSIZE)>0)
        {
        }

          resMsg=md5(inbuf);

        if (nbytes != 0) 
            exit(2); 
         WritePipe(ch2prPipe[1],resMsg.c_str(),32);


         close(ch2prPipe[1]);
         pid = getppid();
        kill(pid,SIGTERM);

         
         for(;;){sleep(2);};
        
    }
    else//parent process
    {
        pid = getpid();
        signal(SIGTERM,sigCatch);
        WritePipe(pr2chPipe[1],MsgUser,MSGSIZE);
        delete[] MsgUser;
        close(pr2chPipe[1]);
        
        close(ch2prPipe[1]); 
        sleep(2);
    
        while ((nbytes = ReadPipe(ch2prPipe[0],inbuf,32)) > 0)
           {
            if (nbytes != 0) 
            {
                if(bLen32==true)
                {
                   printf("encrypted by process %d : %s\n",getpid(),inbuf);
                }
            }
         }
           
           kill(pRoll, SIGTERM);
            
            wait(NULL); 
            
    }
    return 0; 
  } 

