#include<stdio.h>
#include"error.h"
#include"lexical.h"
int errnum=0;
extern int line;
int error(int errortype)
{
	errnum++;
	printf("Error %d:line:%d ",errnum,line+1);
	switch (errortype)
	{
	case UNKNOWN_SYMBOL:
		printf("unknown symbol in line");break;
	case STRING_TOO_LONG:
		printf("string is too long");break;
	case WRONG_FORMAT_INTEGER:
		printf("wrong format integer");break;
	case WITHOUT_DECLARATION:
		printf("identifier is used without declartion");break;
	case RE_DECLARATION:
		printf("re-declaration");break;
	case WRONG_FORMAT_IDEN:
		printf("wrong format identifier");break;
	case WRONG_EXPRESSION:
		printf("wrong expression");break;
	case EMPTY_CONDITION:
		printf("empty condition");break;
	case WRONG_FORMAT_CHAR:
		printf("wrong format char");break;
	case WRONG_CONST_MISSING_TK:
		printf("const statement misses int/char");break;
	case UNFINISHED_STRING:
		printf("\'\"\'dismatch");break;
	case MISSING_SEMICN:
		printf("sentence misses semicn");
		while(read()==SEMICN)	;
		break;
	case PARENT_DISMATCH:
		printf("parent dismatch");break;
	case BRACK_DISMATCH:
		printf("brack dismatch");break;
	}
	printf("\n");
	return errortype;
}
