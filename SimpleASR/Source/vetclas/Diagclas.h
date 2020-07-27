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
//  |   FILE:   diagclas.h                                              |
//  |   FUNCTIONALITY: matematical diagonal matrix                      |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes                                |
//  |   CONTRIBUTIONS: Domenico   claudio				                |
//  |   ACTUAL REVISION: 6.0											|
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 24/9/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//21/9/97 draft revision by claudio
//22/9/97 by Domenico
//        1) corrected bugs on the constructor of the ImpDaigMatrix class
//        2) corrected bugs on the operator [] of the ImpDiagMatrix and 
//           ImpDiagVector classes
//        3) added the operator = in the ImpDiagMatrix class.
//           In the implementation of this function use Save_And_ReDim.
//        4) added the operator = in the ImpDiagVector class. 
//           In the implementation of this function use Save_And_ReDim.
//23/9/97 read code revisio by claudio


// *******************    IMPLEMENTATION   *******************************

// 1) Destroy_And_ReDim() and Save_And_ReDim() with a only parameter 
//    not allowed with matrix
// 2) Set(), Reset() and the boolean functions (operator== and operator!=)
//       inherited from ImpMatVector  
// 3) Inverse() return 0 if the matrix is singular else determinant


#ifndef _DIAGCLAS_H_
#define _DIAGCLAS_H_

#include "../baseclas/baseclas.h"
#include "../baseclas/diagnost.h"
#include "../vetclas/vetclas.h"

#include <math.h>

template <class T>
class ImpDiagVector;

template <class T>
class ImpDiagMatrix : public ImpMatVector<T>
	{

	protected:

	t_index my_row;

public:

	//to avoid unwanted cast to ImpDiagVector
	ImpDiagMatrix():my_row(0UL-1){}

	// ************* scalar product ***********************************


	// scalar product of two diagonal matrix
	ImpDiagMatrix<T> & operator | (const ImpDiagMatrix<T>& mat) const;

	void operator |= (const ImpDiagMatrix<T>& mat);



	ImpMatVector<T> & operator | (const ImpMatVector<T>& vet) const;

	// ************ other functions ***********************************

	void operator =(const ImpMatVector<T>& vet);
	
	//return determinant 0, if singular
	t_real Inverse();

	void Chop ();

	void EigenValues_And_EigenVectors(VetDouble& eigenvalues,
						 ImpDiagMatrix<t_real>& eigenvectors) const;

	ImpDiagVector<T> & operator [] ( t_index index );
	const ImpDiagVector<T>  & operator [] ( t_index index ) const;

	void Destroy_And_ReDim(t_index row,t_index col);
	void Save_And_ReDim(t_index row,t_index col);
    //not allowe calls retun abort
	void Destroy_And_ReDim(t_index row);
	void Save_And_ReDim(t_index row);

	t_index Dim_Row() const
		{ return Dim();}

	t_index Dim_Col() const
		{ return Dim();}

	};



typedef ImpDiagMatrix<t_real> DiagMatrixOfDouble;
typedef ImpDiagMatrix<t_signed> DiagMatrixOfSigned;
typedef ImpDiagMatrix<t_index> DiagMatrixULong;


Boolean Test_Math_Diagonal_Matrix_Function ();

#endif