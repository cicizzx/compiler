#include<stdio.h>
#include"grammer.h"
#include"lexical.h"
#include"error.h"
extern int tokennum,tmp,tptr;
extern char token[];
char all[]="static";
//＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program()
{
	int rflag=0;
	
	getsym();
	read();
	while(tmp==CONSTTK){
		//第一次const已经被读 
		const_state(all);
	} 
		//可能是变量说明也可能是有返回值函数
	if(tmp==INTTK||tmp==CHARTK){
		var_or_deffunc();	//前yes后no 	
		}
	while(tmp==INTTK||tmp==CHARTK||tmp==VOIDTK){
		if(tmp==INTTK||tmp==CHARTK){
			read();
		if(tmp==IDEN){
			read();
			//有返回值函数 
			if(tmp==LPARENT){
				output(DEF_VALFUNC);
				def_func(all);
				
			}
	}
	}	//无返回值函数 
	if(tmp==VOIDTK){
			if(read()==IDEN&&read()==LPARENT){
				output(DEF_VOIDFUNC);
				def_func(all);				
			}
			//主函数 
			else if(keyword()==MAINTK){
				output(MAIN_FUNC);
				main_func();
			}   
		} 
	}
	
		

}
void var_or_deffunc()
{
	char name[];
	int length;
	int isint=(tmp==INTTK);
		read();
		if(tmp==IDEN){
			strcpy(name,token);
			read();
			//有返回值函数 
			if(tmp==LPARENT){
				output(DEF_VALFUNC);
				def_func(name);
				
			}
			
			//变量说明
			else if(token[0]=='['||token[0]==','||token[0]==';'){
				output(VAR_STATE);				
				switch(token[0]){
					case('['):
						if(read()==INTCON){
							if(seek(name,area)==NOTFOUND) 
								entertablearray(ARRAY_VARIBLE,name,0,(isint?INTTK:CHARTK),all,atoi(token));
							else{
								error(RE_DECLARTION);
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
												if(seek(name,area)==NOTFOUND) 
													entertable(SIMPLE_VARIBLE,name,0,(isint?INTTK:CHARTK),all);
												else{
													error(RE_DECLARTION);
												}
												
												continue;
											} 
											else if(token[0]=='['){
												if((read())==INTCON){
													if(seek(name,area)==NOTFOUND) 
														entertablearray(ARRAY_VARIBLE,name,0,(isint?INTTK:CHARTK),all,atoi(token));
													else{
														error(RE_DECLARTION);
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
										var_or_deffunc();
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
						entertable(SIMPLE_VARIBLE,name,0,(isint?INTTK:CHARTK),all);
							do{
								if(read()==IDEN){
									strcpy(name,token);
									read();
									if(token[0]==','){
										if(seek(name,area)==NOTFOUND) 
											entertable(SIMPLE_VARIBLE,name,0,(isint?INTTK:CHARTK),all);
										else{
											error(RE_DECLARTION);
										}
										
										continue;
									} 
									else if(token[0]=='['){
										if(read()==INTCON){
											if(seek(name,area)==NOTFOUND) 
											entertablearray(ARRAY_VARIBLE,name,0,(isint?INTTK:CHARTK),all,atoi(token));
										else{
											error(RE_DECLARTION);
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
										var_or_deffunc();
							}
							else{
									error(MISSING_SEMICN);
								}
							break;
					case(';'):
						if(read()==(INTTK||CHARTK)){						
							//read();
								if(tmp==INTTK||tmp==CHARTK)
									var_or_deffunc();
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
	char name[];
	int positive=0;
	output(CONST_STATE);
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
						error(RE_DECLARTION);
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
							error(RE_DECLARTION);
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
										error(RE_DECLARTION);
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
										error(RE_DECLARTION);
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
											error(RE_DECLARTION);
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
void var_state(char area[])
{
	do{
		def_var(area);
		read();
	}while(tmp==INTTK||tmp==CHARTK);
}
//＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’)
				//{,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
void def_var(char area[])
{
	output(VAR_STATE);
	int isint=(tmp==INTTK);
	char name[];
	if(tmp==INTTK||tmp==CHARTK){
		do{
			if(read()==IDEN){
				strcmp(name,token);
				read();
				if(token[0]==','){
					if(seek(name,area)==NOTFOUND)
						entertable(SIMPLE_VARIBLE,name,0,(isint?INTTK:CHARTK),area);
					else{
						error(RE_DECLARATION);
					} 
					continue;
				} 
				else if(token[0]=='['){
					if(read()==INTCON){
						if(seek(name,area)==NOTFOUND)
							entertablearray(ARRAY_VARIBLE,name,0,(isint?INTTK:CHARTK),area,atoi(token));
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
	}
	
}
//＜无、有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’
void def_func(char name[]) 
{
	int para1=tptr+1;
	int paranum=para_table(name);
	if(seek(name,all)==NOTFOUND)
		entertablefun(type,name,paranum,para1);
	else{
		error(RE_DECLARATION);
	} 
	
	if(token[0]==')'){
		read();
		if(token[0]=='{'){
			com_state(name);
		}
		if(read()==RBRACE)	;
	}
	
	//over
}
//＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void com_state(char area[])
{
	read();
	while(tmp==CONSTTK){
		const_state(area);
	} 
	if(tmp==INTTK||tmp==CHARTK){
		var_state(area);			
		}
	statements(area); 
	
}
//＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
int para_table(char func[])
{
	int type;
	int num=0;
	do{
		read();
		if(tmp==INTTK||tmp==CHARTK){
			type=tmp;
			read();			
			if(tmp==IDEN){
				if(seek(name,func)==NOTFOUND)
					entertable(PERAMETER,token,0,type,func);
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
	if(read()==LPARENT){
		if(read()==RPARENT){
			if(read()==LBRACE){
				com_state(all);
			}
		}
	}
}
//＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
void expression(char func[])
{
	if(tmp==PLUS||tmp==MINUS){
		read();
		term(func);
	}
	if(tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
		term(func);
	}
	while(tmp==PLUS||tmp==MINUS){
		read();
		term(func);
	}
	
} 
//＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void term(char func[])
{
	do{
		if(tmp==MULT||tmp==DIV){
			read();
		}
		factor(func);
		//read();//预读 
	}while(tmp==MULT||tmp==DIV);
}
//＜因子＞    ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜
//				＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
void factor(char func[])
{
	switch(tmp){
		case(IDEN):
			if(seek(token,func)!=NOTFOUND)
				;
			else error(WITHOUT_DECLARATION);
			read();
			if(tmp==LBRACK){//＜标识符＞‘[’＜表达式＞‘]’
				read();
				expression(func);
				if(tmp==RBRACK){
					read();
				}
				else {
					error(BRACK_DISMATCH);
						}
			}
			else if(tmp==LPARENT) {//＜有返回值函数调用语句＞
				val_para(func);//yes
				if(tmp==RPARENT)	read();
				else error(WRONG_EXPRESSION);
			}
			break;	
		case(MULT)://整数
				if(read()==INTCON){
					;
				}
			read();
			break;
		case(DIV)://整数 
				if(read()==INTCON){
					;
				}
			read();
			break;
		case(INTCON)://整数 
			read();
			break;
		case(CHARCON):
			read();
			break;
		case(LPARENT):
			read();
			expression(func);
			if(tmp==RPARENT) 
			    read();
			else error(WRONG_EXPRESSION);		
			break;
		default:
			error(WRONG_EXPRESSION);
			read();
	}
}
/*＜语句＞    ::= ＜条件语句＞｜＜循环语句＞｜<情况语句>|‘{’＜语句列＞‘}’｜
				＜有返回值函数调用语句＞; |＜无返回值函数调用语句＞;｜＜赋值语句＞;
					｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;*/
void statement(char func[])
{
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
			read();
			if(tmp==ASSIGN||tmp==LBRACK)
				assignment(func);
			else if(tmp==LPARENT){
				use_valfunc(func);
			}
			break;
		case(SCANFTK):
			scanf_state();
			break;
		case(PRINTFTK):
			printf_state(func);
			break;
		case(RETURNTK):
			return_state(func);
			break;
		case(SEMICN):
			output(EMPTY);
			break;
	}
}
//＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
void assignment(char func[])
{
	output(ASSIGNMENT);
	if(tmp==ASSIGN){
		read();
		expression(func);//
	}
	else if(tmp==LBRACK){
		read();
		expression(func);
		if(tmp==RBRACK){
			if(read()==ASSIGN){
				read();
				expression(func);
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
			expression(func);
			if(tmp==RBRACK||tmp==SEMICN)
			    ;
			else {
				error(BRACK_DISMATCH);
							}
			return;
		}
	}
	if(tmp==SEMICN)
				;
	else{
		error(MISSING_SEMICN);
	}
}
//＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞
void if_state(func)
{ 
	output(IF_STATE);
	if(read()==LPARENT){
		condition(func);
		read();
		statement(func);
	}
	else{
			error(PARENT_DISMATCH);
			return;
		}
}

//条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
void condition(char func[])
{
	read();
	if(tmp==RPARENT){
		error(EMPTY_CONDITION);
		return;
	}
	expression(func);
	if(tmp>=ASSIGN&&tmp<=NEQ){
		read();
		expression(func);
	}
	if(tmp==RPARENT){
		return;
	}
}
//＜循环语句＞   ::=  do＜语句＞while ‘(’＜条件＞‘)’
void do_while(char func[])
{
	output(DO_WHILE);
	if(tmp==DOTK){
		read();
		statement(func);
		if(read()==WHILETK){
			if(read()==LPARENT){
				condition(func);
				/*if(read()==SEMICN)
					;*/
			}
		}
	}
}
//＜常量＞   ::=  ＜整数＞|＜字符＞ 
void constant()
{
	read();
	if(tmp==PLUS||tmp==MINUS){
		if(read()==INTCON){
			;
		}
	}
	if(tmp==INTCON)	;
	else if(tmp==CHARCON){
		;
	}
}
//＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞ ‘}’
void switch_state(char func[])
{
	output(SWITCH_STATE);
	if(read()==LPARENT){
		read();
		expression(func);
		if(tmp==RPARENT&&read()==LBRACE){
			case_list(func);
		}
	}
	else{
			error(PARENT_DISMATCH);
			return;
		}
}
//＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
void case_list(char func[])
{	
	if(read()==CASETK){
		do{
			case_state(func);
		}while(read()==CASETK);
		if(tmp==RBRACE)
			;
	}
}
//＜情况子语句＞  ::=  case＜常量＞：＜语句＞
void case_state(char func[])
{
	output(CASE_STATE);
	constant();
	if(read()==COLON){
		read();
		statement(func);
	}
	
}
//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
void use_valfunc()
{
	output(VALFUNC);
	if(tmp==LPARENT){
			val_para(func);
		}
	if(read()==SEMICN||tmp==MULT||tmp==DIV||tmp==PLUS||tmp==MINUS||tmp==RPARENT||tmp==RBRACK)
				;
}
//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
void use_voidfunc()
{
	output(VOIDFUNC);
	if(tmp==IDEN){
		if(read()==LPARENT){
			val_para(func);
		}
	}
	if(read()==SEMICN)
				;
	else{
		error(MISSING_SEMICN);
	}
}
//＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void val_para(char func[])
{
	read();
	if(tmp==RPARENT)
		return ;
	expression(func);
	while(tmp==COMMA){
		read();
		expression(func);
	}
	if(tmp==RPARENT){
				;
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
	output(SCANF_STATE);
	if(tmp==SCANFTK){
		if(read()==LPARENT){
			do{
				if(read()==IDEN){
					if(seek(token,func)==NOTFOUND)	error(WITHOUT_DECLARATION);
					read();
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
	output(PRINTF_STATE);
	if(tmp==PRINTFTK){
		if(read()==LPARENT){
			read();
				if(tmp==STRCON){
					read();
					if(tmp==RPARENT){
						;
					}
					else if(tmp==COMMA){
						read();
						if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
							expression(func);
							if(tmp==RPARENT){
								;
							}
						}
					}					 						
					}
				if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
					expression(func);
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
	output(RETURN_STATE);
	if(tmp==RETURNTK){
		read();
		if(tmp==LPARENT){
			read();
			if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
				expression(func);
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
			;
		}
		else{
			error(MISSING_SEMICN);
		}
	}
}
//

void output(int a)
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
}
