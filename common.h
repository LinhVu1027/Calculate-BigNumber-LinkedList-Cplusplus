//#ifndef _common_h_
//#define _common_h_

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>

using namespace std;

#define		MAX_OPERAND			80
#define		MAX_DIGIT			80

extern const int radix;
extern char	operatorArr[MAX_OPERAND];
extern char	operandArr[MAX_OPERAND][MAX_DIGIT];

class digitList;
class Integer;

digitList* addDigitLists(int c, digitList* L1, digitList* L2);
int compareDigitLists(digitList* L1, digitList* L2);
digitList *trimDigitList(digitList* L);
digitList *subDigitLists(int b, digitList* L1, digitList* L2);
ostream& operator<< (ostream& os, digitList *L);
digitList *digitize(int n);
digitList *digitize(char str[80]);
Integer computeValue(int operatorNum);

class digitList
{
public:
	int			digit;
	digitList*	nextDigit;

public:
	digitList():digit(0), nextDigit(NULL){}
	digitList(int d, digitList *next):digit(d), nextDigit(next){}

	int getDigit(){
		return digit;
	}

	digitList* getNextDigit(){
		return nextDigit;
	}

	digitList* copy(){
		if(nextDigit == NULL)
			return new digitList(digit, NULL);
		else
			return new digitList(digit, nextDigit->copy());
	}

	int length()
	{
		if(nextDigit == NULL)
			return 1;
		else
			return 1 + nextDigit->length();
	}

	digitList*	leftDigits(int n)
	{
		if(n <= 0)					return NULL;
		else if(nextDigit == NULL)	return new digitList(digit, NULL);
		else						return new digitList(digit, nextDigit->leftDigits(n-1));

	}

	digitList*	rightDigits(int n)
	{
		if(n <= 0)					return NULL;
		else if(length() <= n)		return NULL;
		else				
		{
			digitList*	pTemp;
			pTemp = this;

			for(int i = 0; i<length() - n; i++)
				pTemp = pTemp->getNextDigit();

			digitList*	pHead = NULL;
			digitList*	pTail = NULL;
			digitList*	node;

			while(pTemp != NULL)
			{
				node = new digitList();
				node->digit = pTemp->getDigit();
				node->nextDigit = NULL;
				
				if(pHead == NULL)
				{
					pHead = node;
					pTail = node;
				}
				else
				{
					pTail->nextDigit = node;
					pTail = node;
				}
				pTemp = pTemp->getNextDigit();
			}
			return pHead;
		}
	}
};

class Integer
{
public :
	digitList*	digits;
	int			sign;

public:
	~Integer(){}
	Integer(): sign(1), digits(NULL){}
	Integer(digitList *L): sign(1), digits(L) {}
	Integer(int i, digitList *L):sign(sgn(i)), digits(L) {}
	Integer(int i): sign(sgn(i)), digits(digitize(abs(i))) { }
	Integer(char str[]) 
	{
		if(str[0] == '-')
		{
			sign = -1;
			
			for(int i = 0; i<strlen(str)-1; i++)
				str[i] = str[i+1];
			str[strlen(str)-1] = '\0'; 
			digits = digitize(str);
		}
		else
		{
			sign = 1;
			digits = digitize(str);
		}
	}

	int			length()
	{
		if (digits == NULL)
			return 0;
		else
			return digits->length();
	}

	Integer copy()
	{
		if(digits == NULL)
			return Integer(0);
		else
			return Integer(sign, digits->copy());
	}

	int sgn(int i)
	{
		if(i < 0)		return -1;
		else			return 1;
	}

	Integer trimDigit()
	{
		return Integer(sign, trimDigitList(digits));
	}

	Integer operator +(Integer L);
	Integer operator -(Integer L);

	Integer leftDigits(int n);
	Integer rightDigits(int n);
	Integer	shift(int n);
	Integer operator *(Integer L);
};


//#endif