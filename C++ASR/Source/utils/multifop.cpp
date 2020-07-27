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
//  |   FILE:   multifop.cpp                                            |
//  |   FUNCTIONALITY:  procedure on multiple files						|
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   AUTHORS: Claudio			                                    |
//  |   ACTUAL REVISION:   6.0                                          |
//  |   DATA ACTUAL REVISION:	19/11/98								|
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 19/5/98                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    Improvement  *******************************


// *******************    VERSION HISTORY  *******************************

// <<< version 1.0 >>>

//*******************   USER INFORMATION   *******************************

//*******************   IMPLEMENTATION INFORMATION   *******************************


//*******************   CLASS INDEX   *******************************


#include "multifop.h"
#include "../resconf/resconf.h"




//************************************************************************
//                                                                      **
//                      SingleFileOp class					            **
//                                                                      **
//************************************************************************

//add in this function your custom class

void SingleFileOp::Configure(const String & config_fname)
	{
		
	ConfigFile conf;
	t_index number_of_blocks,i;
	String section_name,option_label,block_type,value;
	String section="MultipleFileProcedure";

	conf.Open_File(config_fname);

	//get first parameters
	number_of_blocks =conf.Get_Unsigned_Opt(section,
									"Number_Of_Blocks");

	if(number_of_blocks==0)
		merr<<"zero blocks requested in Feature Configuration";

	conf.Get_String_Opt(section, "Label_Of_Blocks",option_label);


	Assert(option_label.Dim()>0);
	//delete previous configuration
	if(operations.Dim()>0)
		{
		//save configuration in diagnostic
		mwarn<<"reconfiguration of feature extraction requested";
		operations.Reset();
		}

	//allocate blocks
	operations.Destroy_And_ReDim(number_of_blocks);
	for(i=0;i< number_of_blocks;i++)
		{
		section_name.Reset();
		section_name<<option_label<<'_'<<(i+1)<<"_";

		conf.Get_String_Opt(section_name, "Type_Of_Block",value,
				"NO_OPTION_FILE_CHECK");

		//***************************************
		//*										*
		//*		 ADD HERE ANY NEW BLOCK !!!		*
		//*										*
		//***************************************
	
		if (value == LoadFile::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(LoadFile);
		else
		if (value == SaveFileWithoutCR::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(SaveFileWithoutCR);
		else
		if (value == ModifySlashForUnix::dsp_block_name  )
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(ModifySlashForUnix);
		else
		if (value== CountStrings::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(CountStrings);
		else
		if (value== InsertHeaderFileatBeginning::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(InsertHeaderFileatBeginning);
		else
		if (value== SaveFile::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(SaveFile);
	
		else
		merr<<"the block type: "<< value<<" section:"<< section_name
			<<" of the file: "<< config_fname<<" does not exist";

		//initialize blocks
		operations[i]->Initialize(config_fname, section_name);
		};

	
	return;
	}


	void SingleFileOp::Perform_Single_File_Operation(const String & file_name)
		{
		t_index i,number_of_blocks;

		number_of_blocks = operations.Dim();

		Assert(number_of_blocks >0);


		for(i=0;i< number_of_blocks;i++)
			operations[i]->Apply(file_name,file);

		return;
		
		}
	void SingleFileOp::Destroy_Procedures()
		{
		t_index i,number_of_blocks;
		number_of_blocks = operations.Dim();

		Assert(number_of_blocks >0);


		for(i=0;i< number_of_blocks;i++)
			operations[i]->Destroy_Procedure();

		return;
		
		}

//************************************************************************
//                                                                      **
//                      FileOperations									**
//                                                                      **
//************************************************************************
const String LoadFile::dsp_block_name="loadfile";
const String SaveFileWithoutCR::dsp_block_name="savefilewithoutcr";
const String SaveFile::dsp_block_name="savesimplyfile";
const String ModifySlashForUnix::dsp_block_name="modifyslashforunix";
const String CountStrings::dsp_block_name="countstrings";
const String InsertHeaderFileatBeginning::dsp_block_name="insertheaderfileatbeginning";

Boolean InsertHeaderFileatBeginning::Apply(const String & filename, String & file_data)
	{
	t_index i,lentot,len=file_data.Dim();
	
	String banner="\
//  *********************************************************************\n\
//  *                   This file is part of RES 6.0.                   *\n\
//  *    RES 6.0 is an original software distributed within the book    *\n\
//  *                                                                   *\n\
//  *      |-----------------------------------------------------|      *\n\
//  *      | \"Speech Recognition: Theory and C++ Implementation\" |      *\n\
//  *      |               John Wiley & Sons, Ltd                |      *\n\
//  *      |    by Claudio Becchetti and Lucio Prina  Ricotti    |      *\n\
//  *      |-----------------------------------------------------|      *\n\
//  *                                                                   *\n\
//  *       See copyright.txt file for further info. on copyright       *\n\
//  *********************************************************************\n\n";	




	lentot=file_data.Dim()+ banner.Dim()+1;
	file_data.Save_And_ReDim(lentot);
	file_data[lentot-1]='\0';

	for(i=0;i<len;i++)
		file_data[lentot-i-2]=file_data[len-i-1];

	for(i=0;i<banner.Dim();i++)
		file_data[i]=banner[i];
	
	return TRUE;
	}


Boolean LoadFile::Apply(const String & filename,String & file_data)
	{
	ifstream file;
	t_index i=0;
	const t_index chunk=500000;
	
	file.open(filename,ios::in|ios::binary);
	if(file.fail())
		merr<<"Could not open file "<<filename;
	file_data.Reset();
	file_data.Destroy_And_ReDim(chunk);
	
	while(file)
		{
		file.get(file_data[i]);
		i++;
		if (i==file_data.Dim())
			file_data.Destroy_And_ReDim(chunk+file_data.Dim());

		}
	file_data.Save_And_ReDim(i);
	file.close();
	return TRUE;
	
	};
Boolean SaveFileWithoutCR::Destroy_Procedure()
	{ 
	mwarn<<"Number of CR deleted "<<counter;
	return TRUE;
	}
Boolean SaveFileWithoutCR::Apply(const String & filename, String & file_data)
	{
	ofstream file;
	t_index i,len=file_data.Dim();
	file.open(filename,ios::binary|ios::out);
	if(file.fail())
		merr<<"Could not open file for write"<<filename;

	for(i=0;i<len;i++)
		if(file_data[i]!='\r')
			file.put(file_data[i]);
			else counter++;

		file.close();
	return TRUE;
	};

Boolean SaveFile::Apply(const String & filename, String & file_data)
	{
	ofstream file;
	t_index i,len=file_data.Dim();
	file.open(filename,ios::binary|ios::out);
	if(file.fail())
		merr<<"Could not open file for write"<<filename;

	for(i=0;i<len;i++)
		file.put(file_data[i]);
		
	file.close();
	return TRUE;
	};
Boolean ModifySlashForUnix::Apply(const String & filename, String & file_data)
	{
	t_index position=0;
	Boolean exit_now,inside_quotes;
	char datum;

	while (Is_SubString_Inside(file_data,"#include",position,position))
		{
		exit_now=FALSE;
		inside_quotes=FALSE;
		do	{
			position++;
			datum=file_data[position];
			if (datum=='\r' OR datum=='\n' )
				exit_now=TRUE;
			else  if (datum=='"' AND inside_quotes==FALSE)
				inside_quotes=TRUE;
			else  if(datum=='"' AND inside_quotes==TRUE)
				exit_now=TRUE;
			else  if(datum=='\\' AND inside_quotes==TRUE)
					{
					counter++;
					file_data[position]='/';
					}
			}
			while (NOT exit_now);
	
		}
	return TRUE;
	};

Boolean ModifySlashForUnix::Destroy_Procedure()
	{ 
	mwarn<<"Number of slash substituted: "<<counter<<" times";
	return TRUE;
	}
Boolean  ModifySlashForUnix::Is_SubString_Inside (const String & actual_file,const String & sub, t_index &position,t_index start_position=0)const
		{

		t_index i=0, j=0,lim;

		position=0;
		if(sub.Dim()==0)
			return FALSE;
		if (actual_file.Dim()==0)
			return FALSE;
		
		i=start_position;
		lim=actual_file.Dim();
		while (i<lim AND actual_file[i]!='\0' AND sub[j]!='\0')
			if(actual_file[i] == sub[j])
				{
				j++;
				i++;
				}
				else	{
						i++;
						position=i;
						j=0;
						}
		if(sub[j]=='\0')
				return TRUE;
			else return FALSE;
		}



Boolean CountStrings::Destroy_Procedure()
	{ 
	mwarn<<"The character ascii"<<(int)char_to_count
			<<" is counted "<<counter<<" times";
	return TRUE;
	};





//************************************************************************
//                                                                      **
//                      MultiFileOperations					            **
//                                                                      **
//************************************************************************


Boolean MultiFileOperations::Perform_Multi_File_Operations(const String & config_file)
	{
	t_index i;
	Configure(config_file);

	procedures.Configure(config_file);
		
	for(i=0;i<file_list.Dim();i++)
		{
		procedures.Perform_Single_File_Operation(file_list[i]);
		cout<<"processed file "<<i+1<<"th of "<<file_list.Dim()<<" :"<<file_list[i]<<'\n';
		}
	procedures.Destroy_Procedures();
	return TRUE;
	}


void MultiFileOperations::Configure(const String & config_file)
	{
	ConfigFile conf;
	String db_file;
	conf.Open_File(config_file);

	conf.Get_String_Opt("MultipleFileProcedure", "FileNameofListOfFilesToBeProcessed", db_file);
	Inizialize_File_List(db_file);

	return;
	}


void MultiFileOperations::Inizialize_File_List(const String & db_file)
	{
	ifstream f_lis;
	String dir;
	String temp;
	t_index i=0;
	t_index num_act_file;
	
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
			file_list[i]=dir;
			f_lis>>temp;
			file_list[i]<<temp;
			}
		 }

	else
		{
		num_act_file=i+1;

		file_list.Destroy_And_ReDim(num_act_file);

		for(i=0;i<num_act_file;i++)
			f_lis>>file_list[i];
			
		}
	mstat<<"Number of files to be processed: "<<num_act_file;

	return;
 }

Boolean CountStrings::Apply(const String & filename, String & file_data)
	{
	t_index i,len=file_data.Dim();
	for(i=0;i<len;i++)
		if(file_data[i]==char_to_count)
			counter++;
	return TRUE;
	}


//************************************************************************
//                                                                      **
//                      main											**
//                                                                      **
//************************************************************************

int main(int argc, char* argv[])
	{
	String config_file_name;
	MultiFileOperations multifileop;

	if ( argc!=2)
		{
		mwarn<<"No config file name specified, using \"util.ini\""<<endl;  
		config_file_name="./util.ini";
		}
		else	
				{
				config_file_name=argv[1];	
				}

	multifileop.Perform_Multi_File_Operations(config_file_name);
	return TRUE;
	}


