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
//  |   FILE:   evalopt.h                                               |
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


// *******************    VERSION HISTORY  *******************************


#ifndef __EVALOPT_H_
#define __EVALOPT_H_

#include "../baseclas/textclas.h"
#include "../resconf/resconf.h"

enum TypeOfSymbols {phonemes, words};

class EvalOption
	{
	public:
      
		TypeOfSymbols t_symbol; //type of recognition symbol
		String symb_file_name;
		String solution_file_name;
		String result_file_name;
		String report_file_name;
		String report_each_sent_file_name;
		String report_conf_matrix_file_name;

		Boolean each_sentence; //result each sentence
		Boolean each_symbol;   //result each symbol
		Boolean conf_matrix;

		Boolean sil_err;
		String sil_sym;

		void Configure (const String & config_file_name);
	};

#endif