//3. Write a system programming your own version of echo command

#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	//if argument is one then print new line
	if(argc==1)
	{
		write(1,"\n",1);
		exit(0);
	}
	//printing number of arguments 
	for(int i=1;i<argc;i++)
	{
		//system call to print the aruments
		write(1,argv[i],strlen(argv[i]));
		write(1," ",1);
	}
	write(1,"\n",1);
}

