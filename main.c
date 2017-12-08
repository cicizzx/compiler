#include<stdio.h>
#include"grammer.h"
#include"lexical.h"
#include"error.h"
FILE *fp=NULL;
int main()
{
	char path[1024]={0};
	
	printf("please input path£º\n");
	gets(path);
	
	while((fp=fopen(path,"r"))==NULL)
	{
		printf("Can not find the file, please re-input£º\n");
		gets(path);
	}

	program();
	
	printf("end!\n");
	fclose(fp);
	return 0;
}


