//WAP create two threads using pthreads and print 
//even no and odd no alternatively using mutex

#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

//declaring mutex variable
pthread_mutex_t m;
int min,max;

//function defination for even odd number checking
void* even_odd() 
{
	//mutex locking 
	pthread_mutex_lock(&m);
	for (; min <= max;) 
	{
		//checking even numbers
		if(min % 2 == 0)
		{
			printf("even=%d\n",min);
		}
		//checking odd numbers
		if(min % 2 ==1)
		{
                        printf("odd=%d\n",min);
                }
		min++;
	}
	//mutex unlocking 
	pthread_mutex_unlock(&m);
}

int main() 
{
	pthread_t t1,t2;
	printf("enter min and max value: ");
	//scaning minmum and maximum
	scanf("%d%d",&min,&max);
	//function call for creating thread one 
	if (pthread_create(&t1, NULL, &even_odd, NULL) != 0) 
	{
		return 1;
	}
	//function call for creating thread two 
	if (pthread_create(&t2, NULL, &even_odd, NULL) != 0) 
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
	//mutex destroied
	pthread_mutex_destroy(&m);
	return 0;
}
