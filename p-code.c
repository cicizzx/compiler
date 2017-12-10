#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include"pcode.h"
#define STRINGLENGTH 128
ptree stack[1024],p;
struct polish po[1024];
int stop=-1,potop=-1,o=-1;//o保持和c中符号一致就行 用来比较 
void four(int type,char dst[],char src1[],char src2[])
{
	switch(type){
		case(PADD):
			printf("ADD %s %s %s\n",dst,src1,src2);
			break;
		case(PSUB):
			printf("SUB %s %s %s\n",dst,src1,src2);
			break;
		case(PMUL):
			printf("MUL %s %s %s\n",dst,src1,src2);
			break;
		case(PDIV):
			printf("DIV %s %s %s\n",dst,src1,src2);
			break;
		case(MOV):
			printf("MOV %s %s %s\n",dst,src1,src2);
			break;
		case(PEQL):
			printf("EQL %s %s %s\n",dst,src1,src2);
			break;
		case(PNEQ):
			printf("NEQ %s %s %s\n",dst,src1,src2);
			break;
		case(PLSS):
			printf("LSS %s %s %s\n",dst,src1,src2);
			break;
		case(PLEQ):
			printf("LEQ %s %s %s\n",dst,src1,src2);
			break;
		case(PGRE):
			printf("GRE %s %s %s\n",dst,src1,src2);
			break;
		case(PGEQ):
			printf("GEQ %s %s %s\n",dst,src1,src2);
			break;
		case(BEQZ):
			printf("BEQZ %s %s\n",dst,src1);
			break;
		case(J):
			printf("J %s\n",dst);
			break;
		case(BNE):
			printf("BNE %s %s %s\n",dst,src1,src2);
			break;
		case(RETURN):
			printf("RETURN %s\n",dst);
			break;
		case(PRINTF):
			printf("PRINTF %s %s\n",dst,src1);
			break;
		case(LABEL):
			printf("LABEL %s\n",dst);
			break;
		case(SCANF):
			printf("SCANF %s %s %s\n",dst,src1,src2);
			break;
		case(CALL):
			printf("CALL %s\n",dst);
			break;
		/*case(DIV):
			printf("DIV %s %s %s\n",dst,src1,src2);
			break;*/
		
	}
}
char* deal_exp(char c[][STRINGLENGTH],int n)//c改为二层数组，需要存标识符！改输出 
{
	int i,j=0,k=0,ltop=-1;
	char swap[3],last[50][3]; 
	for(i=0;i<n;i++){
		if(ov(c[i])==-1){
			potop++;
			strcpy(po[potop].iden,c[i]);
			po[potop].oprator=-1;
		} 
		else{//括号 
			o++;
			if(c[i][0]=='('){
				i=bra(i,c);//c[i-1]==')'||
			}
			else{
				strcpy(last[++ltop],c[i]);
				//last[++ltop]=c[i][0];					
					if(ltop==-1||ov(c[i])>ov(last[ltop-1]))
						;
//					else if(c[i]=='('){
//						i=bra(i);
//					}
					else{
						if(ov(last[ltop-1])>=ov(c[i])){
							strcpy(swap,c[i]);
							//swap=c[i][0];
							ltop--;
							while(ov(last[ltop])>=ov(c[i])){
								po[++potop].oprator=last[ltop--][0];
							}
							strcpy(last[++ltop],swap);
							//last[++ltop]=swap;
						}
						else	po[++potop].oprator=last[ltop--][0];
					}
			}
		}
	}
	for(ltop>-1;ltop>-1;ltop--)
		po[++potop].oprator=last[ltop][0];
	//建二叉树
	j=0;
	for(i=0;i<=potop;i++){
		if(po[i].oprator<0){//如果是操作数，则建立一个单节点树并将指向他的指针推入栈中； 
			p=(ptree)malloc(sizeof(tree));
			strcpy(p->name,po[i].iden);
			p->parent=p->lchild=p->rchild=NULL;
			stack[++stop]=p;
			p->ino=-1;
			p->op=-1;
		}
		else {//就从栈中弹出指向两棵树T1和T2的指针（T1先弹出）并形成一棵新树，树根为该运算符，它的左、右子树分别指向T2和T1，然后将新树的指针压入栈中。
			p=(ptree)malloc(sizeof(tree));
			p->op=po[i].oprator;
			stack[stop]->parent=p;
			p->rchild=stack[stop--];
			stack[stop]->parent=p;
			p->lchild=stack[stop--];
			p->parent=NULL;
			stack[++stop]=p;
			p->ino=j++;
		}
	}
	while(p->parent!=NULL){
		p=p->parent;
	}
	
	//printf("\n%d",inorder(p));
	return inorder(p);
}
int ov(char a[])
{
	if(strcmp(a,"(")==0||strcmp(a,")")==0)
		return 3;
	else if(strcmp(a,"*")==0||strcmp(a,"/")==0)
		return 2;
	else if(strcmp(a,"+")==0||strcmp(a,"-")==0)
		return 1;
	else return -1;
	/*if(a=='('||a==')')
		return 3;
	else if(a=='*'||a=='/')
		return 2;
	else if(a=='+'||a=='-')
		return 1;
	else return -1;*/
}
int bra(int i,char c[][STRINGLENGTH])
{
	char last[50][3],swap;
	int ltop=-1;
	i++;
	while(c[i][0]!=')'){
		if(ov(c[i])==-1){
				potop++;
				strcpy(po[potop].iden,c[i]);
				po[potop].oprator=0;
			} 
		else {
			o++;
			if(c[i][0]=='('){
				i=bra(i,c);
			}
			else{
				strcpy(last[++ltop],c[i]);
				//last[++ltop]=c[i][0];					
					if(ltop==-1||ov(c[i])>ov(last[ltop-1]))
						;
					else if(c[i][0]=='('){
						i=bra(i,c);
					}
					else{
						if(ov(last[ltop-1])>=ov(c[i])){
							strcpy(swap,c[i]);
							//swap=c[i][0];
							ltop--;
							while(ov(last[ltop])>=ov(c[i][0])){
								po[++potop].oprator=last[ltop--];
							}
							//last[++ltop]=swap;
							strcpy(last[++ltop],swap);
						}
						else	po[++potop].oprator=last[ltop--][0];
					}
			}
		}
		i++;
	}
	o++;
	for(ltop>=0;ltop>=0;ltop--)
		po[++potop].oprator=last[ltop][0];
	return i; 
}
char* inorder(ptree t)
{
	char tmp[STRINGLENGTH]={0};
	char inttoa[STRINGLENGTH]={0};
	strcpy(tmp,"t");
	if(t!=NULL){
		if(t->op>0){
			strcat(tmp,itoa(t->ino,inttoa,10));
			if(t->op=='+'){
				four(PADD,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return ;
			}
			else if(t->op=='-'){
				four(PSUB,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return tmp;
			}
			else if(t->op=='*'){
				four(PMUL,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return tmp;
			}
			else if(t->op=='/'){
				four(PDIV,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return tmp;
			}
		}
		return t->name;
}

}