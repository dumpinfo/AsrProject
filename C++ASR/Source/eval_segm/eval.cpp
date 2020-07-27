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
//  |   FILE:   Eval.cpp                                                |
//  |   FUNCTIONALITY: evaluation of segmentation                       |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico  & Fabrizio						        |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:	 4.2	                                        |
//  |   DATA FIRST VERSION: 22/09/98                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#include "eval.h"
#include "../resconf/resconf.h"

Error::Error ()
	{
	five     = 0;
	ten      = 0;
	twenty   = 0;
	over     = 0;
	error    = 0;
	tot      = 0;

	}
	
void Eval::Read_Options (const String & conf_file_name)
	{
	ConfigFile config;
	String string_option;
         
	//open config file
	config.Open_File(conf_file_name);

    if (each_phon=config.Get_Boolean_Opt ("EvalSegm", "EachPhoneme"))
		label.Configure(conf_file_name, "dbaseoptions");
	
    config.Get_String_Opt("EvalSegm", "FileList", file_list_name);
	config.Get_String_Opt("EvalSegm", "AutomFileExt", f_autom_ext);
	config.Get_String_Opt("EvalSegm", "CheckFileExt", f_check_ext);
	config.Get_String_Opt("EvalSegm", "ReportFileName", f_report_name);
	sample_rate=config.Get_Real_Opt ("EvalSegm", "SampleRate");

	return;
	}

void Eval::Load_File_List ()
	{
    ifstream file_in;
	String dir, temp;
	t_index dim;

    file_in.open(file_list_name, ios::in|ios::nocreate);
	if (file_in.fail())
		merr<<"Cannot open file : "<<file_list_name;  

    //to avoid continuous memary allocations
    file_list.Set_Data_Chunk(100);
  
	file_in>>dir;
	if (dir[dir.Dim()-1]=='\\' OR dir[dir.Dim()-1]=='/')
		{
		while (!file_in.eof())
			{ 
			file_in>>temp;
			if (temp[0]!=EOF)
				{
				file_list.Append(dir);
				file_list[file_list.Dim()-1]<<temp;
				}
			}
		}
	else{
		file_list.Append(dir);
		while (!file_in.eof())
			{
			file_in>>temp;
			if (temp[0]!=EOF)
				file_list.Append(temp);
			}
		}

    file_in.close();
    //check that file_list contain at least a only file
    dim=file_list.Dim();
    if (dim==0 OR (dim==1 AND file_list[0][file_list[0].Dim()-1]=='\\'))
		merr<< "The file of files list "<<file_list_name<<" is empty."<<endl;  

	return;
    
	}//end of Load_File_List function

void Eval::Evaluate_File_Segm (const String & file_name)
	{
	String f_autom;
	String f_check;
	String phon_a;
	String phon_m;

	t_index initial_a;
	t_index final_a;
	t_index initial_m;
	t_index final_m;
	t_index diff_initial;
	t_index diff_final;

	t_index act_phon;

	ifstream f_aut;
	ifstream f_ch;

	Boolean fi, te, tw, ov;
	Boolean first_phon;
	Boolean fi_f, te_f, tw_f, ov_f;

	f_autom=file_name;
	f_autom<<'.'<<f_autom_ext;
	f_check=file_name;
	f_check<<'.'<<f_check_ext;

	f_aut.open (f_autom, ios::nocreate|ios::in);
	if (f_aut.fail())
		merr<<"Cannot open file: "<<f_autom;

	f_ch.open (f_check, ios::nocreate|ios::in);
	if (f_ch.fail())
		merr<<"Cannot open file: "<<f_check;

	first_phon=TRUE;

	while (!(f_aut.eof() || f_ch.eof()))
		{
		fi=te=tw=ov=FALSE;
		
		f_aut>>initial_a;
		f_aut>>final_a;
		f_aut>>phon_a;

		f_ch>>initial_m;
		f_ch>>final_m;
		f_ch>>phon_m;

		tot_count_error.tot++;

		diff_final=(t_index)fabs((t_real)final_a-(t_real)final_m);

		if (final_a <= initial_m || initial_a >= final_m)
			tot_count_error.error++;
		else
			{
			if (diff_final <= 5*sample_rate)
				{
				tot_count_error.five++;
				fi=TRUE;
				}
			if (diff_final <= 10*sample_rate)
				{
				tot_count_error.ten++;
				te=TRUE;
				}
			if (diff_final <= 20*sample_rate)
				{
				tot_count_error.twenty++;
				tw=TRUE;
				}
			else
				{
				tot_count_error.over++;
				ov=TRUE;
				}
			}
		if (each_phon)
			{
			
			act_phon=label->Translate_Symbol(phon_a);
			diff_initial=(t_index)fabs((t_real)initial_a-(t_real)initial_m);

			fi_f=te_f=tw_f=ov_f=FALSE;
			
			final_count_err[act_phon].tot++;

			if (final_a <= initial_m || initial_a >= final_m)
				{
				initial_count_err[act_phon].error++;
				final_count_err[act_phon].error++;
				}
			else
				{
				if (diff_final <= 5*sample_rate)
					{
					final_count_err[act_phon].five++;
					fi_f=TRUE;
					}
				if (diff_final <= 10*sample_rate)
					{
					final_count_err[act_phon].ten++;
					te_f=TRUE;
					}
				if (diff_final <= 20*sample_rate)
					{
					final_count_err[act_phon].twenty++;
					tw_f=TRUE;
					}
				else
					{
					final_count_err[act_phon].over++;
					ov_f=TRUE;
					}
				if (!first_phon)
					{
					initial_count_err[act_phon].tot++;
					if (diff_initial <= 5*sample_rate)
						initial_count_err[act_phon].five++;
					if (diff_initial <= 10*sample_rate)
						initial_count_err[act_phon].ten++;
					if (diff_initial <= 20*sample_rate)
						initial_count_err[act_phon].twenty++;
					else
						initial_count_err[act_phon].over++;
					}
				else first_phon=FALSE;	
				}

			}// end of if (each_phon)
			
		} //end of while

	if (fi) tot_count_error.five--;
	if (te) tot_count_error.ten--;
	if (tw) tot_count_error.twenty--;
	if (ov) tot_count_error.over--;
	tot_count_error.tot--;
	if (each_phon)
		{
		if (fi_f) final_count_err[act_phon].five--;
		if (te_f) final_count_err[act_phon].ten--;
		if (tw_f) final_count_err[act_phon].twenty--;
		if (ov_f) final_count_err[act_phon].over--;
		}
	
	
	f_aut.close();
	f_ch.close();
	
	return;
	}	

void Eval::Evaluate_Segmentation ()
	{
	t_index i;
	t_index num_files;
	t_index num_sym;

	num_files=file_list.Dim();

	Check(num_files>0, "Empty file list");

	if (each_phon)
		{
		num_sym=label->Get_Num_Of_Symbols ();
		initial_count_err.Destroy_And_ReDim(num_sym);
		final_count_err.Destroy_And_ReDim(num_sym);
		}

	for (i=0; i<num_files; i++)
		Evaluate_File_Segm (file_list[i]);

	return;
	}

void Eval::Write_Report_File ()
	{
	ofstream f_out;
	
	t_index i;
	t_index num_phon;

	String phoneme;

	f_out.open (f_report_name, ios::out);
	if (f_out.fail())
		merr<<"Cannot open file "<<f_report_name;
	
	f_out<<"\tSegmentation report file"<<endl<<endl;	
	f_out<<"\t\t\t<= 5 ms \t: "<<((t_real)tot_count_error.five/(t_real)tot_count_error.tot)*100.0<<'%'<<endl;
	f_out<<"\t\t\t<= 10 ms \t: "<<((t_real)tot_count_error.ten/(t_real)tot_count_error.tot)*100.0<<'%'<<endl;
	f_out<<"\t\t\t<= 20 ms \t: "<<((t_real)tot_count_error.twenty/(t_real)tot_count_error.tot)*100.0<<'%'<<endl;
	f_out<<"\t\t\toltre 20 ms \t: "<<((t_real)tot_count_error.over/(t_real)tot_count_error.tot)*100.0<<'%'<<endl;
	f_out<<"\t\t\tfonemi errati \t: "<<((t_real)tot_count_error.error/(t_real)tot_count_error.tot)*100.0<<'%'<<endl;
			
	if (each_phon)
		{
		num_phon=initial_count_err.Dim();
		f_out<<endl;
		for (i=0; i<num_phon; i++)
			{
			if (final_count_err[i].tot > 0)
				{
				label->Translate_Symbol(phoneme, i);
				f_out<<endl<<"\tPhoneme "<<phoneme<<endl;
				f_out<<"\t\t\t<= 5 ms \t: "<<((t_real)initial_count_err[i].five/(t_real)final_count_err[i].tot)*100.0<<'%'<<'\t';
				f_out<<((t_real)final_count_err[i].five/(t_real)final_count_err[i].tot)*100.0<<'%'<<endl;
				f_out<<"\t\t\t<= 10 ms \t: "<<((t_real)initial_count_err[i].ten/(t_real)final_count_err[i].tot)*100.0<<'%'<<'\t';
				f_out<<((t_real)final_count_err[i].ten/(t_real)final_count_err[i].tot)*100.0<<'%'<<endl;
				f_out<<"\t\t\t<= 20 ms \t: "<<((t_real)initial_count_err[i].twenty/(t_real)final_count_err[i].tot)*100.0<<'%'<<'\t';
				f_out<<((t_real)final_count_err[i].twenty/(t_real)final_count_err[i].tot)*100.0<<'%'<<endl;
				f_out<<"\t\t\toltre 20 ms \t: "<<((t_real)initial_count_err[i].over/(t_real)final_count_err[i].tot)*100.0<<'%'<<'\t';
				f_out<<((t_real)final_count_err[i].over/(t_real)final_count_err[i].tot)*100.0<<'%'<<endl;
				f_out<<"\t\t\tfonemi errati \t: "<<((t_real)initial_count_err[i].error/(t_real)final_count_err[i].tot)*100.0<<'%'<<'\t';
				f_out<<((t_real)final_count_err[i].error/(t_real)final_count_err[i].tot)*100.0<<'%'<<endl;
				}
			}
		}
	
	f_out.close();
	return;
	}