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
//  |   FILE:   Vetclas.hpp                                             |
//  |   FUNCTIONALITY: matematical vector template implementation       |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes                                |
//  |   CONTRIBUTIONS: Domenico  ,claudio				                |
//  |   ACTUAL REVISION: 6.0  claudio									|
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 24/9/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//19/9/97 read code revision by claudio
//some bugs fixed by domenico 23/09/97
// ************************************************************************
//                                                                        *
//					Mathematical Vectors IMPLEMENTATION                   *
//                                                                        *
// ************************************************************************

#ifndef _VETCLAS_HPP_
#define _VETCLAS_HPP_

#include <math.h>

template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator * (const ImpMatVector<T> &vet)  const
	{

	static ImpMatVector<T> ris;

	Check(Dim()==vet.Dim(),"Vectors of different dimensions :"<<Dim()<<" and "<<vet.Dim());
	ris.Save_And_ReDim(Dim());

	ITERATE_ALL (ris[i]=(*this)[i]*vet[i]);

	return ris;
	}

template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator + (const ImpMatVector<T> & vet) const
	{

	static ImpMatVector<T> ris;

	Check(Dim()==vet.Dim(),"Vectors of different dimensions :"<<Dim()<<" and "<<vet.Dim());
		ris.Save_And_ReDim(Dim());

	ITERATE_ALL(ris[i]=(*this)[i]+vet[i]);

	return ris;
	}

template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator - (const ImpMatVector<T> &vet) const
	{

	static ImpMatVector<T> ris;

	Check(Dim()==vet.Dim(),"Vectors of different dimensions :"<<Dim()<<" and "<<vet.Dim());
	ris.Save_And_ReDim(Dim());

	ITERATE_ALL(ris[i]=(*this)[i]-vet[i]);

	return ris;
	}

template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator / (const ImpMatVector<T> &vet) const
	{

	static ImpMatVector<T> ris;

	Check(Dim()==vet.Dim(),"Vectors of different dimensions :"<<Dim()<<" and "<<vet.Dim());
	ris.Save_And_ReDim(Dim());

	ITERATE_ALL(
		if (fabs((t_real)vet[i])<=REAL_ZERO)
          merr<<"zero elem in division number "<<vet[i];
		ris[i]=(*this)[i]/vet[i];
		);

	return ris;
	}

template <class T>
void ImpMatVector<T>::operator /= (const ImpMatVector<T> & vet)
	{
	 
	Check(Dim()==vet.Dim(),"Vectors of different dimensions :"<<Dim()<<" and "<<vet.Dim());

	ITERATE_ALL(
        if (fabs((t_real)vet[i])<=REAL_ZERO)
		  merr<<"zero elem in division number "<<vet[i];
	    (*this)[i]/=vet[i]
		);
	return;
	}

template <class T>
void ImpMatVector<T>:: operator*= (const ImpMatVector<T> & vet)
	{
	Check(Dim()==vet.Dim(),"Vectors of different dimensions:"<<Dim()<<" and "<<vet.Dim());
	ITERATE_ALL((*this)[i]*=vet[i]);

	return;
	}

template <class T>
void ImpMatVector<T>::operator += (const ImpMatVector<T> & vet)
	{

	Check(Dim()==vet.Dim(),"Vectors of different dimensions "<<Dim()<<" and "<<vet.Dim());

	ITERATE_ALL((*this)[i]+=vet[i]);

	return;
	}

template <class T>
void ImpMatVector<T>:: operator-= (const ImpMatVector<T> & vet)
	{

	Check(Dim()==vet.Dim(),"Vectors of different dimensions:"<<Dim()<<" and "<<vet.Dim());

	ITERATE_ALL((*this)[i]-=vet[i]);

	return;
	}


template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator * (t_real num)  const
	{

	static ImpMatVector<T> ris;

	ris.Save_And_ReDim(Dim());
	ITERATE_ALL(ris[i]=((*this)[i])*(double)num);

	return ris;
	}

template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator / (t_real num)  const
	{

	static ImpMatVector<T> ris;

    if (fabs((t_real)num)<=REAL_ZERO)
	  merr<<"zero elem in division number "<<num;
	ris.Save_And_ReDim(Dim());
	
	ITERATE_ALL(ris[i]=((*this)[i])/(double)num);
	
	return ris;
	}

template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator + (t_real num)  const
	{

	static ImpMatVector<T> ris;

	ris.Save_And_ReDim(Dim());
	ITERATE_ALL(ris[i]=(*this)[i]+num);

	return ris;
	}

template <class T>
ImpMatVector<T> & ImpMatVector<T>::operator - (t_real num)  const
	{

	static ImpMatVector<T> ris;

	ris.Save_And_ReDim(Dim());
	ITERATE_ALL(ris[i]=(*this)[i]-num);

	return ris;
	}

template <class T>
void ImpMatVector<T>::operator *= (t_real num)
	{
	ITERATE_ALL((*this)[i]*=num);
	
	return;
	}

template <class T>
void ImpMatVector<T>::operator += (t_real num)
	{
	
	ITERATE_ALL((*this)[i]+=num);
	
	return;
	}

template <class T>
void ImpMatVector<T>::operator -= (t_real num)
	{
	
	ITERATE_ALL((*this)[i]-=num);
	return;
	}

template <class T>
void ImpMatVector<T>::operator /= (t_real num)
	{

	if (fabs((t_real)num)<=REAL_ZERO)
		merr<<"division by zero error"<<num;

	ITERATE_ALL((*this)[i]/=num)

	return;
	}

//scalar product

template <class T>
T  ImpMatVector<T>::operator | (const ImpMatVector<T> &vet) const
	{
 	t_real ris=0.0;
	Check(Dim()==vet.Dim(),"Vectors of different dimensions :"<<Dim()<<" and "<<vet.Dim());

	ITERATE_ALL(ris+=(*this)[i]*vet[i]);

	return ris;
	}


template <class T>
ImpMatVector<T> & ImpMatVector<T>::Log()
    {

	ITERATE_ALL(
		Assert((*this)[i]>0);
		(*this)[i]=log((*this)[i]); );

	return (*this);
    }

template <class T>
ImpMatVector<T> &  ImpMatVector<T>::Log10()
    {

	ITERATE_ALL(
		Assert((*this)[i]>0);
		(*this)[i]=log10((*this)[i]); );
	 
	return (*this);
    }

template <class T>
t_real ImpMatVector<T>::Mean_Value()
	{
	 
	T ris=0.0;
	Assert(Dim()>0);
	ITERATE_ALL(ris+=(*this)[i]);
	ris/=Dim();
	 
	return ris;
	}


template <class T>
void ImpMatVector<T>::operator ^= (t_real num)
	{
	ITERATE_ALL ((*this)[i]=pow((*this)[i],num));
	 
	return;
	}
   		 

#endif