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

//  _________________________________________________________________________
//  |-----------------------------------------------------------------------|
//  |                                                                       |
//  |   FILE:   Print_feature.cpp                                           |
//  |   FUNCTIONALITY: file for print file of features						|
//  |   PROGRAM:									   					    |
//  |   COMMENTS:                                                           |
//  |   CONTRIBUTIONS: Fabrizio								                |
//  |   ACTUAL REVISION:   6.0                                              |
//  |   DATA ACTUAL REVISION:   19/11/98                                    |
//  |   FIRST VERSION:   4.0                                                |
//  |   DATA FIRST VERSION: 7/7/98                                          |
//  |                                                                       |
//  |-----------------------------------------------------------------------|
//  _________________________________________________________________________

// *******************    VERSION HISTORY  *******************************


#include "../features/feature.h"
#include "../ioclass/soundlab.h"

void main(int argc, char* argv[])
	{
	FeatureExtraction feature;
	DbaseVoc dbase;
	VetDouble sample, feature_vet;
	VetDoubleList pred_list, whole_fon_istance;
	String buffer, aux_string;
	char* string;
	t_index separator=32;
	Boolean not_end_of_dbase=TRUE, new_file=FALSE, new_line;

	ofstream file_out;
	ifstream f_lis;
	String name_fts, name_phn;

	t_index k, dim, aux, index=1, frames=0;
	t_index start, end;
	String config_file;

	if ( argc!=2)
		{
		mstat<<"No configfile name specified, using: res.ini";
		config_file="res.ini";
		}
	else	
		{
		config_file=argv[1];
		}

	dbase.Configure(config_file,TRUE);
	feature.Configure(config_file);
	pred_list.Destroy_And_ReDim(feature.Max_Delta_Feature_Order());
	not_end_of_dbase=(Boolean)TRUE;

	while(not_end_of_dbase)
		{
		not_end_of_dbase=dbase.Get_Sequential_Vet(sample, new_file);

		if( (frames!=0) AND (new_file==TRUE OR not_end_of_dbase==FALSE ) )
			{
			name_fts=aux_string;
			name_fts<<".fts";
			file_out.open(name_fts);
		
			// write file header
		
			file_out<<"FeaturesFile\n\n";
			file_out<<"File="<<name_fts<<endl;
			file_out<<"window_lenght="<<dbase.Window_Lenght()<<endl;
			file_out<<"window_overlap="<<dbase.Window_Overlap()<<endl;
			feature.Actual_Configuration_String(buffer);
		
			k=0;
			while(k<buffer.Dim())
				{
				new_line=FALSE;
				while(buffer[k]==' ' OR buffer[k]=='>')
					{
					k++;
					new_line=TRUE;
					}
				if(new_line)
					file_out<<endl;
				file_out<<buffer[k];
				k++;
				}

			file_out<<endl<<"feature_dim= "<<feature.Feature_Vet_Dim()<<endl;
			file_out<<"feature_first_byte= 1024\n";
			file_out<<"feature_n_bytes= 8\n"<<"feature_byte_format= 01\n";
			file_out<<"end_head\n";

			file_out.close();

			name_phn=aux_string;
			name_phn<<".phn";
			f_lis.open(name_phn,ios::in|ios::nocreate);
			if(f_lis.fail())
				merr<<"Could not open phonetic transcription file: "<<name_phn;

			while (NOT f_lis.eof())
				{
				f_lis>>start;
				f_lis>>end;
				f_lis>>aux_string;
				}
			
			f_lis.close();

			dim=(end-dbase.Window_Lenght())/
				    (dbase.Window_Lenght()-dbase.Window_Overlap())+1;
		
			file_out.open(name_fts,ios::out|ios::binary|ios::app);
			string=(char*)&separator;
			file_out.write(string,8);
			aux=file_out.tellp();
			for(k=0;k<1024-aux;k++)
				file_out.write(string,1);

			//dim=whole_fon_istance.Dim();
			for(k=0;k<dim;k++)
				for(t_index kk=0;kk<whole_fon_istance[k].Dim();kk++)
					{
					string=(char*)&whole_fon_istance[k][kk];
					file_out.write(string,8);
					}
					
			whole_fon_istance.Reset();
			file_out.close();
		
			mstat<<"Num frames: "<<frames;
			mstat<<"END OF PROCESSING FOR FILE : "<<index;
			index++;
			frames=0;
			}

		if ( not_end_of_dbase )
			{
			if(new_file)
				{
				dbase.Get_Utter_FName(aux_string);
			    for(k=0;k<pred_list.Dim();k++)
					pred_list[k] = sample;	
				feature.Get_Previous_Frames_Info(pred_list, dbase.Smp_Rate());
				}

			feature.Extract(feature_vet, sample, dbase.Smp_Rate());
			whole_fon_istance.Append(feature_vet);
			frames++;
			}
		}  

	return;

	}
