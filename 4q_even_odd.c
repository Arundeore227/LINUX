//WAP create two threads using pthreads and print even no and odd no alternatively

#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

//global variables
int min,max;

//function defination of even numbers calculation
void* even() 
{
	//loop for range
	for (; min <= max;) 
	{
		//logic for calculating even numbers
		if(min % 2 == 0)
		{
			printf("even=%d\n",min);
			min++;
		}
	}
}

//function defination of odd numbers calculation
void* odd() 
{
	//loop for range
	for (; min <= max;) 
	{
		//logic for calculating odd numbers
		if(min % 2 == 1)
		{
			printf("odd=%d\n",min);
			min++;
		}
	}
}


int main(int argc, char *argv[]) 
{
	//declaring the thread variables
	pthread_t t1, t2;
	printf("enter min and max value: ");
	//scaning minimum and maximum
	scanf("%d%d",&min,&max);
	//function call for thread one
	if (pthread_create(&t1, NULL, &even, NULL) != 0) 
	{
		return 1;
	}
	//function call for thread two
	if (pthread_create(&t2, NULL, &odd, NULL) != 0) 
	{
		return 2;
	}
	//function call for join for blocking thread one
	if (pthread_join(t1, NULL) != 0) 
	{
		return 5;
	}
	//function call for join for blocking thread two
	if (pthread_join(t2, NULL) != 0) 
	{
		return 6;
	}

	return 0;
}
