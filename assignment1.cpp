//#include "common.h"

#ifndef _common_h_
#include "common.h"
#define _common_h_
#endif

//////////////////////////////////////////////////////////////
//
//			Some common functions
//
//////////////////////////////////////////////////////////////

Integer factorial(Integer L)
{
	if ((L.digits->digit == 0) && (L.digits->nextDigit == NULL)) return Integer(1);
	else if ((L.digits->digit == 1) && (L.digits->nextDigit == NULL)) return L;
	else return L*factorial(L-Integer(1));
}

Integer indexNumber(Integer L, int index)
{
	if ((L.digits->digit == 0) && (L.digits->nextDigit == NULL) && index == 0) return Integer(1);
	if ((L.digits->digit == 0) && (L.digits->nextDigit == NULL)) return Integer("0");
	if (index == 0) return Integer(1);
	else if (index == 1) return  L;
	else return L*indexNumber(L, index-1);
}

digitList* digitize(char str[80])
{
	digitList* L = NULL;
	digitList* node;

	int i;
	for (i = 0; i<strlen(str); i++)					// Example: 1 2 3 '\0', length = 3, i iterates to 0-> 1-> 2
	{
		if (str[i] < '0' || str[i] > '9') break;	// Differ number from 0 to 9 => break 
		node = new digitList(str[i] - '0', L);		// str[i]-'0' return: number, char - char = number
		L = node;
	}
	if (str[i] == '!') L = factorial(Integer(L)).digits;
	if (str[i] == '^')
	{
		int index = 0;
		i++;
		for (i; i<strlen(str); i++)
		{
			if (str[i] < '0' || str[i] > '9') break;
			index += (str[i]-'0')*pow(10,strlen(str)-i-1);
		}
		L = indexNumber(Integer(L),index).digits;
	}
	return L;
}

digitList* trimDigitList(digitList* L)
{
	if (L == NULL) return NULL;
	if (L->nextDigit == NULL) return L;
	else
	{
		digitList* pPre = L;
		digitList* pTemp = new digitList();
		while (pPre->nextDigit != NULL)
		{
			pTemp = pPre->nextDigit;
			if (pTemp->nextDigit == NULL) break;
			else pPre = pPre->nextDigit;
		}
		if (pTemp->digit == 0) 
		{
			pPre->nextDigit = NULL;
			delete pTemp;
			return trimDigitList(L);
		}
		else return L;
	}
}

digitList* subDigitLists(int b, digitList* L1, digitList* L2)
{ 
	if (compareDigitLists(L1, L2) == -1) return subDigitLists(b, L2, L1);
	else if ((L1 == NULL) && (L2 == NULL)) return NULL;
	else if (L2 == NULL)
	{
		int t = L1->getDigit() - b;
		if (t < 0)
		{
			t = L1->getDigit() + 10 - b;
			b = 1;
		}
		else b = 0;
		return new digitList(t, subDigitLists(b, L1->getNextDigit(), NULL));
	}
	else 
	{
		int  t = L1->getDigit() - L2->getDigit() - b;
		if (t < 0) 
		{
			 t = L1->getDigit() + 10 - L2->getDigit() - b;
			 b = 1;
		}
		else b = 0;
		return new digitList(t, subDigitLists(b, L1->getNextDigit(), L2->getNextDigit()));
	}
	return NULL;
}

Integer computeValue (int operatorNum)
{
	Integer L1, L2;
	
	L1 = operandArr[0];

	for (int i = 0; i < operatorNum; i++)
	{
		L2 = operandArr[i+1];
		switch(operatorArr[i])
		{
		case '+':
			L1 = L1 + L2;
			break;
		case '-':
			L1 = L1 - L2;
			break;
		
		case '*':
			L1 = L1 * L2;
			break;
		}
	}
	
	return L1;
}
///////////////////////////////////////////////////////////////////////////
//
//		Some methods of Integer class
//
////////////////////////////////////////////////////////////////////
Integer Integer::operator+ (Integer L)
{
	if (sign == L.sign)
		     return Integer(sign, addDigitLists(0, digits, L.digits));
	else
	{
		if (compareDigitLists(digits, L.digits) == 1)
			return Integer(sign, subDigitLists(0, digits, L.digits)).trimDigit();
		else return Integer(L.sign, subDigitLists(0, L.digits, digits)).trimDigit();
	}
}

Integer Integer::operator- (Integer L)
{
	if (sign == L.sign)
	{
		if (compareDigitLists(digits, L.digits) == 1) 
					return Integer( sign, subDigitLists(0, digits, L.digits)).trimDigit();
		else if (compareDigitLists(digits, L.digits) == 0) 
					return Integer( 1, subDigitLists(0, digits, L.digits)).trimDigit();
		else        return Integer(-sign, subDigitLists(0, L.digits, digits)).trimDigit();
	}
	else            return Integer( sign, addDigitLists(0, digits, L.digits));
}

Integer Integer::leftDigits  (int n)
{
	return Integer(sign, digits->leftDigits(n));
}
Integer Integer::rightDigits (int n)
{
	return Integer(sign, digits->rightDigits(n));
}

Integer Integer::shift       (int n)
{
	Integer iTemp = copy();
	for (int i=0; i<n; i++)
		iTemp.digits = new digitList(0,iTemp.digits);
	return iTemp;
}

Integer Integer::operator* (Integer L)
{
	int node = length() - L.length();
	if (node > 0)
	{
		for (int i = 0; i<node; i++)
		{
			digitList* pTemp = L.digits;
			digitList* pLast = new digitList();
			pLast->digit = 0;
			pLast->nextDigit = NULL;
			while (pTemp->nextDigit != NULL) pTemp = pTemp->nextDigit;
			pTemp->nextDigit = pLast;
		}
	}
	else if (node < 0)
	{
		for (int i = 0; i<-node; i++)
		{
			digitList* pTemp = digits;
			digitList* pLast = new digitList();
			pLast->digit = 0;
			pLast->nextDigit = NULL;
			while (pTemp->nextDigit != NULL) pTemp = pTemp->nextDigit;
			pTemp->nextDigit = pLast;
		}
	}
	
	if ((length() == 1) && (L.length() == 1))
	{
		int t = digits->digit*L.digits->digit;
		if (t != 0)
			return Integer(sign*L.sign, digitize(t));
		else 
			return Integer("0");
	}
	else
	{
		int t1 = length() / 2;
		int t2 = length() - t1;
		return
			( (leftDigits(t1)*L.leftDigits(t1))
			+ ((leftDigits(t1)*L.rightDigits(t2))+(L.leftDigits(t1)*rightDigits(t2))).shift(t1).trimDigit() 
			+ (rightDigits(t2)*L.rightDigits(t2)).shift(2*t1).trimDigit() ); 
	}
}
