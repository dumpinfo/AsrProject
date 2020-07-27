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
//  |   FILE:   TestTspecbase.cpp                                       |
//  |   FUNCTIONALITY:  main for tspecbase test                    		|
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio  Claudio			                    |
//  |   ACTUAL REVISION:                                                |
//  |   DATA ACTUAL REVISION: 19/11/98									|
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION:                                             |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    Improvement  *******************************
//added unix compatibility

// *******************    VERSION HISTORY  *******************************

// <<< version 1.0 >>>

//*******************   USER INFORMATION   *******************************

//*******************   IMPLEMENTATION INFORMATION   *******************************


//*******************   CLASS INDEX   *******************************

#include <fstream.h>

#include "../tspecmod/read_header_file.h"
#include "../tspecmod/tspecbas.h"
#include "../tspecmod/tspecbas.hpp"


void Test_Tspecbase (const String & file_name, t_index num_phonemes)
	{
    t_real preemp;
	Boolean is_full;
	t_index stat_d;
	t_index num_states;
	t_index num_gauss;
	t_index i;

	ifstream inp_file;
	ImpObjectList<RecogCodebook> list_codebook;

    inp_file.open (file_name,ios::in|ios::nocreate);
	if (inp_file.fail())
		merr<<"Cannot open file : "<<file_name<<endl;

	Read_Data_File_Header (inp_file, stat_d, is_full);

    for (i=0; i<num_phonemes; i++)
		{
		list_codebook.Save_And_ReDim(i+1);
		list_codebook[i].Configure(i, stat_d, num_states, num_gauss);
		list_codebook[i].Read(inp_file, is_full);
		}

    String f_name;
	String f_list;
	f_name="result.tra";
	f_list="dbfiles.lst";

    Write_Header_Of_File_Model (f_name, 0, 1, f_list, 512, 384, 0.95, stat_d, is_full);

	for (i=0; i<num_phonemes; i++)
		{
		list_codebook[i].Compute_G_Const();
		list_codebook[i].Write (f_name);
		}
	
    VetDouble v;
    t_real res;

	v.Destroy_And_ReDim(stat_d);
	v.Set(1.2);
    res=list_codebook[0][0][0].Evaluate_Exp_Gauss(v);

    return;
	}

void main ()
	{
    String file_name;
    t_index num_phonemes;
	char ch;

    mstat<<"Type the name of file with acoustic model for test :";
	cin>>file_name;
    mstat<<"Type the number of phonemes :";
	cin>>num_phonemes;

    mstat<<"The name of the output file is : \"result.tra\""<<endl;

    Test_Tspecbase (file_name, num_phonemes);

	mstat<<"The test is ok!"<<endl<<"Type a key and return"<<endl;
	cin>>ch;

	return;
	}
