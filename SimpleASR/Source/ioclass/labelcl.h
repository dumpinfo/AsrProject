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
//  |   FILE:   labelcl.h                                               |
//  |   FUNCTIONALITY: class Database definition                        |
//  |   PROGRAM: required to codes which opens database config file     |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio  & Claudio  & Antonio  					|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:      4.0                                         |
//  |   DATA FIRST VERSION: 18/01/97                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
//23/4/98 introduced ImpPolimorphic in LabelFileInterface
//10/02/98 eliminated const char *
//added AtisReducedLabelsClass
// Fabrizio 
// 8/4/98	Modified AtisLabelsClass: now symbol table has 42 phonemes
//			modified AtisReducedLabelsClass: symbol table now has 38 phonemes,
//			added function Initialize
//************user info**************

// to add a new standard
//1) derive the class from  GenericFileOfLabels
//2) add a enum constant to enum label_file_type
//3) add the proper line in the function LabelFileInterface::Configure


//custom LabelsFromFile are from written in a file
//the labels must be organized in a timit like structure
//the function which gets labels from file must be implemented

//implemented label files are:
//NTimitLabel, NTimitReducedLabel, AtisLabels,	LabelsFromFile
//AtisReducedLabels
//atisLabel employs  TimitReducedLabel alphaber
//while AtisReducedLabels employs only the encountered labels of TimitReducedLabel
//other file types are easily implemented

//+++++++++++++++++++       classes +++++++++++++++++
//1)class  LabelSymbolTable
//this class provides for the translation
//from string labels to numbers
//

//2) class FileOfLabels
//this class handles the labels of the actual file
//

//3) class  GenericFileOfLabels:public FileOfLabels,public LabelSymbolTable
//this class  is the prototype of the labelled file
//the classes derived from this class provide the specific functions
//of the actual database
//

//4)
//this classes implement the specific methods for the specific files
//class  NTimitLabelClass:public GenericFileOfLabels
//class  NTimitReducedLabelClass:public NTimitLabelClass
//class  AtisLabelsClass:public NTimitReducedLabelClass
//class  CustomLabelsFromFile:public GenericFileOfLabels
//class  AtisReducedLabelsClass:AtisLabelsClass

//5) class LabelFileInterface
//this class is the safe container for all the
//GenericFileOfLabels classes in fact any access is made through
//ImpPolimorphicPointer::operator->() which checks the correctness
//of the call



#ifndef _LABELCLASS_H_
#define _LABELCLASS_H_

#include <fstream.h>
#include "../baseclas/textclas.h"
#include "../vetclas/vetclas.h"
#include "../resconf/resconf.h"
#include "../baseclas/polimorf.h"



enum label_file_type {No_Symbol, NTimitLabel, NTimitReducedLabel, NTimit39Label,
					  AtisLabel, LabelsFromFile, AtisReducedLabel, ApasciLabel, ApasciReducedLabel};

//**************************************************************
//                                                             *
//                  LabelSymbolTable                           *
//                                                             *
//**************************************************************
//this class provides for the translation
//from string labels to numbers

class  LabelSymbolTable
	{
	protected:
	ImpObjectList<String> symb_table;

	public:

	t_index Get_Num_Of_Symbols()  const;

	//from string to num of symbol
	virtual t_index Translate_Symbol(const String & sym) const;
		//from num to string of symbol
	void Translate_Symbol(String & symb, t_index num_sym) const;

	};


//**************************************************************
//                                                             *
//                   FileOfLabels                              *
//                                                             *
//**************************************************************

//this class handles the labels of the actual file

class FileOfLabels
	{
	protected:
	class Entry_Symbol_Pos
		{
		public:
		t_index num_sym;
		t_index pos_in_file;

		void operator= (const Entry_Symbol_Pos & val)
			{
			pos_in_file=val.pos_in_file;
			num_sym=val.num_sym;
			return;
			}

		Entry_Symbol_Pos():num_sym(0), pos_in_file(0){};

		~Entry_Symbol_Pos()
			{
			pos_in_file=0;
			num_sym=0;
			}

		};

		t_index prev_entry;
		ImpObjectList<Entry_Symbol_Pos> symb_position;

	public:
		FileOfLabels();
		~FileOfLabels();

		//if act_smp is of type sym return true and new_smp_pos==act_smp
		//else search for the next sample (=new_smp  ) of type sym
		//if founded return TRUE and new_smp_pos!=act_smp
		//else return False
		Boolean Set_Symbol(t_index sym,t_index act_smp, t_index &new_smp_pos);

		//get the number (=sym) of the actula symbol
		//of the sample number (=smp)
		void Get_Symbol(t_index & sym, t_index smp);

		void Get_Label_List_Of_Actual_Utterance(VetULong& utter_label_list);

	};

//**************************************************************
//                                                             *
//                  GenericFileOfLabels                           *
//                                                             *
//**************************************************************
//this class  is the prototype of the labelled file
//the classes derived from this class provide the specific functions
//of the actual database
class  GenericFileOfLabels:public FileOfLabels,public LabelSymbolTable
	{

	public:

	String label_extension;

	enum label_file_type label;
	//file must be opened before
	ofstream & GenericFileOfLabels::Dump_Sentence_List_Transcriptions_On_File(ofstream & file);

	virtual void Reset();
	//load the translation table (symbols<-> numbers)
	//customize the behaviour of LabelSymbolTable  for the actual database
	//this functionis pure virtual
	virtual Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext)=0;
	//load the label file
	//customize the behaviour of FileOfLabels for the actual database
   //this functionis pure virtual
	virtual Boolean Open_Sym(const String &  file_name)=0;
	};

class  NTimitLabelClass:public GenericFileOfLabels
	{
	public:
	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	Boolean Open_Sym(const String &  file_name);

	};
class  NTimitReducedLabelClass:public NTimitLabelClass
	{
	public:
	Boolean Open_Sym(const String &  file_name);

	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	t_index Translate_Symbol(const String & sym) const;
	};

class NTimit39LabelClass:public NTimitLabelClass
	{
	public:
	Boolean Open_Sym(const String &  file_name);

	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	t_index Translate_Symbol(const String & sym) const;
	};
	
class  AtisLabelsClass:public GenericFileOfLabels
	{
	public:
	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	Boolean Open_Sym(const String &  file_name);
	};

//alphabet is a subset of AtisLabelClass
class  AtisReducedLabelsClass:public AtisLabelsClass
	{
	public:
	Boolean Open_Sym(const String &file_name);
	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	t_index Translate_Symbol(const String & sym) const;
	};

class ApasciLabelsClass:public GenericFileOfLabels
	{
	public:
	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	Boolean Open_Sym(const String &  file_name);
	t_index Translate_Symbol(const String & sym) const;

	};

class ApasciReducedLabelsClass: public ApasciLabelsClass
	{
	public:
	Boolean Open_Sym(const String &file_name);
	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	t_index Translate_Symbol(const String & sym) const;
	};

//this class must be implemented
class  CustomLabelsFromFile:public GenericFileOfLabels
	{
	public:
	Boolean Initialize(const String &  file_name,
							const String &  section_name, const String &label_ext);
	Boolean Open_Sym(const String &  file_name);
	};

//**************************************************************
//                                                             *
//                   LabelFileInterface                              *
//                                                             *
//**************************************************************

//this class is the safe container for all the
//GenericFileOfLabels classes in fact any access is made through
//operator->() which checks the correctness of the call

class LabelFileInterface:public ImpPolimorphicPointer<GenericFileOfLabels>
	{
	public:
	Boolean Configure(const String &  config_file,const String &  sect_name);
	};

#endif



