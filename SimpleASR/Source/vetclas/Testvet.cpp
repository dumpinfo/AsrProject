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
//  |   FILE:   testvet.cpp                                             |
//  |   FUNCTIONALITY: test mathematical vector routines                |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 16/09/97                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//version 17/4/98 unix compatible
//16/9/97 read code revision by claudio
#include "../vetclas/vetclas.h"
#include "../vetclas/arraycla.h"
#include "../vetclas/diagclas.h"
#include "../vetclas/diagclas.hpp"

int main ()
	{

	if (Test_Array_Mat_Functions() )
		mwarn << "Test array ok ! "; 

	if (Test_Math_Vector_Functions() AND Imp_Simple_List_Test_Upper_Bound())
		mwarn<<"The test vector ok ! "; 

	if(Test_Math_Diagonal_Matrix_Function ())
		mwarn <<endl<<"Test diagonal matrix is ok ! ";
 
	
	mstat<<"Type a key for terminate program " <<endl;

	char ch;
	cin >> ch;
	return 1;
	}