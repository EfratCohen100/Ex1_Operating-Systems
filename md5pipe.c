
// C program to illustrate 
// pipe system call in C 
// shared by Parent and Child 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include "md5.h"
#include <iostream>
#include <mutex>
#define  MSGSIZE 20 
using namespace std;
 
 int pid =-1;
 char inbuf[32]; 
 bool bLen32=false;
 mutex m_mutex;
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
    m_mutex.lock();
   int sizeOfArray = sizeof(inbuf)/sizeof(inbuf[0]);
  // printf("sizeOfArray : %d,inbuf= %d,inbuf[0]= %d \n",sizeOfArray,sizeof(inbuf),sizeof(inbuf[0]));

   if(sizeOfArray<=32)
   {
      bLen32 = true;
   }
   sleep(1);
    m_mutex.unlock();
};

void sigquit(int pid) 
{ 
    printf("My DADDY has Killed me!!!\n"); 
    exit(0); 
}

int main() 
{ 
    //cout<<endl<<msg2<<endl;
    char* MsgUser=new char[MSGSIZE];
    printf("Get Massege from the Username:\n");
    char charUser;
    string resMsg;
    for(int i=0; i<MSGSIZE; i++)
    {
        scanf("%c",&charUser);
        //printf("MyWords: %c\n",charUser);
        int CharInt=(int)charUser;
        if(CharInt<=19)
        {
            //printf("shira");
        break;
        }
         MsgUser[i]=charUser;
        // printf("Word: %c\n", MsgUser[i]);
    } 
    
    char* result;
    int pr2chPipe[2],ch2prPipe[2] ,pRoll, nbytes; 
    

    if (pipe(pr2chPipe) < 0) 
        exit(1); 
     if (pipe(ch2prPipe) < 0) 
        exit(1);     
  
    /* continued */
    //const char* resMsg=0; 
    // printf("CP1  \n");
     if ((pRoll = fork()) < 0) 
     { 
        perror("fork"); 
        exit(1); 
    } 
   /// printf("CP common \n");
    if (pRoll==0) {
        
      //  printf("My PID: %d \n", getpid());
      
        close(pr2chPipe[1]);
        if(nbytes=ReadPipe(pr2chPipe[0],inbuf,MSGSIZE)>0)
        {
            exit(1);
        }
          resMsg=md5(inbuf);

        if (nbytes != 0) 
            exit(2); 
         WritePipe(ch2prPipe[1],resMsg.c_str(),32);


         close(ch2prPipe[1]);
         pid = getppid();
      //   printf("CPch6  ppid = %d \n",pid);
        kill(pid,SIGTERM);
        signal(SIGQUIT, sigquit); 
   
         
         for(;;){sleep(2);};
        
      // printf("Finished child reading\n"); 
    }
    else//parent process
    {
        pid = getpid();
        //printf("CPpr1 pid = %d \n",pid);
        signal(SIGTERM,sigCatch);
     
       // printf("CPpr3 \n");
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
                   printf("Result Parent: %s\n", inbuf);
                }
            }
         }
           
             sleep(5);
         //  printf("\nPARENT: sending SIGQUIT\n\n"); 
           kill(pRoll, SIGQUIT);
           // printf("Finished Parent\n"); 
            
            wait(NULL); 
            
    }
    return 0; 
  } 

