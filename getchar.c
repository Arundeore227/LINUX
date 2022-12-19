#include <stdio.h>
#include <unistd.h>

char mygetchar(void)
{
	char buff;
	read(0,&buff,1);
	return buff;
}


int main()
{
	char ch;
	ch = mygetchar();
	printf("%c\n",ch);
	//putchar(c);
	return 0;
}
