// Ass1.cpp : Defines the entry point for the console application.
//


#ifndef _common_h_
#include "common.h"
#define _common_h_
#endif

#ifndef _common_cpp_
#include "common.cpp"
#define _common_cpp_
#endif

#ifndef _assignment1_cpp_
#include "assignment1.cpp"
#define _assignment1_cpp_
#endif


const int radix = 10;

char	operatorArr[MAX_OPERAND];
char	operandArr[MAX_OPERAND][MAX_DIGIT];

void writeFileRecur(FILE* f, digitList* head)
{
	if(head == NULL)	return;
	writeFileRecur(f, head->nextDigit);
	char	str[4];
	sprintf(str, "%d", head->digit);
	fwrite(str, 1, sizeof(char),f);
}

void writeFile(char* filename, Integer L)
{
	char* file_name = filename;
	FILE* f = 0;
	
	f = fopen(file_name, "w");
	if (f == NULL)	//file not found || cannot read
		return;

	if(L.sign < 0)
	{
		char	str[80];
		strcpy(str, "-");
		fwrite(str, 1, sizeof(char),f);
	}
	writeFileRecur(f, L.digits);
	
	fclose(f);
}

int readFile(char* filename, int&	operandNum, int& operatorNum)
{
#define MAX_CHARACTER_PER_LINE 80
	char* file_name = filename;
	FILE* f = 0;
	
	f = fopen(file_name, "r");
	if (f == NULL)	//file not found || cannot read
		return 0;
	///////////////////////////////////////////////
	char	str[MAX_CHARACTER_PER_LINE];

	fgets(str, MAX_CHARACTER_PER_LINE, f);
	sscanf(str, "%d", &operandNum);
	operatorNum = operandNum - 1;
	int i;
	for(i = 0; i<operandNum; i++)
	{
		fgets(str, MAX_CHARACTER_PER_LINE, f);
		strcpy(operandArr[i], str);
		for(int j = 0; j < strlen(operandArr[i]); j++)
			if(operandArr[i][j] == '\r' || operandArr[i][j] == '\n')
			{
				operandArr[i][j] = '\0';
				break;
			}
	}
	for(i = 0; i<operatorNum; i++)
	{
		fgets(str, MAX_CHARACTER_PER_LINE, f);
		operatorArr[i] = str[0];
	}
	///////////////////////////////////////////////
	fclose(f);

	return 1;
}


int main(int argc, char* argv[])
{
	int		operandNum;
	int		operatorNum;
	char	filename[80] = "input.txt";
	int		i;
	
	if(readFile(filename, operandNum, operatorNum) == 0)
	{
		cout << "Read data file faild";
		return 0;
	}

	Integer L1;

	L1 = computeValue(operatorNum);

	writeFile("output.txt", L1);
	
	getchar();

	return 0;
}
