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
//  |   FILE:   arraycla.h                                              |
//  |   FUNCTIONALITY: matematical array                                |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes                                |
//  |   CONTRIBUTIONS: Domenico   claudio								|
//  |   ACTUAL REVISION: 6.0					                        |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 24/9/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________
// *******************    VERSION HISTORY  *******************************************
// <<< version 4.0 >>>
//16/9/97 corrected bug  by claudio not revised
//19/09/97 implemented functions chop()
//          and Change_Diag2Full(const ImpDiagMatrix<T>&) by Domenico  
//20/09/97 revised by claudio conversion operators among  Change_Diag2Full <<=
//22/09/97 added by Domenico function |= in the ImpSimpleMatrix class
//23 /9/97 revised code read by claudio
#ifndef __ARRAYCLAS_H
#define __ARRAYCLAS_H

#include <math.h>
#include "../vetclas/vetclas.h"
#include "../vetclas/vetclas.hpp"

template <class E>
class ImpDiagMatrix;

template<class T>
class ImpSimpleMatrix :public ImpObjectList<T>
	{

	 public:

	 //elem by elem

     //********************  biargumental operators of matrixs **************************

	 ImpSimpleMatrix<T> & operator / (const ImpSimpleMatrix<T>& vet) const;
	 ImpSimpleMatrix<T> & operator * (const ImpSimpleMatrix<T>& vet) const;
	 ImpSimpleMatrix<T> & operator + (const ImpSimpleMatrix<T>& vet) const;
	 ImpSimpleMatrix<T> & operator - (const ImpSimpleMatrix<T>& vet) const;


	 //******************* monoargumental operators of matrixs **************************

	 void operator += (const ImpSimpleMatrix<T> & vet);
	 void operator -= (const ImpSimpleMatrix<T> & vet);
	 void operator *= (const ImpSimpleMatrix<T> & vet);


	 //****************** biargumental operators of a matrix and constant ***************

	 ImpSimpleMatrix<T> & operator / (t_real num) const;
	 ImpSimpleMatrix<T> & operator * (t_real num) const;
	 ImpSimpleMatrix<T> & operator + (t_real num) const;
	 ImpSimpleMatrix<T> & operator - (t_real num) const;


	 //***************** monoargumental operators of a vector and constant **************

	 void operator /= (t_real num);
	 void operator *= (t_real num);
	 void operator += (t_real num);
	 void operator -= (t_real num);


	 //***************** scalar products ************************************************

	 ImpSimpleMatrix<T> &  operator | (const ImpSimpleMatrix<T>& vet) const;
	 void operator |= (const ImpSimpleMatrix<T>& vet);

	 T &  operator | (const T & vet) const;


	 //***************** boolean operators ***********************************************

	 Boolean operator==(const ImpSimpleMatrix<T> & sub) const;
	 Boolean operator!=(const ImpSimpleMatrix<T> & sub) const;



	 void operator =(const ImpSimpleMatrix<T> & source);


	 void Reset();

	 //set to num all matrix
	 void Set(t_real num);

	 // rounds machine precision errors
	 void Chop();


	 void Set_Data_Chunk(t_index requested_data_chunk);

	 void Destroy_And_ReDim(t_index row,t_index col);
	 void Save_And_ReDim(t_index row,t_index col);

	 //not allowed
	 void Destroy_And_ReDim(t_index row);
	 void Save_And_ReDim(t_index row);

	 t_index Dim_Row() const
		{ return Dim();}

	 t_index Dim_Col() const
		 { return ((*this)[0]).Dim();}

	};


typedef ImpSimpleMatrix<VetULong> MatrixOfULong;

class MatrixOfDouble :public ImpSimpleMatrix<VetDouble>
	{

	 // perform low-up decomposition
	 void Low_Up_Dcmp(MatrixOfDouble &L, MatrixOfDouble &U);
	 
	 void Solve_Triangular(VetDouble &y, VetDouble &b);

	 void Householder();

	 public:

	 void operator =(const ImpSimpleMatrix<VetDouble> & source)
		{
		ImpSimpleMatrix<VetDouble>::operator =(source);
		return;
		}

	 void operator =(const MatrixOfDouble & source)
		{
		ImpSimpleMatrix<VetDouble>::operator =(source);
		return;
		}

	 void Transpose_Of(const MatrixOfDouble & x);
    
	 void EigenValues_And_EigenVectors(VetDouble& eigenvalues,
						MatrixOfDouble& eigenvectors) const;

	//return determinant of (*this)rix is destructive
	//return zero if singular
	t_real Inverse();

	};


	Boolean Test_Array_Mat_Functions();
	 //change from diag matrix form to full

	 template <class E,class T>
	 void operator<<=(ImpSimpleMatrix<T> & full,const ImpDiagMatrix<E>& diag)
		{
		t_index i,lim=diag.Dim();
		full.Destroy_And_ReDim(lim, lim);

		for ( i=0;i<lim;i++)
		  full[i][i]=diag[i][i];

		return;
		}

#endif