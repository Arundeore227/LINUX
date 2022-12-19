//3. WAP your own version of cat command using mmap system call

#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char **argv)
{
	struct stat size;
	int fd;
	char *buff=NULL;
	//if arguments are less than 2 then give proper file
	if(argc<2)
	{
		printf("give proper file name to be open\n");
		return 0;
	}
	//file open in read mosde
	fd=open(argv[1],O_RDONLY);
	//if file not present then it will give error
	if(fd<0)
	{
		perror("Error opening file for reading");
	        exit(EXIT_FAILURE);
	}
	//reading the size
	fstat(fd,&size);
	// function call for mmaping
	buff=mmap(0,size.st_size,PROT_READ,MAP_PRIVATE,fd,0);
	if (buff == MAP_FAILED)
	{
        	close(fd);
        	perror("Error mmapping the file");
        	exit(EXIT_FAILURE);
    	}
	//printing the charcter one by one 
	for(int i=0;i<size.st_size;i++)
		printf("%c",buff[i]);
	close(fd);
}

