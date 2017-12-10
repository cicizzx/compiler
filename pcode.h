#ifndef PCODE_H
#define PCODE_H
#define STRINGLENGTH 128
enum pcode_name{
	PADD,PSUB,PDIV,PMUL,MOV,
	PEQL,PNEQ,PGRE,PGEQ,PLSS,PLEQ,
	BEQZ,J,BNE,CALL,
	RETURN,PRINTF,LABEL,SCANF
};
typedef struct node{
	char name[1024];
	int ino;
	char op;
	struct node *lchild,*rchild,*parent;
}tree,*ptree; 
struct polish{
	char iden[1024];
	char oprator;
};
int ov(char);
char* inorder(ptree t);
int bra(int i,char c[][STRINGLENGTH]);//返回i值继续循环 
char* deal_exp(char c[][STRINGLENGTH],int n);
#endif
