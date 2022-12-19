//7. Write a system programming your own version of getchar() standard C Library

#include <stdio.h>
#include <unistd.h>


int my_getchar(void)
{
	char buff[1000];
	int s=0,r;
	//reading the  characters
	r = read(0,buff,1000);
	//passing only one character
	return buff[s++];
}


int main(){
	char c;
	//function call 
	c = my_getchar();
	//printing character
	write(1,&c,1);
	printf("\n");
	return 0;
}
