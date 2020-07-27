//  *********************************************************************
//  *                   This file is part of RES 6.0.                   *
//  *    RES 6.0 is an original software distributed within the book    *
//  *                                                                   *
//  *      |-----------------------------------------------------|      *
//  *      | "Speech Recognition: Theory and C++ Implementation" |      *
//  *      |               John Wiley & Sons, Ltd                |      *
//  *      |    by Claudio Becchetti and Lucio Prina  Ricotti    |      *
//  *      |-----------------------------------------------------|      *
//  *                                                                   *
//  *       See copyright.txt file for further info. on copyright       *
//  *********************************************************************

 //  _____________________________________________________________________
//  |-------------------------------------------------------------------|
//  |                                                                   |
//  |   FILE:   baseclas.cpp                                            |
//  |   FUNCTIONALITY: test program for memory allocation (baseclas)    |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio											|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                   |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 12/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#define BASECLAS_VERSION "6.0 of 19/11/98 " ## __FILE__ ##"c. "##__DATE__

// added check operator=
// *******************    VERSION HISTORY  *******************************
////16/9/97 read code revision by claudio

// *******************          Code       *******************************

#include "../baseclas/baseclas.h"


//CLASSES TEST

class test
	{

	public:
	t_real ma;
	static t_real num;
#if (__BCPLUSPLUS__ ==	0x340)
	char car[8];
#else
	char car[3];
#endif

	//this operator is always required if the class must be used in
	//ImpObjectList
	test & operator=(const test & a)
		{
		ma=a.ma;
		car[0]=a.car[0];
		return ((*this));
		}
	Boolean operator==(const test & a)
		{
		return (ma==a.ma AND car[0]==a.car[0]AND car[1]==a.car[1]
			AND car[2]==a.car[2] );
		}
	test()
		{
		car[0]=car[1]=car[2]='a';
		ma=num;
		num++;
		}
	test(int val)
		{
		ma=val;
		return;
		};
	~test()
		{
		car[0]=car[1]=car[2]='0';
		ma=0.0;
		}

	};
t_real test::num=0;


Boolean Test_Base_Class(t_index max_num)
	{
	t_index i;
	ImpObjectList<test> mat;
	test compare;
  	mwarn<<"Base class        Version " <<BASECLAS_VERSION;


	mwarn<<"testing base class allocating " <<max_num<<" elems of class test";
	mat.Destroy_And_ReDim(max_num);

	mwarn<<"checking inizialization of " <<max_num<<" elems of class test";
	for (i=0;i<max_num;i++)
		{
		compare.ma=i+1;
		if (NOT (mat[i]==compare))
			{
			mwarn<<"Inizialization error on element: "<<i;
			return FALSE;
			}
		}

	mwarn<<"checking access for " <<max_num<<" elems of class test";
	for (i=0;i<max_num;i++)
		mat[i].ma=i+1;

	for (i=0;i<max_num;i++)
			if (mat[i].ma!=i+1)
				{
				mwarn<<"Inizialization error on element: "<<i;
				return FALSE;
				}
	{
	ImpSimpleTypeList<long> list1, list2 ;
	mwarn<<"check operator =";
	list1.Destroy_And_ReDim(70000UL);
	list1.Set(5);
	list2=list1;

	if (list2!=list1)
		{
		mwarn<<"operator = doesn't work on ImpSimpleList<long> ";
		return FALSE;
		}
	}
	mwarn<<"check complete";
	return TRUE;
	}


