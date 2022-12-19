//4. Program for user define head & tail command

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
	int fd,i=0,j=0,n,choice;
	char ch,**p=NULL,*buff=NULL;
	//file open in read mode
	fd=open(argv[1],O_RDONLY);
	//if file is not present then it will throw error
	if(fd<0)
	{
		perror("Error:");
		return -1;
	}
	//reading one character from file
	while((read(fd,&ch,1))>0)
	{
		//allocating memory to store one line
		buff=realloc(buff,i+1);
		//assigning one character to buffer
		buff[i++]=ch;
		if(ch=='\n')
		{
			buff=realloc(buff,i+1);
			buff[i++]='\0';
			//creating array of pointer to store the one line address
			p=realloc(p,(j+1)*sizeof(p));
			p[j++]=buff;
			//making buff NULL to store another line
			buff=NULL;
			i=0;
		}
	}
	printf("enter the choice\n");
	//choice 1 for head and choice 2 for tell
	printf("1.head\n2.tail\n3.exit\n");
	scanf("%d",&choice);
	switch(choice)
	{
		//printing ten lines if line is more than or equal to 10 then 
		//it will print only 10 line from the starting
		case 1:if(j>=10)
		       {
				for(int k=0;k<10;k++)
					printf("%s",p[k]);
		       }
		       //if lines are less than or equal to 10 then it will print all line
		       else if(j>=1 && j<=10)
		       {
				for(int k=0;k<j;k++)
					printf("%s",p[k]);
		       }
		       //for no line in file
		       else
			       printf("file is empty\n");
		       break;
		//if lines are less than or equal to 10 then it will print all line
		case 2:if(j>=1 && j<=10)
		       {
				for(int k=0;k<j;k++)
					printf("%s",p[k]);
		       }
		       //if line is more than or equal to 10 then it will print 10 lines from the down
		       else if(j>10)
		       {
			       n=j-10;
				for(int k=n;k<j;k++)
					printf("%s",p[k]);
		       }
		       //for no line in file
		       else
			       printf("file is empty\n");
		       break;
		case 3:return 0;
		default:printf("invalid choice\n");
	}


}




