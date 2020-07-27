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
//  |   FILE:   diagclas.hpp                                            |
//  |   FUNCTIONALITY: matematical diagonal matrix                      |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:implementation function of template classes            |
//  |   CONTRIBUTIONS: Domenico   claudio				                |
//  |   ACTUAL REVISION: 6.0						                    |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 24/9/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#define DIAG_CLASS_VERSION "V. 6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__

// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
// 28/4/98 fixed bug on operator[] of diagonal matrix
//const T & operator[]() const  becomes T  operator[]() const

#ifndef _DIAGMAT_HPP_
#define _DIAGMAT_HPP_

#include "../vetclas/diagclas.h"


template <class T>
class ImpDiagVector : public ImpDiagMatrix<T>
	{

	public:

	//necessary for the functions inherited from ImpMatVector class
	void operator = (const ImpMatVector<T> & vet);

	inline T& operator [] ( t_index index );

	inline T operator [] ( t_index index ) const;
	};

template <class T>
inline T ImpDiagVector<T>::operator [] ( t_index index ) const
	{

    Assert(my_row<Dim() AND index<Dim());

    if(my_row!=index)
		return ((T) 0.0);
	else return (lis[index]);
    }

template <class T>
inline T& ImpDiagVector<T>::operator [] ( t_index index )
	{
	Assert(my_row<Dim() AND index<Dim());

	if(my_row!=index)
		 merr<<"extradiagonal element ("<<my_row<<","<<index<< ") cannot be changed in diag matrixes";
	return (T&)lis[index];
	}


template <class T>
ImpDiagVector<T> & ImpDiagMatrix<T>::operator [] ( t_index index )
	{
	my_row=index;

	return(  ImpDiagVector<T> & )(*this);

	}

template <class T>
const ImpDiagVector<T> & ImpDiagMatrix<T>::operator [] ( t_index index ) const
	{
	t_index * p_index;

	p_index=(t_index *) &(my_row);

	(*p_index)=index;

	return( const ImpDiagVector<T> & )(*this);

	}

template <class T>
ImpDiagMatrix<T>& ImpDiagMatrix<T>:: operator | (const ImpDiagMatrix<T> & mat) const
	{

	static ImpDiagMatrix<T> ris;

	t_index lim=Dim();

	Check(Dim()==mat.Dim(),                                                            
		 "Diagonals matrixs of different dimensions :"<<Dim()<<" and "<<mat.Dim());

	ris.Save_And_ReDim(lim,lim);

	ITERATE_ALL(ris[i][i]=(*this)[i][i]*mat[i][i]);

	return ris;
	}

template <class T>
void ImpDiagMatrix<T>:: operator |= (const ImpDiagMatrix<T>& mat)
	{

	Check(Dim()==mat.Dim(),
		 "Diagonals matrixs of different dimensions :"<<Dim()<<" and "<<mat.Dim());

	ITERATE_ALL((*this)[i][i]*=mat[i][i]);

	return;
	}

template <class T>
ImpMatVector<T> & ImpDiagMatrix<T>:: operator | (const ImpMatVector<T>& vet) const
	{
	static ImpMatVector<T> ris;
	t_index lim=Dim();

	Check(lim==vet.Dim(),
		 "Different dimensions in the scalar product:"<<Dim()<<" and "<<vet.Dim());

	ris.Save_And_ReDim(lim);

	ITERATE_ALL(ris[i]=(*this)[i][i]*vet[i]);

	return ris;
	}

template <class T>
void ImpDiagMatrix<T>::operator =(const ImpMatVector<T>& vet)
	{
	t_index lim=vet.Dim();

	(*this).Destroy_And_ReDim(lim, lim);

	ITERATE_ALL((*this)[i][i]=vet[i]);

	return;
	}

template <class T>
t_real ImpDiagMatrix<T>:: Inverse()
	{
	t_real det=1.0,temp;;
	ITERATE_ALL(
		temp=(*this)[i][i];
		if (fabs(temp)<=REAL_ZERO)
		  return 0.0;
		(*this)[i][i]=(T)(1/temp);
		det*= temp;
		);

	return det;

   }  

template <class T>
void ImpDiagMatrix<T>::Chop ()
	{
	t_index aux, i, col=Dim();
	t_real	dif,integer;

	for(i=0;i<col;i++)
		{
		aux= (t_index)(*this)[i][i];
		if (aux>0) 
			integer=floor(aux+PRECISION*1000.0); 
		else integer= ceil(aux-PRECISION*1000.0);

		dif=integer-(*this)[i][i];
		if (fabs(dif)<=PRECISION)
			 (*this)[i][i]=integer;
		}

	return;
	}

template <class T>
void ImpDiagMatrix<T>::EigenValues_And_EigenVectors
							 (VetDouble& eigenvalues,ImpDiagMatrix<t_real>& eigenvectors) const
	{
	t_index i,lim=Dim();

	eigenvalues.Destroy_And_ReDim(lim);

	for(i=0;i<lim;i++)
		eigenvalues[i]=(*this)[i][i];
	eigenvectors.Destroy_And_ReDim(lim,lim);

	eigenvectors.Set(1.0);

	return;
	}

template <class T>
void ImpDiagMatrix<T>::Destroy_And_ReDim(t_index row,t_index col)
	{
	Check(row==col, 
		 "row!=col in Destroy_And_ReDim() of a diagonal matrix: row="<<row<<" col="<<col);

	ImpMatVector<T>::Destroy_And_ReDim(row);
	//assure that this field is not used for mistake
	my_row = 0UL-1;

	return;
	}

template <class T>
void ImpDiagMatrix<T>::Save_And_ReDim(t_index row,t_index col)
	{
	Check(row==col,
		 "row!=col in Save_And_ReDim() of a diagonal matrix: row="<<row<<" col="<<col);

	ImpMatVector<T>::Save_And_ReDim(row);

	return;
	}

template <class T>
void ImpDiagMatrix<T>::Destroy_And_ReDim(t_index row)
	{
	//not allowed call abort program
	merr<<"function Destroy_And_ReDim with one parameter only not allowed with diagonal matrix: "<<row;
	return;
	}

template <class T>
void ImpDiagMatrix<T>::Save_And_ReDim(t_index row)
	{
		//not allowed call abort program
	merr<<"function Save_And_ReDim with one parameter only not allowed with diagonal matrix: "<<row;
	return;
	}

#endif
