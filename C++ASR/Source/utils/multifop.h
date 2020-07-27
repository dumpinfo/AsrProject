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
//  |   FILE:   multifop.h                                              |
//  |   FUNCTIONALITY: utility for multifile processing				    |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   AUTHORS: Claudio			                                    |
//  |   ACTUAL REVISION:                                                |
//  |   DATA ACTUAL REVISION:                                           |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 19/4/98                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    Improvement  *******************************


// *******************    VERSION HISTORY  *******************************

// <<< version 1.0 >>>

//*******************   USER INFORMATION   *******************************

/*
available signal processing blocks (classes)
1) GenericOperation (abstract)
2) LoadFile
3) CountStrings
4) ModifySlashForUnix
5) SaveFileWithoutCR
6) SaveFile
7) InsertHeaderFileatBeginning
*/

//to add a  processing new block
//1) derive the class form GenericOperation
//2) implement at least the pure virtual function Apply
//3) declare a public static const string dsp_block_name an initialize to the name 
		//	of the cblock as found in config file

//4) add the proper instruction in SingleFileOp::Configure
//5) update file res.opt
//*******************   IMPLEMENTATION INFORMATION   *******************************


//*******************   CLASS INDEX   *******************************




#ifndef __MULTIFOP_H_CLASS
#define __MULTIFOP_H_CLASS

#include "../baseclas/polimorf.h"


//***********************************************
//*												*
//*			GenericOperation					*
//*												*
//***********************************************
//abstract  processing block
//derive always your block from this one

class GenericOperation
	{
	protected:

	public:

	//derive this data and initialize with the same name
	//of the block found in the config file
	static  const String block_name;

		//get options from configuration file
	virtual Boolean Initialize(const String & filename, const String & section){ return TRUE;};

		//perform operation
	virtual Boolean Apply(const String & filename,String & file)=0;
	virtual Boolean Destroy_Procedure(){ return TRUE;};
	};
//***********************************************
//*												*
//*			actual available Operation			*
//*												*
//***********************************************

class LoadFile:public GenericOperation
	{

	public:
	static  const String dsp_block_name;
	Boolean Apply(const String & filename,String & file);
	};

class InsertHeaderFileatBeginning:public GenericOperation
	{

	public:
	static  const String dsp_block_name;
	Boolean Apply(const String & filename,String & file);
	};



class CountStrings:public GenericOperation
	{

	char char_to_count;
	protected:
	t_index counter;
	public:

	CountStrings()
		{
		counter=0;
		char_to_count='\n';
		return;
		}

	static  const String dsp_block_name;
	Boolean Apply(const String & filename, String & file);
	Boolean Destroy_Procedure();
	};

class ModifySlashForUnix:public CountStrings
	{
	Boolean  Is_SubString_Inside (const String & actual_string,const String & sub, t_index &position,t_index start_position)const;
	
	public:

	static const String dsp_block_name;
	Boolean Apply(const String & filename, String & file);
	Boolean Destroy_Procedure();

	};


class SaveFileWithoutCR:public CountStrings
	{

	public:
	static  const String dsp_block_name;
	
	
	Boolean Apply(const String & filename, String & file);
	Boolean Destroy_Procedure();
	};

class SaveFile:public GenericOperation
	{

	public:
	static  const String dsp_block_name;
	Boolean Apply(const String & filename, String & file);
	};

//***********************************************
//*												*
//*			Single file processing				*
//*												*
//***********************************************

class SingleFileOp 
	{
	ImpPolimorphicVectorPtr<GenericOperation> operations;
	String file;

	public:
	void Configure(const String & config_file);
	void Perform_Single_File_Operation(const String & file_name);
	void Destroy_Procedures();
	};
//***********************************************
//*												*
//*			Multi file processing				*
//*												*
//***********************************************


class MultiFileOperations
	{
	ImpObjectList<String> file_list;

	SingleFileOp procedures;

	void 	Configure(const String & db_file);
	void 	Inizialize_File_List(const String & db_file);
	public:

	Boolean Perform_Multi_File_Operations(const String & db_file);
	};


#endif