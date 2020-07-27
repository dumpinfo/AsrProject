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
//  |   FILE:   soundfil.h                                              |
//  |   FUNCTIONALITY: sound file management                            |
//  |   PROGRAM: required to codes which opens database config file     |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19//11/98									|
//  |   FIRST VERSION:                                                  |
//  |   DATA FIRST VERSION: 16/1/98                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
//23/4/98 introduced ImpPolimorphic in SoundFileInterface
//4/2/98 Read now returns a VetDouble
//
// Fabrizio 
// 02/04/98 added FeaturesFile class for read files of features
// 22/04/98 now MSwavSoundFile is derived from SoundFileGeneric class
//          instead of TimitwavSoundFile class

#ifndef _IOCLASS_H_
#define _IOCLASS_H_

#include <fstream.h>
#include "../baseclas/textclas.h"
#include "../vetclas/vetclas.h"
#include "../resconf/resconf.h"
#include "../baseclas/polimorf.h"

#define RES_NEWIO_CLASS_VERSION "V. 6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__


//************user info**************
/*
1)sound file offers a common interface to read different type sound data
	files
2)different standards are treated using a safe generalization technique
	implemented in this class which allows safe pointer use
3) the different standard are treated through safe use of late
	(dynamic)  binding (virtual and inheritance
4)sound file has a pointer to a soundfilegeneric which gives the
	generic functions required
5) this functions are  specialized in the derived classes (
		TimitWavFile and MSWAVFile)

//implemented types are
//"ntimit"
//"atis"
// "tidigits"
//"mswav" that must be specified in the option "SoundFileType"

//buffered input is also implemented
*/


//**************************************************************
//                                                             *
//                   SndHeader		                           *
//                                                             *
//**************************************************************
//store the required data to handle the sound files
class SndHeader
	{
	public:

	SndHeader();

	void Reset();

	String header_type;
	t_index first_byte;
	String database_id;
	t_index num_smp_file;
	t_index sample_rate;
	t_index bytes_per_sample;
	t_index byte_format;

};


//**************************************************************
//                                                             *
//                   SoundFileGeneric                          *
//                                                             *
//**************************************************************

//generic class for all the derived classes
//implementing the actual standards
//to introduce a new standard derive a class form this one
//and  add the proper lines to  SoundFileInterface::Configure

class SoundFileGeneric : public ifstream
	{
	String buffer;
	protected:
	SndHeader header;
	String file_type;


	public:

	SoundFileGeneric ();
	
	virtual ~SoundFileGeneric ();

	virtual void Reset();

	virtual const String & File_Type() const;

	//get eventual parameters from config file
	virtual Boolean Initialize(const String& config_file,const String& section);

	virtual Boolean Open(const String& file);

	virtual Boolean Read(VetDouble & vet,t_index len_win_sample);

	virtual inline t_index Get_Actual_Position() ;

	// Set Position from the beginning of the samples of the file
	virtual inline void Set_Absolute_Position(t_index position);
	//Set Position from the actual position
	virtual inline void Set_Relative_Position(t_signed position);
	t_index Get_Num_Smp_In_File() const;
	t_index Smp_Rate() const;


	};

//**************************************************************
//                                                             *
//                   TimitwavSoundFile                          *
//                                                             *
//**************************************************************
//Nist 1A files
class TimitwavSoundFile:public SoundFileGeneric
	{

	void Search_In_Header(const String& option);
	public:
	TimitwavSoundFile();
	Boolean Open(const String& file);
	};

//**************************************************************
//                                                             *
//                  MSwavSoundFile                             *
//                                                             *
//**************************************************************
//MS wav files
class MSwavSoundFile:public SoundFileGeneric
	{
	public:
	MSwavSoundFile();
	Boolean Open(const String& file);
	};

//**************************************************************
//                                                             *
//                  ApasciSoundFile                            *
//                                                             *
//**************************************************************

class ApasciSoundFile:public SoundFileGeneric
	{
	public:
	ApasciSoundFile();
	virtual Boolean Open (const String& file);
	};

//**************************************************************
//                                                             *
//                  ApasciTelSoundFile                         *
//                                                             *
//**************************************************************

class ApasciTelSoundFile:public ApasciSoundFile
	{
	public:
	ApasciTelSoundFile();
	Boolean Open (const String& file);
	};

//**************************************************************
//                                                             *
//                       FeaturesFile                          *
//                                                             *
//**************************************************************
//class for read file of spectral features

class FeaturesFile:public SoundFileGeneric
	{
	t_index features_dim;
	
public:

	FeaturesFile();

	Boolean Open(const String& file);
	
	Boolean Read(VetDouble & vet,t_index len_win_sample);

	void Search_In_Header(const String& option);

};


//**************************************************************
//                                                             *
//                   SoundFileInterface                        *
//                                                             *
//**************************************************************


class SoundFileGeneric;
//provide a safe interface to different sound file standards
//which derive from SoundFileGeneric

class SoundFileInterface : public ImpPolimorphicPointer<SoundFileGeneric>
	{
	public:
	Boolean Configure(const String& config_file,const String& sect_name);
	};


#endif
