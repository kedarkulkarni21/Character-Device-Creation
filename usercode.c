#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define DEVICE "/dev/my_char_device"

int main()
{
	char buffer[7000000];
	int fp, i;
	int bytes_read;
	fp = open(DEVICE, O_RDONLY);
	if(fp == -1)
	{
		printf("file %s does not exist or has been occupied by someone else\n", DEVICE);
		exit(-1);
	}
	else
	{
			//Reading the bytes from kernel module
			bytes_read = read(fp, buffer, 700000);
			if(bytes_read == 0)
			{
				printf("\nBuffer is empty\n");
			}
			else
			{
				printf("\n************PROCESSES****************\n %s", buffer);
			}
	}
	close(fp);
	return 0;
}

