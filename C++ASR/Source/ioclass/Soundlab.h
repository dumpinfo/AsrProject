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
//  |   FILE:   soundlab.h                                              |
//  |   FUNCTIONALITY: class sound file finition                        |
//  |   PROGRAM: required to codes which opens database config file     |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio                                          |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION:19/11/98									|
//  |   FIRST VERSION:                                                  |
//  |   DATA FIRST VERSION: 20/01/98                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
//27/01/98 eliminated a useless const
//05/02/98 elimiated VetShortInt as a returned type  added info over
//total minutes analyzed

//*********** user info
 //The functions of dbasevoc are changed ( in the previous versions data were direclty
	  //available  through public variables
	  //snd_type label_type  WindowLenght WindowOverlap db_file
//	  const char * Snd_Type() const;
//	  const char * Label_Type() const;
//	  t_index Window_Overlap();
//	  t_index WindowLenght();
// Db_File_List_Name()


#ifndef __SOUNDLBL__H_
#define __SOUNDLBL__H_
#include "../ioclass/soundfil.h"
#include "../ioclass/labelcl.h"

typedef ImpObjectList<VetDouble> VetDoubleList;

//**************************************************************
//                                                             *
//                   SoundLabelledFile                         *
//                                                             *
//**************************************************************
//scopes:
//1) joint management of label and sound files
//2) overlap and window length management

class SoundLabelledFile
	{
	Boolean initialized;
	protected:
	t_index len_win_sample;
	t_signed overlap;

	Boolean read_transcription;

	class SoundFileInterface snd_file;
	class LabelFileInterface label_file;

  public:

	SoundLabelledFile();

	void Reset();
	Boolean Initialize(const String& config_file,const String& section,Boolean rd_tran);

	Boolean Open(const String& file_name);

	// Functions for SEQUENTIAL access
	Boolean Get_Sequential_Vet(VetDouble & vet);
	Boolean Get_Sequential_Vet(VetDouble & vet, t_index & sym);
	Boolean Get_Sequential_Vet(VetDouble & vet, Boolean& new_file);

	// Functions for FILETERED access

	Boolean Get_Filtered_Sequential_Vet(VetDouble &vet,
									 t_index  sym, Boolean &is_new_fone );

	Boolean Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(VetDouble &vet,
					t_index sym, Boolean &is_new_fone, VetDoubleList& pred_list);


	// Functions for RANDOM access

	Boolean Get_Random_Vet(VetDouble & vet,t_index num,t_index & sym);
	Boolean Get_Random_Vet(VetDouble & vet,t_index num);

	//info over sound file
	
	t_index Get_Num_Vet_In_File() const;
	t_index Smp_Rate() const;

	//These functions are changed ( in the previous versions data were direclty
	//available  through public variables snd_type label_type  WindowLenght WindowOverlap

	const String&	Snd_Type() const;
	enum label_file_type 	Label_Type() const;
	t_index Window_Overlap() const;
	t_index Window_Lenght() const;

	//info over label file
	t_index Get_Num_Of_Symbols() const;
	t_index Translate_Symbol(const String & sym) const;
	void Translate_Symbol(String & symb,  t_index num_sym) const;
	void Get_Label_List_Of_Actual_Utterance(VetULong& utter_label_list);

	};


class EntryAgenda
	{
	public:
	String name;
	t_index tel;
	EntryAgenda():
		tel(0){}
	~EntryAgenda()
		{
		tel=0;
		}
	};

//**************************************************************
//                                                             *
//                   DbaseVoc                                  *
//                                                             *
//**************************************************************
typedef ImpObjectList<EntryAgenda> Agenda;

class DbaseVoc:public SoundLabelledFile
	{

	Agenda file_list;

	t_index num_act_file;
	t_index vets_cardinality;

	Boolean read_transcription;

	void Reset();
	void Inizialize_File_List();
	Boolean Load_Next_File();
	Boolean Load_File(t_index index);
	void Set_Options(const String& config_fname);

	String db_file;

public:

	DbaseVoc();
	~DbaseVoc();

	void Configure(const String & db_file, Boolean rd_tran);

	void Dump_File_Of_Sentence_List_Transcriptions(const String& filename);

	void Restart();

	//FILTERED access type

	Boolean Get_Filtered_Sequential_Vet(VetDouble &vet,t_index sym,
															Boolean &is_new_fone );
	Boolean Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(VetDouble &vet,  t_index sym,
							  Boolean &is_new_fone, VetDoubleList& pred_list);

	// SEQUENTIAL access type

	Boolean Get_Sequential_Vet(VetDouble &vet, Boolean & newfile);

	Boolean Get_Sequential_Vet(VetDouble &vet,t_index & sym);

	Boolean Get_Sequential_Vet(VetDouble &vet);

	// RANDOM access type

	Boolean Get_Random_Vet(VetDouble &vet,t_index num,t_index  & sym );

	Boolean Get_Random_Vet(VetDouble &vet,t_index num );

	//Info over database

	//report the  ListOfSoundFNames information
	const String& Db_File_List_Name() const;
	t_index	Get_Vets_Cardinality() const;
	void Get_Utter_FName(String& utter_fname);
	const String& Dbase_Name() const;
	  
	};

void Test_IO_Class();

#endif