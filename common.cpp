//#include "common.h"
#ifndef _common_h_
#include "common.h"
#define _common_h_
#endif
//////////////////////////////////////////////////////
//
//			Some common functions
//
//////////////////////////////////////////////////////

digitList *digitize(int n)
{
	if(n == 0)
		return NULL;
	else
		return new digitList(n % radix, digitize(n / radix));
}


int compareDigitLists(digitList* L1, digitList* L2)
{
	// L1 == L2 : return 0
	// L1 > L2  : return 1
	// L1 < L2  : return -1
	if((L1 == NULL) && (L2 == NULL))	return 0;
	else if(L1 == NULL)					return -1;
	else if(L2 == NULL)					return 1;
	else
	{
		switch(compareDigitLists(L1->getNextDigit(), L2->getNextDigit()))
		{
		case	1:	return 1;
		case   -1:	return -1;
		case    0:
			if(L1->getDigit() > L2->getDigit())
				return 1;
			else if(L1->getDigit() == L2->getDigit())
				return 0;
			else
				return -1;
		}
	}
}

digitList* addDigitLists(int c, digitList* L1, digitList* L2)
{
	if((L1 == NULL) && (L2 == NULL))	return digitize(c);
	else if(L1 == NULL)					return addDigitLists(c, L2, NULL);
	else if(L2 == NULL)					
	{
		int t = c + L1->getDigit();
		return new digitList(t % radix, addDigitLists( t / radix, L1->getNextDigit(), NULL));
	}
	else
	{
		int t = c + L1->getDigit() + L2->getDigit();
		return new digitList(t % radix, addDigitLists( t / radix, L1->getNextDigit(), L2->getNextDigit()));
	}
}

//////////////////////////////////////////////////////////////
//
//		Some methods of Integer class
//
///////////////////////////////////////////////////////////////
