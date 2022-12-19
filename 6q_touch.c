//6. Write a system programming your own version of touch command

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	//if argument is one the file is not created
	if(argc==1)
	{
		perror("ERROR: missing file operand");
		exit(EXIT_FAILURE);
	}

	for(int i=1;i<argc;i++)
	{
		int fd;
		//creating the file according to argument
		fd=open(argv[i],O_WRONLY|O_CREAT,0777);
		if(fd<0)
		{
			perror("ERROR: file not created ");
			return -1;
		}
	}
}


