#ifndef  __GRAMMAR_H_
#define __GRAMMAR_H_
enum out{
	CONST_STATE,
	VAR_STATE,
	DEF_VALFUNC,
	DEF_VOIDFUNC,
	MAIN_FUNC,
	IF_STATE,
	DO_WHILE,
	SWITCH_STATE,
	CASE_STATE,
	VALFUNC,
	VOIDFUNC,
	ASSIGNMENT,
	PRINTF_STATE,
	SCANF_STATE,
	RETURN_STATE,
	EMPTY
	
};
void program();
void var_or_deffunc();
void const_state();
void def_const();
void integer();
void var_state();
void def_var();
void def_func();
void com_state();
void para_table();
void main_func();
void expression();
void term();
void factor();
void statement();
void assignment();
void if_state();
void condition();
void do_while();
void constant();
void switch_state();
void case_list();
void case_state();
void use_valfunc();
void use_voidfunc();
void val_para();
void statements();
void scanf_state();
void printf_state();
void return_state();
void output(int a);
#endif
