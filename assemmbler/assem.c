#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char * disd(char *, int);
int disc(char *,int);
char * num2bin(int , char *);
typedef struct assembler{
    char *a;
    char *num;
}pair;
pair dest[8]={
    {"","000"},
    {"M","001"},{"D","010"},{"MD","011"},
    {"A","100"},{"AD","110"},{"AM","101"},{"AMD","111"}
};
pair comp[28]={
    {"0","0101010"},{"1","0111111"},{"-1","0111010"},{"D","0001100"},{"A","0110000"},
    {"!D","0001101"},{"!A","0110001"},{"-D","0001111"},{"-A","0110011"},{"D+1","0011111"},{"A+1","0110111"},
    {"D-1","0001110"},{"A-1","0110010"},{"D+A","0000010"},{"D-A","0010011"},{"A-D","0000111"},{"D&A","0000000"},
    {"D|A","0010101"},
    {"M","1110000"},{"!M","1110001"},{"-M","1110011"},{"M+1","1110111"},{"M-1","1110010"},
    {"D+M","1000010"},{"D-M","1010011"},{"M-D","1000111"},{"D&M","1000000"},
    {"D|M","1010101"}
};
pair jump[8]={
    {"","000"},{"JGT","001"},{"JEQ","010"},{"JGE","011"},{"JLT","100"},{"JNE","101"},{"JLE","110"},{"JMP","111"}
};
struct symbio{
    char b[20];
    int fnum;
}sym[30000]={
    {"R0",0},{"R1",1},{"R2",2},{"R3",3},{"R4",4},{"R5",5},{"R6",6},{"R7",7},{"R8",8},{"R9",9},{"R10",10},{"R11",11},{"R12",12},{"R13",13},{"R14",14},{"R15",15},
    {"SCREEN",16384},{"KBD",24576},{"SP",0},{"LCL",1},{"ARG",2},{"THIS",3},{"THAT",4}
};
int main(void)
{
	char openfile[20];
	char writefile[20];
	scanf("%s",openfile);
	strcpy(writefile,openfile);
	strcat(openfile,".asm");
	strcat(writefile,".hack");
	int k=23;
	FILE *file1 = fopen(openfile, "r");
	FILE *file3 =fopen(writefile,"w");
	int num;
	int flag=0,i;
	char code[50];
	char d[3],comp[3],j[4];
	char code1[50];
	char word[48];
	while(feof(file1)==0)
	{
		fgets(code,40,file1);
		if(code[0]=='(')
		{
			sscanf(code,"(%[^)])",sym[k].b);  //sscanf(code,"(%[^)])",word); 
			sym[k].fnum=flag;			 //sym[k].b=word;
			k++;
		}
		else
		flag++;
	}
	fclose(file1);
	FILE *file2 = fopen(openfile,"r");
	while(feof(file2)==0)
	{
		char bincode[50]={0};
		fgets(code,40,file2);
		int num=0;
		if(code[0]=='@') //instruction A
		{
			if(code[1]>47 && code[1]<59) //num part
			{
				sscanf(code,"@%d",&num);
				num2bin(num,bincode);
				sprintf(bincode,"%s\n",bincode);
				printf("%s",code);
				printf("%s",bincode);
				fwrite(bincode,sizeof(char),17,file3);   
			}
			else
			{
				sscanf(code,"@%s",code1);
				num=disc(code1,k);
				num2bin(num,bincode);
				sprintf(bincode,"%s\n",bincode);
				printf("%s",code);
				printf("%s",bincode);
				fwrite(bincode,sizeof(char),17,file3);
			}
		}
		else if(strchr(code, '=') != NULL)
		{
			sscanf(code, "%[^=]=%s", d, comp);
			sprintf(bincode,"111%s%s000\n",disd(comp,0),disd(d,1));
			printf("%s",code);
			printf("%s",bincode);
			fwrite(bincode,sizeof(char),17,file3);
		}
		else if(strchr(code, ';')!= NULL)
		{
			sscanf(code, "%[^;];%s", comp, j);
			sprintf(bincode,"111%s000%s\n",disd(comp,0),disd(j,2));
			printf("%s",code);
			printf("%s",bincode);
			fwrite(bincode,sizeof(char),17,file3);
		}
	}
	fclose(file2);
	fclose(file3);
}
int disc(char *d,int k)
{
	int i;
	int static jj=0; //�ϥ��R�A��ƵL�kgcc -o? 
	for(i=0;i<k+jj;i++)
	{
		if(strcmp(d,sym[i].b)==0)
		return sym[i].fnum;
	}
	strcpy(sym[i].b,d);
	sym[i].fnum=jj+16;
	jj++;
	return sym[i].fnum;
}
char * disd(char *d,int thing) //�P�_�F��Ǧ^�Ʀr 
{
	int i;
	for(i=0;i<28;i++)
	{
		if(thing==0)
		{
			if(strcmp(d,comp[i].a)==0)
			return comp[i].num;
		}
		else if(thing ==1)
		{
			if(strcmp(d,dest[i].a)==0)
			return dest[i].num;
		}
		else
		{
			if(strcmp(d,jump[i].a)==0)
			return jump[i].num;
		}
	}
}
char * num2bin(int num,char *biochar) //�N�Ʀr��2�i��
{
	int bio[16];
	int i;
	char dd[10];
	for(i=0;i<16;i++)
	{
		bio[15-i]=num%2;
		num=num/2;
	}
	for(i=0;i<16;i++)
	{
		sprintf(dd, "%d", bio[i]);
		biochar[i]=dd[0];
	}
	return biochar;
}
