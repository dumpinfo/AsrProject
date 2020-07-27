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
//  |   FILE:   testIO.cpp                                              |
//  |   FUNCTIONALITY: testing I/O class                                |
//  |   PROGRAM: required to codes which opens databases                |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio 										|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION:   19/11/98                                |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION:                                             |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#include "../ioclass/soundlab.h"
#include "../baseclas/diagnost.h"
#include <stdio.h>


int main()
	{
	String key;
	Test_IO_Class();
	mstat<<"Type a key to exit";
	cin>>key;

	return TRUE;
	}

