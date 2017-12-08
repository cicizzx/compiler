#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#define STRINGLENGTH 512
FILE *fp=NULL,*f=NULL;
int line=0,tokennum=0,res=0;
char token[1024]={0},ch=0;
char getsym();
int chartype(char);
//void delete_space();
// void back(int);
 void clear();
enum symbol{
	IFTK,	//if
	DOTK,//do
	WHILETK,//while
	SWITCHTK,//switch
	CASETK,//case
	RETURNTK,//return
	SCANFTK,//scanf
	PRINTFTK,//printf
	MAINTK,//main
	CONSTTK,//const
	INTTK,//int
	CHARTK,//char
	VOIDTK,//void
	IDEN,//�Ǳ����ִ�
	INTCON,//����
	CHARCON,//char
	STRCON,//string
	SPACE ,//�ո�س��Ʊ� 
	PLUS ,//�ӷ������ 
	MINUS ,//���������
	MULT ,//�Ǻ�
	DIV ,//����
	LPARENT ,//������
	RPARENT ,//������
	LBRACE ,//������
	RBRACE ,//�һ�����
	LBRACK,//������
	RBRACK,//�ҷ����� 
	COMMA ,//����
	SEMICN ,//�ֺ�
	COLON ,//ð��
	ASSIGN ,//��ֵ����
	EQL ,//���ж�
	GRE ,//���ں� 
	GEQ,//���ڵ���
	LSS,//С�ں�
	LEQ,//С�ڵ���
	NEQ,//�����ں�
};
enum charsymbol{
	SPACEC=1,//�ո�
	TABC,//�Ʊ�
	NEWLINEC,//�س�
		
	DIGITC,//����
	LETTERC,//��ĸ
	
};
int main()
{
	char path[STRINGLENGTH]={0};

	printf("��ѡ���������ı��ľ���·����\n");
	gets(path);
	f=fopen("res.txt","w");
	while((fp=fopen(path,"r"))==NULL)
	{
		printf("�Ҳ������ļ������������룺\n");
		gets(path);
	}
	getsym();
	do{
		getword();
		
	}while(ch!=EOF);
	fprintf(f,"�����ϣ�\n");
	fclose(fp);
	fclose(f);
	return 0;
}
int getword()
{
	//�ַ���������������� 
	int flag=0;//���Ż��ǵ���
	int id=0;//��ʶ����������
	//delete_space();
	token[tokennum++]=ch;
	//fprintf(f,"now:%d\n",ftell(fp));
	{	while(chartype(token[tokennum-1])==DIGITC||chartype(token[tokennum-1])==LETTERC){
			if(chartype(token[tokennum-1])==LETTERC) 
				id=1;
			token[tokennum++]=getsym();
			flag=1;
		}	
		if(flag)
			token[tokennum-1]=0;
		if(flag==1&&id==1){
			//back(1);
			if(keyword(token)==IDEN&&chartype(token[0])!=LETTERC)
				fprintf(f,"error����ʶ��Ӧ������ĸ���»��߿�ͷ��\n");
			clear();
			return 0;
		}
		else if(flag==1&&id==0){
			//back(1);
			fprintf(f,"%d %s %s\n",res++,token,"NUMBER");
			clear();
			return 1;
		}
	} 
	//�ַ������� 
	{
		if(token[0]=='\"'||token[0]=='\'') {
			token[tokennum++]=getsym();
			while(token[tokennum-1]!='\"'&&token[tokennum-1]!='\''&&token[tokennum-1]!='\n'&&token[tokennum-1]!=EOF) {
				flag=1;
				token[tokennum++]=getsym();
			}
			if(flag){
				token[tokennum]=0;
				if(token[tokennum-1]=='\"'){
					fprintf(f,"%d %s %s\n",res++,token,"STRING");
					getsym();
					clear();
					return 1;
				}
				else if(token[tokennum-1]=='\''){
					if(strlen(token)==3)
						fprintf(f,"%d %s %s\n",res++,token,"CHAR");
					else{
						fprintf(f,"%d %s %s\n",res++,token,"CHAR");
						fprintf(f,"��������ֻ����һ���ַ�\n");
					}
					getsym();
					clear();
					return 1;
				}
				 else if(token[tokennum-1]=='\n'||token[tokennum-1]==EOF){
				 	if(strlen(token)==2)
				 		fprintf(f,"%d %s %s\n",res++,token,"CHAR");
				 	else fprintf(f,"%d %s %s\n",res++,token,"STRING");
				 	fprintf(f,"ȱ��һ������\n");
				 	getsym();
					clear();
					return 1;
				 	
				 }
			}
				
		
		}
	} 
	 
	//������Ŵ��� 

	{
		if(token[tokennum-1]=='!'||token[tokennum-1]=='='||token[tokennum-1]=='>'||token[tokennum-1]=='<'){
			getsym();
			
			if(ch=='='){
				token[tokennum++]=ch;
				getsym();
			}
			flag=1;
		}
		else if(token[tokennum-1]!=' '&&token[tokennum-1]!='\t'&&chartype(token[tokennum-1])!=LETTERC&&chartype(token[tokennum-1])!=DIGITC){
			token[tokennum++]=getsym();
			flag=2;
		}
		if(flag==2)
			token[tokennum-1]=0;
		if(flag!=0&&symtype()==0)	{
			fprintf(f,"�ķ���û�ж��壺%s\n",token);
			clear();
		}
	}
	//�հ��ַ�����
	{
	
	while(token[tokennum-1]==' '||token[tokennum-1]=='\t'){
		token[tokennum-1]=getsym();
	} 
	while(ch=='\n'){
		line++;
		ch=fgetc(fp);
	}
	clear();
	}
}

char getsym()
{	
	//nlflag=0;
	ch=fgetc(fp);
	//fprintf(f,"now:%d\n",ftell(fp));	
	return ch;
}

int stringtype(char c)
{
	return (c==32||c==33||(c>=35&&c<=126));
}

int chartype(char c)
{
	if(c<='9'&&c>='0')	return DIGITC;
	else if(c<='z'&&c>='a')	return LETTERC;
	else if(c<='Z'&&c>='A')	return LETTERC;
	else if(c=='_')			return LETTERC;

	else if(c=='\t')			return TABC;
	else if(c==' ')			return SPACEC ;
	else if(c=='\n')			return NEWLINEC;
	

	/*else if(ch=='+')			return PLUSC;
	else if(ch=='-')			return MINUSC;
	else if(ch=='*')			return STARC;
	else if(ch=='/')			return DIVIC;
	
	else if(ch==':')			return COLONC;
	else if(ch==';')			return SEMIC;
	else if(ch=='=')			return EQUC;
	else if(ch==',')			return COMMAC;
	else if(ch=='!')			return EXCLAMC;
	
	else if(ch=='\'')			return SINQUOC;
	else if(ch=='\"')			return DBLQUOC;
	else if(ch=='(')			return LPARC;
	else if(ch==')')			return RPARC;
	else if(ch=='<')			return LABRAC;
	else if(ch=='>')			return RABRAC;
	else if(ch=='{')			return LBRACEC;
	else if(ch=='}')			return RBRACEC;
	else if(ch=='[')			return LBRACKC;
	else if(ch==']')			return RBRACKC;

	return 0;*/
}

int symtype()
{
	if(strcmp(token,"+")==0){
		fprintf(f,"%d %s %s\n",res++,token,"PLUSC");
		return 1;
	}
	if(strcmp(token,"-")==0){
		fprintf(f,"%d %s %s\n",res++,token,"MINUSC");
		return 1;
	}
	if(strcmp(token,"*")==0){
		fprintf(f,"%d %s %s\n",res++,token,"MULC");
		return 1;
	}
	if(strcmp(token,"/")==0){
		fprintf(f,"%d %s %s\n",res++,token,"DIVIC");
		return 1;
	}
	
	if(strcmp(token,"<")==0){
		fprintf(f,"%d %s %s\n",res++,token,"LSS");
		return 1;
	}
	if(strcmp(token,">")==0){
		fprintf(f,"%d %s %s\n",res++,token,"GRE");
		return 1;
	}
	if(strcmp(token,"<=")==0){
		fprintf(f,"%d %s %s\n",res++,token,"LEQ");
		return 1;
	}
	if(strcmp(token,">=")==0){
		fprintf(f,"%d %s %s\n",res++,token,"GEQ");
		return 1;
	}
	if(strcmp(token,"!=")==0){
		fprintf(f,"%d %s %s\n",res++,token,"NEQ");
		return 1;
	}
	if(strcmp(token,"==")==0){
		fprintf(f,"%d %s %s\n",res++,token,"EQL");
		return 1;
	}
	
	if(strcmp(token,":")==0){
		fprintf(f,"%d %s %s\n",res++,token,"COLONC");
		return 1;
	}
	if(strcmp(token,";")==0){
		fprintf(f,"%d %s %s\n",res++,token,"SEMIC");
		return 1;
	}
	if(strcmp(token,"=")==0){
	fprintf(f,"%d %s %s\n",res++,token,"EQUC");
		return 1;	
	}
	if(strcmp(token,",")==0){
		fprintf(f,"%d %s %s\n",res++,token,"COMMAC");
		return 1;
	}
	
	if(strcmp(token,"\'")==0){
		fprintf(f,"%d %s %s\n",res++,token,"SINQUOC");
		return 1;
	}
	if(strcmp(token,"\"")==0){
		fprintf(f,"%d %s %s\n",res++,token,"DBLQUOC");
		return 1;
	}
	if(strcmp(token,"(")==0){
		fprintf(f,"%d %s %s\n",res++,token,"LPARC");
		return 1;
	}
	if(strcmp(token,")")==0){
		fprintf(f,"%d %s %s\n",res++,token,"RPARC");
		return 1;
	}
	if(strcmp(token,"{")==0){
		fprintf(f,"%d %s %s\n",res++,token,"LBRACEC");
		return 1;
	}
	if(strcmp(token,"}")==0){
		fprintf(f,"%d %s %s\n",res++,token,"RBRACEC");
		return 1;
	}
	if(strcmp(token,"[")==0){
		fprintf(f,"%d %s %s\n",res++,token,"LBRACKC");
		return 1;
	}	
	if(strcmp(token,"]")==0){
		fprintf(f,"%d %s %s\n",res++,token,"RBRACKC");
		return 1;
	}	
	return 0;
}

int keyword()
{
	if(strcmp(token,"if")==0){
		fprintf(f,"%d %s %s\n",res++,token,"IFTK");
		return IFTK;
	}		
	else if(strcmp(token,"printf")==0){
		fprintf(f,"%d %s %s\n",res++,token,"PRINTFTK");
		return PRINTFTK;
	}	
	else if(strcmp(token,"scanf")==0){
		fprintf(f,"%d %s %s\n",res++,token,"SCANFTK");
		return SCANFTK;
	}	
	else if(strcmp(token,"const")==0)	{
		fprintf(f,"%d %s %s\n",res++,token,"CONSTTK");
		return CONSTTK;
	
	} 
	else if(strcmp(token,"int")==0)		{
		fprintf(f,"%d %s %s\n",res++,token,"INTTK");
		return INTTK;
		} 
	else if(strcmp(token,"char")==0)		{
		fprintf(f,"%d %s %s\n",res++,token,"CHARTK");
		return CHARTK;
	} 
	else if(strcmp(token,"do")==0)		{
		fprintf(f,"%d %s %s\n",res++,token,"DOTK");
		return DOTK;
	} 
	else if(strcmp(token,"while")==0)	{
		fprintf(f,"%d %s %s\n",res++,token,"WHILETK");
		return WHILETK;
	} 
	else if(strcmp(token,"return")==0)	{
		fprintf(f,"%d %s %s\n",res++,token,"RETURNTK");
		return RETURNTK;
	}
	else if(strcmp(token,"void")==0)		{
		fprintf(f,"%d %s %s\n",res++,token,"VOIDTK");
		return VOIDTK;
	}
	else if(strcmp(token,"main")==0)		{
		fprintf(f,"%d %s %s\n",res++,token,"MAINTK");
		return MAINTK;
	}
	else if(strcmp(token,"switch")==0)	{
		fprintf(f,"%d %s %s\n",res++,token,"SWITCHTK");
		return SWITCHTK;
	}
	else if(strcmp(token,"case")==0)	{
		fprintf(f,"%d %s %s\n",res++,token,"CASETK");
		return CASETK;
		}
	fprintf(f,"%d %s %s\n",res++,token,"IDEN");
	return IDEN;
}

void clear()
{
	memset(token,0,sizeof(token));
	tokennum=0;
}

