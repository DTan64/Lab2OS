#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	int userBuffer = 0;
	char ans = 'a';
	char writeBuffer[100];
	int file;
	int offset = 0;
	int whence = 0;
	char* data;
	file = open("/dev/simple_char_device", O_RDWR);
	while(1)
	{
		printf("\n\nWhat do you want to do?\n");
		printf("Press r to read from device\n");
		printf("Press w to write from device\n");
		printf("Press s to seek into the device\n");
		printf("Press e to exit from the device\n");
		printf("Enter command\n");
		scanf("%c", &ans);
		if(ans == 'r')
		{
		printf("How many bytes to you want to read?\n");
		scanf("%i", &userBuffer);
		//char data[userBuffer];
		char* data = (char *)malloc(userBuffer);
		read(file, data, userBuffer);
		printf("%s\n", data);
		while(getchar() != '\n');
		}
		else if(ans == 'w')
		{
			printf("What do you want to write?\n");
			scanf("%s", writeBuffer);
			int userBytes = sizeof(writeBuffer);
			write(file, writeBuffer, userBytes);
			while(getchar() != '\n');
		}
		else if(ans == 's')
		{
			printf("Offset value?\n");
			scanf("%i", &offset);
			printf("Whence value?\n");
			scanf("%i", &whence);
			lseek(file, offset, whence);
			while(getchar() != '\n');
		}
		else if(ans == 'e')
		{
			close("/dev/simple_char_device");
			return 0;
		}
		else
		{
			printf("Invalid option.\n");
			while(getchar() != '\n');
		}
	}	
	
	
/*
	int file;
	char data[100];
	file = open("/dev/simple_char_device", O_RDWR);
	write(file, "Hello, World!asdfassasdfasfsdfsdasfasfassdfdasdasddfasdfffasdfsmash64", 100);
	//write(file, "test", 10);
	//lseek(file, 10, SEEK_SET);
	read(file, data, 90); 
	printf("%s\n", data);
	close("/dev/simple_char_device");
	return 0;*/
}
