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
//  |   FILE:   tspecbas.hpp                                            |
//  |   FUNCTIONALITY: implementation of template function              |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 29/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


#ifndef _TSPECBASE_HPP_
#define _TSPECBASE_HPP_

#include "../tspecmod/tspecbas.h"

//**********************************************************************/
//                                                                      /
//                      NodeSpecShape                                   /
//                                                                      /
//**********************************************************************/

template <class T>
void NodeSpecShape<T>::Read(ifstream& input_f, t_index num_gauss, t_index state, 
		                       Boolean is_full, const t_index stat_dim)
	{
    String str_input;
	t_index num_state;
	t_index i;

    input_f>>str_input;

	if (str_input!="State:")
		merr<<"Wrong input file of acoustic model"<<endl;
       
	input_f>>num_state;
	if (num_state!=state)
		merr<<"Wrong input file of acoustic model"<<endl;

	for (i=0; i<num_gauss; i++)
		{
		(*this)[i].full_cov=is_full;
		(*this)[i].Read (input_f, stat_dim);
		}

	return;
    }



//**********************************************************************/
//                                                                      /
//                         Hmm                                     /
//                                                                      /
//**********************************************************************/

template <class T>
Boolean Hmm<T>:: Compute_G_Const()
	{
    t_index i, j;
	Boolean valid, flag=TRUE;

    for (i=0; i<num_states-2; i++)  
		for (j=0; j<num_gauss; j++)
			{
			valid=(* this)[i][j].Compute_G_Const();
			if(!valid)
				{
				flag=FALSE;
				mwarn<<"Invalid inverse matrix in state "<<i<<" num_gauss "<<j;
				}
			}
	
    return flag;
	}


template <class T>
void Hmm<T>:: Read_Transition_Matrix (ifstream & input_f)
	{
    String str_input;
	t_index i, j;

	input_f.precision(OUTPUT_SIZE);
  
    //read Transition probabilities matrix
	input_f>>str_input;
  	if (str_input!="Transition_probabilities_matrix:")
		merr<<"Wrong input file of acoustic models";

    trans_mat.Destroy_And_ReDim(num_states, num_states);

	for (i=0; i<num_states; i++)
		for (j=0; j<num_states; j++)
			input_f>>trans_mat[i][j];

	return;

	}


template <class T>
void Hmm<T>::Configure (t_index phon, t_index st_d, t_index stat, t_index gauss)
	{
    phoneme=phon;
    num_states=stat;
	num_gauss=gauss;
	stat_dim=st_d;
	return;
	}


template <class T>
void Hmm<T>::Read(ifstream& input_f, Boolean is_full)
	{
    String str_input, string, str_space;
	t_index ind_ph;
	t_index i;
	
	input_f>>str_input;

	if (str_input!="SPECTRAL_DESCRIPTION_OF_PHONEME:")
		merr<<"Wrong input file of acoustic model : phoneme "<<phoneme<<endl;

    //read the index of phonemes
    input_f>>ind_ph;
	//if (ind_ph!=phoneme)
    // merr<<"Wrong input file of acoustic model : phoneme"<<phoneme<<endl;    
	
	while (ind_ph!=phoneme)
		{
		do
			input_f>>str_input;
		while (str_input!="SPECTRAL_DESCRIPTION_OF_PHONEME:" AND !input_f.eof());
		if (input_f.eof())
			merr<<"Wrong input file of acoustic model : phoneme"<<phoneme<<endl;    
		else input_f>>ind_ph;
		}

	//read "Graph_nodes_number:"  
	input_f>>str_input;
	if (str_input!="Graph_nodes_number:")
		merr<<"Wrong input file of acoustic model : phoneme"<<phoneme<<endl;

	input_f>>num_states;

	//first and last state are dummy states so they haven't got any
	//spectral representation associated, so that:
	Destroy_And_ReDim(num_states-2);

    //read "Num_gauss:"
	input_f>>str_input;
    if (str_input!="Num_gauss:")
		merr<<"Wrong input file of acoustic model : phoneme"<<phoneme<<endl;

	input_f>>num_gauss;

    //for each states
	for (i=0; i<num_states-2; i++)
		{
		//allocate memory
		(*this)[i].Destroy_And_ReDim(num_gauss);

		//read the mixture of the i-th state
		(*this)[i].Read (input_f, num_gauss, i, is_full, stat_dim);
		}

    Read_Transition_Matrix (input_f);
    //Nel riconoscitore ho la funzione 
    //Set_Single_Phon_Graph
    //che legge la matrice di transizione del modello di Markov
    //e costruisce il livello 0 della struttura statica
    //in questa nuova impostazione, la funzione suddetta
	//dovrà solo costruire il livello 0 leggendo
    // la matrice di transizione dall'oggetto di tipo RecogCodebook

    input_f>>str_input;
    if (str_input!="END_OF_PHONEME_DESCRIPTION.")
		merr<<"Wrong input file of acoustic model : phoneme"<<phoneme<<endl;

    return;
	}

template <class T>
void Hmm<T>::Write (const String & f_out_name)
	{
    ofstream f_output;
	t_index i, j;
	t_index num_rows, num_cols;
	
	f_output.open (f_out_name, ios::app);
	if (f_output.fail())
		merr<<"Cannot open file : "<<f_out_name;

    f_output<<"SPECTRAL_DESCRIPTION_OF_PHONEME: "<<phoneme<<endl;
	f_output<<"Graph_nodes_number: "<<num_states<<endl;
    f_output<<"Num_gauss: "<<num_gauss<<endl;

    for (i=0; i<num_states-2; i++)
		{
		f_output<<endl<<"State: "<<i<<endl;
		for (j=0; j<num_gauss; j++)
			(* this)[i][j].Write (f_output);
		}

    f_output<<"Transition_probabilities_matrix:"<<endl;
	num_rows=trans_mat.Dim_Row();
	num_cols=trans_mat.Dim_Col();

	f_output.precision(OUTPUT_SIZE);
	for (i=0; i<num_rows; i++)
		{
		for (j=0; j<num_cols; j++)
	    f_output<<trans_mat[i][j]<<'\t';
		f_output<<endl;
		} 

	f_output<<endl<<"END_OF_PHONEME_DESCRIPTION."<<endl<<endl;
	
	return;
	
	}

#endif