#include<stdio.h>
#include"grammer.h"
#include"lexical.h"
#include"error.h"
extern int tokennum,tmp,tptr;
extern char token[];
char all[]="static";
//������    ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program()
{
	int rflag=0;
	
	getsym();
	read();
	while(tmp==CONSTTK){
		//��һ��const�Ѿ����� 
		const_state(all);
	} 
		//�����Ǳ���˵��Ҳ�������з���ֵ����
	if(tmp==INTTK||tmp==CHARTK){
		var_or_deffunc();	//ǰyes��no 	
		}
	while(tmp==INTTK||tmp==CHARTK||tmp==VOIDTK){
		if(tmp==INTTK||tmp==CHARTK){
			read();
		if(tmp==IDEN){
			read();
			//�з���ֵ���� 
			if(tmp==LPARENT){
				output(DEF_VALFUNC);
				def_func(all);
				
			}
	}
	}	//�޷���ֵ���� 
	if(tmp==VOIDTK){
			if(read()==IDEN&&read()==LPARENT){
				output(DEF_VOIDFUNC);
				def_func(all);				
			}
			//������ 
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
			//�з���ֵ���� 
			if(tmp==LPARENT){
				output(DEF_VALFUNC);
				def_func(name);
				
			}
			
			//����˵��
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
// ������˵���� ::=  const���������壾;{ const���������壾;}
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
//���������壾   ::=   int����ʶ��������������{,����ʶ��������������}
                            //| char����ʶ���������ַ���{,����ʶ���������ַ���}
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
					read(); //�������� 
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
									
									read(); //�������� 
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
									
									read(); //�������� 
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
//��������        ::= �ۣ������ݣ��޷�������������
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
//������˵����  ::= ���������壾;{���������壾;}
void var_state(char area[])
{
	do{
		def_var(area);
		read();
	}while(tmp==INTTK||tmp==CHARTK);
}
//���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��)
				//{,(����ʶ����|����ʶ������[�����޷�����������]��) }
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
//���ޡ��з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
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
//��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
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
//��������    ::=  �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}| ���գ�
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
//����������    ::= void main��(����)�� ��{����������䣾��}��
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
//�����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}
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
//���     ::= �����ӣ�{���˷�������������ӣ�}
void term(char func[])
{
	do{
		if(tmp==MULT||tmp==DIV){
			read();
		}
		factor(func);
		//read();//Ԥ�� 
	}while(tmp==MULT||tmp==DIV);
}
//�����ӣ�    ::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ�����
//				���з���ֵ����������䣾|��(�������ʽ����)��
void factor(char func[])
{
	switch(tmp){
		case(IDEN):
			if(seek(token,func)!=NOTFOUND)
				;
			else error(WITHOUT_DECLARATION);
			read();
			if(tmp==LBRACK){//����ʶ������[�������ʽ����]��
				read();
				expression(func);
				if(tmp==RBRACK){
					read();
				}
				else {
					error(BRACK_DISMATCH);
						}
			}
			else if(tmp==LPARENT) {//���з���ֵ����������䣾
				val_para(func);//yes
				if(tmp==RPARENT)	read();
				else error(WRONG_EXPRESSION);
			}
			break;	
		case(MULT)://����
				if(read()==INTCON){
					;
				}
			read();
			break;
		case(DIV)://���� 
				if(read()==INTCON){
					;
				}
			read();
			break;
		case(INTCON)://���� 
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
/*����䣾    ::= ��������䣾����ѭ����䣾��<������>|��{��������У���}����
				���з���ֵ����������䣾; |���޷���ֵ����������䣾;������ֵ��䣾;
					��������䣾;����д��䣾;�����գ�;����������䣾;*/
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
//����ֵ��䣾   ::=  ����ʶ�����������ʽ��|����ʶ������[�������ʽ����]��=�����ʽ��
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
//��������䣾  ::=  if ��(������������)������䣾
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

//������    ::=  �����ʽ������ϵ������������ʽ���������ʽ��
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
//��ѭ����䣾   ::=  do����䣾while ��(������������)��
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
//��������   ::=  ��������|���ַ��� 
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
//�������䣾  ::=  switch ��(�������ʽ����)�� ��{��������� ��}��
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
//�������   ::=  ���������䣾{���������䣾}
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
//���������䣾  ::=  case��������������䣾
void case_state(char func[])
{
	output(CASE_STATE);
	constant();
	if(read()==COLON){
		read();
		statement(func);
	}
	
}
//���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
void use_valfunc()
{
	output(VALFUNC);
	if(tmp==LPARENT){
			val_para(func);
		}
	if(read()==SEMICN||tmp==MULT||tmp==DIV||tmp==PLUS||tmp==MINUS||tmp==RPARENT||tmp==RBRACK)
				;
}
//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
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
//��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
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
//������У�   ::=������䣾��
void statements(char func[]) 
{
	while(tmp!=RBRACE){
		statement(func);
		read();
	}
} 
//������䣾    ::=  scanf ��(������ʶ����{,����ʶ����}��)��
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
//��д��䣾    ::=  printf��(�����ַ�����,�����ʽ����)��|printf ��(�����ַ�������)��
//					|printf ��(�������ʽ����)��
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
//��������䣾   ::=  return[��(�������ʽ����)��]
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
