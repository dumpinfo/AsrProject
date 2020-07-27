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
//  |   FILE:   soundlab.cpp                                            |
//  |   FUNCTIONALITY: joint sound label handling                       |
//  |   PROGRAM: required to codes which opens database config file     |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio				                            |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION:19/11/98                                   |
//  |   FIRST VERSION: 4.0                                              |
//  |   DATA FIRST VERSION: 20/01/98                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
//25/4/98 added GNU compatibility
//12/02/98 eliminated operator += String
//27/01/98  eliminated a useless const
//05/02/98 elimiated VetShortInt as a returned type  added info over
//total minutes analyzed

//Fabrizio 
//09/03/98 eliminated const char *
//10/03/98 added Assert(new_smp>=0) in function Set_Absolute_Position,
//         corrected read of previous samples in function 
//         Get_Filtered_Sequential_VetSmp_And_Its_Predecessors

#include "../ioclass/soundlab.h"

//**************************************************************
//                                                             *
//                   SoundLabelledFile                              *
//                                                             *
//**************************************************************

SoundLabelledFile::SoundLabelledFile()
	{
	initialized=FALSE;
	Reset();
	return;
	}



void SoundLabelledFile::Reset()
	{
	if (initialized)
		  {
		  snd_file->Reset();
		  label_file->Reset();
		  }
	len_win_sample=0;
	overlap=0;
	return;
	}


Boolean SoundLabelledFile::Initialize(const String&	config_file,const String& section,Boolean rd_tran)

	{
	ConfigFile conf;
	String buffer, filetype, labeltype;

	Reset();
	read_transcription=rd_tran;

	conf.Open_File(config_file);

	len_win_sample=conf.Get_Unsigned_Opt(section, "WindowLenght");

	overlap=conf.Get_Unsigned_Opt(section, "WindowOverlap");
	Check(len_win_sample >(t_index)overlap,
		  "Window lenght must be greater than overlap");


	if( label_file.Configure(config_file,section)==FALSE)
		return FALSE;

	if( snd_file.Configure(config_file,section)==FALSE)
		return FALSE;

	initialized=TRUE;
	return TRUE;
	}


Boolean SoundLabelledFile::Open(const String& file_name)
	{
	if (read_transcription)
		if (NOT label_file->Open_Sym(file_name))
			return FALSE;
	if(NOT snd_file->Open(file_name))
		return FALSE;
	return TRUE;
	}


// SEQUENTIAL access to file
Boolean SoundLabelledFile::Get_Sequential_Vet(VetDouble & vet)
	{
	if(NOT snd_file->Read(vet,len_win_sample) )
		return FALSE;

	//overlapping operation: get stream pointer of overlap samples
		if (overlap>0)
			snd_file->Set_Relative_Position(-overlap);

	return (Boolean)TRUE;
	}

// Take a vector of time samples and his phone name

Boolean SoundLabelledFile::Get_Sequential_Vet(VetDouble & vet, t_index & sym)
	{
	if (Get_Sequential_Vet(vet))
		{
		label_file->Get_Symbol(sym,(snd_file->Get_Actual_Position()));
		return (Boolean)TRUE;
		}
		else return (Boolean)FALSE;
	}

// Take a vector of time samples;
// new_file is true if is the first vector of file

Boolean SoundLabelledFile::Get_Sequential_Vet(VetDouble & vet, Boolean& new_file)
	{
	if (snd_file->Get_Actual_Position()==0)
		new_file= (Boolean)TRUE;
		else new_file= (Boolean)FALSE;

	return Get_Sequential_Vet(vet);
	}


// Return a vector of time samples in a FILTERED access.
// is_new_fone is true if change istance of phonema

Boolean SoundLabelledFile::Get_Filtered_Sequential_Vet(VetDouble &vet,
					 t_index  sym, Boolean &is_new_fone )
	{
	t_index  new_smp,act_smp;
	Boolean valid;

	act_smp=snd_file->Get_Actual_Position();
	valid=label_file->Set_Symbol(sym,act_smp,new_smp);

	//no more  phonema requested in file
	if (NOT valid)
		{
		return (Boolean)FALSE;
		}

	// we are always in the same istance of phonema
	if( (act_smp==new_smp) AND	(new_smp!=0) )
		{
		Get_Sequential_Vet(vet);
		is_new_fone = (Boolean)FALSE;
		}

	//changing istance of phonema
		else	{
				snd_file->Set_Absolute_Position(new_smp);
				Get_Sequential_Vet(vet);
				is_new_fone = (Boolean)TRUE;
				}
	return (Boolean)TRUE;
	}


// Get_Filtered_Sequential_Vet function in order to return  pred_list.Dim() number of
// predecessors for inizializing delta and deltadelta vet when requested for filtered
// access
Boolean SoundLabelledFile::Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(VetDouble &vet,
			t_index sym, Boolean &is_new_fone, VetDoubleList& pred_list)
	{
	t_index act_smp, new_smp;
	t_index i;
	t_index num_pred;
	Boolean valid;


	act_smp=snd_file->Get_Actual_Position();
	valid=label_file->Set_Symbol(sym,act_smp,new_smp);
	
	//no more  phonema requested in file
	if (NOT valid)
		{
		return (Boolean)FALSE;
		}

	// we are always in the same istance of phonema
	if( (act_smp==new_smp) AND	(new_smp!=0) )
		{
		Get_Sequential_Vet(vet);
		is_new_fone = (Boolean)FALSE;

		} else 	{  //changing istance of phonema

				num_pred = pred_list.Dim();

				t_index backshift;

				Assert(len_win_sample>(t_index)overlap);
				
				// back shift in order to take into account of previous vectors
				backshift = num_pred*(len_win_sample - overlap);

				// predecessors managing
				if(new_smp == 0 OR new_smp<backshift)
					{
					snd_file->Set_Absolute_Position(new_smp);
					// first sample of sound file (usually the first frame of
					// silence is requested) => zero all the predecessor vectors
					Get_Sequential_Vet(vet);
					for(i=0;i<num_pred;i++)
					// just a little trick: starting frames initialized with
					// the value of the first frame: in order to have a starting value
					pred_list[i] = vet;

					} else  {
							// set the new position value to the first vetsample to extract
							
							snd_file->Set_Absolute_Position(new_smp - backshift);

							// get predecessors
							for(i=num_pred;i>0;i--)
								Get_Sequential_Vet(pred_list[i-1]);
							// get actual vetsmp
							Get_Sequential_Vet(vet);
	
							}
					is_new_fone = (Boolean)TRUE;
					}
	return (Boolean)TRUE;
	}


// Take a vector of time samples in a RANDOM access from start_smp
// sym is the name of phone of vet
Boolean SoundLabelledFile::Get_Random_Vet(VetDouble & vet,t_index start_smp,
								t_index & sym)
	{
	Boolean valid;
	valid=Get_Random_Vet(vet,start_smp);
	label_file->Get_Symbol(sym,snd_file->Get_Actual_Position());
	return valid;
	}


// Take a vector of time samples in a RANDOM access from start_vet
Boolean SoundLabelledFile::Get_Random_Vet(VetDouble & vet,t_index start_vet)
	{

	if (start_vet>=Get_Num_Vet_In_File())
		{
		merr<<"number of vector not allowed in Get_Random_Vec";
		return FALSE;
		}
	snd_file->Set_Absolute_Position((len_win_sample-overlap)*start_vet);

	return (Get_Sequential_Vet(vet));
	}



t_index SoundLabelledFile::Get_Num_Vet_In_File() const
	{
	return snd_file->Get_Num_Smp_In_File()/(len_win_sample-overlap);
	}

t_index SoundLabelledFile::Smp_Rate() const
	{
	return snd_file->Smp_Rate();
	}

const String& SoundLabelledFile::Snd_Type() const
	{
	return snd_file->File_Type();
	}

enum label_file_type SoundLabelledFile::Label_Type() const
	{
	return label_file->label;
	}

t_index SoundLabelledFile::Window_Overlap()const
	{
	return (t_index) overlap;
	}

t_index SoundLabelledFile::Window_Lenght()const
	{
	return len_win_sample;
	}

t_index SoundLabelledFile::Translate_Symbol(const String & sym) const
	{
	return label_file->Translate_Symbol(sym);
	}

void SoundLabelledFile::Translate_Symbol(String & symb,  t_index num_sym) const
	{
	label_file->Translate_Symbol(symb, num_sym);
	}

t_index SoundLabelledFile::Get_Num_Of_Symbols()  const
	{
	return label_file->Get_Num_Of_Symbols();
	}

void SoundLabelledFile::Get_Label_List_Of_Actual_Utterance(VetULong& utter_label_list)
	{
	label_file->Get_Label_List_Of_Actual_Utterance(utter_label_list);
	}


// **********************
//    Class DBaseVoc
// **********************

DbaseVoc::DbaseVoc()
	{
	num_act_file=0;
	vets_cardinality=0;
	return;
	}


DbaseVoc::~DbaseVoc()
	{
	Reset();
	return;
	}


void DbaseVoc::Reset()
	{
	file_list.Reset();
	num_act_file=0;
	vets_cardinality=0;
	return;
	}


//private function
//load list of file
//calculate number of vectors

void DbaseVoc::Inizialize_File_List()
	{
	ifstream f_lis;
	String dir;
	String temp;
	t_index i=0;

	f_lis.open(db_file,ios::in|ios::nocreate);
	if(f_lis.fail())
		merr<<"Could not open file list files:"<<db_file;

	//calculate num files
	f_lis>>dir;
	while (NOT f_lis.eof())
		{
		f_lis>>temp;
		if(NOT(f_lis.eof() AND temp[0]==EOF))
			i++;
		}


	f_lis.clear();
	f_lis.seekg(0,ios::beg);

	//check if first dir is path directory
	if (dir[dir.Dim()-1]=='\\' OR dir[dir.Dim()-1]=='/')
		{
		num_act_file=i;

		file_list.Destroy_And_ReDim(num_act_file);
		f_lis>>dir;
		for(i=0;i<num_act_file;i++)
			{
			file_list[i].name=dir;
			f_lis>>temp;
			file_list[i].name<<temp;

			snd_file->Open(file_list[i].name);

			file_list[i].tel = Get_Num_Vet_In_File();
			//sum num vector
			if(i!=0)
				file_list[i].tel+=file_list[i-1].tel;

			}
		} else  {
				num_act_file=i+1;
				mstat<<"Number of acoustic file: "<<num_act_file;

				file_list.Destroy_And_ReDim(num_act_file);

				for(i=0;i<num_act_file;i++)
					{
					f_lis>>file_list[i].name;
					snd_file->Open(file_list[i].name);
					file_list[i].tel = Get_Num_Vet_In_File();

					if(i!=0)
						file_list[i].tel+=file_list[i-1].tel;
					}
				}
	vets_cardinality=file_list[i-1].tel;

	return;
	
	}


//open next file in the list
//load new symbols and set to first vector
Boolean DbaseVoc::Load_Next_File()
	{
	num_act_file++;
	if (num_act_file>=file_list.Dim())
		return (Boolean)FALSE;
		else return (Open(file_list[num_act_file].name));
	};

// Open a file in position "index" in database
Boolean DbaseVoc::Load_File(t_index index)
	{
	if (index>=file_list.Dim())
		return (Boolean)FALSE;
	else return (Open(file_list[index].name));

	};



// Configure Dbase class with the desired parameter	read from config_file
// must be specified if read or not phonetic file transcription

void DbaseVoc::Configure(const String & config_file, Boolean rd_tran=TRUE)
	{
	ConfigFile conf;
	t_real sec;

	//inizialize files
	read_transcription=rd_tran;

	conf.Open_File(config_file);

	conf.Get_String_Opt("DBaseOptions", "ListOfSoundFNames", db_file);

	Initialize(config_file,"DBaseOptions",read_transcription);

	Inizialize_File_List();

	sec=((t_real)(Get_Vets_Cardinality()))*
				((t_real)Window_Lenght()-Window_Overlap())/(t_real)Smp_Rate();
	mwarn<<"Total Time: hh::mm::ss = "<<
		(t_index)(sec/3600.0)<<"::"<<(((t_index)(sec))%3600)/60<<"::"<<(t_index)(sec)%60;

	//open the first file
	Restart();
	return;
	}


// Write a file of dbase phonetic transcription (if it was been read)
void DbaseVoc::Dump_File_Of_Sentence_List_Transcriptions(const String& filename)
	{
	String tempsymb;
	ofstream file;
	file.open(filename);
	Assert(NOT file.fail());
	Check(read_transcription==TRUE,"Phonetic transcription not loaded");

	for(t_index i=0;i<file_list.Dim();i++)
		{
		label_file->Open_Sym(file_list[i].name);
		label_file->Dump_Sentence_List_Transcriptions_On_File( file);
		}

	file.close();

	return;
	}


void DbaseVoc::Restart()
	{
	Open(file_list[0].name);
	num_act_file=0;
	return;
	}

// get vector in a sequential way
// of len_windows and  overlapped as in the inizialize
// of type  num sym
// return false on end of Dbase
// is_new_fone is true if the returned vet is the first vet of
// an istance of phonema
Boolean DbaseVoc::Get_Filtered_Sequential_Vet(VetDouble &vet,
				t_index sym,Boolean & is_new_fone)
	{
	Boolean not_end_of_dbase = (Boolean)TRUE;

	while(not_end_of_dbase 	AND
		  NOT SoundLabelledFile::Get_Filtered_Sequential_Vet(vet,sym, is_new_fone))
		 not_end_of_dbase = Load_Next_File();

	return (Boolean)(not_end_of_dbase);
	}


Boolean DbaseVoc::Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(VetDouble &vet,
				t_index sym, Boolean &is_new_fone, VetDoubleList& pred_list)
	{
	Boolean not_end_of_dbase = (Boolean)TRUE;

	while(not_end_of_dbase AND
		  NOT SoundLabelledFile::Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(vet,sym,
												is_new_fone, pred_list))
		  not_end_of_dbase = Load_Next_File();


	return (Boolean)(not_end_of_dbase);
	}


//get vector in a sequential way
//newfile is true if vet is the first vector of file
Boolean DbaseVoc::Get_Sequential_Vet(VetDouble &vet,	Boolean & newfile)
	{

	Boolean not_end_of_dbase = (Boolean)TRUE;

	while(not_end_of_dbase AND
		  NOT SoundLabelledFile::Get_Sequential_Vet(vet, newfile))
		not_end_of_dbase = Load_Next_File();

	return (Boolean)(not_end_of_dbase);
	}


//get vector in a sequential way
//of len_windows and  overlapped as in the inizialize
//sym is the  number of  the actual symbol
//(the first sample is considered)
//return false on end of Dbase
Boolean DbaseVoc::Get_Sequential_Vet(VetDouble &vet, t_index & sym)
	{
	Boolean eof;

	if (NOT SoundLabelledFile::Get_Sequential_Vet(vet,sym))
		{
		if(Load_Next_File())
			{
			eof=SoundLabelledFile::Get_Sequential_Vet(vet,sym);
			Assert(eof==TRUE);
			return (Boolean)TRUE;
			}
		else return (Boolean)FALSE;
		};
	return (Boolean)TRUE;
	};


//get vector in a sequential way
//of len_windows and  overlapped as in the inizialize
//return false on end of Dbase
Boolean DbaseVoc::Get_Sequential_Vet(VetDouble &vet)
	{
	Boolean eof;

	if (NOT SoundLabelledFile::Get_Sequential_Vet(vet))
		{
		if(Load_Next_File())
			{
			eof=SoundLabelledFile::Get_Sequential_Vet(vet);
			Assert(eof==TRUE);
			return (Boolean)TRUE;
			}
		else return (Boolean)FALSE;
		};
	return (Boolean)TRUE;

	};

// Random access

// get the num global vector in a random way
// of lenwin dim. overlapped return sym=num of actual type sym
// return false on num> of  Get_Vets_Cardinality
Boolean DbaseVoc::Get_Random_Vet(VetDouble &vet,t_index num )
	{
	t_index i=0;

	if (num>=vets_cardinality)
		merr<<"This vector number is greater than the total number of vect in Get_Random_Vet";


	while(i<file_list.Dim() AND	num>file_list[i].tel)
		i++;

	if(num_act_file!=i)
		Load_File(i);

	if(i!=0)
		{
		//starting point inside file
		num-=file_list[i-1].tel;
		num_act_file=i;
		}

	return (SoundLabelledFile::Get_Random_Vet(vet,num));
	}


//get the num global vector in a random way
//of lenwin dim. overlapped
//return false on num> of  vets_cardinality
Boolean DbaseVoc::Get_Random_Vet(VetDouble &vet,t_index num,t_index  & sym  )
	{
	t_index i=0;

	if (num>=vets_cardinality)
		merr<<"this vector number is greater than the total number of vect in Get_Random_Vet";

	while(i<file_list.Dim() AND	num>file_list[i].tel)
		i++;

	if(num_act_file!=i)
		Load_File(i);

	if(i!=0)
		{
		//starting point inside file
		num-=file_list[i-1].tel;
		num_act_file=i;
		}

	return (SoundLabelledFile::Get_Random_Vet(vet,num,sym));

	}


t_index DbaseVoc::Get_Vets_Cardinality() const
	{
	return vets_cardinality;
	}

void DbaseVoc::Get_Utter_FName(String& utter_fname)
	{
	utter_fname = (file_list[num_act_file].name);
	return ;
	};

const String& DbaseVoc::Db_File_List_Name() const
	{
	return db_file;
	}

const String& DbaseVoc::Dbase_Name() const
	{
	return snd_file->File_Type();
	}

// Function to test classes for I/O
void Test_IO_Class()
	{

	VetDouble vet;
	VetULong utter_label_list;
	ImpObjectList<VetDouble> pred_list;
	String utter_fname, dummy, key, symbol;
	
	DbaseVoc test;
	t_index sym,num;
	Boolean is_new_fone, newfile;
	
	pred_list.Destroy_And_ReDim(2);
	
	test.Configure("res.ini");
	
	test.Translate_Symbol(symbol,0);
	mstat<<"Phoneme: "<<symbol;
	
	num=test.Get_Vets_Cardinality();
	mstat<<"Number of vector in database: "<<num;
	num/=2;
	
	test.Get_Label_List_Of_Actual_Utterance(utter_label_list);
	sym=utter_label_list[5];
	
	// FILTERED access test
	
	test.Get_Filtered_Sequential_Vet(vet, 44, is_new_fone);
	test.Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(vet, sym,
																is_new_fone, pred_list);
	
	test.Restart();
	// SEQUENTIAL access test
	
	test.Get_Sequential_Vet(vet, newfile);
	test.Get_Sequential_Vet(vet, sym);
	test.Get_Sequential_Vet(vet);
	
	test.Restart();
	//RANDOM access test
	
	test.Get_Random_Vet(vet, num);
	test.Get_Random_Vet(vet, num/2, sym);

	num=test.Get_Num_Of_Symbols();
	mstat<<"Number of phonemes in symbol table: "<<num;

	num=test.Smp_Rate();
	mstat<<"Sample rate: "<<num;
	
	test.Restart();
	test.Get_Utter_FName(utter_fname);
	mstat<<"Name of actual file: "<<utter_fname;

	test.Dump_File_Of_Sentence_List_Transcriptions("dbase.sol");
	mstat<<"Sentence transcriptions written in dbase.sol";

	mstat<<"Test is ok!";

	};

