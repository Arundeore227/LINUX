//1. Write a C program that makes a copy of a file using standard I/O

#include<stdio.h>
int main(int argc,char *argv[])
{
	FILE *src,*des;
	char ch;
	//if argument is less than 3 then file will not open for read and write
	if(argc<3)
	{
		printf("less argument\n");
		return 0;
	}
	//opening file in read mode
	src=fopen(argv[1],"r");
	//if file is not present then will show file not exist 
	if(src==NULL)
	{
		printf("file not exist\n");
		return 0;
	}
	//opening bfile in write mode
	des=fopen(argv[2],"w");
	//coping one character to destination file
	while((ch=fgetc(src))!=EOF)
	{
		fputc(ch,des);
	}
	//closing the file
	fclose(src);
	fclose(des);
}
