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
//  |   FILE:   tspecbas.cpp                                            |
//  |   FUNCTIONALITY:                                                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico , Fabrizio					            |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 29/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// Fabrizio 
// 13/03/1998 eliminated write of preemphasis
// 13/03/1998 eliminated read  of preemphasis

#include "../tspecmod/tspecbas.h"
#include "../tspecmod/tspecbas.hpp"


void Read_Data_File_Header (ifstream & input_file, t_index & stat_dim, 
                                              Boolean & is_full)
	{
    String str_input;
	t_index value;
	
    //read the string "DBASE_OPERATIVE_FEATURES:"
    input_file >> str_input;
	if (str_input!="DBASE_OPERATIVE_FEATURES:")
		merr<<"Wrong input file of acoustic models"<<endl;

	//read the string "file_type:" and the value
	input_file>>str_input;
	if (str_input!="file_type:")
		merr<<"Wrong input file of acoustic models"<<endl;
    input_file>>str_input;
	
	//read the string "label:" and the value
	input_file>>str_input;
	if (str_input!="label:")
		merr<<"Wrong input file of acoustic models"<<endl;
	input_file>>value;
	
	//read the string "file_list:" and the name of this file 
    input_file>>str_input;
	if (str_input!="file_list:")
		merr<<"Wrong input file of acoustic models"<<endl;
	input_file>>str_input;

	//read the string "window_lenght:" and the value
	input_file>>str_input;
	if (str_input!="window_lenght:")
		merr<<"Wrong input file of acoustic models"<<endl;
	input_file>>value;

	//read the string "window_overlap:" and the value
    input_file>>str_input;
	if (str_input!="window_overlap:")
		merr<<"Wrong input file of acoustic models"<<endl;
	input_file>>value;
 
    //read the string "statistics_dimension:" and 
	//stores tha value in the "stat_dim" parameter
	input_file>>str_input;
	if (str_input!="statistics_dimension:")
		merr<<"Wrong input file of acoustic models"<<endl;
    input_file>>stat_dim;

	//read the string "type_of_matrix:" and stores the value in the "t_mat" parameter 
    input_file>>str_input;
	if (str_input!="type_of_matrix:")
		merr<<"Wrong input file of acoustic models"<<endl;
    input_file>>str_input;
	if (str_input=="full")
		is_full=TRUE;
	else 
	    if (str_input=="diagonal")
			is_full=FALSE;
        else merr<<"Wrong input file of acoustic models"<<endl;
	return;

	}


void Write_Header_Of_File_Model (const String & f_out_name, const String& file_type, 
              const t_index label, const String & f_list, const t_index w_lenght, 
			  const t_index w_overlap, const t_index stat_d, const Boolean is_full)
	{
    ofstream f_output;
	f_output.open(f_out_name);
	if (f_output.fail())
	 merr<<"Cannot open file : "<<f_out_name;

    f_output<<"DBASE_OPERATIVE_FEATURES:"<<endl;

	f_output<<"file_type: "<<file_type<<endl;

	Check(label<=8, "Unknown label type");
    f_output<<"label: "<<label<<endl;

	f_output<<"file_list: "<<f_list<<endl;
	f_output<<"window_lenght: "<<w_lenght<<endl;
	f_output<<"window_overlap: "<<w_overlap<<endl<<endl;
    f_output<<"statistics_dimension: "<<stat_d<<endl<<endl;
    f_output<<"type_of_matrix: ";
	if (is_full)
		 f_output<<"full"<<endl<<endl;
	else f_output<<"diagonal"<<endl<<endl;

    f_output.close();

	return;
	}


//*********************************************************************/
//                                                                     /
//                     Gaussian                                         /
//                                                                     /
//*********************************************************************/


Boolean Gaussian::Compute_G_Const ()
	{
    t_real tpi, log_cov_det, cov_det;
	t_index gauss_dim;
	Boolean res=TRUE;

    gauss_dim = mean.Dim();
	tpi = gauss_dim * log(2.0*PI) /2.0;

	if(full_cov)
		cov_det=inv_cov.Inverse();
	else cov_det=diag_inv_cov.Inverse();

	if(cov_det<=0)
		res=FALSE;

	if (cov_det<REAL_ZERO)
		log_cov_det = LOGZERO;
	else log_cov_det = log((t_real)cov_det);

	gConst = -(tpi+ log_cov_det/2);
	
	return res; 
	}


// Exp_Gauss: computes  and returns the argument of exponential
t_real Gaussian::Evaluate_Exp_Gauss (const VetDouble& obs_vet)const
	{
	VetDouble xmm;   //obs_vet minus mean
	t_real sum;
	t_index i, j;
	t_index vec_size;

	vec_size=mean.Dim();
	xmm = obs_vet;

	//calculate observation vector-mean vector
	xmm -= mean;

	sum = 0.0;
	if(full_cov)
		{
		//since inv_cov is simmetric 
		//we add only the elements of the super diag part ...
		for (i=0;i<vec_size-1;i++)
			for (j=i+1;j<vec_size;j++)
				sum += xmm[i]*inv_cov[i][j]*xmm[j];
		
		sum *= 2;
		// ... and now add main diagonal elements
		for (i=0;i<vec_size;i++)
			sum += xmm[i]*xmm[i]*inv_cov[i][i];
		}
	else for(i=0;i<vec_size;i++)
			 sum += xmm[i]*xmm[i]*diag_inv_cov[i][i];
	
	return -0.5*sum;
	}


void Gaussian::Read (ifstream & input_f, const t_index stat_dim)
	{
    String str_input;
	t_index i, j;

	input_f.precision(OUTPUT_SIZE);
    
	//read the weight of the gaussian
	input_f>>str_input;
	if (str_input!="Weight:")
		merr<<"Wrong input file of acoustic models";

	input_f>>weight;

    //read the mean value vector of the gaussian
	input_f>>str_input;
	if (str_input!="Mean_vector:")
		merr<<"Wrong input file of acoustic models";

    mean.Destroy_And_ReDim(stat_dim);

	for (i=0; i<stat_dim; i++)
		input_f>>mean[i];
	   
	//read the inverse covariance matrix
	input_f>>str_input;
	if (str_input!="Inverse_covariance_matrix:")
		merr<<"Wrong input file of acoustic model";

    if (full_cov)
		{
		inv_cov.Destroy_And_ReDim(stat_dim, stat_dim);
		for (i=0; i<stat_dim; i++)
			for (j=0; j<stat_dim; j++)
				input_f>>inv_cov[i][j];
		}
	else{
		diag_inv_cov.Destroy_And_ReDim(stat_dim, stat_dim);
		for (i=0; i<stat_dim; i++)
	    input_f>>diag_inv_cov[i][i];
		}

	//read gConst value of gaussian
	input_f>>str_input;
	if (str_input!="gConst:")
		merr<<"Wrong input file of acoustic models";

	input_f>>gConst;

	return;
	}
	  

void Gaussian::Write (ofstream& f_output)
	{
    t_index i, j, dim=mean.Dim();

	f_output.precision(OUTPUT_SIZE);
	f_output<<"Weight: "<<weight<<endl;
    f_output<<"Mean_vector:"<<endl;
	for (i=0; i<dim; i++)
		f_output<<mean[i]<<'\t';

   	f_output<<endl<<"Inverse_covariance_matrix:"<<endl;
    if(full_cov)
		for (i=0; i<dim; i++)
			{
			for (j=0; j<dim; j++)
				f_output<<inv_cov[i][j]<<'\t';
			f_output<<endl;
			}
	else //if is_diagonal
		{
		for (i=0; i<dim; i++)
			f_output<<diag_inv_cov[i][i]<<'\t';
		f_output<<endl;
		}

	f_output<<"gConst: "<<gConst<<endl<<endl;

    return;
	}

