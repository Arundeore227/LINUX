//program to implement our own system call

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
void my_system(char *);

int main(int argc, char* argv[])
{
	//function call for my system call
	my_system(argv[1]);
	exit(0);
}

//
void my_system(char * buff)
{
	pid_t pid;
	int ret, status;
	//creating child process
	pid = fork();
	//child process
	if (pid == 0)
	{
		//in side child process exec function call
		ret = execl("/bin/sh", "sh", "-c", buff, (char*) NULL);
		if (ret == -1)
		{
			perror("Execve fails");
			exit(0);
		}
	}
	//parent process
	else
	{
		wait(&status);
	}
}
