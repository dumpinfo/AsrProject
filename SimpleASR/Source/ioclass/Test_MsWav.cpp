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
//  |   FILE:   Test_MsWav.cpp                                          |
//  |   FUNCTIONALITY:  										        |
//  |   PROGRAM: 														|
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Fabrizio		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION:19/11/98	                                |
//  |   FIRST VERSION:    4.0                                           |
//  |   DATA FIRST VERSION: 24/04/98                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
#include "../ioclass/soundlab.h"

void main()
	{
	DbaseVoc dbase;
	VetDouble vet_smp;
	ofstream file_out;
	ifstream file_in;
	t_index i, count=0;
	t_index smp;
	Boolean not_end_of_dbase=TRUE, new_file=FALSE;
	String file_name;
	char *aux;
	
	dbase.Configure("res.ini",FALSE);
	
	vet_smp.Destroy_And_ReDim(dbase.Window_Lenght());
	
	while(not_end_of_dbase)
		{
		not_end_of_dbase=dbase.Get_Sequential_Vet(vet_smp, new_file);
		
		if(new_file)
			{
			if(file_name.Dim()!=0)
				mstat<<"File: "<<file_name<<"\tSamples copied: "<<count;
			if(file_out.is_open())
				file_out.close();
			dbase.Get_Utter_FName(file_name);
			file_name<<"_copy"<<WAVE_EXTENCTION;
			file_out.open(file_name,ios::binary);
			count=0;
			}
		
		if(not_end_of_dbase)
			{
			for(i=0;i<vet_smp.Dim();i++)
				{
				smp=(t_index) vet_smp[i];
				aux=(char*)&smp;
				file_out.write(aux,2);
				}
			count+=vet_smp.Dim();
			}
		}
	
	file_out.close();
	mstat<<"File: "<<file_name<<"\tSamples copied: "<<count;
	mstat<<"\nPush a key to end";
	cin>>file_name;
	return;
	
	}
