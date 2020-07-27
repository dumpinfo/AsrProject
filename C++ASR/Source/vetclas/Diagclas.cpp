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
//  |   FILE:   diagclas.cpp                                            |
//  |   FUNCTIONALITY: diagonal matrix                                  |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes				                |
//  |   CONTRIBUTIONS: Domenico   claudio								|
//  |   ACTUAL REVISION: 6.0  											|
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 24/9/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//23/9/97read code revised claudio 

// *******************    Code             *******************************

#include "../vetclas/diagclas.h"
#include "../vetclas/diagclas.hpp"
#include "../vetclas/arraycla.h"
#include "../vetclas/arraycla.hpp"

Boolean Test_Math_Diagonal_Matrix_Function ()
	{
	DiagMatrixOfDouble u,v,z,c;

	t_index dim=10, i;

	mwarn<<"diagclas version"<<DIAG_CLASS_VERSION;

	u.Destroy_And_ReDim(dim,dim);
	v.Destroy_And_ReDim(dim,dim);
	z.Destroy_And_ReDim(dim,dim);
	c.Destroy_And_ReDim(dim,dim);

	Check (v.Dim()==dim, "Error in Destroy_And_ReDim() function");

	u.Set(2);
	v.Set(3);
	z.Set(2);
	z*=u*v*u*u*5;
		
	c.Set(240);

	Check(c==z,"diagonal matrix routines for the product don't work");

	c+=u+80+v;
	z.Set(325);

	Check(c==z,"diagonal matrix routines for the addition don't work");
	Check(z[dim-1][dim-1]==c[0][0],"diagonal matrix routine operator[] don't work");
 
	c/=25;
	for (i=0;i<dim;i++)
		Check(c[i][i]==13.0,"diagonal routines operator/= don't work");

	c=u|v|v;
	for (i=0;i<dim;i++)
		Check(c[i][i]==18.0,"diagonal routines operator| don't work");

	c=u|v;
	for (i=0;i<dim;i++)
		Check(c[i][i]==6.0,"diagonal routines operator| don't work");

	VetDouble vect, ris;

	vect.Destroy_And_ReDim(dim);
	vect.Set(2);

	ris=c|vect;
	for (i=0;i<dim;i++)
		Check(ris[i]==12.0,"diagonal routines operator| don't work");

	v.Set(3);

	c|=v;
	for (i=0;i<dim;i++)
		Check(c[i][i]==18.0,"diagonal routines operator|= don't work");

	u[0][0]=12.0;
	u[1][1]=22.5;
	u[2][2]=343.1;
	u[3][3]=125.4;
	u[4][4]=2.0;
	u[5][5]=458.1;
	u[6][6]=75.2;
	u[7][7]=45.126;
	u[8][8]=75.2;
	u[9][9]=45.3;

	v=u;
	u.Inverse();
	c=v|u;
	c.Chop();

	for(i=0;i<dim;i++)
		Check(c[i][i]==1.0,
		"diagonal matrix routines Inverse() don't work : c["<<i<<"]="<<c[i][i]);

	DiagMatrixOfDouble eigvect;

	v.Set(3);
	v.EigenValues_And_EigenVectors(vect,eigvect);

	for(i=0;i<dim;i++)
		u[i][i]=vect[i];

	Check(u==v,"error in function EigenValues_And_EigenVectors");

	return TRUE;
	}

