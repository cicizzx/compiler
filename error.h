#ifndef ERROR_H
#define ERROR_H
enum errorname
{
	NO_ERROR=0,
	UNKNOWN_SYMBOL=1,//unknown symbol in line
	STRING_TOO_LONG,//string is too long
	WRONG_FORMAT_INTEGER,//wrong format integer
	WRONG_FORMAT_IDEN,//wrong format IDEN
	WRONG_FORMAT_CHAR,
	WRONG_CONST_MISSING_TK,
	WRONG_EXPRESSION,
	UNFINISHED_STRING,//\'\"\'dismatch
	MISSING_SEMICN,//sentence misses semicn
	PARENT_DISMATCH,//parent dismatch
	BRACK_DISMATCH,
	WITHOUT_DECLARATION,
	RE_DECLARATION,
	EMPTY_CONDITION
};
int error (int errortype);
#endif
