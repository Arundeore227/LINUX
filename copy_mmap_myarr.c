//4. Write an Linux System Programming copy one file content to
//another file using mmap() system call.

#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char **argv)
{
	struct stat size;
	int fd,des,s;
	char *buff=NULL,*arr=NULL;
	//give the proper file name for opening
	if(argc<2)
	{
		printf("give proper file name\n");
		return 0;
	}
	// file open in read mode
	fd=open(argv[1],O_RDONLY);
	//if file is not present it will throw error
	if(fd<0)
	{
		perror("error opening file for reading");
		exit(EXIT_FAILURE);
	}
	//file open in writting mode
	des=open(argv[2],O_WRONLY|O_CREAT,0777);
	//getting the file size
	fstat(fd,&size);
	s=size.st_size;
	// memory allocting for storing the mmap file data
	arr=calloc(s,1);
	// mmap function calling
	buff=mmap(0,size.st_size,PROT_READ,MAP_PRIVATE,fd,0);
	//assigning one by one character to dynamic array
	for(int i=0;i<size.st_size;i++)
		arr[i]=buff[i];
	//coping the data to destination file
	write(des,arr,s);
	printf("copy done\n");
	close(fd);
	close(des);
}

