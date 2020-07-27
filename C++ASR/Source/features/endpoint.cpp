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
//  |   FUNCTIONALITY: file for endpoint detector							|
//  |   PROGRAM:									   					    |
//  |   COMMENTS:                                                           |
//  |   CONTRIBUTIONS: Fabrizio  & Domenico					                |
//  |   ACTUAL REVISION:   6.0                                              |
//  |   DATA ACTUAL REVISION:19/11/98                                       |
//  |   FIRST VERSION:   4.0                                                |
//  |   DATA FIRST VERSION: 20/7/98                                         |
//  |                                                                       |
//  |-----------------------------------------------------------------------|
//  _________________________________________________________________________

// *******************    VERSION HISTORY  *******************************


#include "../features/feature.h"
#include "../ioclass/soundlab.h"

void Print_Feature(t_index start,t_index end, const VetDoubleList &vet_feat, ofstream &f_out)
	{
	char* string;
	t_index k, kk;
	
	Assert(start<=end);
	Assert(end<=vet_feat.Dim());

	for(k=start;k<end;k++)
		for(kk=0;kk<vet_feat[k].Dim();kk++)
			{
			string=(char*)&vet_feat[k][kk];
			f_out.write(string,8);
			}
		
	return;
	}


void main(int argc, char* argv[])
	{
	FeatureExtraction feature;
	DbaseVoc dbase;
	VetDouble sample, feature_vet;
	VetDoubleList pred_list, whole_phon_instance;
	String buffer, aux_string;
	char* string;
	Boolean not_end_of_dbase=TRUE, new_file=FALSE, new_line;
	VetULong vet_bound;
	t_index start;
	t_real threshold;
    VetDouble vet_energy;
	ImpSimpleTypeList<Boolean> mark;
	t_index i;
	t_real min_energy=100.0;
	t_real max_energy=0.0;
    
    const t_index SEPARATOR=32;
	const t_index MIN_LEN_SPEECH=30;
	const t_index MIN_LEN_SIL=35;
	const t_index SIL_MARGIN=1;
	const t_index ENERGY_INDEX=12;
	const t_real PERCENTAGE=0.12;
		
	ofstream file_out;
	ifstream f_lis;
	String name_fts, name_phn;

	t_index k, dim, aux, index=1, frames=0;
	String config_file;

	if ( argc!=2)
		{
		mstat<<"No configfile name specified, using: res.ini";
		config_file="res.ini";
		}
	else config_file=argv[1];
		
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

			file_out.open(name_fts,ios::out|ios::binary|ios::app);
			string=(char*)&SEPARATOR;
			file_out.write(string,8);
			aux=file_out.tellp();
			for(k=0;k<1024-aux;k++)
				file_out.write(string,1);

			dim=whole_phon_instance.Dim();
			
			vet_energy.Destroy_And_ReDim(dim);
			for(k=0; k<dim; k++)
			    {
				vet_energy[k]=whole_phon_instance[k][ENERGY_INDEX];
                if (vet_energy[k]>max_energy)
					max_energy=vet_energy[k];
                if (vet_energy[k]<min_energy)
					min_energy=vet_energy[k];
				}
			threshold=min_energy+(max_energy-min_energy)*PERCENTAGE;

            mark.Destroy_And_ReDim(dim);
			for(k=0;k<dim;k++)
				if (vet_energy[k]<threshold)
					mark[k]=FALSE;
				else mark[k]=TRUE;

			for(k=0;k<dim;k++)
				if(mark[k]==FALSE)
				{
				start=k;
				k++;
				while(k<dim AND mark[k]==FALSE)
					k++;
				if((k-start)>MIN_LEN_SIL)
					{
					vet_bound.Append(start);
					vet_bound.Append(k-SIL_MARGIN);
					}
				}


            if (vet_bound.Dim()>1)
				{
				if (vet_bound[0]<MIN_LEN_SPEECH)
					vet_bound[0]=0;
				for (k=2; k<vet_bound.Dim()-1; k+=2)
					if ((vet_bound[k]-vet_bound[k-1])<MIN_LEN_SPEECH)
						{
						for (i=k; i<vet_bound.Dim()-1; i++)
							vet_bound[i-1]=vet_bound[i+1];
						vet_bound.Save_And_ReDim(vet_bound.Dim()-2);
						k-=2;
						}
				}

			if(vet_bound.Dim()==0)
				Print_Feature(0,whole_phon_instance.Dim(),whole_phon_instance,file_out);
			else{
				Print_Feature(0,vet_bound[0],whole_phon_instance,file_out);
				for(k=1; k<vet_bound.Dim()-1; k+=2)
					Print_Feature(vet_bound[k]+1,vet_bound[k+1],whole_phon_instance,file_out);
				if(vet_bound[vet_bound.Dim()-1]<dim)
					Print_Feature(vet_bound[vet_bound.Dim()-1]+1,dim,whole_phon_instance,file_out);
				}

			whole_phon_instance.Reset();
			vet_bound.Reset();
			
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
			whole_phon_instance.Append(feature_vet);
			frames++;
			}
		}  

	return;
	}
