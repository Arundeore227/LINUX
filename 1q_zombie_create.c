// program to create a zombie process

#include<stdio.h>
#include<unistd.h>
int main()
{
	int i;
	int pid; 
	//creating child process
	pid= fork();
	//child process
	if (pid == 0)
	{
		for (i=0; i<20; i++)
			printf("I am Child\n");
	}
	//parent process
	else
	{
		printf("I am Parent\n");
		while(1);
	}
}
