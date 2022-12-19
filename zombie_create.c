//1. Program to create zombie process

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int child;
	child =fork();
	printf("%d\n",getpid());
	sleep(20);
	if(child>0)
		exit(0);
	else
	{
		sleep(100);
		exit(0);
	}
	return 0;
}





