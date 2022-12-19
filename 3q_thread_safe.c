//implement of thread safe function

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int odd_sum=0, even_sum=0,max,min;
pthread_mutex_t mutex; 

//function defination for calculating the sum of even and odd numbers
void* even_odd_sum()
{ 
	//locking the thread
	pthread_mutex_lock(&mutex);

	for(; min <= max;)
	{
		//calculating the sum of even numbers
		if(min % 2 == 0)
		{
			even_sum += min;
		}
		//calculating the sum of odd numbers
		else 
		{
			odd_sum +=  min;
		}
		min++;
	}

	//unlocking the thread
	pthread_mutex_unlock(&mutex);    

}
 
int main()
{ 



	printf("enter the min and max range :");
	scanf("%d%d",&min,&max);

	pthread_t even,odd; 

	//creating two threads and calling the function
	pthread_create(&even,NULL,&even_odd_sum,NULL);
	pthread_create(&odd,NULL,&even_odd_sum,NULL);


	//join will block the state of the thread 
	if(pthread_join(even,NULL) !=0 )
		return 2;

	//join will block the state of the thread
	if(pthread_join(odd,NULL) !=0 )
		return 3; 

	//printing the sum of odd and even numbers 
	printf("sum of even numbers = %d\n",even_sum);
	printf("sum of odd numbers = %d\n",odd_sum);

	//destroying the mutex
	pthread_mutex_destroy(&mutex);   
}
