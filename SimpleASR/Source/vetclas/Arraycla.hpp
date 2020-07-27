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
//  |   FILE:   arraycla.hpp                                            |
//  |   FUNCTIONALITY: matematical array                                |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes                                |
//  |   CONTRIBUTIONS:  Domenico claudio	                            |
//  |   ACTUAL REVISION: 6.0 						                    |
//  |   DATA ACTUAL REVISION:											|
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 24/9/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________
// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//16/9/97 corrected bug  by claudio not revised
//19/09/97 corrected bug by domenico
//21/09/97 fixed some bugs by claudio 


#ifndef ARRAYCLA_HPP
#define ARRAYCLA_HPP

#include "../vetclas/arraycla.h"


// **********************************************************************
// *                                                                    *
// *                        ImpSimpleMatrix                             *
// *                                                                    *
// **********************************************************************

//elem by elem

//****************** biargumental operators of a matrix and constant ********************

  template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator / (const ImpSimpleMatrix<T>& vet)const
	{

	static ImpSimpleMatrix<T> ris;

	Check(vet.Dim_Row()==Dim_Row()	AND vet.Dim_Col()==Dim_Col(),
						"Different dimensions in ImpSimpleMatrix this = ("<<Dim_Row()<<","<<Dim_Col()<<" vet= ("<<vet.Dim_Row()<<","<<vet.Dim_Col()<<")");

	ris.Save_And_ReDim(Dim_Row(),Dim_Col());

	//the check over zero elem si made by the relative vector operation
	ITERATE_ALL( ris[i]= (*this)[i]/vet[i] );

	return ris;
	}

template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator * (const ImpSimpleMatrix<T>& vet)const
	{
	static ImpSimpleMatrix<T> ris;

	Check(vet.Dim_Row()==Dim_Row()	AND vet.Dim_Col()==Dim_Col(),
						"Different dimensions in ImpSimpleMatrix this = ("<<Dim_Row()<<","<<Dim_Col()<<" vet= ("<<vet.Dim_Row()<<","<<vet.Dim_Col()<<")");

	ris.Save_And_ReDim(Dim_Row(),Dim_Col());

	ITERATE_ALL(ris[i]= (*this)[i]*vet[i]);

	return ris;
	}

template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator + (const ImpSimpleMatrix<T>& vet)const
	{
	 
	static ImpSimpleMatrix<T> ris;

	Check(vet.Dim_Row()==Dim_Row()	AND vet.Dim_Col()==Dim_Col(),
						"Different dimensions in + this = ("<<Dim_Row()<<","<<Dim_Col()<<" vet= ("<<vet.Dim_Row()<<","<<vet.Dim_Col()<<")");

	ris.Save_And_ReDim(Dim_Row(),Dim_Col());

	ITERATE_ALL(ris[i]= (*this)[i] + vet[i]);

	return ris;
	}

template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator - (const ImpSimpleMatrix<T>& vet)const
	{
	 
	static ImpSimpleMatrix<T> ris;

	Check(vet.Dim_Row()==Dim_Row()	AND vet.Dim_Col()==Dim_Col(),
						"Different dimensions in - this = ("<<Dim_Row()<<","<<Dim_Col()<<" vet= ("<<vet.Dim_Row()<<","<<vet.Dim_Col()<<")");

	ris.Save_And_ReDim(Dim_Row(),Dim_Col());

	ITERATE_ALL(ris[i]= (*this)[i] - vet[i]);

	return ris;
	}

//******************* monoargumental operators of matrixs **********************************

template <class T>
void ImpSimpleMatrix<T>::operator += (const ImpSimpleMatrix<T> & vet)
	{

	Check(vet.Dim_Row()==Dim_Row()	AND vet.Dim_Col()==Dim_Col(),
						"Different dimensions in += this = ("<<Dim_Row()<<","<<Dim_Col()<<" vet= ("<<vet.Dim_Row()<<","<<vet.Dim_Col()<<")");

	ITERATE_ALL((*this)[i] += vet[i]);

	return;
	}

template <class T>
void ImpSimpleMatrix<T>::operator -= (const ImpSimpleMatrix<T> & vet)
	{

	Check(vet.Dim_Row()==Dim_Row()	AND vet.Dim_Col()==Dim_Col(),
						"Different dimensions in -= this = ("<<Dim_Row()<<","<<Dim_Col()<<" vet= ("<<vet.Dim_Row()<<","<<vet.Dim_Col()<<")");

	ITERATE_ALL((*this)[i] -= vet[i]);

	return;
	}

template <class T>
void ImpSimpleMatrix<T>::operator *= (const ImpSimpleMatrix<T> & vet)
	{

	Check(vet.Dim_Row()==Dim_Row()	AND vet.Dim_Col()==Dim_Col(),
						"Different dimensions in *= this = ("<<Dim_Row()<<","<<Dim_Col()<<" vet= ("<<vet.Dim_Row()<<","<<vet.Dim_Col()<<")");

	ITERATE_ALL((*this)[i] *= vet[i]);

	return;
	}

//****************** biargumental operators of a matrix and constant *********************

template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator / (t_real num)const
	{

	static ImpSimpleMatrix<T> ris;

	if (num<=REAL_ZERO)
		merr<<"zero elem in division:"<<num;

    ris.Save_And_ReDim(Dim_Row(),Dim_Col());
	
    ITERATE_ALL(ris[i]= (*this)[i]/num);

	return ris;
	}


template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator * (t_real num)const
	{
	static ImpSimpleMatrix<T> ris;

	ris.Save_And_ReDim(Dim_Row(),Dim_Col());
	 
	ITERATE_ALL(ris[i]= (*this)[i]*num);

	return ris;
	}


template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator + (t_real num)const
	{
 	 
	static ImpSimpleMatrix<T> ris;

	ris.Save_And_ReDim(Dim_Row(),Dim_Col());
	 
	ITERATE_ALL(ris[i]= (*this)[i]+num);

	return ris;
	}

template <class T>
ImpSimpleMatrix<T> & ImpSimpleMatrix<T>::operator - (t_real num)const
	{

	static ImpSimpleMatrix<T> ris;

	ris.Save_And_ReDim(Dim_Row(),Dim_Col());
	 
	ITERATE_ALL(ris[i]= (*this)[i]-num);

	return ris;
	}

//***************** monoargumental operators of a vector and constant **********************

template <class T>
void ImpSimpleMatrix<T>::operator /= (t_real num)
	{

	if (fabs((double)num)<=REAL_ZERO)
		merr<<"zero elem in division : "<<num;
	 
	ITERATE_ALL((*this)[i]/=num);

	return;
	}

template <class T>
void ImpSimpleMatrix<T>::operator *= (t_real num)
 	{

	ITERATE_ALL((*this)[i]*=num);

	return;
	}

template <class T>
void ImpSimpleMatrix<T>::operator += (t_real num)
 	{
	 
	ITERATE_ALL((*this)[i]+=num);

	return;
	}

template <class T>
void ImpSimpleMatrix<T>::operator -= (t_real num)
	{

	ITERATE_ALL((*this)[i]-=num);

	return;
	}

//********************* scalar products *************************************************

template <class T>
ImpSimpleMatrix<T> &  ImpSimpleMatrix<T>::operator | (const ImpSimpleMatrix<T>& vet) const
	{
	t_index k,row,col, num_row,num_col,num_iter;
	static ImpSimpleMatrix<T> ris;

	Check(vet.Dim_Row()==Dim_Col(), "Error on dimensions in scalar product "<<"vet.Dim_Row()="<<vet.Dim_Row()<<" Dim_Col()"<<Dim_Col());

	num_row = Dim_Row();
	num_col = vet.Dim_Col();
	num_iter = Dim_Col();

	//check if there is a situation like a=b|a
	if (this==&ris OR &ris==&vet)
		{
		ImpSimpleMatrix<T> aux;

		aux.Destroy_And_ReDim(num_row,num_col);

		for (row=0;row<num_row;row++)
			for (col=0;col<num_col;col++)
				for (k=0;k<num_iter;k++)
					aux[row][col]+=(*this)[row][k]*vet[k][col];
		ris=aux;
		}
	else
		{
		ris.Destroy_And_ReDim(num_row,num_col);

		for (row=0;row<num_row;row++)
			for (col=0;col<num_col;col++)
				for (k=0;k<num_iter;k++)
					ris[row][col]+=(*this)[row][k]*vet[k][col];
		}

	return ris;
	}

template <class T>
void ImpSimpleMatrix<T>::operator |= (const ImpSimpleMatrix<T>& vet)
	{
	t_index k,row,col, num_row,num_col,num_iter;

	Check(vet.Dim_Row()==Dim_Col(), 
		 "Error on dimensions in scalar product "<<"vet.Dim_Row()="<<vet.Dim_Row()<<" Dim_Col()"<<Dim_Col());

	num_row = Dim_Row();
	num_col = vet.Dim_Col();
	num_iter = Dim_Col();

	ImpSimpleMatrix<T> aux;

	aux.Destroy_And_ReDim(num_row,num_col);

	for (row=0;row<num_row;row++)
	    for (col=0;col<num_col;col++)
			for (k=0;k<num_iter;k++)
				aux[row][col]+=(*this)[row][k]*vet[k][col];
	
	(*this)=aux;
	 
	return;
	}

template <class T>
T&  ImpSimpleMatrix<T>::operator | (const T & vet)   const
	{
	t_index k,row, num_row,num_iter;
	static T ris;

	Check(vet.Dim()==Dim_Col(), "Error on dimensions in scalar product "<<"vet.Dim_Row()="<<vet.Dim()<<" Dim_Col()"<<Dim_Col());

	num_row = Dim_Row();
	num_iter = Dim_Col();

	ris.Destroy_And_ReDim(num_row);

	for (row=0;row<num_row;row++)
		for (k=0;k<num_iter;k++)
			ris[row] += ((*this)[row][k])*vet[k];

	return ris;
	}

//******************* boolean operator***************************************************

template <class T>
Boolean ImpSimpleMatrix<T>::operator== (const ImpSimpleMatrix<T>& sub) const
	{
	if(Dim()!=sub.Dim())
		return FALSE;
	else
		{
		t_index i=0,lim=Dim();
		while (i<lim AND (*this)[i]==sub[i])
			i++;
		if (i==lim)
			return TRUE;
		else return FALSE;
		}
	}

template<class T> 
Boolean ImpSimpleMatrix<T>::operator != (const ImpSimpleMatrix<T>& sub) const
	{
	if ((*this)==sub)
		return FALSE;
	else return TRUE;
	}


template<class T>
void ImpSimpleMatrix<T>::operator = (const ImpSimpleMatrix<T> & source)
	{
	t_index i,j;
	t_index row, col;

	row = source.Dim_Row();
	col = source.Dim_Col();

	Destroy_And_ReDim(row,col);

	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			(*this)[i][j]=source[i][j];
	return;
	}

template<class T>
void ImpSimpleMatrix<T>::Reset()
	{

	ITERATE_ALL((*this)[i].Reset());

	ImpObjectList<T>::Reset();
	return;
	}

template<class T>
void ImpSimpleMatrix<T>::Set(t_real num)
	{
	 
	ITERATE_ALL((*this)[i].Set(num));

	return;
	}

template <class T>
void ImpSimpleMatrix<T>:: Chop ()
	{

	t_index i, j, row=Dim_Row(), col=Dim_Col();
	double aux, integer,dif;

	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			{
			aux= (*this)[i][j];
			if (aux>0) 
				integer=floor(aux+PRECISION*1000.0); 
			else integer= ceil(aux-PRECISION*1000.0);

			dif=integer-(*this)[i][j];
			if (fabs(dif)<=PRECISION)
				 (*this)[i][j]=integer;
			}

	return;
	}

template<class T>
void ImpSimpleMatrix<T>::Set_Data_Chunk(t_index requested_data_chunk)
	{
	(*this).Set_Data_Chunk (requested_data_chunk);
	return;
	}

template<class T>
void ImpSimpleMatrix<T>::Destroy_And_ReDim(t_index row,t_index col)
	{
	t_index i;

	ImpObjectList<T>::Destroy_And_ReDim(row);

	for(i=0;i<row;i++)
		(*this)[i].Destroy_And_ReDim(col);

	return;
	}

template<class T>
void ImpSimpleMatrix<T>::Save_And_ReDim(t_index row,t_index col)
	{
	t_index i;
	ImpObjectList<T>::Save_And_ReDim(row);
	for(i=0;i<row;i++)
		(*this)[i].Save_And_ReDim(col);

	return;
	}

template<class T>
void ImpSimpleMatrix<T>::Destroy_And_ReDim(t_index dimension)
	{
	dimension++; //dummy function
	merr<<"function Destroy_And_ReDim with one parameter only not allowed with matrix";
	}

template<class T>
void ImpSimpleMatrix<T>::Save_And_ReDim(t_index col)
	{
	col++;//dummy function
	merr<<"function Save_And_ReDim with one parameter only not allowed with matrix";
	}



#endif