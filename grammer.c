#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"pcode.h"
#include"grammer.h"
#include"lexical.h"
#include "table.h"
#include"error.h"
#define STRINGLENGTH 128
extern int tokennum,tmp,tptr;
extern char token[];
extern struct tb table[];
int off=0,ifnum=0,donum=0,casenum=0,switchnum=0,offset=0;
char all[]="static";
//＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program()
{
	int rflag=0;
	int type;
	char name[STRINGLENGTH]={0};
	getsym();
	read();
	while(tmp==CONSTTK){
		//第一次const已经被读 
		const_state(all);
	} 
		//可能是变量说明也可能是有返回值函数
	if(tmp==INTTK||tmp==CHARTK){
		var_or_func();	//前yes后no 	
		}
	while(tmp==INTTK||tmp==CHARTK||tmp==VOIDTK){
		if(tmp==INTTK||tmp==CHARTK){
			type=tmp;
			read();
		if(tmp==IDEN){
			strcpy(name,token);
			read();
			//有返回值函数 
			if(tmp==LPARENT){
				//output(DEF_VALFUNC);
				def_func(name,type);
				
			}
	}
	}	//无返回值函数 
	if(tmp==VOIDTK){
			if(read()==IDEN&&keyword()!=MAINTK){
				//output(DEF_VOIDFUNC);
				strcpy(name,token);
				if(read()==LPARENT)
						def_func(name,VOIDTK);				
			}
			//主函数 
			else if(keyword()==MAINTK){
				//output(MAIN_FUNC);
				main_func();
			}   
		} 
	}
	
		

}
void var_or_func()
{
	char name[STRINGLENGTH];
	int length;

	int isint=(tmp==INTTK);
		read();
		if(tmp==IDEN){
			strcpy(name,token);
			read();
			//有返回值函数 
			if(tmp==LPARENT){
				//output(DEF_VALFUNC);
				def_func(name,isint);
				
			}
			
			//变量说明
			else if(token[0]=='['||token[0]==','||token[0]==';'){
				//output(VAR_STATE);				
				switch(token[0]){
					case('['):
						if(read()==INTCON){
							if(seek(name,all)==NOTFOUND) {
								entertablearray(ARRAY_VARIABLE,name,off,(isint?INTTK:CHARTK),all,atoi(token));
								offset=offset+atoi(token)*4;
							}
								
							else{
								error(RE_DECLARATION);
							}
							
							read(); 
							if(token[0]=']'){
								read();
								if(token[0]==','){
									do{
										if(read()==IDEN){
											strcpy(name,token);											
											read();
											if(token[0]==','){
												if(seek(name,all)==NOTFOUND) {
													entertable(SIMPLE_VARIABLE,name,off,(isint?INTTK:CHARTK),all);
													offset=offset+4;
												}
												else{
													error(RE_DECLARATION);
												}
												
												continue;
											} 
											else if(token[0]=='['){
												if((read())==INTCON){
													if(seek(name,all)==NOTFOUND) {
														entertablearray(ARRAY_VARIABLE,name,off,(isint?INTTK:CHARTK),all,atoi(token));
														offset=offset+atoi(token)*4;
													}
														
													else{
														error(RE_DECLARATION);
													}
													
													if(token[0]==']'){
														read();
														if(token[0]==','){
															continue;
														} 
													}
													else {
														error(BRACK_DISMATCH);
													}
												}
											}
										} 
									}while(token[0]==',');
								}
								if(token[0]==';'){
									read();
									if(tmp==INTTK||tmp==CHARTK)
										var_or_func();
								}
								else{
									error(MISSING_SEMICN);
								}
							}
							else {
								error(BRACK_DISMATCH);
							}
						}
						break;
					case(','):
						entertable(SIMPLE_VARIABLE,name,off,(isint?INTTK:CHARTK),all);
						offset=offset+4;
							do{
								if(read()==IDEN){
									strcpy(name,token);
									read();
									if(token[0]==','){
										if(seek(name,all)==NOTFOUND) {
											entertable(SIMPLE_VARIABLE,name,off,(isint?INTTK:CHARTK),all);
											offset=offset+4;
										}
										else{
											error(RE_DECLARATION);
										}
										
										continue;
									} 
									else if(token[0]=='['){
										if(read()==INTCON){
											if(seek(name,all)==NOTFOUND) {
												entertablearray(ARRAY_VARIABLE,name,off,(isint?INTTK:CHARTK),all,atoi(token));
												offset=offset+atoi(token)*4;
											}
												
											else{
												error(RE_DECLARATION);
											}
											read();
											if(token[0]==']'){
												read();
												if(token[0]==','){
													continue;
												} 
											}
											else {
												error(BRACK_DISMATCH);
											}
										}
									}
								} 
							}while(token[0]==',');
							if(token[0]==';'){
								read();
									if(tmp==INTTK||tmp==CHARTK)
										var_or_func();
							}
							else{
									error(MISSING_SEMICN);
								}
							break;
					case(';'):
						if(read()==(INTTK||CHARTK)){						
							//read();
								if(tmp==INTTK||tmp==CHARTK)
									var_or_func();
						}
							break;
				}
				//read();//maybe error 
			}
		} 
} 
// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void const_state(char area[])
{
	
	do{
		def_const(area);
		if(token[0]!=';'){
			error(MISSING_SEMICN);
		}
		read();
	}while(keyword()==CONSTTK);
	
}
//＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
                            //| char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void def_const(char area[])
{
	char name[STRINGLENGTH];
	int positive=0;
	//output(CONST_STATE);
	read();
	if(keyword()==INTTK){
		do{
			if(read()==IDEN){
				strcpy(name,token);				
				if(read()==ASSIGN){
					positive=integer();
					if(seek(name,area)==NOTFOUND) 
						entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
					else{
						error(RE_DECLARATION);
					}
					read(); //单步进入 
				}
			}
		}while(token[0]==',');		
	}
	else if(keyword()==CHARTK){
		do{
			if(read()==IDEN){
				strcpy(name,token);
				if(read()==ASSIGN){
					if(read()==CHARCON){
						if(seek(name,area)==NOTFOUND) 
							entertable(CONSTANT,name,token[1],CHARTK,area);
						else{
							error(RE_DECLARATION);
						}
						
						read();
						
					}
				}
			}
		}while(token[0]==',');
		
	}
	else if(keyword()==IDEN){
		strcpy(name,token);
		error(WRONG_CONST_MISSING_TK);
		if(read()==ASSIGN){
					read();
					if(tmp==PLUS||tmp==MINUS){
						positive=(tmp==PLUS)?1:-1;	
						if(read()==INTCON) {
							entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
							read();
						}
						while(token[0]==','){
							if(read()==IDEN){
								if(read()==ASSIGN){
									positive=integer();
									if(seek(name,area)==NOTFOUND) 
										entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
									else{
										error(RE_DECLARATION);
									}
									
									read(); //单步进入 
								}
							}
						}
					}
					else if(tmp==INTCON){
						entertable(CONSTANT,name,atoi(token),INTTK,area);
						read();
						while(token[0]==','){
							if(read()==IDEN){
								if(read()==ASSIGN){
									positive=integer();
									if(seek(name,area)==NOTFOUND) 
										entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
									else{
										error(RE_DECLARATION);
									}
									
									read(); //单步进入 
								}
							}
						}
					}
					else if(tmp==CHARCON){
						read();
						while(token[0]==','){
							if(read()==IDEN){
								if(read()==ASSIGN){
									if(read()==CHARCON){
										if(seek(name,area)==NOTFOUND) 
											entertable(CONSTANT,name,token[1],CHARTK,area);
										else{
											error(RE_DECLARATION);
										}
										
										read();
										
									} 
								}
							}
						}
					}
				}
	}
}
//＜整数＞        ::= ［＋｜－］＜无符号整数＞｜０
int integer()
{
	read();
	if(token[0]=='+'){
		
		if(read()==INTCON){
			return 1;
		}
	}
	else if(token[0]=='-'){
		
		if(read()==INTCON){
			return -1;
		}
	}
	else if(tmp==INTCON){
		return 1;
	}
}
//＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
void var_state(char area[],int *offfun)
{
	do{
		def_var(area,offfun);
		read();
	}while(tmp==INTTK||tmp==CHARTK);
}
//＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’)
				//{,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
void def_var(char area[],int *offfun)
{
	int isint=(tmp==INTTK)?1:0;
	char name[STRINGLENGTH];
	//output(VAR_STATE);
	if(tmp==INTTK||tmp==CHARTK){
		do{
			if(read()==IDEN){
				strcpy(name,token);
				read();
				if(token[0]==','){
					if(seek(name,area)==NOTFOUND){
						entertable(SIMPLE_VARIABLE,name,*offfun,(isint?INTTK:CHARTK),area);
						*offfun+=4;
					}
						
					else{
						error(RE_DECLARATION);
					} 
					continue;
				} 
				else if(token[0]=='['){
					if(read()==INTCON){
						if(seek(name,area)==NOTFOUND){
							entertablearray(ARRAY_VARIABLE,name,*offfun,(isint?INTTK:CHARTK),area,atoi(token));
							*offfun=*offfun+atoi(token)*4;
						}
						else{
							error(RE_DECLARATION);
						} 
						
						read();
						if(token[0]==']'){
							read();
							if(token[0]==','){
								continue;
							} 
						}
						else {
								error(BRACK_DISMATCH);
							}
					}
				}
				else if(token[0]==';'){
					if(seek(name,area)==NOTFOUND){
						entertable(SIMPLE_VARIABLE,name,*offfun,(isint?INTTK:CHARTK),area);
						*offfun+=4;
					}

					else{
						error(RE_DECLARATION);
					} 
					continue;
				} 
			} 
		}while(token[0]==',');
	}
	
}
//＜无、有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’
void def_func(char name[],int type) 
{
	int para1=tptr+1;
	int offfun=0;
	int paranum=para_table(name,&offfun);	
	four(LABEL,name,"","");
	if(seek(name,all)==NOTFOUND)
		entertablefun(type,name,paranum,para1);
	else{
		error(RE_DECLARATION);
	} 
	
	if(token[0]==')'){
		read();
		if(token[0]=='{'){
			com_state(name,&offfun);
		}
		if(read()==RBRACE)	;
	}
	
	//over
}
//＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void com_state(char area[],int *offfun)
{
	ifnum=0;donum=0;switchnum=0;
	read();
	while(tmp==CONSTTK){
		const_state(area);
	} 
	if(tmp==INTTK||tmp==CHARTK){
		var_state(area,offfun);			
		}
	statements(area); 
	
}
//＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
int para_table(char func[],int *offfun)
{
	int type;
	int num=0;
	do{
		read();
		if(tmp==INTTK||tmp==CHARTK){
			type=tmp;
			read();			
			if(tmp==IDEN){
				if(seek(token,func)==NOTFOUND){
					entertable(PARAMETER,token,*offfun,type,func);
					offfun+=4;}
				else{
					error(RE_DECLARATION);
				} 
				
				num++;
				read();
			}
		}
	}while(token[0]==',');
	return num;
}
//＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
void main_func()
{
	int offfun=0;
	four(LABEL,"main","","");
	if(read()==LPARENT){
		if(read()==RPARENT){
			if(read()==LBRACE){
				com_state(all,&offfun);
			}
		}
	}
}
//＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}error 正负可能不能处理……四元式里面 
//把表达式的每个操作符和操作数分开储存到exp里 
char* expression(char func[])
{
	char exp[1024][STRINGLENGTH]={0};
	int e=0,i=0;
	if(tmp==PLUS||tmp==MINUS){
		strcpy(exp[(e)++],token);
		e--;
		read();
		i=(i==1?1:term(func,exp,&e));
	}
	if(tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
		i=(i==1?1:term(func,exp,&e));
	}
	while(tmp==PLUS||tmp==MINUS){
		//strcpy(exp[(e)++],token);
		read();
		i=(i==1?1:term(func,exp,&e));
	}
	//memset(exp[e-1],0,sizeof(exp[e-1]));
	if(i==0)
		return deal_exp(exp,e-1);
	else return "error";
} 
//＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
int term(char func[],char exp[][STRINGLENGTH],int *e)
{
	do{
		if(tmp==MULT||tmp==DIV){
			read();
		}
		if(factor(func,exp,e))	return 1;
		//read();//预读 
	}while(tmp==MULT||tmp==DIV);
}
//＜因子＞    ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜
//				＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
int factor(char func[],char exp[][STRINGLENGTH],int *e)
{
	char name[STRINGLENGTH]={0};
	switch(tmp){
		case(IDEN):
			if(seek(token,func)!=NOTFOUND)
				;
			else error(WITHOUT_DECLARATION);
			strcpy(name,token);
			read();
			
			if(tmp==LBRACK){//＜标识符＞‘[’＜表达式＞‘]’
				strcat(name,token); 
				read();
				strcat(name,expression(func));
				//strcpy(exp[(*e)++],token);
				if(tmp==RBRACK){
					strcat(name,token);
					strcat(exp[(*e)++],name);
					read();
					strcpy(exp[(*e)++],token);
				}
				else {
					error(BRACK_DISMATCH);
						}
			}
			else if(tmp==LPARENT) {//＜有返回值函数调用语句＞
				strcat(name,token);
				val_para(func,name);//yes
				if(tmp==RPARENT)	{
					strcat(exp[(*e)++],name);
					read();
					strcpy(exp[(*e)++],token);
				}
				else error(WRONG_EXPRESSION);
			}
			else{//iden
				strcat(exp[(*e)++],name);
				strcpy(exp[(*e)++],token);
				if(tmp==SEMICN||tmp==RPARENT||tmp==COMMA||tmp==RBRACK||(tmp>=EQL&&tmp<=NEQ))
					;//(*e)++;
			}
			break;	
		case(PLUS)://整数
			strcpy(exp[(*e)],token);
				if(read()==INTCON){
					strcat(exp[(*e)++],token);
				}
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(MINUS)://整数 
			strcpy(exp[(*e)],token);
				if(read()==INTCON){
					strcat(exp[(*e)++],token);
				}
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(INTCON)://整数 
			strcat(exp[(*e)++],token);
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(CHARCON):
			strcpy(exp[(*e)++],token);
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(LPARENT):
			strcpy(name,token);
			read();
			strcat(name,expression(func));
			if(tmp==RPARENT) {
				strcat(name,token);
				strcpy(exp[(*e)++],name);
				read();
				strcpy(exp[(*e)++],token);

			}		    
			else error(WRONG_EXPRESSION);		
			break;
		default:
			error(WRONG_EXPRESSION);
			read();
			return 1;
	}
	return 0;
}
/*＜语句＞    ::= ＜条件语句＞｜＜循环语句＞｜<情况语句>|‘{’＜语句列＞‘}’｜
				＜有返回值函数调用语句＞; |＜无返回值函数调用语句＞;｜＜赋值语句＞;
					｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;*/
void statement(char func[])
{
	char name[STRINGLENGTH];
	switch(tmp){
		case(IFTK):
			if_state(func);
			break;
		case(DOTK):
			do_while(func);
			break;
		case(SWITCHTK):
			switch_state(func);
			break;
		case(LBRACE):
			read();
			statements(func);
			break;
		case(VOIDTK):
			use_voidfunc(func);
			break;
		case(IDEN)://assign,func
			if(seek(token,func)==NOTFOUND)	error(WITHOUT_DECLARATION);
			strcpy(name,token);
			read();
			if(tmp==ASSIGN||tmp==LBRACK)
				assignment(func,name);
			else if(tmp==LPARENT){
				use_valfunc(func,name);
			}
			break;
		case(SCANFTK):
			scanf_state(func);
			break;
		case(PRINTFTK):
			printf_state(func);
			break;
		case(RETURNTK):
			return_state(func);
			break;
		case(SEMICN):
			//output(EMPTY);
			break;
	}
}
//＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
void assignment(char func[],char name[])
{
	char src[STRINGLENGTH];
	//output(ASSIGNMENT);
	if(tmp==ASSIGN){
		read();
		strcpy(src,expression(func));//
	}
	else if(tmp==LBRACK){
		strcat(name,token);
		read();
		strcat(name,expression(func));
		if(tmp==RBRACK){
			strcat(name,token);
			if(read()==ASSIGN){
				read();
				strcpy(src,expression(func));
			}
			if(tmp==RBRACK||tmp==SEMICN)
			    ;
			else {
				error(BRACK_DISMATCH);
							}
			//read();
		} 
		else if(tmp==ASSIGN){
			error(BRACK_DISMATCH);
			read();
			strcpy(src,expression(func));
			if(tmp==RBRACK||tmp==SEMICN)
			    ;
			else {
				error(BRACK_DISMATCH);
							}
			return;
		}
	}
	four(MOV,name,src,"");
	if(tmp==SEMICN)
				;
	else{
		error(MISSING_SEMICN);
	}
}
//＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞
void if_state(char func[])
{ 

	char a[STRINGLENGTH]={0},b[STRINGLENGTH],c[STRINGLENGTH];
	itoa(ifnum,a,10);
	strcpy(b,a);
	strcpy(c,a);
	//output(IF_STATE);
	if(read()==LPARENT){
		condition(func,strcat(a,"ifEnd"),strcat(b,"ifStart"));
		strcpy(b,c);
		strcpy(a,b);
		four(LABEL,strcat(a,"ifStart"),"","");
		strcpy(a,b);
		read();
		statement(func);
		four(LABEL,strcat(a,"ifEnd"),"","");
		strcpy(a,b);
	}
	else{
			error(PARENT_DISMATCH);
			return;
		}
	ifnum++;
}

//条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
void condition(char func[],char end[],char start[])
{
	int op=0;
	char src1[STRINGLENGTH],src2[STRINGLENGTH];
	read();
	if(tmp==RPARENT){
		error(EMPTY_CONDITION);
		return;
	}
	strcpy(src1,expression(func));
	if(tmp>=EQL&&tmp<=NEQ){
		op=tmp;
		read();
		strcpy(src2,expression(func));
		switch(op){
			case (EQL):
				four(PEQL,src1,src1,src2);
				break;
			case (NEQ):
				four(PNEQ,src1,src1,src2);
				break;
			case (LSS):
				four(PLSS,src1,src1,src2);
				break;
			case (LEQ):
				four(PLEQ,src1,src1,src2);
				break;
			case (GRE):
				four(PGRE,src1,src1,src2);
				break;
			case (GEQ):
				four(PGEQ,src1,src1,src2);
				break;
		}
	}
	if(tmp==RPARENT){
		four(BEQZ,end,src1,"");
		four(BNE,start,src1,"0");
		return;
	}
}
//＜循环语句＞   ::=  do＜语句＞while ‘(’＜条件＞‘)’
void do_while(char func[])
{
	char a[STRINGLENGTH]={0},b[STRINGLENGTH],c[STRINGLENGTH];
	itoa(donum,a,10);
	strcpy(b,a);
	strcpy(c,a);
	//output(DO_WHILE);
	
	if(tmp==DOTK){
		four(LABEL,strcat(a,"doStart"));
		strcpy(a,b);
		read();
		statement(func);
		if(read()==WHILETK){
			if(read()==LPARENT){
				condition(func,strcat(a,"doEnd"),strcat(b,"doStart"));
				strcpy(b,c);
				strcpy(a,b);
			}
		}
	}
	four(LABEL,strcat(a,"doEnd"));
	strcpy(a,b);
	donum++;
}
//＜常量＞   ::=  ＜整数＞|＜字符＞ 
char* constant()
{
	char con[STRINGLENGTH];
	read();
	strcpy(con,token);
	if(tmp==PLUS||tmp==MINUS){
		if(read()==INTCON){
			strcat(con,token);
		}
	}
	if(tmp==INTCON)	;
	else if(tmp==CHARCON){
		;
	}
	return con;
}
//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞ ‘}’
void switch_state(char func[])
{
	char exp[STRINGLENGTH]={0};
	char a[STRINGLENGTH]={0},b[STRINGLENGTH];
	itoa(switchnum,a,10);
	strcpy(b,a);
	casenum=0;
	//output(SWITCH_STATE);
	if(read()==LPARENT){
		read();
		strcpy(exp,expression(func));
		if(tmp==RPARENT&&read()==LBRACE){
			case_list(func,exp,strcat(a,"switchEnd"));
			strcpy(a,b);
		}
	}
	else{
			error(PARENT_DISMATCH);
			return;
		}
	memset(a,0,sizeof(a));
	itoa(casenum,a,10);
	four(LABEL,strcat(a,"case"));
	strcpy(a,b);
	four(LABEL,strcat(a,"switchEnd"));
	strcpy(a,b);
	switchnum++;
}
//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
void case_list(char func[],char exp[],char end[])
{	
	if(read()==CASETK){
		do{
			case_state(func,exp,end);
		}while(read()==CASETK);
		if(tmp==RBRACE)
			;
	}
}
//＜情况子语句＞  ::=  case＜常量＞：＜语句＞
void case_state(char func[],char exp[],char end[])
{
	char con[STRINGLENGTH];
	char a[STRINGLENGTH]={0},b[STRINGLENGTH];
	itoa(casenum,a,10);
	strcpy(b,a);
	//output(CASE_STATE);
	four(LABEL,strcat(a,"case"));
	strcpy(a,b);
	strcpy(con,constant());
	memset(a,0,sizeof(a));
	itoa(casenum+1,a,10);
	four(BNE,strcat(a,"case"),con,exp);
	strcpy(a,b);
	if(read()==COLON){
		read();
		statement(func);
		four(J,end);
	}
	casenum++;
}
//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
void use_valfunc(char func[],char name[])
{
	four(CALL,name,"","");
	//output(VALFUNC);
	if(tmp==LPARENT){
			val_para(func,name);
		}
	if(read()==SEMICN||tmp==MULT||tmp==DIV||tmp==PLUS||tmp==MINUS||tmp==RPARENT||tmp==RBRACK)
				strcat(name,token);
}
//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
void use_voidfunc(char name[])
{
	char debug[STRINGLENGTH]={0};
	four(CALL,name,"","");
	//output(VOIDFUNC);
	strcpy(debug,name);

	if(tmp==IDEN){
		if(read()==LPARENT){
			val_para(name,debug);
		}
	}
	if(read()==SEMICN)
				;
	else{
		error(MISSING_SEMICN);
	}
}
//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void val_para(char func[],char returnexp[])
{
	read();
	if(tmp==RPARENT){
		strcat(returnexp,token);
		return ;}
	else if(returnexp!=NULL)
		strcat(returnexp,expression(func));
	while(tmp==COMMA){
		read();
		if(returnexp!=NULL)
			strcat(returnexp,expression(func));
	}
	if(tmp==RPARENT){
		if(returnexp!=NULL)
			strcat(returnexp,token);
			}
}
//＜语句列＞   ::=｛＜语句＞｝
void statements(char func[]) 
{
	while(tmp!=RBRACE){
		statement(func);
		read();
	}
} 
//＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
void scanf_state(char func[])
{
	//output(SCANF_STATE);
	if(tmp==SCANFTK){
		if(read()==LPARENT){
			do{
				if(read()==IDEN){
					int i;
					if((i=seek(token,func))==NOTFOUND)	error(WITHOUT_DECLARATION);
					else{
						four(SCANF,token,table[i].length,"");
						read();
				}
				}
			}while(tmp==COMMA);
			if(tmp==RPARENT){
				;
			}
		}
		else{
			error(PARENT_DISMATCH);
			return;
		}
		if(read()==SEMICN)
				;
		else{
		error(MISSING_SEMICN);
	}
	}
}
//＜写语句＞    ::=  printf‘(’＜字符串＞,＜表达式＞‘)’|printf ‘(’＜字符串＞‘)’
//					|printf ‘(’＜表达式＞‘)’
void printf_state(char func[])
{
	char string[STRINGLENGTH];
	//output(PRINTF_STATE);
	if(tmp==PRINTFTK){
		if(read()==LPARENT){
			read();
				if(tmp==STRCON){
					strcpy(string,token);
					read();
					if(tmp==RPARENT){
						four(PRINTF,string,"","");;
					}
					else if(tmp==COMMA){
						read();
						if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
							four(PRINTF,string,expression(func),"");
							if(tmp==RPARENT){
								;
							}
						}
					}
					}
				if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
					four(PRINTF,"",expression(func),"");
					if(tmp==RPARENT){
						;
					}
					else{
						error(PARENT_DISMATCH);
						return;
					}
				}
			if(read()==SEMICN)
				;
			else{
					error(MISSING_SEMICN);
				}
		}
		else{
			error(PARENT_DISMATCH);
			return;
		}
	}
}
//＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
void return_state(char func[])
{
	//output(RETURN_STATE);
	if(tmp==RETURNTK){
		read();
		if(tmp==LPARENT){
			read();
			if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
				four(RETURN,expression(func),"","");
				if(tmp==RPARENT){
					read();//
					if(tmp==SEMICN){
						;
					}
					else{
						error(MISSING_SEMICN);
					}
				}
			}
			if(tmp==SEMICN){
						;
					}
			else{
				error(MISSING_SEMICN);
			}
		}
		else if(tmp==SEMICN){
			four(RETURN,"","","");
		}
		else{
			error(MISSING_SEMICN);
		}
	}
}
//

/*void //output(int a)
{
	printf("This is a");
	switch(a){
		case(CONST_STATE):
			printf(" constant description.\n");
			break;
		case(VAR_STATE):
			printf(" var description.\n");
			break;
		case(DEF_VALFUNC):
			printf(" return value function definition.\n");
			break;
		case(DEF_VOIDFUNC):
			printf(" void function definition.\n");
			break;
		case(MAIN_FUNC):
			printf(" main function statement.\n");
			break;
		case(IF_STATE):
			printf("n if statement.\n");
			break;
		case(DO_WHILE):
			printf(" do-while statement.\n");
			break;
		case(SWITCH_STATE):
			printf(" switch statement.\n");
			break;
		case(CASE_STATE):
			printf(" case statement.\n");
			break;
		case(VALFUNC):
			printf(" return value function use.\n");
			break;
		case(VOIDFUNC):
			printf(" void function use.\n");
			break;
		case(ASSIGNMENT):
			printf("n assignment.\n");
			break;
		case(PRINTF_STATE):
			printf(" printf statement.\n");
			break;
		case(SCANF_STATE):
			printf(" scanf statement.\n");
			break;
		case(RETURN_STATE):
			printf(" return statement.\n");
			break;
		case(EMPTY):
			printf("n empty statement.\n");
			break;
	}
}*/
