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
//  |   FILE:   evalopt.cpp                                             |
//  |   FUNCTIONALITY: evaluate option                                  |
//  |   PROGRAM: required to all codes                                  |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico                                         |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 1996                                        |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#include "../eval_rec/evalopt.h"

void EvalOption::Configure (const String & config_file_name)
	{
    ConfigFile config;
	String string_option;
         
	//open config file
	config.Open_File(config_file_name);

    //type of recognition symbols
	config.Get_String_Opt("Evaluate", "SymbolType", string_option);
	if (string_option=="phonemes")
		t_symbol=phonemes;
	else //if (string_option=="words")
		t_symbol=words;

	config.Get_String_Opt("Evaluate", "SymbolFileName", symb_file_name);
    config.Get_String_Opt("Evaluate", "SolutionFileName", solution_file_name);
	config.Get_String_Opt("Evaluate", "ResultFileName", result_file_name);
    config.Get_String_Opt("Evaluate", "ReportFileName", report_file_name);
	config.Get_String_Opt("Evaluate", "ReportEachSentenceFileName", report_each_sent_file_name);
	config.Get_String_Opt("Evaluate", "ReportConfMatrixFileName", report_conf_matrix_file_name);
	
    each_sentence=config.Get_Boolean_Opt ("Evaluate", "ReportEachSentence");
	each_symbol=config.Get_Boolean_Opt ("Evaluate", "ReportEachSymbol");
	conf_matrix=config.Get_Boolean_Opt ("Evaluate", "ReportConfMatrix");
	sil_err = config.Get_Boolean_Opt ("Evaluate", "SilenceErrorCount");
	config.Get_String_Opt ("Evaluate", "SilenceSymbol", sil_sym);

	return;
	}