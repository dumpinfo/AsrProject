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
//  |   FILE:   baumwelc.cpp                                            |
//  |   FUNCTIONALITY: class training implementation                    |
//  |   PROGRAM:														|
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio & Massimo & Fabrizio 					|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98					                |
//  |   FIRST VERSION:						                            |
//  |   DATA FIRST VERSION:			                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// Fabrizio 
// 13/03/1998 eliminated function Dump_Header_Of_File_Of_Spectral_Parameters,
//            inserted function Read_Data_File_Header

#include"../training/baumwelc.h"


void Training::Configure(const String& config_file)
	{
	String buffer;
	t_index position;

	ConfigFile config;
	
	feature.Configure(config_file);
	dbase.Configure(config_file, TRUE);

	config.Open_File(config_file);
	config.Get_String_Opt("SingleUnitReestimation", "modelsfilename", models_file_input);
	config.Get_String_Opt("SingleUnitReestimation", "checklogfname", models_stat_output);
	maxiter_num = config.Get_Unsigned_Opt("SingleUnitReestimation", "maxnumiteration");
	conv_threshold = config.Get_Real_Opt("SingleUnitReestimation", "convergencethreshold");
	reestimate_transition=config.Get_Boolean_Opt("SingleUnitReestimation", "reestimate_transition");
	config.Get_String_Opt("SingleUnitReestimation","SymbolRange",buffer);

	if(buffer=="partial")
		config.Get_Range_Opt("SingleUnitReestimation","SymbolList", symbol_list);
	else{
		t_index i, num_of_symbols;

		num_of_symbols=dbase.Get_Num_Of_Symbols();
		symbol_list.Destroy_And_ReDim(num_of_symbols);
		for(i=0;i<num_of_symbols;i++)
			symbol_list[i]=i;
		}
		
	models_file_input.Is_SubString_Inside(".", position);
	models_file_output.Destroy_And_ReDim(position);
	models_file_output.Take(models_file_input,0,position);
	models_file_output<<"tra";	

	return;
	};


//  HIGHER LEVEL FUNCTION THAT PERFORMS THE TRAINING OVER ALL THE PHONEMES OF
//  THE ACTUAL DBASE BY BAUM-WELCH PROCEDURE ( one phoneme at times )
void Training::Baum_Welch()
	{
	String buffer;
	t_index i, act_phon;
	ofstream trai_statf;
	ifstream init_spcf;

	init_spcf.open(models_file_input,ios::nocreate|ios::in);
	
	if(init_spcf.fail())
		merr<<"Cannot open "<<models_file_input;

	Read_Data_File_Header (init_spcf, vec_size, full_covariance);

	Write_Header_Of_File_Model (models_file_output, dbase.Snd_Type(), 
              dbase.Label_Type(), dbase.Db_File_List_Name(), dbase.Window_Lenght(), 
			  dbase.Window_Overlap(), vec_size, full_covariance);

	trai_statf.open(models_stat_output);

	for(i=0; i<symbol_list.Dim(); i++)
		{
		act_phon=symbol_list[i];
		states_info.phoneme=act_phon;
		states_info.stat_dim=vec_size;
			
		mstat<<"EXECUTING TRAINING OF PHONEME: "<<act_phon;
		trai_statf<<"EXECUTING TRAINING OF PHONEME: "<<act_phon<<endl;
		states_info.Read(init_spcf, full_covariance);

		num_states=states_info.Dim()+2;
		num_mix=states_info[0].Dim();
		Allocate_Mem_For_Parameters();
		ReEstimate_Model(act_phon, trai_statf);

		states_info.Store_Codebook(models_file_output, act_phon);
		dbase.Restart();
		mstat<<"END OF TRAINING OF PHONEME: "<<act_phon;
		}

	trai_statf.close();

	return;
	}


// *  *  *  *  *  FIRST LEVEL FUNCTIONS  *  *  *

// dimensionate and implicitly initialize accumulator vectors and matrix's
void Training::Allocate_Mem_For_Parameters()
	{
	t_index i, j;

	tran.Destroy_And_ReDim(num_states-1,num_states-1);
	c.Destroy_And_ReDim(num_states,num_mix);
	occ_beg.Destroy_And_ReDim(num_states);
	occ.Destroy_And_ReDim(num_states);
	occ_end.Destroy_And_ReDim(num_states);
	mu.Destroy_And_ReDim(num_states-1);   // not first component

	if(full_covariance)
		va.Destroy_And_ReDim(num_states-1);   // not first component
	else diagva.Destroy_And_ReDim(num_states-1);
	for(i=1;i<num_states-1;i++)
		{
		mu[i].Destroy_And_ReDim(num_mix,vec_size);
		if(full_covariance)
			{
			va[i].Destroy_And_ReDim(num_mix);
			for(j=0;j<num_mix;j++)
				va[i][j].Destroy_And_ReDim(vec_size,vec_size);
			}
		else{
			diagva[i].Destroy_And_ReDim(num_mix);
			for(j=0;j<num_mix;j++)
				diagva[i][j].Destroy_And_ReDim(vec_size,vec_size);
			}
		}
	return;
	}


//ReEstimateModel: top level of algorithm
void Training::ReEstimate_Model(const t_index act_phon, ofstream &trai_statf)
	{
	Boolean converged;
	Boolean not_end_of_dbase, is_new_phone;

	t_index iteration;
	t_index istance_counter;   
	t_index num_frames, num_frames_per_ist;
	t_real oldP,newP;
	t_real delta;
	static VetDouble observation_vet;

	VetDoubleList pred_list;
	pred_list.Destroy_And_ReDim(feature.Max_Delta_Feature_Order());

	VetDouble vetsmp;
	// list of cepstral vectors forming the same phoneme istance
	t_mfcvet_list whole_phon_istance; 
					    
	MatrixOfDouble alfa;
	MatrixOfDouble beta;
	MatrixOfDouble b;
	Tri_D_Matrix mix_out_lprob;

	iteration = 1;
	oldP = LOGZERO;

	do	{
		// as the procedure doesn't converge new loop over training procedure
		num_frames = 0;
		num_frames_per_ist = 0;

		istance_counter=0; 
		not_end_of_dbase = (Boolean)TRUE;

		Reset_Parameters();

		while(not_end_of_dbase)
			{
			not_end_of_dbase=dbase.Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(vetsmp,
									act_phon, is_new_phone, pred_list);
			
		    if( (num_frames_per_ist!=0) AND (is_new_phone==TRUE OR not_end_of_dbase==FALSE ) )
			    {
				//  TRAINING STEP
				//   for each phoneme istance
				Assert(num_frames_per_ist == whole_phon_istance.Dim());

				Build_Observation(b, mix_out_lprob, whole_phon_istance, num_frames_per_ist);
				Build_Alfa(alfa, istance_counter, num_frames_per_ist, states_info.trans_mat, b);
				Build_Beta(beta, istance_counter, num_frames_per_ist, states_info.trans_mat, b);
				Update_Parameters(alfa, beta , b, mix_out_lprob, istance_counter,
									num_frames_per_ist, whole_phon_istance);
				
				istance_counter++;
				num_frames += num_frames_per_ist; // compute the global number of read phoneme
				num_frames_per_ist=0;
				whole_phon_istance.Reset();
			    }

		    if ( not_end_of_dbase )
			    {
				if(is_new_phone)
					feature.Get_Previous_Frames_Info(pred_list, dbase.Smp_Rate());
				
				// memorize in phon_istance the next istance of act_phon
				feature.Extract(observation_vet, vetsmp, dbase.Smp_Rate());
				whole_phon_istance.Append(observation_vet);
				num_frames_per_ist++;
				}
			}  // end while

		if (num_frames != 0)
			{
			Update_Model(istance_counter, act_phon);

			newP=Sequences_Total_LProb()/(t_real)istance_counter;
			delta=newP-oldP;
			converged=(Boolean)(fabs(delta)<conv_threshold);
			oldP=newP;
			mstat<<"Average Prob after iteration "<<iteration<<" = "<< oldP;
			iteration++;
			if (iteration > 1)
				mstat<<"variation = "<<delta;

			dbase.Restart();
			}

		} while ((iteration <= maxiter_num) AND !converged AND (num_frames != 0) );

	if(num_frames == 0)
	    {
		trai_statf<<"Neither one istance of phoneme n. "<<act_phon<<endl;
		mstat<<"Neither one istance of phoneme n. "<<act_phon;
	    }
	if (converged)
		{
		trai_statf<<"Estimation converged at iteration "<<iteration<<endl;
		mstat<<"Estimation converged at iteration "<<iteration;
		trai_statf<<"Obs_sequence likelyhood after iteration "<<iteration
							<<" = "<< oldP<<endl<<endl;
		}
	else{
		trai_statf<<"Estimation aborted at iteration "<<iteration<<endl;
		mstat<<"Estimation aborted at iteration "<<iteration;
		trai_statf<<"Obs_sequence likelyhood after iteration "<<iteration
							<<" = "<< oldP<<endl;
		trai_statf<<"variation = "<<delta<<endl<<endl;
	    }
	mstat<<"Final Model";
	
	return;
	};


// *  *  *  *  * SECOND LEVEL FUNCTIONS * * * * * *

void Training::Append_New_Frame_To_PhonIstance(t_mfcvet_list& phon_istance,
						const VetDouble& vetsmp)
	{
	static VetDouble observation_vet;
	feature.Extract(observation_vet, vetsmp, dbase.Smp_Rate());
	phon_istance.Append(observation_vet);

	return ;
	};

//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * //
//                                                                      //
//           OBSERVATIONS SEQUENCE PROBABILITIES MATRICES                //
//                                                                      //
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * //

t_real Training::Sequences_Total_LProb()
	{
	t_real temp;
	t_index i;

	temp=sequences_lprob[0];
	for(i=1;i<sequences_lprob.Dim();i++)
		temp+=sequences_lprob[i];      

	return temp;
	};

void Training::Build_Observation(MatrixOfDouble& b, Tri_D_Matrix& mix_out_lprob,
				const t_mfcvet_list & phon_istance,
				const t_index num_frames)
	{
	t_index time, i;

	b.Destroy_And_ReDim(num_states-1, num_frames);
	mix_out_lprob.Destroy_And_ReDim(num_states-1);
	b.Set(LOGZERO);

	for(i=0; i<num_states-1; i++)
		{
	    mix_out_lprob[i].Destroy_And_ReDim(num_mix, num_frames);
	    mix_out_lprob[i].Set(LOGZERO);
		}
	
	//we put in observation the probability so we must not
	//re-calculate it when building beta

	for(time=0;time<phon_istance.Dim();time++)
		Calculate_b_By_Mixture_Observation(b, mix_out_lprob, phon_istance[time], time);
	
	return;
	};

// compute for each state j the logprobability of observed vector obsvect
// at istant time; and store it in b[j][time]
void Training::Calculate_b_By_Mixture_Observation(MatrixOfDouble & b,
				 Tri_D_Matrix& mix_out_lprob, const VetDouble& obs_vect,
						   const t_index time)
	{
	t_index j, m;
	t_real sigma;
	t_real log_weight_jm;
	t_real log_ob_jm;

	for(j=1;j<num_states-1;j++)
		{
		sigma=LOGZERO;
		//  state=j-th state of [this_phoneme] in GenericStaticHmm;
		// compute probability
		for(m=0;m<num_mix;m++)
			{
			log_weight_jm=states_info[j-1][m].weight;
			log_ob_jm=states_info[j-1][m].Cluster_Distance(obs_vect);
			sigma = LogAdd(sigma,log_weight_jm+log_ob_jm);
			// contribute of the m-th gaussian of state j
			// to probability of observed vector obs_vect at instant time
			mix_out_lprob[j][m][time]=log_ob_jm;
			}
		b[j][time]=sigma;
		Assert(sigma<0);
		}
	return;
	};	

void Training::Build_Alfa(MatrixOfDouble& alfa, const t_index ist_idx,
			      const t_index num_frames, const MatrixOfDouble& a, 
				  const MatrixOfDouble& b)
	{
	t_index i, j, time;
	t_real sigma;

	alfa.Destroy_And_ReDim(num_states-1, num_frames);
	alfa.Set(LOGZERO);
	//start from j=1 because there is not a
	//loop on initial dummy state by its definition
	//build col 1 from entry state
	for(j=1;j<num_states-1;j++)
		{
		if (a[0][j]<LOGSMALL)
		   alfa[j][0]= LOGZERO;
		else
		   alfa[j][0]=a[0][j]+b[j][0];
		}
	// forward pass compute alfa
	// compute columns 1 to T-1 of alfa matrix
	for(time=1;time<num_frames;time++)
		{
		for(j=1;j<num_states-1;j++)
			{
			sigma=LOGZERO;
			for(i=1;i<num_states-1;i++)
				{
				if(a[i][j]>LOGSMALL)
				    sigma=LogAdd(sigma,alfa[i][time-1]+a[i][j]);
				}
			alfa[j][time]=sigma+b[j][time];
			Assert(alfa[j][time]<0);
			}
		}//end alfa calculation

	//compute sequence prob for gamma denominators  (factor 1/Pr)
	//where Pr=sequences_lprob[r] remember Pr is stored in log real
	sigma = LOGZERO;
	for (i=1;i<num_states-1;i++)
	    if (a[i][num_states-1]>LOGSMALL)
			sigma=LogAdd(sigma, alfa[i][num_frames-1]+a[i][num_states-1]);

	sequences_lprob.Save_And_ReDim(ist_idx+1);
	sequences_lprob[ist_idx]=sigma;

	return;
	}

void Training::Build_Beta(MatrixOfDouble& beta, const t_index ist_idx,
			const t_index num_frames, const MatrixOfDouble & a,
			    const MatrixOfDouble & b )
	{
	t_index k, i, j;
	t_signed time;
	t_real sigma;

	beta.Destroy_And_ReDim(num_states, num_frames);
	beta.Set(LOGZERO);
	//initialize beta
	for(i=1;i<num_states-1;i++)
		{
		//Col T-1 from exit state
		beta[i][num_frames-1]=a[i][num_states-1];
		}

	//backward pass: calculate beta
	for(time=(num_frames-2);time>=0;time--)
		{
		// Col t from col (t+1)
		for(k=1;k<num_states-1;k++)
		    beta[k][time]=LOGZERO;
		for(j=1;j<num_states-1;j++)
			{
			sigma=b[j][time+1]+beta[j][time+1];
			for(i=1;i<num_states-1;i++)
			    if(a[i][j]>LOGSMALL)
					beta[i][time]=LogAdd(beta[i][time],sigma+a[i][j]);
			}
		}

	// sequences_lprob[ist_idx] may be computed from beta and then
	// verify that it is equal to the one derived from alpha
	sigma=LOGZERO;
	for(j=1;j<num_states-1;j++)
	    if ( a[0][j]>LOGSMALL )
			sigma=LogAdd(sigma,beta[j][0]+a[0][j]+b[j][0] );

	return;
	}

//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  //
//                                                                       //
//                   ESTIMATION OF PARAMETERS                            //
//                                                                       //
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  //

void Training::Reset_Parameters()
	{
	t_index i,j;

	occ.Set(0.0);
	occ_beg.Set(0.0);
	occ_end.Set(0.0);
	tran.Set(0.0);
	c.Set(0.0);

	for (i=1;i<num_states-1;i++)
		{
		mu[i].Set(0.0);
		if(full_covariance)
			for (j=0;j<num_mix;j++)
				va[i][j].Set(0.0);
		else for (j=0;j<num_mix;j++)
					diagva[i][j].Set(0.0);
		}
	return;
	}

// function that loads in local vectors the values of means and covariances
// of the actual phoneme
void Training::Update_Parameters(const MatrixOfDouble & alfa, const MatrixOfDouble & beta,
				const MatrixOfDouble & b, const Tri_D_Matrix& mix_out_lprob,
				const t_index ist_idx, const t_index num_frames,
					const t_mfcvet_list & phon_istance)
	{
	t_real Pr;

	Pr=sequences_lprob[ist_idx];  
	// given  Pr,alfa,beta return new_a
	Up_Transition_Parameters(Pr,alfa,beta,b,num_frames);
	// given  Pr,alfa,beta return new_par
	Up_Multi_Mixture_Parameters(Pr, alfa, beta, mix_out_lprob,
						num_frames, phon_istance);
	return;
	}

//UpTranParms: implements the transition matrix reestimation
void Training::Up_Transition_Parameters(t_real Pr, const MatrixOfDouble & alfa,
			      const MatrixOfDouble & beta,  const MatrixOfDouble & b,
			      const t_index num_frames)
	{
	t_index i,j,time;
	t_real sigma;

	for (i=1;i<num_states-1;i++)
		{
		// update state occupation counters
		sigma = alfa[i][0]+beta[i][0]-Pr;
		if (sigma>MINEXPARG) //if sigma <= threshold approximate with the tangent
			occ_beg[i] += exp(sigma);
		sigma = alfa[i][num_frames-1]+beta[i][num_frames-1]-Pr;
		if (sigma>MINEXPARG)
			occ_end[i] += exp(sigma);

		sigma=LOGZERO;

		for (time=1;time<num_frames-1;time++)
			sigma=LogAdd(sigma,alfa[i][time]+beta[i][time]);
		sigma -= Pr;
		if (sigma>MINEXPARG)
			occ[i]+=exp(sigma);

		//Update Transition counter
		if(reestimate_transition)
			for (j=1;j<num_states-1;j++)
				{
				if (states_info.trans_mat[i][j]>LOGSMALL)
					{
					sigma=LOGZERO;
					for (time=0;time<num_frames-1;time++)
						sigma=LogAdd(sigma,	alfa[i][time]+states_info.trans_mat[i][j]
						+b[j][time+1]+beta[j][time+1]);
					sigma -= Pr;
					if (sigma>MINEXPARG)
						tran[i][j] += exp(sigma);//Update Transition counter
					}
				}
		}
	return;
	}

void Training::Up_Multi_Mixture_Parameters(t_real Pr, const MatrixOfDouble& alfa,
				   const MatrixOfDouble& beta, const Tri_D_Matrix& mix_out_lprob,
				   const t_index num_frames, const t_mfcvet_list& phon_istance)
	{
	t_index i,j,k,kk,m,t;

	VetDouble  mean;
	VetDouble  ot;

	t_real sigma;
	
	t_real y, zmean1, zmean2;

	for(j=1;j<num_states-1;j++)
		{
		for(m=0;m<num_mix;m++)
			{ 
			//Update mu,sigma,omega counter for mixture m
			// cause mean appears only as a right value we may assign
			mean = states_info[j-1][m].mean;   
			if (states_info[j-1][m].weight>LMINMIX) 
				{
				//USING STRAIGHT LOG QUANTITIES
				for(t=0;t<num_frames;t++)
					{
					ot = phon_istance[t];//the t-th frame of the istance
					if(t==0)
						sigma = states_info.trans_mat[0][j];
					else{
						sigma = LOGZERO;
						for(i=1;i<num_states-1;i++)
							sigma=LogAdd(sigma,alfa[i][t-1]+states_info.trans_mat[i][j]);
						Assert(sigma<0);
						}

					if(sigma>LOGSMALL)     
						{
						sigma += mix_out_lprob[j][m][t]+states_info[j-1][m].weight+beta[j][t]-Pr;

						if(sigma>MINEXPARG)
							{
							y = exp(sigma);
							for(k=0;k<vec_size;k++)
								{
								mu[j][m][k] += ot[k]*y;
								zmean1=ot[k]-mean[k];
								if(full_covariance)
									for(kk=k;kk<vec_size;kk++)
										{
										zmean2 = ot[kk]-mean[kk];
										va[j][m][k][kk] += zmean1*zmean2*y;
										}  // end forkk
								else diagva[j][m][k][k] += zmean1*zmean1*y;
								}  // end fork
							c[j][m] += y;
							} 
						} 
					}  
				} 
			}  // end form
		}
	return;
	}

void Training::Update_Model(const t_index phoneme_instance, t_index act_phon)
	{
    t_index i, j, k, kk, m;
	t_real cfloor, occi, sigma, p;
	Boolean valid, flag=TRUE;

    VetDouble mean;         // auxiliary structure for doing mean correction
    MatrixOfDouble covariance;
	DiagMatrixOfDouble diag_covariance;

    t_index first_state; //to become methods
    t_index last_state;

	char sect[5], mix[5];
	String warning_strg;

    cfloor = LMINMIX;       // using all log probabilities
    first_state=0;
    last_state = num_states-1;

    mean.Destroy_And_ReDim(vec_size);
	if(full_covariance)
		covariance.Destroy_And_ReDim(vec_size, vec_size);
	else diag_covariance.Destroy_And_ReDim(vec_size, vec_size);

    for (i=1;i<num_states-1;i++)
	    {
		occi = occ_beg[i]+occ[i]+occ_end[i];
		if (reestimate_transition)
			{
			p = occ_beg[i] / phoneme_instance;  
			if(p>1 AND p<(1+10E-10)) p=1;    // here and later: rough method
											 // to prevent precision problems
				if(p<MINLOGARG)
					states_info.trans_mat[first_state][i] =LOGZERO;
				else states_info.trans_mat[first_state][i] = log((t_real)p);

			for (j=1;j<num_states-1;j++)
				{
				p = tran[i][j]/occi;
				if(p>1 AND p<(1+10E-10)) p=1;
					if(p<MINLOGARG)
						states_info.trans_mat[i][j] =LOGZERO;
					else states_info.trans_mat[i][j] = log((t_real)p);
				}
			p = occ_end[i]/occi;
			if(p>1 AND p<(1+10E-10)) p=1;
				if(p<MINLOGARG)
					states_info.trans_mat[i][last_state] = LOGZERO;
				else states_info.trans_mat[i][last_state] = log((t_real)p);
			}
		for (m=0;m<num_mix;m++) 
			{
			if(num_mix==1)
				c[i][m] =  occi ;
		
			sigma =  c[i][m]/occi;
			if (sigma>1.0)
				{
				mwarn<<"Section: "<<sect<<"; mixture: "<<mix<<" mix too big!";
				sigma = 1.0;
				}

			if (sigma<cfloor)
				{
 				mwarn<<"Section: "<<sect<<"; mixture: "<<mix<<" mix too small!";
				states_info[i-1][m].weight=LOGZERO; 
				}
			else states_info[i-1][m].weight=log((t_real)sigma);

			if(full_covariance)
				{
				for (k=0;k<vec_size;k++)
					{
					for (kk=k;kk<vec_size;kk++)
						{
						t_real y;
						y = va[i][m][k][kk]/c[i][m];
						covariance[k][kk]=y;
						covariance[kk][k]=y;
						}
					}
			
				states_info[i-1][m].inv_cov = covariance;
				}
			else{
				for(k=0;k<vec_size;k++)
					diag_covariance[k][k] = diagva[i][m][k][k]/c[i][m];
				states_info[i-1][m].diag_inv_cov=diag_covariance;
				}

			for (k=0;k<vec_size;k++)
				mean[k] = mu[i][m][k]/c[i][m];

			states_info[i-1][m].mean=mean;
			valid=states_info[i-1][m].Compute_G_Const();
			if(!valid)
				{
				flag=FALSE;
				mwarn<<"Invalid inverse matrix in state "<<i<<" num_gauss "<<m;
				}
			}// end m
		}  // end i

	if(!flag)
		mwarn<<"Actual phoneme: "<<act_phon;
	return;
	}

// ***************    END ESTIMATION PARAMETERS    ******************

//  *  *  END 2nd LEVEL

//LogAdd: Return sum x + y on log scale, sum < LOGSMALL is floored to LZERO
t_real Training:: LogAdd(t_real x, t_real y)
	{
	t_real temp;

	if(x<y)
		{
		temp = x;
		x = y;
		y = temp;
		}
	temp = y-x;
	if(temp<MINLOGEXP)
		if(x<LOGSMALL)
			return LOGZERO;
		else return x;
	else return x+log((t_real)1.0+ exp(temp));
	}

//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  //
//                                                     //
//                 TRAIN CODEBOOK                      //
//                                                     //
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  //


void TrainCodebook::Store_Codebook(const String& output_fname, const t_index act_phon)
	{
	String f_out;
		
	num_states=(*this).Dim()+2;
	num_gauss=(*this)[0].Dim();
    stat_dim=(*this)[0].Dim();

	phoneme=act_phon;

	f_out=output_fname;

	Write(f_out);

	return;
	};
	

//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * //
//                                                          //
//                    TRAINING CLUSTER                      //
//                                                          //
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * //


t_real TrainCluster::Cluster_Distance(const VetDouble & obs_vet)
	{
	t_real sigma;
	sigma=0;
	
	//spectrum of num_cluster-th state and num_mix-th mixture
	sigma=Evaluate_Exp_Gauss(obs_vet);

	sigma += gConst;
	Assert(sigma<=0);

	return sigma;
	}

