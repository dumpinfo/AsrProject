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
//  |   FILE:   testconf.cpp                                            |
//  |   FUNCTIONALITY: testing configuration class                      |
//  |   PROGRAM: required to codes which opens ires config file         |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio      claudio                            |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98					                |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 18/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

//9/2/98 eliminated const char *

#include "../resconf/resconf.h"
#include "../baseclas/diagnost.h"
#include <stdio.h>


void main()
	{
	String a;
	Test_Config_Class();
	mstat<<"push a key to exit";
	cin>>a;
	return;
	}

