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
//  |   FILE:   soundfil.cpp                                            |
//  |   FUNCTIONALITY: class sound file finition                        |
//  |   PROGRAM: required to codes which opens database config file     |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio Fabrizio                                 |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:                                                  |
//  |   DATA FIRST VERSION: 17/01/98                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
//28/4/98 added GNU compatibility introduced TWO_BYTE_SHORT
//23/2/98 introduced ImpPolimorphic in SoundFileInterface
//12/02/98 eliminated  operator+=(String)
//4/2/98 Read now returns a VetDouble
//9/3/98 eliminated const char *

// Fabrizio 
// 02/04/98 added class FeaturesFile for read file of features
// 22/04/98 corrected function Open of MSwavSoundFile to read file
//          in Windows PCM format
// 28/04/98 corrected function Read of SoundFileGeneric class, now return
//          false if at the end of file or if was been read all samples
// 02/05/98 added WAVE_EXTENCTION for sound file exteanction


#include "../ioclass/soundfil.h"
//************user info**************

//first initialize the class
//implemented file types ntimit atis mswav
//then do all the operations of SoundFileGeneric


//implemented types are
//"ntimit"
//"atis"
// "tidigits"
//"mswav" that must be specified in the option "SoundFileType"



//**************************************************************
//                                                             *
//                   SndHeader                                 *
//                                                             *
//**************************************************************

	SndHeader::SndHeader()
		{
		Reset();
		return;
		}

	void SndHeader::Reset()
		{
		header_type="no_header";
		first_byte=0;
		database_id="no_database";
		num_smp_file=0;
		sample_rate=0;
		bytes_per_sample=0;
		byte_format=0;
		return;
		}

//**************************************************************
//                                                             *
//                   SoundFileInterface                                 *
//                                                             *
//**************************************************************


	Boolean SoundFileInterface::Configure(const String& config_file,const String& sect_name)
		{
		ConfigFile conf;
		String filetype;
#ifdef  INSTANTIATE_DUMMY_CLASSES_FOR_STRANGE_COMPILERS
		VetShortInt  vet;
#endif

		conf.Open_File(config_file);

		conf.Get_String_Opt(sect_name, "SoundFileType", filetype);

		/*      add your driver here
		if ( filetype=="my_standard")
			{
			actual_file=new(MyStandard);
			} else
		*/
		if ( filetype=="featuresfile")
			{
			New_For_Polimorphic_Pointer_With_Allocated_Class(FeaturesFile);
			} else
		if ( filetype=="ntimit" OR filetype=="atis" OR filetype=="tidigits")
			{
			New_For_Polimorphic_Pointer_With_Allocated_Class(TimitwavSoundFile);
			} else
		if (filetype=="mswav")
			{
			New_For_Polimorphic_Pointer_With_Allocated_Class(MSwavSoundFile);
			} else
		if (filetype=="apasci")
			{
			New_For_Polimorphic_Pointer_With_Allocated_Class(ApasciSoundFile);
			} else
		if (filetype=="telapasci")
			{
			New_For_Polimorphic_Pointer_With_Allocated_Class(ApasciTelSoundFile);
			}
			else merr<<"the sound file type:"<< filetype  <<"is not implemented";
		(*this)->Initialize(config_file, sect_name);
		return TRUE;
		}


//**************************************************************
//                                                             *
//                   SoundFileGeneric                          *
//                                                             *
//**************************************************************

	void SoundFileGeneric::Reset()
		{
		file_type ="no sound file type";
		header.Reset();
		return;
		}

	Boolean SoundFileGeneric::Initialize(const String& config_file,const String& sect_name)
		{
		ConfigFile conf;
		t_index size;


		conf.Open_File(config_file);


		size =conf.Get_Unsigned_Opt(sect_name, "Buffer_Size_K");

#ifndef __32BIT_CODE__
		if (size>32)
			{
			mwarn<<"buffer size "<<size*1000<<
				"> 32000 not allowed in 16 bit platforms size set to 32000";
			size=32;
			}
#endif
		size*=1000;
		if (size>0)
			buffer.Destroy_And_ReDim(size);
		return TRUE;
		}

	SoundFileGeneric::SoundFileGeneric ()
		{
		file_type ="generic type sound file";
		return;
		}

	SoundFileGeneric::~SoundFileGeneric ()
		{
		#ifdef __BORLANDC__
		if((rdbuf())->is_open()) close();
		#else
		if(is_open()) close();
		#endif
		return;
		}

	 const String & SoundFileGeneric::File_Type() const
		{
		return file_type;
		}


	Boolean SoundFileGeneric::Open(const String& file_name)
		{
		String temp_name;

		temp_name<<file_name<<WAVE_EXTENCTION;

#ifdef __BORLANDC__
		if((rdbuf())->is_open()) close();
#else
		if(is_open()) close();
#endif

		open(temp_name, ios::in|ios::nocreate|ios::binary);
		if(fail())
			merr<<"Could not open file: "<<temp_name;

#ifdef __BORLANDC__
		//buffer treatment
		if (buffer.Dim()>0)
			(rdbuf())->setbuf(&(buffer[0]),buffer.Dim());
#else
		if (buffer.Dim()>0)
			setbuf(&(buffer[0]),buffer.Dim());
#endif

		return (TRUE);
		}


	inline t_index SoundFileGeneric::Get_Actual_Position() 
		{
		return((istream::tellg()-header.first_byte)/header.bytes_per_sample);
		}


	//from start
	inline void SoundFileGeneric::Set_Absolute_Position(t_index new_smp)
		{
		Assert(NOT fail());
		Assert(new_smp>=0);
		seekg(new_smp*header.bytes_per_sample+header.first_byte,ios::beg);
		return;
		}

	//from the actual position
	inline void SoundFileGeneric::Set_Relative_Position(t_signed position)
		{
		Assert(NOT fail());
		seekg(position*header.bytes_per_sample,ios::cur);
		return;
		}


	// SEQUENTIAL access to file
	Boolean SoundFileGeneric::Read(VetDouble & out,t_index len_win_sample)
		{
		char* temp;
		char aux;
		t_signed i;
		t_index end_of_file;
		static VetShortInt vet;

		Assert(header.bytes_per_sample<3);
		Assert(len_win_sample>0);
		#ifdef __BORLANDC__
		Assert( (rdbuf())->is_open() );
		#else
		Assert(is_open());
		#endif


		if (vet.Dim()!=len_win_sample)
				vet.Save_And_ReDim(len_win_sample);

		temp= (char*) & vet[0];

		end_of_file=header.num_smp_file*header.bytes_per_sample+header.first_byte;
		
		read(temp,len_win_sample*header.bytes_per_sample);
		if ((t_index)gcount()!=len_win_sample*header.bytes_per_sample
			OR (t_index)tellg()>end_of_file)
			{
			return (Boolean)FALSE;
			}
			else 	{
					if(header.byte_format==10)
						{
						Assert(header.bytes_per_sample==2);
						for(i=0;i<(t_signed)len_win_sample*2;i+=2)
							{
							//swap byte operation
							aux = temp[i];
							temp[i] = temp[i+1];
							temp[i+1] = aux;
							}
						}
					}
		//char -> two byte type
		if (header.bytes_per_sample==1)
			{
			if (out.Dim()!=len_win_sample)
				out.Save_And_ReDim(len_win_sample);

			for(i=vet.Dim();i>0;i--)
				out[i-1]=(TWO_BYTE_SHORT) (temp[i-1]);
			}
			else out=vet;

		return (Boolean)TRUE;
		}


	t_index SoundFileGeneric::Get_Num_Smp_In_File() const
		{
		return header.num_smp_file;
		}

	t_index SoundFileGeneric::Smp_Rate() const
		{
		return header.sample_rate;
		}




//**************************************************************
//                                                             *
//                   TimitwavSoundFile                          *
//                                                             *
//**************************************************************

	void TimitwavSoundFile::Search_In_Header(const String& option)
		{
		String temp;

		seekg(0L,ios::beg);
		clear();

		while(temp!=option AND  NOT( eof()) )
			(*this)>>temp;

		if(eof())
		    merr<<option<<" not found in Timit";

		(*this)>>temp;

		return;

		}



	TimitwavSoundFile::TimitwavSoundFile()
		{
		file_type ="NIST_file";
		return;
		}

	Boolean TimitwavSoundFile::Open(const String& file_name)
		{
		//read the header first
		String temp_name,temp;

		temp_name<<file_name<<WAVE_EXTENCTION;

	#ifdef __BORLANDC__
		if((rdbuf())->is_open()) close();
	#else
		if(is_open()) close();
	#endif

		open(temp_name,ios::nocreate|ios::in);

		if(fail())
			merr<<"Could not open file: "<<temp_name;

		(*this)>>temp;

		if(temp!="NIST_1A")
			merr<<" Not a Nist_1A file";

		(*this)>>header.first_byte;

		Search_In_Header("database_id");
		(*this)>>header.database_id;

		Search_In_Header("sample_count");
		(*this)>>header.num_smp_file;

		Search_In_Header("sample_rate");
		(*this)>>header.sample_rate;

		Search_In_Header("sample_n_bytes");
		(*this)>>header.bytes_per_sample;

		Search_In_Header("sample_byte_format");
		(*this)>>header.byte_format;

		close();
		open(temp_name,ios::in|ios::nocreate|ios::binary);

		Set_Absolute_Position(0);

		return TRUE;

		}

//**************************************************************
//                                                             *
//                       ApasciFile                            *
//                                                             *
//**************************************************************

ApasciSoundFile::ApasciSoundFile()
	{
	file_type="Apasci";
	return;
	}

Boolean ApasciSoundFile::Open (const String& file_name)
	{
	String temp_name;
	String temp;

	temp_name<<file_name<<WAVE_EXTENCTION;

#ifdef __BORLANDC__
	if((rdbuf())->is_open()) close();
#else
	if(is_open()) close();
#endif

	open(temp_name,ios::nocreate|ios::in);

	if(fail())
		merr<<"Could not open file: "<<temp_name;

	header.byte_format=1;
	header.bytes_per_sample=2;
	header.first_byte=0;
	header.sample_rate=16000;
	seekg(0, ios::end);
	header.num_smp_file=(tellg()/header.bytes_per_sample);

	close();
		
	open(temp_name, ios::in|ios::nocreate|ios::binary);
	if(fail())
		merr<<"Could not open file: "<<temp_name;

	seekg(header.first_byte,ios::beg);
	
	return (TRUE);
	}

//**************************************************************
//                                                             *
//                       ApasciTelFile                         *
//                                                             *
//**************************************************************

ApasciTelSoundFile::ApasciTelSoundFile()
	{
	file_type="ApasciTel";
	return;
	}

Boolean ApasciTelSoundFile::Open (const String& file_name)
	{
	String temp_name;
	String temp;

	temp_name<<file_name<<WAVE_EXTENCTION;

#ifdef __BORLANDC__
	if((rdbuf())->is_open()) close();
#else
	if(is_open()) close();
#endif

	open(temp_name,ios::nocreate|ios::in);

	if(fail())
		merr<<"Could not open file: "<<temp_name;

	header.byte_format=1;
	header.bytes_per_sample=2;
	header.first_byte=0;
	header.sample_rate=8000;
	seekg(0, ios::end);
	header.num_smp_file=(tellg()/header.bytes_per_sample);

	close();
		
	open(temp_name, ios::in|ios::nocreate|ios::binary);
	if(fail())
		merr<<"Could not open file: "<<temp_name;

	seekg(header.first_byte,ios::beg);
	
	return (TRUE);
	}
	
//**************************************************************
//                                                             *
//                       FeaturesFile                          *
//                                                             *
//**************************************************************

	FeaturesFile::FeaturesFile()
		{
		file_type ="Features_file";
		return;
		}

	Boolean FeaturesFile::Open(const String& file_name)
		{
		//read the header first
		String temp_name,temp;
		t_index position;

		header.Reset();
		temp_name<<file_name<<".fts";

	#ifdef __BORLANDC__
		if((rdbuf())->is_open()) close();
	#else
		if(is_open()) close();
	#endif

		open(temp_name,ios::nocreate|ios::in);

		if(fail())
			merr<<"Could not open file: "<<temp_name;

		(*this)>>temp;

		if(temp!="FeaturesFile")
			merr<<" Not a Features file";

		header.database_id=temp;

		Search_In_Header("feature_dim=");
		(*this)>>features_dim;

		Search_In_Header("feature_first_byte=");
		(*this)>>header.first_byte;

		Search_In_Header("feature_n_bytes=");
		(*this)>>header.bytes_per_sample;

		Search_In_Header("feature_byte_format=");
		(*this)>>header.byte_format;

		seekg(0,ios::end);
		position=tellg();
		header.num_smp_file=(position-header.first_byte
							 -header.bytes_per_sample)/8;

		close();
		open(temp_name,ios::in|ios::nocreate|ios::binary);

		Set_Absolute_Position(0);

		return TRUE;

		}

	Boolean FeaturesFile::Read(VetDouble & out,	t_index len_win_sample)
		{
		char* temp;
		static VetDouble vet;

		Assert(header.bytes_per_sample<9);
		Assert(len_win_sample>0);
	#ifdef __BORLANDC__
		Assert( (rdbuf())->is_open() );
	#else
		Assert(is_open());
	#endif

		if(len_win_sample!=features_dim)
			merr<<"Bad window lenght; Check your config file!"; 

		if (vet.Dim()!=len_win_sample)
			vet.Save_And_ReDim(len_win_sample);

		temp= (char*) & vet[0];

		read(temp,len_win_sample*header.bytes_per_sample);

		if ((t_index)gcount()!=len_win_sample*header.bytes_per_sample)
			{
			return (Boolean)FALSE;
			}
	
		out=vet;

		return (Boolean)TRUE;
	}

	void FeaturesFile::Search_In_Header(const String& option)
		{
		String temp;

		seekg(0L,ios::beg);
		clear();

		while(temp!=option AND  NOT( eof()) )
			(*this)>>temp;

		if(eof())
			merr<<option<<" not found in FeaturesFile";

		return;

		}


//**************************************************************
//                                                             *
//                  MSwavSoundFile                             *
//                                                             *
//**************************************************************


	MSwavSoundFile::MSwavSoundFile()
		{
		file_type ="MS wav file";
		return;
		}

	Boolean MSwavSoundFile::Open(const String& name)
		{
		t_index wFmtTag;
		TWO_BYTE_SHORT nChannels;
		t_index nSamplesPerSec;
		t_index nAvgBytesPerSec;
		TWO_BYTE_SHORT nBlockAlign;
		TWO_BYTE_SHORT BitsPerSample;

		String temp;
		t_index chunkSize;

		temp<<name<<WAVE_EXTENCTION;
	
	#ifdef __BORLANDC__
		if((rdbuf())->is_open()) close();
	#else
		if(is_open()) close();
	#endif
		
		open(temp,ios::binary|ios::in|ios::nocreate);
	if(fail())
		merr<<"Could not open file: "<<name;

		temp="FOUR";
		read(&(temp[0]),4);
		if(temp!= "RIFF")	   
			merr<<"not a wave riff file";

		while(( temp!="fmt ") AND  NOT(eof()) )
			{
			seekg(-3L,ios::cur);
			read(&(temp[0]),4);
			}

		if (eof())
			merr<<"fmt chunk not found in wave file";

		read((char *)(&wFmtTag),4);
		ignore(2);
		read((char *)(&nChannels),2);
		if (nChannels!=1)
			merr<<"not stereo sound file allowed!";
		read((char *)(&nSamplesPerSec),4);
		read((char *)(&nAvgBytesPerSec),4);
		read((char *)(&nBlockAlign),2);
		read((char *)(&BitsPerSample),2);
	
		while((temp!="data") AND  NOT(eof()))
			{
			seekg(-3L,ios::cur);
			read(&(temp[0]),4);
			}

		if (this->eof())
			merr<<"data chunk not found in wave";

		read((char *)(&chunkSize),4);

		header.first_byte=tellg();
		header.bytes_per_sample=BitsPerSample/8;
		header.num_smp_file=chunkSize/header.bytes_per_sample;
		header.sample_rate=nSamplesPerSec;

		return TRUE;
		}

