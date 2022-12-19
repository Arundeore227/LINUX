//program to demonstarte my_own kill program 
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<stdarg.h>

int main(int argc, char **argv)
{
    //variable declaration
    int i, sig;

    //validation
    if(argc!=3)
    {
        printf("Less argument error found\n");
        exit(1);
    }

    //atoi to convert string to integer
    sig = atoi(argv[2]);

    //stroing killed res into var i
    i = kill(atoi(argv[1]), sig);
    printf(" i is %d\n", i);

    //condition to check if 
    if(sig!=0)
    {
        if(i == -1)
        {
            perror("KILL");
            exit(1);
        }
    }
    else
    {
        if(i == 0)
        {
            printf("errors\n");
            exit(1);
        }
        else
        {
                // Operation not permitted
            if(errno == EPERM)
            {
                printf("Process exists, No permisssion to sedn signal\n");
                exit(1);
            }
            else if (errno == ESRCH)
            {
                 printf("Process does not exist\n");
                          exit(1);
            }
            else
            {
                perror("kill");
            }
        }
    }
}
