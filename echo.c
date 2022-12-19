#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	if(argc==1)
	{
		printf("\n");
		exit(0);
	}
	for(int i=1;i<argc;i++)
		printf("%s ",argv[i]);
	printf("\n");
}

