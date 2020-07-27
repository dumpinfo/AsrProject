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
//  |   FILE:   eval_rec.cpp											|
//  |   FUNCTIONALITY: main program of evaluate module                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico											|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 23/1/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************


#define EVALUATE_VERSION "V.6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__


#include "../eval_rec/evaluate.h"
#include "../eval_rec/evalopt.h"


int main (int argc, char * argv[])
	{
	String conf_file_name;
	EvalOption option;
	Evaluate eval;

	if (argc!=2)
		{
		mwarn << "No configure file name specified, using \"res.ini\""<<endl;
		conf_file_name= "res.ini";
		}
	else conf_file_name=argv[1];

	option.Configure(conf_file_name);
	eval.Evaluate_Recog_Performance (option);

	return 1;
	}