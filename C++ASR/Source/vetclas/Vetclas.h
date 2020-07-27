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
//  |   FILE:   Vetclas.h                                               |
//  |   FUNCTIONALITY: matematical vector                               |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes                                |
//  |   CONTRIBUTIONS: Domenico   claudio				                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 24/9/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    Improvement  *******************************
// added all operazion

// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//added unix compatibility inserted TWO_BYTE_SHORT
//16/9/97 read code revision by claudio
//18/9/97 fixed bug on mean
//25/9/97 added VetInt required for reconf
//10/01/98 added ImpMatVector<t_real> & operator +
//				(double arg, const ImpMatVector<t_real> & arg2)
// to allow vet= doublenum+ vet
//not all the operations are implemented only sum


#ifndef __VETCLASS_H_
#define __VETCLASS_H_


#include <math.h>
#include "../baseclas/baseclas.h"
#include "../baseclas/diagnost.h"

// ************************************************************************
//                                                                        *
//								Mathematical Vectors                      *
//                                                                        *
// ************************************************************************

template <class T>
class ImpMatVector:public ImpSimpleTypeList<T>
	{
	public:

	//elem by elem

	//***********  biargumental operators of vectors ****************

	ImpMatVector<T> & operator * (const ImpMatVector<T>& vet) const;
	ImpMatVector<T> & operator + (const ImpMatVector<T>& vet) const;
	ImpMatVector<T> & operator - (const ImpMatVector<T>& vet) const;
    ImpMatVector<T> & operator / (const ImpMatVector<T>& vet) const;
	
	//***********  monoargumental operators of vectors ****************

	void operator /= (const ImpMatVector<T>& vet);
	void operator *= (const ImpMatVector<T>& vet);
	void operator += (const ImpMatVector<T>& vet);
	void operator -= (const ImpMatVector<T>& vet);


	//***********  biargumental operators of a vector and constant*****

	ImpMatVector<T> & operator * (t_real cost) const;
	ImpMatVector<T> & operator + (t_real cost) const;
	ImpMatVector<T> & operator - (t_real cost) const;
	ImpMatVector<T> & operator / (t_real cost) const;

	//***********  monoargumental operators of a vector and constant*****

	void operator *= (t_real cost) ;
	void operator += (t_real cost) ;
	void operator -= (t_real cost) ;
	void operator /= (t_real cost) ;

	//scalar product

	T operator | (const ImpMatVector<T>& vet) const;

	//***********  other operators *****

	ImpMatVector<T> &  Log();
	ImpMatVector<T> & Log10();
	t_real Mean_Value ();
    
	//raise power elem by elem

	void operator ^= (t_real num);

	};



typedef ImpMatVector<t_index> VetULong;
typedef ImpMatVector<TWO_BYTE_SHORT> VetShortInt;
typedef ImpMatVector<long> VetLong;

typedef ImpMatVector<t_signed> VetInt;


class VetDouble : public ImpMatVector<t_real>
	{

	 public:

	 void operator = (const ImpMatVector<t_real> & vet);
	 void operator = (const ImpMatVector<t_signed> & vet);
	 void operator = (const VetShortInt & vet);

	};
//this allows vet= doublenum+vet;
ImpMatVector<t_real> & operator + (double arg, const ImpMatVector<t_real> & arg2);

Boolean Test_Math_Vector_Functions();
Boolean Imp_Simple_List_Test_Upper_Bound();

#endif

