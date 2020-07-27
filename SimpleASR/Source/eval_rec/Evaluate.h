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
//  |   FILE:   Evaluate.h                                              |
//  |   FUNCTIONALITY: evaluate recognition                             |
//  |   PROGRAM: required to all codes                                  |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico				                            |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 1996                                        |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************


#ifndef __EVALUATE_H_
#define __EVALUATE_H_

#include "../vetclas/vetclas.h"
#include "../vetclas/vetclas.hpp"
#include "../vetclas/arraycla.h"
#include "../vetclas/arraycla.hpp"
#include "../baseclas/textclas.h"
#include "../eval_rec/evalopt.h"


#include <fstream.h>   

class Evaluate
	{
		OrderedList<String,String> symbol_table;
		ImpObjectList<VetULong> solution;
		ImpObjectList<VetULong> result;
		MatrixOfULong cost;

		t_index insertion;
		t_index omission;
		t_index substitution;

		VetULong omiss;     
		VetULong insert;    
		MatrixOfULong sost; 

		VetULong sym_sol;  
		VetULong sym_res;  

		void Build_Symbol_Table (const String & file_name);

		void Read_Sentences (const String & f_name, ImpObjectList<VetULong> & sentences,
	               VetULong & sym_list, Boolean sil_err, const String & sil_sym);
        
		void Build_Cost_Matrix (const VetULong & sol, const VetULong & res);
		void Compute_Error_On_One_Sentence (const VetULong & sol, const VetULong & res,
                     Boolean each_sentence, const String & f_name, t_index sent);
		void Compute_Error (const EvalOption & option);

	public:
      
		void Evaluate_Recog_Performance (const EvalOption & option);

	};

   
#endif __EVALUATE_H_