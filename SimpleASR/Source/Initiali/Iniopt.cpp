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
//  |   FILE:   iniopt.cpp                                              |
//  |   FUNCTIONALITY: class implementation for read options            |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio  & Fabrizio	Domenico	                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98						            |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 24/4/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


#include "../initiali/iniopt.h"

void IniOptions::Set_Options(const String& config_fname, t_index num_of_symbols)
	{

	ConfigFile conf;
	String buffer;  
	
	conf.Open_File(config_fname);
	
	conf.Get_String_Opt("Initialization","InitializedModelsFName", InitializedModelsFName);
		 
	load_one_mixture=conf.Get_Boolean_Opt("Initialization", "LoadOneMixture");

	unif_sect=conf.Get_Boolean_Opt("Initialization", "UniformSectionate");
	
	if(load_one_mixture)
		conf.Get_String_Opt("Initialization","ModelsFileInput", models_file_input);
	
	conf.Get_String_Opt("Initialization","SymbolRange",buffer);
	
	if(buffer=="all")
		{
		SymbolRange = All;
		} 
	else if(buffer=="partial")
			{
			SymbolRange = Partial;
			}
	else if(buffer=="category")
			{
			SymbolRange = Category;
			}
	else {
		 merr<<"Initialisation options. Bad symbol range specified!!";
		 }	  

	 
	conf.Get_String_Opt("Initialization","FileOfSymbolModels", FileOfSymbolModels);
		 
	if(SymbolRange==Partial)
		conf.Get_Range_Opt("Initialization","SymbolList", SymbolList);
	else {
		 t_index i;
		 SymbolList.Destroy_And_ReDim(num_of_symbols);
		 for(i=0;i<num_of_symbols;i++)
			SymbolList[i]=i;
		 }

    conf.Get_String_Opt("Initialization", "ModelType", buffer);
	if (buffer=="model_0")
		model_type=Model_0;
	else if (buffer=="model_1")
				model_type=Model_1;
	else if (buffer=="model_2")
				model_type=Model_2;
	else model_type=Model_3;

	full_covariance=conf.Get_Boolean_Opt("initialization", "full_covariance");

	return;
	}