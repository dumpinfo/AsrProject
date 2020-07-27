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
//  |   FILE:   resconf.cpp                                             |
//  |   FUNCTIONALITY: class ConfigFile implementation                  |
//  |   PROGRAM: required to codes which opens ires config file         |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio      claudio				            |
//  |   ACTUAL REVISION: 6.0					                        |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 20/11/95                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************
//12/02/98 eliminated NULL and operator+=(String)




// *******************   USER INFORMATION   ******************************

// *******************   Implementation ISSUES   ******************************

#define RES_CONFIGURATION_CLASS_VERSION "V. 6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__
#include "../resconf/resconf.h"
#include <string.h>

ConfigFile::ConfigFile()

	{
	file_open=FALSE;
	}

ConfigFile::~ConfigFile()
	{
	//pay attention closing twice produces unpredictable results
	if (file_open)
		{
		file_ini.close();
		file_opt.close();
		file_open=FALSE;
		}
	return;
	};

void ConfigFile::Open_File(const String & file_name)
	{

	if (file_open)
		{
		file_open=FALSE;
		file_ini.close();
		file_opt.close();
		}

	if (file_name.Dim()==0)
		{
		file_ini.open("res.ini",ios::in|ios::nocreate);
		}
		else	{
				file_ini.open(file_name,ios::in|ios::nocreate);
				}

	file_opt.open("res.opt",ios::in|ios::nocreate);

	if (file_opt.fail())
		merr<<"Cannot open system file: res.opt";
	if (file_ini.fail())
		merr<<"Cannot open config file: "<<file_name;

	file_open=TRUE;
	return;
	}



void ConfigFile::Get_Option(ifstream & file,const String & win_section,
		const String & win_option_type,	String &option)

	{
	String section, option_type, buffer,temp;
	char line[MAX_LEN_INPUT_LINE+2];
	t_index i,j;

	option="";

	if (NOT file_open)
		merr<<"Error ! You must open config file before try to read options";

	//search for [section]

	section<<"["<<(win_section)<< ']';
	section.Make_Lower();
	option_type=win_option_type;
	option_type<<'=';
	option_type.Make_Lower();

	if(file.eof())
		file.clear();

	file.seekg(0L,ios::beg);
	
	do	{
		file.getline(line,MAX_LEN_INPUT_LINE);
		buffer=line;
		if(file.eof())
			merr<<"Section: "<<win_section<< " not found in config file"<<endl;
		buffer.Make_Lower();
		buffer.Delete_Non_Print_Space_Tab_Chars();
		}
	while (!(buffer[0]!=';' AND buffer.Is_SubString_Inside(section,i)));

	//search for option

	line[0]=' ';

	do	{

		if(file.eof() OR line[0]=='[')
			merr<<"Option: "<<option_type<<"  not found in config file";

		file.getline(line,MAX_LEN_INPUT_LINE);
		buffer=line;
					
		buffer.Make_Lower();
		buffer.Delete_Non_Print_Space_Tab_Chars();
		}
		while (!(buffer[0]!=';' AND buffer.Is_SubString_Inside(option_type,i)));

		temp.Take(buffer,i,buffer.Dim()-1);

		//eliminates remarks
		if(temp.Is_SubString_Inside(";",j)==FALSE)
			j=temp.Dim();

		//eliminates option type
		i=option_type.Dim();
		if(i<j) 
			option.Take(temp,i,j-1);
	return;
	}

void ConfigFile::Take_Left_And_Right(const String & option, const String & str,
					String &left, String & right)
	{
	t_index pos,dim;
	String buffer_opt, buffer_str;

	buffer_opt=option;
	buffer_str=str;
	left="";
	right="";

	if(buffer_opt.Is_SubString_Inside(str,pos))
		{ 
		if(pos>0)
			left.Take(buffer_opt,0,pos-1);
		dim=buffer_opt.Dim();
		if(pos+2<=dim-1)
			 right.Take(buffer_opt,pos+buffer_str.Dim(),buffer_opt.Dim()-1);
		}
		else left=buffer_opt;
	return;
	}


void ConfigFile::Get_String_Opt(const String & win_section, const String & win_option_type,
											 String &option,const String & win_opt_section)
	{
	String buffer_ini, buffer_opt, buffer, deflt;
	t_index pos;


	option="NO_OPTION_FILE_CHECK";

	// option !=win_opt_section is equal to  "NO_OPTION_FILE_CHECK"!=win_opt_section
	if (option!=win_opt_section)
		{

		if (win_opt_section.Dim()==0)
			Get_Option(file_opt, win_section, win_option_type,buffer_opt);
			else Get_Option(file_opt, win_opt_section, win_option_type,buffer_opt);
		}
	Get_Option(file_ini, win_section, win_option_type, buffer_ini);

	Take_Left_And_Right(buffer_opt, "::", buffer, deflt);

	if(buffer_ini.Dim()<1)
		merr<<"Option: "<<win_option_type <<" not found in config file";

	// option !=win_opt_section is equal to  "NO_OPTION_FILE_CHECK"!=win_opt_section
		if(buffer.Dim()<1 AND option !=win_opt_section)
		mwarn<<"No information about valid string on "<<win_option_type ;
	else if(option !=win_opt_section
				 AND buffer.Is_SubString_Inside(buffer_ini,pos)==FALSE
			AND buffer!="file_name" AND buffer != "string")
		merr<<"Non valid option in config file: "<<buffer;

	option=buffer_ini;
	return;
	}


t_index ConfigFile::Get_Unsigned_Opt (const String & win_section,
		const String &win_option_type,const String & win_opt_section)
	{
	String buffer, buffer_opt, buffer_ini, deflt;
	t_index i, min=0, max=0, var;

	if (win_opt_section.Dim()==0)
		Get_Option(file_opt, win_section, win_option_type,buffer_opt);
		else Get_Option(file_opt, win_opt_section, win_option_type,buffer_opt);

	Get_Option(file_ini,win_section, win_option_type, buffer_ini);

	Take_Left_And_Right(buffer_opt, "::", buffer, deflt);
		
	if(buffer.Dim()<1)
		mstat<<"No information about valid range in config file: option:"<<win_option_type;


	buffer.Cut_Char('[');
	buffer.Cut_Char(']');

	buffer_opt.Reset();

	for(i=0;i<buffer.Dim();i++)
	  	{ 
		if(buffer[i]!='<')
			buffer_opt<<buffer[i];
		else{ 
			min=atol(buffer_opt);
			buffer_opt.Reset();
			}
		}

	max=atol(buffer_opt);

	if(buffer_ini.Dim()>0)
		{ 
		var=atol(buffer_ini);
		if( (var<min OR var>max) AND max>min)
			 merr<<"Option "<<win_option_type<<" out of valid range : "<<var<<" not in ["<<min<<" , "<<max<<"]";
		}
		else merr<<"Option not found in config file"<<win_option_type;

	return var;
	}

		
t_real ConfigFile::Get_Real_Opt (const String & win_section,
				const String & win_option_type,const String & win_opt_section)
	{
	String buffer, buffer_opt, buffer_ini, deflt;
	t_index i;
	t_real min=0.0, max=0.0, var;

	if (win_opt_section.Dim()==0)
		Get_Option(file_opt, win_section, win_option_type,buffer_opt);
		else Get_Option(file_opt, win_opt_section, win_option_type,buffer_opt);

	Get_Option(file_ini,win_section, win_option_type, buffer_ini);

	Take_Left_And_Right(buffer_opt, "::", buffer, deflt);
		  
	if(buffer.Dim()<1)
		mstat<<"No information about valid range in config file option:"<<win_option_type;

	buffer_opt.Reset();

	buffer.Cut_Char('[');
	buffer.Cut_Char(']');

	//get the allowed bounds
	for(i=0;i<buffer.Dim();i++)
		{ 
		if(buffer[i]!='<')
		        buffer_opt<<buffer[i];
			else 	{ 
					min=atof(buffer_opt);
					buffer_opt.Reset();
					}
		}
			  
	max=atof(buffer_opt);
			    
	if(buffer_ini.Dim()>0)
		{ 
		var=atof(buffer_ini);
		if( (var<min OR var>max) AND max>min)
				 merr<<"Option "<<win_option_type<<" out of valid range : "<<var<<" not in ["<<min<<" , "<<max<<"]";
		}
		else merr<<"Option not found in config file:"<<win_option_type;

	return var;
	}


Boolean ConfigFile::Get_Boolean_Opt(const String & win_section,
							const String & win_option_type)
	{

	String buffer;

	Get_Option(file_ini,win_section,win_option_type, buffer);

	if(buffer.Dim()<1)
		merr<<"Option not found in config file: "<<win_option_type;

	if (buffer=="yes" OR  buffer=="true" OR buffer=="sì" OR buffer=="si")
			return (Boolean)TRUE;
	if (buffer=="no" OR buffer=="false")
		 return (Boolean)FALSE;

	merr<<"Not valid boolean option in config file: "<<buffer<<" option:"<<win_option_type;

	return FALSE;
	}


void ConfigFile::Get_Range_Opt(const String & win_section, const String & win_option_type,
											VetULong &range,const String & win_opt_section)
	{
	String buffer_opt, buffer, deflt, temp;
	t_index dim,i=0,start,end,ind=0;
	t_signed min=0,max=0;

	VetLong temp_range;

	range.Reset();
	temp_range.Reset();

	if (win_opt_section.Dim()==0)
		Get_Option(file_opt, win_section, win_option_type,buffer_opt);
		else Get_Option(file_opt, win_opt_section, win_option_type,buffer_opt);


	Take_Left_And_Right(buffer_opt, "::", buffer, deflt);

	buffer.Cut_Char('[');
	buffer.Cut_Char(']');
	buffer_opt.Reset();

	for(i=0;i<buffer.Dim();i++)
		{
		if(buffer[i]!='<')
			buffer_opt<<buffer[i];
			else 	{ 
					min=atol(buffer_opt);
					buffer_opt.Reset();
					} 
		}
			  
	max=atol(buffer_opt);

	Get_Option(file_ini, win_section, win_option_type, buffer);
			
	dim=buffer.Dim();

	if(dim<1)
		merr<<"Option not found in config file: "<<win_option_type;

	i=0;

	while(i<dim)
		{
		start=i;
		while(buffer[i]!=',' AND buffer[i]!='-' AND i<dim)
			i++;
		end=i;
		if(start==end)
			i++;
			else 	{ 
				temp_range.Save_And_ReDim(ind+1);
				temp.Take(buffer,start,end-1);
				if(start>0)
					{
					if(buffer[start-1]=='-')
						{
						temp_range[ind]=-atoi(temp);
						if(ind AND (abs(temp_range[ind-1])>abs(temp_range[ind]) OR temp_range[ind-1]<0))
							  merr<<"Bad range option in config file: "<<win_option_type;
				          	}
					   	else temp_range[ind]=atoi(temp);
					}
						else temp_range[ind]=atoi(temp);
					if((abs(temp_range[ind])<min OR abs(temp_range[ind])>max) AND max>min)
							 merr<<"Option "<<win_option_type<<" out of valid range : "<<temp_range[ind]<<" not in ["<<min<<" , "<<max<<"]";

					ind++;
					i++;
				}
		}

		dim=1;
		t_index tmp_dim=temp_range.Dim(), j;
		i=0;

		while(i<tmp_dim)
		{
			if(temp_range[i]<0 AND i==0)
			{
				for(j=0;j<=(t_index)abs(temp_range[i]);j++)
				{
					range.Save_And_ReDim(dim);
					range[dim-1]=j;
					dim++;
				}
				i++;
			}
			
			else if(i<tmp_dim-1)
			{
				if(temp_range[i+1]<0)
				{
					for(j=temp_range[i];j<=(t_index)abs(temp_range[i+1]);j++)
					{
						range.Save_And_ReDim(dim);
						range[dim-1]=j;
						dim++;
					}
					i+=2;
				}
				else
				{
					range.Save_And_ReDim(dim);
					range[dim-1]=temp_range[i];
					dim++;
					i++;
				}
			}
			else
			{
				range.Save_And_ReDim(dim);
				range[dim-1]=temp_range[i];
				dim++;
				i++;
			}
		}

	for(i=0;i<dim-2;i++)
		if(range[i+1]<range[i]) merr<<"Bad range in config file!";
		
	return;
	}


	Boolean Test_Config_Class()
		{
		ConfigFile config;
		String option;


		t_index unsig;
		t_real rl;
		
		VetULong vet;
		mwarn<<"testing config class Version:"<<RES_CONFIGURATION_CLASS_VERSION;
		config.Open_File("res.ini");

		config.Get_String_Opt("dbaseoptions","soundfiletype",option);
		mstat<<"dbaseoptions,"<<"soundfiletype: "<<"String Option : "<<option<<endl;

		config.Get_String_Opt("dbaseoptions","listofsoundfnames",option);
		mstat<<"initialization,"<<"listofsoundfnameS:"<<"String Option : "<<option<<endl;

		unsig=config.Get_Unsigned_Opt("segment","treevolume");
		mstat<<"segment,"<<"treevolume:"<<"Unsigned Option : "<<unsig<<endl;

		rl=config.Get_Real_Opt("recognition","beamcoefficientinitial");
		mstat<<"recognition,"<<"beamcoefficient:"<<"Double Option : "<<rl<<endl;

	
		config.Get_Range_Opt("Initialization","symbollist",vet);
        mstat<<"Initialization,"<<"symbollist";
		for(t_index i=0; i<vet.Dim();i++)
		  mstat<<vet[i]<<" ";
		return TRUE;
		}
