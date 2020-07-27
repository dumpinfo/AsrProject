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
//  |   FILE:   initiali.cpp                                            |
//  |   FUNCTIONALITY: class for initialization                         |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio & Fabrizio Domenico                      |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98  								|
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 18/6/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// 24/04/98 added unix compatibility changed some names by fabrizio
#include"initiali.h"


// Fabrizio 
// 29/04/98 added in Configure read of max num frames for section
//          from models configuration file

// 11/05/98 Domenico  & Fabrizio 
//          added uniform sectioning

// ******************************************************************
// *																*
// *							K_MEANS								*
// *																*
// ******************************************************************


Boolean Initiali::Configure(const IniOptions& options, const String& config_fname,
							t_index dim_symb_table)
	{
	String symbol_models_fname;
	t_index num_symbols, i;
	
	String mix_opt, sect_opt, mix_opt_row, sect_opt_row;
	String frame_opt_row, frame_opt;
			
	ConfigFile conf;

	// configure feature extraction
	feature.Configure(config_fname);

	stat_dim = feature.Feature_Vet_Dim();
	
	symbol_models_fname = options.FileOfSymbolModels;

	conf.Open_File(symbol_models_fname);
   	init_symb_list = options.SymbolList;
	num_symbols = init_symb_list.Dim();

   	num_mix_per_symbol.Destroy_And_ReDim(num_symbols);
	num_sections_per_symbol.Destroy_And_ReDim(num_symbols);
	maxfra_sect.Destroy_And_ReDim(dim_symb_table);
	
	// read models of symbols to initialize
	for(i=0;i<num_symbols;i++)
		{
		mix_opt_row="mix_order_symb_";
		sect_opt_row="num_sections_symb_";
		frame_opt_row="max_frame_section_symb_";

		sect_opt=sect_opt_row;
		sect_opt<<init_symb_list[i];
		mix_opt=mix_opt_row;
		mix_opt<<init_symb_list[i];
		frame_opt=frame_opt_row;
		frame_opt<<init_symb_list[i];
				
		num_sections_per_symbol[i]=conf.Get_Unsigned_Opt("SymbolModels", sect_opt);
		num_mix_per_symbol[i]=conf.Get_Unsigned_Opt("SymbolModels", mix_opt);
		maxfra_sect[init_symb_list[i]]=conf.Get_Unsigned_Opt("SymbolModels", frame_opt);

 		}

	// allocate adequate quantity of memory in order to avoid 
	// groviera-like effect on memory.
	phons_limits.Set_Data_Chunk(100UL);
	phons_limits.Destroy_And_ReDim(2000UL);
	phons_limits.Reset();

	return (Boolean)TRUE;
	}


// HIGHER LEVEL FUNCTION:  for each phonema of the vocal dbase:
//			   1) for each phonema istance computes section limits;
//			      	computes a codebook consisting of one gaussian for
//					each section;
//			   2) splits the cluster with higher distortion iteratively
//			         till the desired num of gaussians for section is reached
//			   3) stores the codebooks in a only file

void Initiali::Symb_Model_Calculation(const String& out_fname, const String& in_fname,
						 DbaseVoc& dbase, Boolean full_cov, Boolean load_one_mixture,
						 Boolean unif_sect, const ModelType model_type)
	{
	t_index num_mix;
	t_index num_frames;
	t_index phons_card, act_phon, i;
	ifstream one_gauss;

	if(load_one_mixture)
		{
		one_gauss.open(in_fname,ios::in|ios::nocreate);
		if(one_gauss.fail())
				merr<<"Cannot open "<<in_fname;
		Read_Data_File_Header (one_gauss, stat_dim, full_cov);
		}

	Write_Header_Of_File_Model (out_fname, dbase.Snd_Type(), 
              dbase.Label_Type(), dbase.Db_File_List_Name(), dbase.Window_Lenght(), 
			  dbase.Window_Overlap(), stat_dim, full_cov);


	phons_card=dbase.Get_Num_Of_Symbols();
	
	for(i=0; i<init_symb_list.Dim();i++)
		{
		act_phon = init_symb_list[i];
		mstat<<"act_phon: "<<act_phon;  // monitoring elaboration
		states_info.Initialize(num_sections_per_symbol[i], num_mix_per_symbol[i], 
				                   stat_dim, full_cov);
		Calculate_One_Mixture_Codebook(act_phon, num_frames, dbase, unif_sect);
		num_mix=1;
		if(load_one_mixture)
			{
			states_info.phoneme=act_phon;
			states_info.stat_dim=stat_dim;
			states_info.Read(one_gauss, full_cov);
			num_mix=states_info.num_gauss;
			}
		
		dbase.Restart();
		states_info.Compute_Whole_Codebook_Clusters_Distortions();
		while ( num_mix < num_mix_per_symbol[i] AND (num_frames!=0) )
			{
			mstat<<"Splitting of codebook number: "<<num_mix;
			Split_Higher_Distortion_Cluster_Of_Each_Section(act_phon, num_frames, num_mix, dbase);
			states_info.Compute_Whole_Codebook_Clusters_Distortions();
			dbase.Restart();
			}
		states_info.Compute_Cluster_Weights();
		states_info.Store_Codebook(out_fname, act_phon, load_one_mixture, model_type);
		mstat<<"END OF PROCESSING FOR PHONEME :"<<act_phon;
		}
	return;
	};

// ***********************************************************************
// 
//  CALCULATE SECTION LIMITS FOR EACH ISTANCE OF ACT-PHON AND ACCUMULATE
// 
// 			 SECTION MEAN AND COVARIANCE OVER ALL PHONEME ISTANCES
//
// ***********************************************************************
void Initiali::Calculate_One_Mixture_Codebook( t_index act_phon, t_index & num_frames,
												DbaseVoc& dbase, Boolean unif_sect)
	{
	t_index k, num_istances;
	VetDouble vet_features, vetsmp;
	VetDoubleList pred_list;
	VetLong vet_limit;

	//accumulate frames
	ImpObjectList<VetDouble> whole_fon_istance;
	
	Boolean not_end_of_dbase,is_new_phone;
	
	//ofstream f_out;
	String file_out_name;
	num_istances=0;
	num_frames=0;

	pred_list.Destroy_And_ReDim(feature.Max_Delta_Feature_Order());

	not_end_of_dbase=(Boolean)TRUE;

	phons_limits.Reset();

	while(not_end_of_dbase)
		{
		not_end_of_dbase = dbase.Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(vetsmp,
									act_phon, is_new_phone, pred_list);

		if(num_frames!=0 AND (is_new_phone==TRUE OR  !(not_end_of_dbase)))
			{
			Sectionate(whole_fon_istance,vet_limit, maxfra_sect[act_phon], unif_sect);
			phons_limits.Append(vet_limit);
			states_info.Accumulate(whole_fon_istance,vet_limit);
			whole_fon_istance.Reset();
			num_istances++;
			}

		if(not_end_of_dbase==TRUE)
			{
			num_frames++;
			if(is_new_phone)
				feature.Get_Previous_Frames_Info(pred_list, dbase.Smp_Rate());
  			feature.Extract(vet_features, vetsmp, dbase.Smp_Rate());
			whole_fon_istance.Append(vet_features);
			}
		}//end while

	for(k=0;k<states_info.Dim();k++)
		{
		t_index cluster=0;   // here only one cluster per section
		states_info[k][cluster].Do_Averages();
		}

	return;
	};

void Initiali::Sectionate(const ImpObjectList<VetDouble> & phon_ist,
				 VetLong & frame_limits, t_index maxfra_sect, Boolean unif_sect)
	{
	//these are constant of the routine ridefiniable
	t_index minfra_sect=1;  // don't modify this parameter otherwise
							// problems in sectioning when few frames
							// in each phon istance
	const t_real DB_MAX = 1.0E+38;
	
	t_index i, j, k, t;
	t_signed n;
		
	//index's for calculating section's limits
	t_signed imin, imax, imin1, imax1, imin2, imax2, ibest, temp;
	t_index num_frames, frames_dim, num_sect;
	t_index assigned_frames;
	
	t_real dist, dist_min;   // aux var for distortion
	t_real square_norm, sum_square_norm;
	t_real aux; // auxiliar variable for evaluating series

	MatrixOfDouble distortion;   // distortion matrix
	MatrixOfDouble back_p;     // back pointer
	MatrixOfDouble dist_acc;  //accumulator for distortion matrix elements
	VetDouble sum_vect;	       // sum of the MFC samples for each frame

	num_sect=states_info.Dim();
	frame_limits.Destroy_And_ReDim(num_sect+1);

	num_frames=phon_ist.Dim();
	frames_dim=phon_ist[0].Dim(); // dim of the generic MFC vector

	// explicit sectioning when few frames
	if(num_frames<=num_sect)
		{
		if(num_frames==1)
			{
			// assign first frame to first section
			frame_limits.Set(1);
 			frame_limits[0]	= 0;
			}
		else{
			for(i=0;i<num_frames;i++)
				frame_limits[i]=i;
			for(i=num_frames;i<num_sect;i++)
				frame_limits[i]=num_frames-1;
			frame_limits[num_sect]	= num_frames;
			}

		return;
		}

    // verify elaboration parameters
	if(minfra_sect*num_sect>=num_frames)
		minfra_sect=num_frames/num_sect;

	if(maxfra_sect*num_sect<num_frames)
		maxfra_sect=(t_index)ceil((t_real)num_frames/num_sect);

    if (unif_sect)
		{
		frame_limits[0]=0;
		assigned_frames=0;
		for (i=1; i<=num_sect; i++)
			{
			frame_limits[i]=(num_frames-assigned_frames)/(num_sect-i+1)+assigned_frames;
			assigned_frames=frame_limits[i];
			}

		return;
		}
	
	distortion.Destroy_And_ReDim(num_frames,num_frames);
	back_p.Destroy_And_ReDim(num_sect+1,num_frames);
	dist_acc.Destroy_And_ReDim(num_sect+1,num_frames);
	sum_vect.Destroy_And_ReDim(frames_dim);

	//calculate distortion matrix
	distortion.Set(0.0);
	// for every element of its super diagonal triangular matrix
	
	for(i=0;i<num_frames-1;i++)
		{
		for(j=i+1;j<num_frames;j++)
			{
		    sum_vect.Set(0.0);
		    square_norm=0.0;
		    for(k=i;k<=j;k++)
				for(t=0;t<frames_dim;t++)
					{
					aux=phon_ist[k][t];   // t-th MFC sample of the k-th frame
					square_norm+=aux*aux; // accumulate energies
					sum_vect[t]+=aux; // accumulate MFC samples at t
					}
			sum_square_norm=0.0;
		    for(t=0;t<frames_dim;t++)
				{
				aux=sum_vect[t];
				sum_square_norm+=aux*aux;
				}
			aux=(t_real)(j-i+1);
		    distortion[i][j]=square_norm-sum_square_norm/aux;
			}
		}
	// calculate segment limits
	
	dist_acc[0]=distortion[0];
	back_p[0].Set(0);
	for(n=1;n<(t_signed)num_sect;n++)
		{
		for(j=1;j<num_frames;j++)
			{
			imin1=(n)*minfra_sect;
			imax1=(n)*maxfra_sect;
			imin2=j-maxfra_sect;
			imax2=j-minfra_sect;

			if(imin1>=imin2) imin=imin1;
			else imin=imin2;

			if(imax1<=imax2) imax=imax1;
			else imax=imax2;

			if ( (imin>=0) AND (imax>=imin) AND (imax<(t_signed)j) )
				{
				dist_min=DB_MAX;
				for(i=imin;i<=(t_index)imax;i++)
					{
					dist=dist_acc[(n-1)][i]+distortion[i][j];
					if(dist<dist_min)
						{
						dist_min=dist;
						ibest=i;
						}	
					}
				dist_acc[n][j]=dist_min;
				back_p[n][j]=ibest;

				}
			else{
				aux=(t_real)1.0E+10;
				dist_acc[n][j]=DB_MAX/aux;
				back_p[n][j]=0;
				}
			}
		}

	frame_limits[0]=0;
	frame_limits[num_sect]=num_frames;
	frame_limits[num_sect-1]=(t_index)back_p[num_sect-1][num_frames-1];

	for(n=(num_sect-2);n>=1;n--)
		{
		temp=frame_limits[n+1];
		frame_limits[n]=(t_index)back_p[n][temp];
		}

	return;
	};

// Routine that splits for each section the cluster with the higher distortion

void Initiali::Split_Higher_Distortion_Cluster_Of_Each_Section(const t_index act_phon,
						const t_index num_frames,
						   t_index& num_mix, DbaseVoc& dbase)
// num_frames= global number of frames act phon in the dbase
// num_mix= number of splitting cycles performed
	{
	ImpSimpleTypeList<t_index> clusters_to_split;
    VetDouble vetsmp;
    VetDouble vet_features;
    t_index cluster;

    Boolean not_end_of_dbase, is_new_phone;  // usefull for coherent dbase-reading control
    t_index frame_counter;      	// dbase global number of frames counter

    t_index k, p, t;
    t_index its_clus;
    t_index kid_clus_idx;

    // the algorithm is based on the intuitively verified modelling hypotesis
    // that splitting  always produces clusters with population >> 1

    IndexOfClustersToSplit(clusters_to_split);

	VetDoubleList pred_list;
	pred_list.Destroy_And_ReDim(feature.Max_Delta_Feature_Order());
	
	// loop for adding a new cluster to codebook structure for each section k
    for (k=0;k<states_info.Dim();k++)
		{
    	states_info.Add_One_Cluster_To_Section(k, stat_dim);
		kid_clus_idx = states_info[k].Dim()-1;  // kid cluster is
												// the one just added
		states_info.Calculate_New_Centroids(k, clusters_to_split[k], kid_clus_idx);
		// reset all clusters statistics in order to use mean and
		//  covariance as accumulators in the following passages
		states_info.Reset_Clusters_Statistics(k);
	    }

    frame_counter=0;
    not_end_of_dbase = (Boolean)TRUE;
    vet_features.Destroy_And_ReDim(stat_dim);

    // loop for splitting cluster (cluster_to_split[k]) of codebook structure
    // all the frames are read and assigned to the nearer centroid

    // asserts control the correctness of frame reading stream
	for (p=0;p<phons_limits.Dim();p++)  //for each phonistance
		for (k=0;k<states_info.Dim();k++)  //for each section
			for (t=phons_limits[p][k];t<(t_index)phons_limits[p][k+1];t++)// for each frame of the section
				{
				Assert(not_end_of_dbase == TRUE);
				not_end_of_dbase=dbase.Get_Filtered_Sequential_VetSmp_And_Its_Predecessors(vetsmp,
									act_phon, is_new_phone, pred_list);
				if(is_new_phone)
					feature.Get_Previous_Frames_Info(pred_list, dbase.Smp_Rate());
	  			
				feature.Extract(vet_features, vetsmp, dbase.Smp_Rate());
				frame_counter++; // one more frame read
				Assert(frame_counter<=num_frames);
				// verify if the frame read belongs to the cluster to split(
				// higher distortion cluster
				its_clus=Assigne_Actual_Frame_To_Its_Cluster(vet_features, num_mix, k);
				// update statistics
				states_info.Up_Normalized_Sum_SqrSum_And_Pop(vet_features,
					states_info[k][its_clus].mean, states_info[k][its_clus].inv_cov,
					states_info[k][its_clus].diag_inv_cov, states_info[k][its_clus].population);
				}   // end for t
		// end for k
	// end for p

	Assert(dbase.Get_Filtered_Sequential_Vet(vetsmp, act_phon, is_new_phone)
								==FALSE);
	for(k=0;k<states_info.Dim();k++)
		for(cluster=0;cluster<=num_mix;cluster++)
			states_info[k][cluster].Do_Averages();
    num_mix++; //number_of_splitting passed as a reference parameter
    return;
	};

void Initiali::IndexOfClustersToSplit(ImpSimpleTypeList<t_index>& hi_dist_clus)
	{
	t_index k, clus;
	t_real hi_dist;
	
	hi_dist_clus.Destroy_And_ReDim(states_info.Dim());
	hi_dist_clus.Set(0L);

	for(k=0;k<states_info.Dim();k++)
		{
		hi_dist=0;
		for(clus=0;clus<states_info[k].Dim();clus++)
		    if(states_info[k][clus].distortion>hi_dist)
			    {
				hi_dist_clus[k] = clus;
				hi_dist=states_info[k][clus].distortion;
				} // endif
			// end for clus
		} // end for k
	return;
	}

t_index Initiali::Assigne_Actual_Frame_To_Its_Cluster(const VetDouble& mfcvet,
						      const t_index num_mix, const t_index section)
	{
	t_index mindist_clus, clus;  // index of the nearer cluster for the actual frame
	t_real mindist, dist; // distances of a mfcvet from clusters centroid

	mindist_clus=0;
	mindist=Distance_From_Centroid(mfcvet, section, mindist_clus);

	for(clus=1;clus<=num_mix;clus++)
		{
		dist = Distance_From_Centroid(mfcvet, section, clus);
		if( dist < mindist )
			{
			mindist=dist;
			mindist_clus = clus;
			}	
		}

	return mindist_clus;
	}

t_real Initiali::Distance_From_Centroid(const VetDouble& mfcvet,
								const t_index section, const t_index cluster)
	{
	t_index i;
	t_real sqr_dist_acc, diff;

	diff=0;
	sqr_dist_acc=0;

	for(i=0;i<stat_dim;i++)
		{
		diff= mfcvet[i]-states_info[section][cluster].centroid[i];
		sqr_dist_acc += diff * diff;
		}
	return  sqr_dist_acc;
	};


// **********************************************************************
// *									
// *			InitCodebook    		
// *									
// **********************************************************************

void InitCodebook::Accumulate(const ImpObjectList<VetDouble> & phon_istance,
		const VetLong & frame_limit)
	{
	t_index k,l,pop_acc;
	VetDouble mean_acc;
	MatrixOfDouble covar_acc;
	DiagMatrixOfDouble diag_covar_acc;


	if(full_covariance)
		covar_acc.Destroy_And_ReDim((*this)[0][0].mean.Dim(),
									(*this)[0][0].mean.Dim());
	else diag_covar_acc.Destroy_And_ReDim((*this)[0][0].mean.Dim(),
									(*this)[0][0].mean.Dim());

	mean_acc.Destroy_And_ReDim((*this)[0][0].mean.Dim());

	// accumulate MFCvect sum and MFCvect external product sum
	for(k=0;k<(*this).Dim();k++)
		{
	    pop_acc=0;
	    mean_acc.Set(0);
	    covar_acc.Set(0);
		diag_covar_acc.Set(0);

	    // * * PARTIAL (INTRA ISTANCE) ACCUMULATION OF PARAMETERS * *
	    for(l=frame_limit[k];l<(t_index)frame_limit[k+1];l++)
			Up_Normalized_Sum_SqrSum_And_Pop(phon_istance[l], mean_acc, 
											 covar_acc, diag_covar_acc,  pop_acc);

	    // * * ACCUMULATION OF PARAMETERS OVER ALL INSPECTED ISTANCES * * * //
		// at first step only one mixture for section
		(*this)[k][0].mean+=mean_acc;

		if(full_covariance)
			 (*this)[k][0].inv_cov+=covar_acc;
		else (*this)[k][0].diag_inv_cov+=diag_covar_acc;

		(*this)[k][0].population+=(frame_limit[k+1]-frame_limit[k]);
		}
		
	return;
	}

void InitCodebook::Initialize(t_index num_sect, const t_index num_mix, 
							  t_index statistics_dim, Boolean full_covar)
	{
	t_index i;
	(*this).Set_Data_Chunk(0);
	(*this).Destroy_And_ReDim(num_sect);
	full_covariance=full_covar;
	for(i=0;i<num_sect;i++)
		{
		// dimensionate memory area
		(*this)[i].Set_Data_Chunk(num_mix-1);; 
		(*this)[i].Destroy_And_ReDim(1); //at the beginning only one
										 //mixture
		(*this)[i][0].mean.Destroy_And_ReDim(statistics_dim);
		
		if(full_covar)
			(*this)[i][0].inv_cov.Destroy_And_ReDim(statistics_dim,
							 statistics_dim);
		else (*this)[i][0].diag_inv_cov.Destroy_And_ReDim(statistics_dim, 
							 statistics_dim);

		(*this)[i][0].population=0;
		(*this)[i][0].centroid.Destroy_And_ReDim(statistics_dim);
		(*this)[i][0].distortion=0.0;
		(*this)[i][0].weight=0.0;
		(*this)[i][0].gConst=0.0;
		(*this)[i][0].full_cov=full_covar;
		}
	return;
	};

void InitCodebook::Up_Normalized_Sum_SqrSum_And_Pop	(const VetDouble& act_frame, 
	VetDouble& sum,	MatrixOfDouble&  square_sum, DiagMatrixOfDouble &diag_square_sum, t_index & cardinality)
	{
	t_index i,j;
	t_index stat_dim;
	stat_dim=act_frame.Dim();
	// use mean_acc & cov_acc as local(istance) accumulators
	sum+=act_frame;

	// covariance matrix is simmetrical so evaluate separately
	// its diagonal and the 2 symmetrical super and sub diagonal
	// parts.  Here accumulate external products in order
	// to implement(later): cov=(1/N^2)[sigma(externalMFCvect)-
	// N*sigma(externa(meanMFCvect))]

	if(full_covariance)
		{
		// for diagonal
		for(i=0;i<stat_dim;i++)
		square_sum[i][i] +=  act_frame[i]*act_frame[i];
		// for triangular parts
		for(i=0;i<stat_dim-1;i++)
			for(j=i+1;j<stat_dim;j++)
				square_sum[j][i]=square_sum[i][j] += act_frame[i]*act_frame[j];
		}
	else for(i=0;i<stat_dim;i++)
			 diag_square_sum[i][i] +=  act_frame[i]*act_frame[i];

	cardinality++;
	return;
	}


void InitCodebook::Add_One_Cluster_To_Section(const t_index section,
						const t_index stat_dim)
	{
	t_index old_dim;

	old_dim = (*this)[section].Dim();
	(*this)[section].Save_And_ReDim(old_dim+1);
	// allocate memory and implicitly initialize
	(*this)[section][old_dim].full_cov=full_covariance;
	(*this)[section][old_dim].mean.Destroy_And_ReDim(stat_dim);
	if(full_covariance)
		(*this)[section][old_dim].inv_cov.Destroy_And_ReDim(stat_dim,stat_dim);
	else (*this)[section][old_dim].diag_inv_cov.Destroy_And_ReDim(stat_dim, stat_dim);
	
	(*this)[section][old_dim].population=0;

	(*this)[section][old_dim].centroid.Destroy_And_ReDim(stat_dim);
	(*this)[section][old_dim].distortion=0.0;

	return;
	}

void InitCodebook::Compute_Whole_Codebook_Clusters_Distortions()
	{
	t_index k, i;

	for(k=0;k<(*this).Dim();k++)
	    for(i=0;i<(*this)[k].Dim();i++)
			(*this)[k][i].Compute_Cluster_Distortion();
	return;
	}

void InitCodebook::Calculate_New_Centroids(const t_index section,
				    const t_index father_cluster, const t_index son_cluster)
	{
	t_index i, clus;
    t_index p_max; // pointer to higher eigenvalue
    t_index stat_dim;  // statistics dimention
    MatrixOfDouble eigenvectors;
	DiagMatrixOfDouble diag_eigenvectors;
    VetDouble eigenvalues; // displacement from old centroid

    t_real displ_module;   // expression teorically evaluated
				    //minimizing a distortion measure
    VetDouble displ_vect;

    for(clus=0;clus<(*this)[section].Dim();clus++)
		{
		if( (clus != father_cluster) AND (clus != son_cluster) )
			// assigne to centroid its real value
			(*this)[section][clus].centroid = (*this)[section][clus].mean;
		else{
			// compute eigenvalues and NORMALIZED eigenvectors
			if(full_covariance)
				(*this)[section][father_cluster].inv_cov.
					EigenValues_And_EigenVectors(eigenvalues, eigenvectors);
			else (*this)[section][father_cluster].diag_inv_cov.
				   EigenValues_And_EigenVectors(eigenvalues, diag_eigenvectors);

			stat_dim = (*this)[section][father_cluster].mean.Dim();

			p_max=0;
			for(i=0;i<stat_dim;i++)
				{
				if (eigenvalues[i]>eigenvalues[p_max])
			    p_max=i;
				}

			// compute displacement module
			displ_module = sqrt(2*eigenvalues[p_max]/PI);

			// compute displacement vector
			if(full_covariance)
				displ_vect = eigenvectors[p_max];
			else{
				displ_vect.Destroy_And_ReDim(stat_dim);
				displ_vect[p_max]=1.0;
				}

			displ_vect *= displ_module;

			// take new centroids in the direction of the eigenvector
			// of maximum eigenvalue
			// new centroids = mean value for father cluster +/- displ_vect

			(*this)[section][son_cluster].centroid=
									(*this)[section][father_cluster].mean;
			(*this)[section][son_cluster].centroid += displ_vect;
			(*this)[section][father_cluster].centroid=
									(*this)[section][father_cluster].mean;
			(*this)[section][father_cluster].centroid-= displ_vect;
			}
		}
	return;
	}

void InitCodebook::Reset_Clusters_Statistics(t_index section)
	{
	t_index i;
	for(i=0;i<(*this)[section].Dim();i++)
		{
		(*this)[section][i].mean.Set(0);
		(*this)[section][i].inv_cov.Set(0);
		(*this)[section][i].diag_inv_cov.Set(0);
		(*this)[section][i].population=0;
		(*this)[section][i].weight=0.0;
		}
	return;
	}

void InitCodebook::Compute_Cluster_Weights()
	{
	t_index k, clus;
	t_index sect_popul;
	t_real log_arg;

	for(k=0;k<(*this).Dim();k++)
		{
		sect_popul=0;
		for(clus=0;clus<(*this)[k].Dim();clus++)
			{
		    sect_popul += (*this)[k][clus].population;
		    (*this)[k][clus].weight = (*this)[k][clus].population;
			}
		for(clus=0;clus<(*this)[k].Dim();clus++)
			{
			log_arg=(*this)[k][clus].weight/(t_real)sect_popul;
			if(log_arg<MINLOGARG)
				(*this)[k][clus].weight=LOGZERO;
			else (*this)[k][clus].weight=log(log_arg);
			}
		}
	return;
	}


void InitCodebook::Store_Codebook(const String& output_fname, 
						const t_index act_phon, Boolean load_mixture, 
						const ModelType model_type)
	{
	String f_out;
	t_real log_prob;
	t_index i;

	num_states=(*this).Dim()+2;
	num_gauss=(*this)[0].Dim();
    stat_dim=(*this)[0].Dim();

	phoneme=act_phon;

	if(!Compute_G_Const())
		mwarn<<"Actual phoneme: "<<act_phon;
	
	if(!load_mixture)
		{
		// state transition logprobability matrix
		trans_mat.Destroy_And_ReDim(num_states, num_states); 
		// the first and the last
		// states are dummy states
		trans_mat.Set(LOGZERO);

		switch (model_type)
			{
			case 0:	//model_type==Model_0
					trans_mat[0][1]=0;
					log_prob=log(0.5);

					for(i=1;i<num_states-1;i++)
						trans_mat[i][i]=trans_mat[i][i+1]=log_prob;
					break;

		   case 1: //model_type==Model_1
		           trans_mat[0][1]=0;
				   log_prob=log(0.5);
		           trans_mat[1][1]=trans_mat[1][2]=trans_mat[1][num_states-2]=log(1.0/3.0);
				   for (i=2; i<num_states-1; i++)
					   trans_mat[i][i+1]=trans_mat[i][i]=log_prob;
				   break;

		   case 2: //model_type==Model_2
		           trans_mat[0][1]=trans_mat[0][2]=log(0.5);
				   trans_mat[num_states-2][num_states-2]=trans_mat[num_states-2][num_states-1]=log(0.5);
				   log_prob=log(1.0/3.0);
				   for (i=1; i<num_states-2; i++)
					   trans_mat[i][i]=trans_mat[i][i+1]=trans_mat[i][i+2]=log_prob;
				   break;

		   case 3: //model_type==Model_3
		           trans_mat[0][1]=trans_mat[0][2]=trans_mat[0][3]=log(1.0/3.0);
				   log_prob=log(1.0/4.0);
				   for (i=1; i<num_states-3; i++)
				     trans_mat[i][i]=trans_mat[i][i+1]=trans_mat[i][i+2]=trans_mat[i][i+3]=log_prob;
				   trans_mat[num_states-3][num_states-3]=trans_mat[num_states-3][num_states-2]=trans_mat[num_states-3][num_states-1]=log(1.0/3.0);
                   trans_mat[num_states-2][num_states-2]=trans_mat[num_states-2][num_states-1]=log(0.5);
                   break;
			};       
	}

	f_out=output_fname;
	Write(f_out);
	return;
	};

// ***********************************************************
//                     INITCLUSTER                           *
// ***********************************************************

void InitCluster::Do_Averages()
	{
	t_index i, j;// clus_idx;
	t_index stat_dim;
	//double square_popul;
	MatrixOfDouble square_mean ;
	DiagMatrixOfDouble diag_square_mean ;

	stat_dim=mean.Dim();

	// * * * COMPLETE PARAMETER EVALUATION * * * //
								// cluster of section k
	if(population>0)   // avoids zero divition
		{
		// utilize mean & cov accumulators to explicitly calculate..
		// for covariance
		// square of population
		// external product mean vector
		// main diagonal
		if(full_cov)
		{
		square_mean.Destroy_And_ReDim(stat_dim,stat_dim);
		for(i=0;i<stat_dim;i++)
			square_mean[i][i] = mean[i] * mean[i];
	    // triangular parts together
		for(i=0;i<stat_dim-1;i++)
			for(j=i+1;j<stat_dim;j++)
				square_mean[j][i]=square_mean[i][j] = mean[i] * mean[j];
		square_mean /= (t_real)population;
		inv_cov -= square_mean;
		inv_cov /= (t_real)population;
		}
		else{
			diag_square_mean.Destroy_And_ReDim(stat_dim, stat_dim);
			for(i=0;i<stat_dim;i++)
				diag_square_mean[i][i] = mean[i] * mean[i];
			diag_square_mean /= (t_real)population;
			diag_inv_cov -= diag_square_mean;
			diag_inv_cov /= (t_real)population;
			}

		// for the mean
		mean /= (t_real)population;

		} // endif
	return;
	}

void InitCluster::Compute_Cluster_Distortion()
	{
	t_index i, stat_dim, p_max;

	MatrixOfDouble eigenvectors;    
	DiagMatrixOfDouble diag_eigenvectors;
	VetDouble eigenvalues;

	if(full_cov)
		inv_cov.EigenValues_And_EigenVectors(eigenvalues, eigenvectors);
	else diag_inv_cov.EigenValues_And_EigenVectors(eigenvalues, diag_eigenvectors);

	stat_dim=mean.Dim();
	// p_max points to higher eigenvalue
	p_max=0;
	for(i=0;i<stat_dim;i++)
		{
		if (eigenvalues[i]>eigenvalues[p_max])
		    p_max=i;
		}

	distortion=eigenvalues[p_max];

	return;
	}
