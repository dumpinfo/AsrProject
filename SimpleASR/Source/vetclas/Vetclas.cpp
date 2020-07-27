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
//  |   FILE:   vetclas.cpp                                             |
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
#define VETCLAS_VERSION "6.0 of 19/11/98 " ## __FILE__ ##"c. "##__DATE__

// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//16/9/97 read code revision by claudio
//19/9/97 eliminated cerr
//10/01/98 added ImpMatVector<t_real> & operator +
//				(double arg, const ImpMatVector<t_real> & arg2)
// to allow vet= doublenum+ vet
//not all the operations are implemented only sum


// *******************    Code             *******************************
#include "../vetclas/vetclas.h"
#include "../vetclas/vetclas.hpp"
#include <math.h>

ImpMatVector<t_real> & operator + (double arg, const ImpMatVector<t_real> & arg2)
	{
	return (arg2 + arg);
	}



void VetDouble::operator = (const ImpMatVector<t_real> & vet)
	{
	t_index i,lim=vet.Dim();

	Destroy_And_ReDim(lim);

	for(i=0;i<lim;i++)
		(*this)[i]=(t_real)vet[i];
	return ;
	}

void VetDouble::operator = (const ImpMatVector<t_signed> & vet)
	{
	t_index i,lim=vet.Dim();

	Destroy_And_ReDim(lim);

	for(i=0;i<lim;i++)
		(*this)[i]=(t_real)vet[i];
	return ;
	}

void VetDouble::operator= (const VetShortInt & vet)
	{
	t_index i,len;
	len=vet.Dim();

	Destroy_And_ReDim(len);

	for(i=0;i<len;i++)
		(*this)[i]=(t_real)vet[i];
	return;
	}

Boolean Test_Math_Vector_Functions()
	{
	VetDouble v;
	VetDouble u;
	VetDouble z;
	VetDouble c;
	
	t_index i, dim=10;
	
	mwarn<<"vetclass version"<< VETCLAS_VERSION;

	u.Destroy_And_ReDim(dim);
	v.Destroy_And_ReDim(dim);
	z.Destroy_And_ReDim(dim);
	c.Destroy_And_ReDim(dim);

	u.Set(2);
	v.Set(3);

	z=1.0 +u;
	Assert( v==z);

	z=u*v*u*u*2;
	c.Set(48);

	if(c!=z)
		merr<<"vet routines do not work";
	v.Destroy_And_ReDim(dim);
	v.Destroy_And_ReDim(dim);
	
	Check (v.Dim()==10, "Error on dimension of vector v : "<<v.Dim());

	u.Set(9.4);
	for (i=0; i<dim; i++)
		Check (u[i]==9.4,"Error in the instruction \"u.Set(9.4)\" : u["<<i<<"] = "<<u[i]);
	
	z=u;
    for (i=0; i<dim; i++)
		Check (z[i]==9.4,
		  "Error in the instruction \"z=u\" : u["<<i<<"] = "<<u[i]<<" z["<<i<<"] = "<<z[i]);
	
	u/=3.2;
    for (i=0; i<dim; i++)
		Check(u[i]==2.9375, "Error in the instruction \"u/=3.2\" : u["<<i<<"] = "<<u[i]);
    
	v=u+u+z;
    for (i=0; i<dim; i++)
		Check(v[i]==15.275, "Error in the instruction \"v=u+u+z\" : v["<<i<<"] = "<<v[i]);
    
	u*=2.0;
    for (i=0; i<dim; i++)
		Check(u[i]==5.875, "Error in the instruction \"u*=2.0\" : u["<<i<<"] = "<<u[i]);
	
	v=u*2.0;
	for (i=0; i<dim; i++)
		Check(v[i]==11.75, "Error in the instruction \"v=u*2.0\" : v["<<i<<"] = "<<v[i]);
    
	v[3]=4.35;
    Check(v[3]==4.35, "Error in the instruction \"v[3]=4.35\" : v[3] = "<<v[i]);

	v[3]=11.75;
	v=v/2;
	for (i=0; i<dim; i++)
		Check(v[i]==5.875, 
	      "Error in the instructions \"v[3]=11.75;v=v/2;\" : v["<<i<<"] = "<<v[i]);
    
	u+=(v+z-u);
    for (i=0; i<dim; i++)
		Check(u[i]==15.275, 
	      "Error in the instruction \"u+=(v+z-u)\" : u["<<i<<"] = "<<u[i]);
    
	u*=u/dim/u[0];
    for (i=0; i<dim; i++)
		Check(u[i]==1.5275, 
	      "Error in the instruction \"u*=u/dim/u[0]\" : u["<<i<<"] = "<<u[i]);
    
	u.Destroy_And_ReDim(3);
	t_index dim_u=u.Dim();
    
	u[0]=2.0;
	u[1]=4.0;
	u[2]=12.0;

	t_real mean = u.Mean_Value();
	Check(mean==6, "Error in the instruction \"mean = u.Mean_Value()\" : mean = "<<mean);

	v.Destroy_And_ReDim(3);
	v=u+3;
    for (i=0; i<dim_u; i++)
		Check(v[i]==u[i]+3, 
	      "Error in the instruction \"v=u+3\" : v["<<i<<"] = "<<v[i]);
     
    v^=2;
    Check(v[0]==25.0 AND v[1]==49.0 AND v[2]==225.0, 
	      "Error in the instruction \"v^=2\"");
     
	t_real res=u|v;
	Check(res==2946, "Error in the instruction \"res=u|v\", res = "<<res);
	
    v=v/u;
    Check(v[0]==12.5 AND v[1]==12.25 AND v[2]==18.75, 
	      "Error in the instruction \"v=v/u\"");

	u.Set(2.0);

	u^=(-2);
    for (i=0; i<dim_u; i++)
		Check(u[i]==0.25, 
	      "Error in the instruction \"u^=(-2)\" : u["<<i<<"] = "<<u[i]);
   	
	u.Log();
	for (i=0; i<dim_u; i++)
		Check(u[i]-(-1.38629436)<=PRECISION, 
	      "Error in the instruction \"u.Log()\" : v["<<i<<"] = "<<v[i]);

	return TRUE;
	}

Boolean Imp_Simple_List_Test_Upper_Bound()
	{
	t_index i;
	ImpSimpleTypeList<char> vet;
	vet.Destroy_And_ReDim(120000UL);
	i=0;
	while(i<120000UL AND vet[i]=='\0')
		i++;

	if(i!=120000UL)
		{
		merr<<"Inizialization failed at index:"<<i;
		return FALSE;
		}
	vet.Set('a');
	for(i=70000UL;i<120000UL;i++)
		vet[i]='w';

	i=0;
	while(i<70000UL AND vet[i]=='a')
		i++;

	if(i==70000UL)
		return TRUE;
	merr<<"Out of Bound at index:"<<(70000UL-i-1)<<"maximum allowed lenght of array "<<(70000UL-i-1);
	return FALSE;
	}
