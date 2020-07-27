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
//  |   FILE:   main_eval.cpp                                           |
//  |   FUNCTIONALITY:                                                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico  & Fabrizio								|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION:  19/11/98                                 |
//  |   FIRST VERSION:   4.2                                            |
//  |   DATA FIRST VERSION:   22/09/98                                  |
//  |                                                                   |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#include "eval.h"

void main (int argc, char * argv[])
	{
    String conf_file_name;
	Eval eval;
  
	if (argc!=2)
		{
		mwarn << "No configure file name specified, using \"res.ini\""<<endl;
		conf_file_name= "res.ini";
		}
	else conf_file_name=argv[1];

	eval.Read_Options(conf_file_name);

	eval.Load_File_List ();

	eval.Evaluate_Segmentation ();

	eval.Write_Report_File ();

	return;
	}