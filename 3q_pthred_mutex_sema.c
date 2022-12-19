/*producer consumer problem using Pthreads, semaphores and Mutex*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>



// function declaration
void * producer1();
void * producer2();
void * consumer();

// variable declaration
int count = 0, limit=0;
char buffer[5];



//declairing the mutex
pthread_mutex_t mutex1,mutex2;
sem_t full, empty;

int main()
{
	pthread_t pro1,pro2,con;
	sem_init(&full,0,0);
	sem_init(&empty,0,5);
	//thread creation producer1
	if(pthread_create(&pro1,NULL,&producer1,NULL) !=0)
	{
		perror("Thread failed to create\n");
		exit(EXIT_FAILURE);
	}
	//thread creation producer2
	if(pthread_create(&pro2,NULL,&producer2,NULL) !=0)
	{
		perror("Thread failed to create\n");
		exit(EXIT_FAILURE);
	}
	//thread creation consumer
	if(pthread_create(&con,NULL,&consumer,NULL) !=0)
	{
		perror("Thread failed to create\n");
		exit(EXIT_FAILURE);
	}
	// Joining the thread producer    
	if(pthread_join(pro1, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}
	// Joining the thread producer
	if(pthread_join(pro2, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}
	//Joining the thread consumer
	if(pthread_join(con, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

// function for producer to produce
void *producer1()
{
	int i = 0;
	char c = 'Z';
	// infinite while loop
	while(1)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&mutex1);
		while(count == 5);
		//producing 
		buffer[i] = c;
		i = (i + 1) % 5;
		if (i == 0)
		{
			c = 'Z';
			printf("Producer 1 : %s\n", buffer);
		}
		else
		{
			c--;
		}
		count++;
		pthread_mutex_unlock(&mutex1);
		sem_post(&full);
	}
	pthread_exit("exit");
}

// function for producer to produce
void *producer2()
{
	int i = 0;
	char c = 'A';
	// infinite while loop
	while(1)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&mutex1);
		while(count == 5);
		//producing
		buffer[i] = c;
		i = (i + 1) % 5;
		if (i == 0)
		{
			c = 'A';
			printf("Producer 2 : %s\n", buffer);
		}
		else
		{
			c++;
		}
		count++;
		pthread_mutex_unlock(&mutex1);
		sem_post(&full);
	}
	pthread_exit("exit");
}

//function for consumer to consume
void *consumer()
{
	int i = 0;
	char data[5];
	while(1)
	{
		sem_wait(&full);
		pthread_mutex_lock(&mutex2);
		while(count == 0);
		//consuming
		data[i] = buffer[i];
		i = (i+1) % 5;
		if (i == 0)
		{
			printf("Consumer : %s\n",data);
		}
		count--;
		pthread_mutex_unlock(&mutex2);
		sem_post(&empty);
	}
	pthread_exit("exit");
}
