/* program to check the producer consumer */
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
int limit=0;


int main()
{
	pthread_t t1,t2;



	// first thread creation(producer)
	if(pthread_create(&t1,NULL,&producer,NULL) !=0)
	{
		perror("Thread failed to create\n");
		exit(EXIT_FAILURE);
	}

	// second thread creation(consumer)
	if(pthread_create(&t2,NULL,&consumer,NULL) !=0)
	{
		perror("Thread failed to create\n");
		exit(EXIT_FAILURE);
	}



	// Joining the thread
	if(pthread_join(t1, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}



	// Joining the thread
	if(pthread_join(t2, NULL)!=0)
	{
		perror("Thread failed to join\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}



// function for producr to produce
void *producer()
{
	//    int  limit =0;
	// declaring an variable
	int i = 0;
	char c = 'Z';

	// infinite while loop
	while(1)
//	while(limit <=13)
	{
		while(counter == 5);
		buffer[i] = c;
		i = (i + 1) % 5;
		if (i == 0)
		{
			c = 'Z';
			printf("in p: ");
			printf("Producer : %s\n", buffer);
//			limit++;
	//	if(limit==10)
	//		pthread_exit("exit");
		}

		else
		{
			c--;
		}
		counter++;
	//	limit++;
//		if(limit==13)
//			pthread_exit("exit");

	}
//	printf("limit = %d\n",limit);
}



//function for consumer consumes
void *consumer()
{    
	int i = 0;
	char data[5];

//	printf("enty lim c %d\n",limit);
//	while(limit >=0)
	while(1)
	{
//	printf("lim c %d\n",limit);
		while(counter == 0);
		data[i] = buffer[i];
		i = (i+1) % 5;
		if (i == 0)
		{
			printf("in c: ");
			printf("Consumer : %s\n",data);
//			limit--;
	//	if(limit==0)
	//		pthread_exit("exit");
		}
		counter--;
	//	limit--;
//		if(limit==0)
//			pthread_exit("exit");
	}
	//pthread_mutex_unlock(&mutex);
//	printf("limit 1 = %d\n",limit);
}
