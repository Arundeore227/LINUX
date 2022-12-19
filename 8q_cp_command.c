//8. Write a system programming your own version of cp command

#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main(int argc,char *argv[])
{
	int src,des,size;
	char ch,*buff=NULL;
	//if argument is less than 3 then file is not present
	if(argc<3)
	{
		printf("less argument\n");
		return 0;
	}
	//opening the file in read mode
	src=open(argv[1],O_RDONLY);
	//if file is not persent then it will throw error
	if(src<0)
	{
		perror("ERROR: ");
		return -1;
	}
	//calculating size of file
	size=lseek(src,0,SEEK_END);
	//rewinding
        lseek(src,0,SEEK_SET);
	//creating the file
        des=creat(argv[2],0777);
	//truncating the data
        ftruncate(des,size);
	//opening the file in write mode
        des=open(argv[2],O_WRONLY,0777);
	//allocating memory dynamically
	buff=calloc(size,1);
	//reading the data from source file to buff
	read(src,buff,size);
	//writing the data from buff to destination file
	write(des,buff,size);
	//closing the file
	close(src);
	close(des);
}
