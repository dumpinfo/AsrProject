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
//  |   FILE:   baumwelc.cpp                                            |
//  |   FUNCTIONALITY: main function for Baum Welch algorithm           |
//  |   PROGRAM:														|
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio & Massimo & Fabrizio 					|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98					                |
//  |   FIRST VERSION:                                                  |
//  |   DATA FIRST VERSION:			                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________



#ifndef _TRAINING_
#include "../training/baumwelc.h"
#endif

	
int main (t_index argc, char* argv[])
	{
	Training train;
	String config_file;
			
	if ( argc!=2)
		{
		mstat<<"No configfile name specified, using: res.ini";
		config_file="res.ini";
		}
	else config_file=argv[1];
		
	train.Configure(config_file);

	train.Baum_Welch();
	cout<<"ok";
	cout<<"ok"<<endl;
	return 1;
	}
