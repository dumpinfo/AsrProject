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
//  |   FILE:   mean_feature.cpp                                        |
//  |   FUNCTIONALITY: compute mean of features							|
//  |   PROGRAM: required to feature extraction                         |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:			4.0							            |
//  |   DATA FIRST VERSION:		25/4/1998	                            |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************

#include "../features/feature.h"
#include "../ioclass/soundlab.h"

void main(int argc, char* argv[])
{
	FeatureExtraction feature;
	DbaseVoc dbase;
	ofstream f_out;

	String config_file, file_name;
	Boolean not_end_of_dbase;
	VetDouble vet_features, vet_sum_features;
	VetDouble vetsmp;
	t_index num_frames=0,dim;

	if ( argc!=2)
		{
		mstat<<"No configfile name specified, using: res.ini";
		config_file="res.ini";
		}
	else{
		config_file=argv[1];
		}
	
	cout<<"Type name of file of mean value: ";
	cin>>file_name;
	
	feature.Configure(config_file);
	dbase.Configure(config_file,0);

	dim=feature.Feature_Vet_Dim();

	vetsmp.Destroy_And_ReDim(dbase.Window_Lenght());
	vet_features.Destroy_And_ReDim(dim);
	vet_sum_features.Destroy_And_ReDim(dim);

	not_end_of_dbase=TRUE;
	
	while(not_end_of_dbase)
		{
		not_end_of_dbase = dbase.Get_Sequential_Vet(vetsmp);

		if(not_end_of_dbase==TRUE)
			{
			num_frames++;
			feature.Extract(vet_features, vetsmp, dbase.Smp_Rate());
			vet_sum_features+=vet_features;
			}
		}

	vet_sum_features/=num_frames;

	f_out.open(file_name);
	f_out<<"[mean_value]"<<endl<<endl;
	f_out<<"vet_dim: "<<dim<<endl<<endl;
	for(t_index i=0;i<dim;i++)
		f_out<<vet_sum_features[i]<<"\t";

	return;
	}