/*producer consumer problem using Pthreads and counting semaphores*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

// function declaration
void * producer();
void * consumer();

// variable declaration
int count = 0, limit=0;
char buffer[5];
sem_t full, empty;

int main()
{
	pthread_t prod,cons;
	sem_init(&full,0,0);
	sem_init(&empty,0,5);

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

	// Joining the thread producer
	if(pthread_join(prod, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}

	// Joining the thread consumer
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
	int i = 0;
	char c = 'Z';

	// infinite while loop
	while(1)
	{
		sem_wait(&empty);
		while(count == 5);
		//producer producing
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
		count++;
		sem_post(&full);
	}
	pthread_exit("exit");
}



//function defination for consumer
void *consumer()
{
	int i = 0,limit1 =0;
	char data[5];
	while(1)
	{
		sem_wait(&full);
		while(count == 0);
		//consumer consuming
		data[i] = buffer[i];
		i = (i+1) % 5;
		if (i == 0)
		{
			printf("Consumer : %s\n",data);
		}
		count--;
		sem_post(&empty);
	}
	pthread_exit("exit");
}
