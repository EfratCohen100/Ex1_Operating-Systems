/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


void Sigcatch(int pid)
{
    printf("PID %d Caught one\n",getpid());
}

int main()
{
    int pid;
    signal(SIGINT,Sigcatch);
    int MyChildren [5];

    for(int i=0; i<5; i++)
    {
       // printf("MyChildren");
         if((pid=fork())==0)
        {
          pause();// wait for signal
          sleep(2);
          exit(0);  
        }
        
        else 
        {
            MyChildren[i]=pid;
            printf("PID %d ready\n",pid); 
        }
        
    }
    for(int j=4; j>=0; j--)
    {
        signal(SIGINT,Sigcatch);
        sleep(1);
        if(j==4)
        {
            kill(MyChildren[4],SIGINT);
        }
        
         if(j>=0 && j!=4){
            
            kill(MyChildren[j],SIGINT);

        }

    }
        sleep(2);
        for(int i=0; i<5;i++)
        {
            
          kill(SIGTERM,MyChildren[i]);
          printf("Process %d is dead\n",MyChildren[i]);
        }
        
    
    return 0;
}
