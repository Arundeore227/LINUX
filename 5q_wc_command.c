//program to demonstarte wc command using system calls

#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int main(int argc, char **argv)
{
	//variable declaration
	char ch, prev= ' ';
	int fd, size, ch_cnt=0, w_cnt=0, l_cnt=0;

	//opening file in readonly mode
	fd = open(argv[1], O_RDONLY);    

	if (argc == 1)
	{
		while(1)
		{ }
	}
	//validating file is cretaed or not 
	if(fd < 0)  
	{
		perror("file can't be open\n");
		return -1;
	}
	
	//reading char by char data from the file and it will be stored in ch
	while ((read(fd, &ch, 1)) > 0)   
	{
		ch_cnt++;                                      
		if((ch == ' ' || ch == '\t' || ch == '\n'))
		{
			//logic to count words only once
			if (prev != ' ' && prev != '\t' && prev != '\n')     
			{
				w_cnt++;     
			} 
		}
		if (ch == '\n'){
			l_cnt++;
		} 
		//making backup of ch to avaid muliple count of same word
		prev = ch;
	}
	printf(" %d  %d %d %s\n", l_cnt, w_cnt, ch_cnt, argv[1]);
}
