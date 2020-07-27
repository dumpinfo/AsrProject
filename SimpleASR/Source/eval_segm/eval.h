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
//  |   FILE:   Eval.h                                                  |
//  |   FUNCTIONALITY:                                                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico   & Fabrizio							|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION:  19/11/98                                 |
//  |   FIRST VERSION:   4.2                                            |
//  |   DATA FIRST VERSION: 22/09/98                                    |
//  |                                                                   |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#ifndef _EVAL_H_
#define _EVAL_H_

#include "../baseclas/textclas.h"
#include "../ioclass/labelcl.h"


class Error
	{
	public:
		
	t_index five;
	t_index ten;
	t_index twenty;
	t_index over;
	t_index error;
	t_index tot;

	Error ();
	};

class Eval
	{
	
	Boolean each_phon;

	t_real sample_rate;

	LabelFileInterface label;

	Error tot_count_error;

	ImpObjectList<String> file_list;
	String file_list_name;
	
	String f_autom_ext;
	String f_check_ext;
	String f_report_name;

	ImpObjectList<Error> final_count_err;
	ImpObjectList<Error> initial_count_err;

	void Evaluate_File_Segm (const String & file_name);

	public:

	void Read_Options (const String & conf_file_name);

	void Load_File_List ();

	void Evaluate_Segmentation ();

	void Write_Report_File ();


	};
#endif