//2. Write a system programming your own version of cat command

#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	char *buff=NULL,ch; 
	int fd,size;
	//opening the file in read mode
	fd = open(argv[1], O_RDONLY);
	//if file discrepter is less than zero then file is not present
	if(fd<0)
	{
		perror("ERROR ");
		return -1;
	}
	//calcukating the size of file 
	size=lseek(fd,0,SEEK_END);
	//allocating memory dynamically
	buff=calloc(size,1);
	//rewinding the file
	lseek(fd,0,SEEK_SET);
	//reading the data from the file to buff
	read(fd,buff,size);
	//displaying the data to output console
	write(1,buff,size);
	exit(0);
}
