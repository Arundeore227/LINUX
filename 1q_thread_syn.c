/*1.producer thread and consumer thread synchronization using pthreads without
  synchronization mechanisms*/


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>



// function declaration
void *producer();
void *consumer();

// variable declaration
int counter = 0;
char buffer[5];

//drive code
int main()
{
	//pthread declaration
	pthread_t prod,cons;



	// first thread creation producer
	if(pthread_create(&prod,NULL,&producer,NULL) !=0)
	{
		perror("Thread failed to create\n");
		exit(EXIT_FAILURE);
	}

	// second thread creation consumer
	if(pthread_create(&cons,NULL,&consumer,NULL) !=0)
	{
		perror("Thread failed to create\n");
		exit(EXIT_FAILURE);
	}

	// Joining the thread for producer
	if(pthread_join(prod, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}



	// Joining the thread for consumer
	if(pthread_join(cons, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}



// function defination for producer
void *producer()
{
	// declaring an variable
	int i = 0;
	char c = 'Z';

	// infinite while loop
	while(1)
	{
		while(counter == 5);
		buffer[i] = c;
		i = (i + 1) % 5;
		if (i == 0)
		{
			c = 'Z';
			printf("Producer : %s\n", buffer);
		}

		else
		{
			c--;
		}
		counter++;
	}
	pthread_exit("exit");
}



//function defination for consumer
void *consumer()
{    
	int i = 0;
	char data[5];
	while(1)
	{
		while(counter == 0);
		data[i] = buffer[i];
		i = (i+1) % 5;
		if (i == 0)
		{
			printf("Consumer : %s\n",data);
		}
		counter--;
	}
	pthread_exit("exit");
}
