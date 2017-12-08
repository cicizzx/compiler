#ifndef TABLE_H
#define TABLE_H
#define COMPARAMETERSNUMBER 10
#define TMAX 128
enum tablename{
	NOTFOUND,
	CONSTANT,
	PARAMETER,
	SIMPLE_VARIABLE,
	ARRAY_VARIABLE,
	FUNCTION
};
struct table
{
	
	int type;//FUNCTION,VARIABLE,CONSTANT
	int kind;//char\int
	char name[1024];
	char area[1024];//有效函数区域(VARIABLE,CONSTANT)
	int value;//常量（char\int）存值，简单变量存函数内地址偏移
	int length;//数组,函数长度，参数个数

};
struct funtable
{
	int type;//char int void
	char name[1024];
	//int enterpnum;
	int paranum;
	int para1;
};

void entertable(int type,char id[],int value,int kind,char func[]);
void entertablearray(int type,char id[],int value,int kind,char func[],int length);
void entertablefun(int type,char id[],int paranum,int para1ptr);//name,function var...,area
int check(char id[], char funid[]);
int seek(char id[], char funid[]);
void poptable(char funid[]);
#endif
