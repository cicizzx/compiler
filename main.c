#include<stdio.h>
#include"grammer.h"
#include"lexical.h"
#include"error.h"
FILE *fp=NULL;
int main()
{
	char path[128]={0};
	
	/*printf("please input path��\n");
	gets(path);
	
	while((fp=fopen(path,"r"))==NULL)
	{
		printf("Can not find the file, please re-input��\n");
		gets(path);
	}*/
	fp=fopen("a.txt","r");
	program();
	
	printf("end!\n");
	fclose(fp);
	return 0;
}


