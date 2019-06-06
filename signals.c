
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include<sys/wait.h> 

void sigCathcher(int sig);

 int arrChilds[5];         // pids of the children

 int j;                   // index to arrChilds

  

int main()

{

  int i;

  int zombie;

  int status;

  int pid;

 signal(SIGINT,sigCathcher);  

 

 for(i=0; i<5; i++)

 {

    if((pid=fork()) ==  0)  	// new child

    {                        

      	printf("PID %d ready\n", getpid());

      	j = i-1;

      	pause(); 			// wait for signal

     	exit(0);  			// end process (become a zombie)

    }

    else            			//father updates the arrChilds array.

      arrChilds[i] = pid;	  

}





sleep(2);     	                	

kill(arrChilds[4], SIGINT);     		// send signal to first child

sleep(2);                              // wait for children to become zombies 





for(i=0; i<5; i++)                      // collect zombies

{

    zombie = wait(&status); 		       

    printf("process %d is dead\n", zombie);

}

  

  exit(0);

        

}



void sigCathcher(int sig)

{

  signal(SIGINT,sigCathcher);  // re-assign the signal catcher

  printf("PID %d caught one\n", getpid());

  if(j > -1)

    	kill(arrChilds[j], SIGINT);  // send signal to next child in arrChilds

}
