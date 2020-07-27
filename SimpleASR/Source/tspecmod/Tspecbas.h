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
//  |   FILE:   tspecbas.h                                              |
//  |   FUNCTIONALITY: spectral model of phonemes                       |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico  , Fabrizio				                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 12/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *********** version history *******************************************


#ifndef __TSPECBAS_H_
#define __TSPECBAS_H_

#include <fstream.h>
#include "../vetclas/vetclas.h"
#include "../vetclas/arraycla.h"
#include "../vetclas/arraycla.hpp"
#include "../vetclas/diagclas.h"
#include "../vetclas/diagclas.hpp"
#include "../baseclas/textclas.h"


class Gaussian
	{

	public:

		Boolean full_cov;
		t_real weight;
		VetDouble mean;
		MatrixOfDouble inv_cov;
		DiagMatrixOfDouble diag_inv_cov;

		t_real gConst;

		Boolean Compute_G_Const ();
		t_real Evaluate_Exp_Gauss(const VetDouble& obs_vet)const;
        void Read(ifstream& s, const t_index stat_dim);
		void Write(ofstream& s);
	};



template <class T>
class NodeSpecShape:public ImpObjectList<T>
	{
	public:

		void Read(ifstream& input_f, t_index num_gauss, t_index state, 
		                                   Boolean is_full, const t_index stat_dim);

	};

template <class E>
class Hmm:public ImpObjectList<E>
	{

		void Read_Transition_Matrix (ifstream & input_f);
	
	public:

		Boolean full_covariance;

		t_index phoneme;
		t_index num_states;
		t_index num_gauss;
		t_index stat_dim;
		MatrixOfDouble trans_mat;

		void Configure (t_index phon, t_index st_d, t_index stat, t_index gauss);

		void Read (ifstream& input_f, Boolean is_full);
		void Write (const String & f_out_name);
    
		Boolean Compute_G_Const ();
     
	};

void Read_Data_File_Header (ifstream & input_file, t_index & stat_dim,
							Boolean & is_full);

void Write_Header_Of_File_Model (const String & f_out_name, const String& file_type, const t_index label, 
              const String & f_list, const t_index w_lenght, const t_index w_overlap, 
			  const t_index stat_d, const Boolean is_full);


#endif
