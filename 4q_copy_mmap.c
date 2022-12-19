//copy command using mmap

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
	char *buff=NULL,*arr=NULL,*temp;
	//if argument is less than two then give proper argument
	if(argc<2)
	{
		printf("give file name to be open\n");
		return 0;
	}
	//opening the file in read mode
	fd=open(argv[1],O_RDONLY);
	//file is not present then it will throw error
	if(fd<0)
	{
		write(1,"file is not opened\n",1);
		return -1;
	}
	//calculating size
	s=lseek(fd,0,SEEK_END);
	//rewinding the file
	lseek(fd,0,SEEK_SET);
	//creating the file 
	des=creat(argv[2],0777);
	//truncating the size
	ftruncate(des,s);
	//opening the file in write mode
	des=open(argv[2],O_WRONLY|O_CREAT,0777);
	//mmaping function call for reading the data from the file
	buff=mmap(0,1,PROT_READ,MAP_SHARED,fd,0);
	//mmaping function call for writting the data
	temp=mmap(0,1,PROT_WRITE,MAP_SHARED,des,0);
	//writting the data one by one
	for(int i=0;i<s;i++)
		write(des,&buff[i],1);
	
	// unmapping the files from source
    	munmap(buff,size.st_size);
    	munmap(temp,size.st_size);
	//closing the files
	close(fd);
	close(des);
}

