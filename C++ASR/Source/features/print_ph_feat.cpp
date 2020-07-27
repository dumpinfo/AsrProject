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
//  |   ACTUAL REVISION: 6.0		                                        |
//  |   DATA ACTUAL REVISION: 19/11/98                                      |
//  |   FIRST VERSION:   4.0                                                |
//  |   DATA FIRST VERSION: 25/3/98                                         |
//  |                                                                       |
//  |-----------------------------------------------------------------------|
//  _________________________________________________________________________

// *******************    VERSION HISTORY  *******************************

// <<< version 4.0 >>>
//old version 02/04/98, 24/4/98 added Unix compatibility

#include "../features/feature.h"
#include "../ioclass/soundlab.h"

#define MAX_SIL_FRAMES 50

void main(int argc, char* argv[])
	{
	FeatureExtraction feature;
	DbaseVoc dbase;
	VetDouble sample, feature_vet;
	String buffer;
	char* string;
	t_index separator=32;
	VetULong init_symb_list;
	ConfigFile conf;
	Boolean not_end_of_dbase=TRUE, is_new_phone, new_line;

	ofstream file_out, file_phn;
	String name_fts, name_phn;

	VetDoubleList pred_list, whole_fon_istance;
	t_index i, num_of_symbols, act_phon, num_frames, k=0;
	t_index index, dim, aux;

	String config_file;

	if ( argc!=2)
		{
		mstat<<"No configfile name specified, using: res.ini";
		config_file="res.ini";
		} else  {
				config_file=argv[1];
				}

	dbase.Configure(config_file,TRUE);
	
	num_of_symbols=dbase.Get_Num_Of_Symbols();

	feature.Configure(config_file);

	pred_list.Destroy_And_ReDim(feature.Max_Delta_Feature_Order());

	conf.Open_File(config_file);

	conf.Get_String_Opt("Initialization","SymbolRange",buffer);
	
	if(buffer=="partial")
		conf.Get_Range_Opt("Initialization","SymbolList", init_symb_list);
	else {
		 init_symb_list.Destroy_And_ReDim(num_of_symbols);
		 for(i=0;i<num_of_symbols;i++)
			init_symb_list[i]=i;
		 }


	for(i=0; i<init_symb_list.Dim();i++)
		{
		act_phon = init_symb_list[i];
		index=0;
		
		mstat<<"act_phon: "<<act_phon;  // monitoring elaboration
		
		name_fts="phon";
		name_phn="phon";
		name_phn<<act_phon<<".phn";
		name_fts<<act_phon<<".fts";
		file_out.open(name_fts);
		file_phn.open(name_phn);

		// write file header
		
		file_out<<"FeaturesFile\n\n";
		file_out<<"Phoneme="<<act_phon<<endl;
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
		
		file_out.open(name_fts,ios::out|ios::binary|ios::app);
		string=(char*)&separator;
		file_out.write(string,8);
		aux=file_out.tellp();
		for(k=0;k<1024-aux;k++)
			file_out.write(string,1);

		// extract features for each phoneme 

		not_end_of_dbase=(Boolean)TRUE;
		num_frames=0;
		aux=feature.Feature_Vet_Dim();
		
		while(not_end_of_dbase)
			{
			not_end_of_dbase = dbase.Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(sample,
									act_phon, is_new_phone, pred_list);

			if(num_frames!=0 AND (is_new_phone==TRUE OR  !(not_end_of_dbase)))
				{
				dim=whole_fon_istance.Dim();
			
				String symbol;
				dbase.Translate_Symbol(symbol,act_phon);
				if(symbol=="sil" AND dim>MAX_SIL_FRAMES)
					{
					whole_fon_istance.Save_And_ReDim(MAX_SIL_FRAMES);
					dim=MAX_SIL_FRAMES;
					}
			
				// Print file of features

				for(k=0;k<dim;k++)
					{
					for(t_index kk=0;kk<whole_fon_istance[k].Dim();kk++)
						{
						string=(char*)&whole_fon_istance[k][kk];
						file_out.write(string,8);
					}
				}
			
			string=(char*)&separator;
			file_out.write(string,8);
					
			// Print file of features index
		
			file_phn<<index<<"\t";
			index+=dim*aux;
			dbase.Translate_Symbol(buffer, act_phon);
			file_phn<<index<<"\t"<<buffer<<endl;
			file_phn<<index<<"\t";
			index++;
			file_phn<<index<<"\t***"<<endl;
					
			whole_fon_istance.Reset();
			}

		if(not_end_of_dbase==TRUE)
			{
			num_frames++;
			
			if(is_new_phone)
				{
				feature.Get_Previous_Frames_Info(pred_list, dbase.Smp_Rate());
	  			}
  			feature.Extract(feature_vet, sample, dbase.Smp_Rate());

			whole_fon_istance.Append(feature_vet);
			}
		}//end while


		dbase.Restart();
		file_out.close();
		file_phn.close();

  		mstat<<"END OF PROCESSING FOR PHONEME :"<<act_phon;
		}
	
	return;

	}
