//program to demonstrate my own du

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, char const *argv[])
{
   //variable declaration
    int fd1 ;
    float fd2 , ret ;
    
    //declarinng struc variable
    struct stat statbuf;
    ret = stat(argv[1],&statbuf);
  //getting size using stat function
fd2 = (statbuf.st_blksize);
printf("%ld\t %s\n",statbuf.st_blocks/2, argv[1]);
return 0;
}
