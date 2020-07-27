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
//  |   FILE:   Test_feature.cpp                                            |
//  |   FUNCTIONALITY: Test for feature extraction                          |
//  |   PROGRAM:															|
//  |   COMMENTS:                                                           |
//  |   CONTRIBUTIONS: Claudio   & Antonio,	Fabrizio	                    |
//  |   ACTUAL REVISION: 6.0								                |
//  |   DATA ACTUAL REVISION: 19/11/98                                      |
//  |   FIRST VERSION:   1.0                                                |
//  |   DATA FIRST VERSION: 25/2/96                                         |
//  |                                                                       |
//  |-----------------------------------------------------------------------|
//  _________________________________________________________________________


#include "../features/feature.h"
#include "../ioclass/soundlab.h"
//5/02/98 eliminted vetshortint
void Test_Features()
	{
	FeatureExtraction feature;
	DbaseVoc dbase;
	VetDouble sample, feature_vet;
	String config;

	VetDoubleList prev_vetsmp_list;
	t_real smp_rate;
	t_index i, order=2;

	dbase.Configure("res.ini",TRUE);
	dbase.Get_Sequential_Vet(sample);
	smp_rate=dbase.Smp_Rate();

	mstat<<"Configure feature class...";
	feature.Configure("res.ini");
	mstat<<"configure ok!";

	feature.Actual_Configuration_String(config);
	mstat<<"Feature Configuration:";
	mstat<<config;

	mstat<<"Memory order = "<<feature.Max_Delta_Feature_Order() ;
	mstat<<" size of vector from the last block (0= same size of input) = "
			<<feature.Feature_Vet_Dim();
	mstat<<"Extract feature...";
	feature.Extract(feature_vet, sample, smp_rate);
	mstat<<"feature ok!";

	prev_vetsmp_list.Destroy_And_ReDim(order);
	for(i=0;i<order;i++)
		dbase.Get_Sequential_Vet(prev_vetsmp_list[i]);

	feature.Get_Previous_Frames_Info(prev_vetsmp_list, smp_rate);

	mstat<<"Test is ok!";

	return;

	}

void main()
	{
	String key;
	t_index max_num=2666UL;
	Boolean res;
	Test_Diag_Routines();
	res = Test_Base_Class(max_num);

	Test_Features();
	mstat<<"push a key to exit";
	cin>>key;
	return;

	}