/* impliment our own kill command*/

//adding header files
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<errno.h>

//driving code
int main(int argc, char **argv)
{
    //variable declaration
    int i, signal;
    
    //checking the command line argument
    //if it less than 3 printing the usage message
    if(argc!=3)
    {
        printf("Less argument error found\n");
        exit(1);
    }
    
    //taking the signal number from command line 
    //and convert to intiger
    //signal = atoi(argv[2]);
    
    //taking the pid of process from command line and convert to intiger
    i = kill(atoi(argv[1]), atoi(argv[2]));
        
   
    if(i == -1)
    {
       perror("KILL");
       exit(1);
    }
}
