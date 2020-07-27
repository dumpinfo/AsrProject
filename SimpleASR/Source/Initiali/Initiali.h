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
//  |   FILE:   initiali.h                                              |
//  |   FUNCTIONALITY: class for initialization                         |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio & Fabrizio 	Domenico                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98									|
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 18/6/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// class K_MEANS
// vers. 18/06/1995

// k-means initialization procedure for a phoneme graph structure

// Definitively (hope so) modified initialization algorithm with reassigning of
// all frames to nearer cluster after splitting higher distortion cluster

// Fabrizio 
// 13/03/98 erased function Dump_Header_Of_File_Of_Spectral_Parameters

//Domenico 
//06/05/98 uniform sectionate

#ifndef _INITIALISATION_
#define _INITIALISATION_

#include "iniopt.h"
#include "../baseclas/baseclas.h"
#include "../baseclas/baseclas.hpp"
#include "../ioclass/soundlab.h"
#include "../features/feature.h"
#include "../resconf/resconf.h"
#include "../tspecmod/tspecbas.h"
#include "../tspecmod/tspecbas.hpp"


//classi specifiche per initiali
class InitCluster : public Gaussian
	{
    public:

    t_index population;
	VetDouble centroid;
	t_real distortion;

	void Compute_Cluster_Distortion();
	void Do_Averages();
	};

class InitNodeSpecShape : public NodeSpecShape<InitCluster> {};

class InitCodebook : public Hmm<InitNodeSpecShape>
	{
    public:

	void Up_Normalized_Sum_SqrSum_And_Pop
		(const VetDouble& act_frame,VetDouble & sum, MatrixOfDouble &  square_sum,
		 DiagMatrixOfDouble &diag_square_sum, t_index & cardinality);

	void Initialize(t_index num_sections, const t_index num_mix, t_index statistics_dim
					, Boolean full_covar);

	void Accumulate(const ImpObjectList<VetDouble>& phon_istance,
				const VetLong& limit);

	void Add_One_Cluster_To_Section(const t_index section,
						const t_index stat_dim);
	void Compute_Whole_Codebook_Clusters_Distortions();

	void Calculate_New_Centroids(const t_index section,
					    const t_index father_cluster,
							 const t_index son_cluster);
	void Reset_Clusters_Statistics(t_index section);

	void Compute_Cluster_Weights();

	void Store_Codebook(const String& output_fname,	const t_index act_phon,
						Boolean load_mixture, const ModelType model_type);
	};


class Initiali
	{
	InitCodebook states_info;
	FeatureExtraction feature;

	t_index stat_dim;
	
	VetULong init_symb_list;
	VetULong num_sections_per_symbol;	 // number of section for each initializing phoneme
	VetULong num_mix_per_symbol;		 // order of mixture for each initializing phoneme
	VetULong maxfra_sect;	// max number of frame for section for each initializing phoneme

	ImpObjectList<VetLong> phons_limits; // dimension (number of istances of
					 						// the actual phon * num_sect +1)

	void Calculate_One_Mixture_Codebook( t_index act_sym, t_index & num_frames, DbaseVoc& dbase, Boolean unit_segm);

	void Sectionate(const ImpObjectList<VetDouble> & phon_ist, VetLong & frame_limits,
					t_index maxfra_sect, Boolean unif_segm);

	//multimixtures;
	void Split_Higher_Distortion_Cluster_Of_Each_Section(const t_index act_phon,
								const t_index num_frames,
								t_index& split_num, DbaseVoc& dbase);

	void IndexOfClustersToSplit(ImpSimpleTypeList<t_index>& hi_dist_clus);

	t_index Assigne_Actual_Frame_To_Its_Cluster(const VetDouble& mfc_vet,
								const t_index num_split, const t_index section);

	t_real Distance_From_Centroid(const VetDouble& mfcvet,
								const t_index section,	const t_index cluster);

public:

	Boolean Configure(const IniOptions& options, const String& config_fname,
														t_index dim_symb_table);

	void Symb_Model_Calculation(const String& out_fname, const String& in_fname,
					DbaseVoc& dbase, Boolean full_cov, Boolean load_one_mixture,
					Boolean unif_segm, const ModelType model_type);

	};


#endif
