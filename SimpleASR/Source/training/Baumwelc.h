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
//  |   FILE:   baumwelc.h                                              |
//  |   FUNCTIONALITY: class training definition                        |
//  |   PROGRAM: perform basic Baum-Welch re-estimation					|
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio & Massimo & Fabrizio 					|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION:  19/11/98						            |
//  |   FIRST VERSION:      1.0                                         |
//  |   DATA FIRST VERSION: 31/7/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________




// Training class performs basic Baum-Welch re-estimation of the parameters using
// a set of observation sequences

// solved division by zero problem: although in current use each phoneme
// would have large number of istance and there wouldbe no division by zero
// problem testing the routine on a reduced number of files problems may be
// encountered

// Fabrizio 
// <version 4.0>
// 11/03/98 eliminated all const char*


#ifndef _TRAINING_
#define _TRAINING_

#include <fstream.h>
#include "../baseclas/baseclas.h"
#include "../baseclas/textclas.h"
#include "../ioclass/soundlab.h"
#include "../features/feature.h"
#include "../vetclas/arraycla.h"
#include "../tspecmod/tspecbas.h"
#include "../tspecmod/tspecbas.hpp"

	

typedef ImpObjectList<MatrixOfDouble> Tri_D_Matrix;
typedef ImpObjectList<DiagMatrixOfDouble> DiagTri_D_Matrix;
typedef ImpObjectList<Tri_D_Matrix> Quattuor_D_Matrix;
typedef ImpObjectList<DiagTri_D_Matrix> DiagQuattuor_D_Matrix;

typedef ImpObjectList<VetDouble> t_mfcvet_list;


const t_real LMINMIX=log(1.E-3);


class TrainCluster : public Gaussian
	{
    public:

	// Cluster_Distance: given the observed frame (obs_vet) returns its
	// log-probability for the actual cluster
	t_real Cluster_Distance(const VetDouble & obs_vet);
    };

class TrainNodeSpecShape : public NodeSpecShape<TrainCluster> {};

class TrainCodebook : public Hmm<TrainNodeSpecShape>
	{
    public:

	// likelyhood of the observation sequence given the final trained
    // model (parameter for comparing) 
	t_real obs_likelyhood;

	void Store_Codebook(const String& output_fname, const t_index act_phon);
    };


class Training
	{
	Boolean full_covariance;
	Boolean reestimate_transition;
	TrainCodebook states_info;   // phoneme spectral representation

	// INPUT DATA
	DbaseVoc dbase;
	FeatureExtraction  feature;
	VetULong symbol_list;
	t_real preemphasis;   // preemphasis
	String models_file_input;
	
	String models_file_output;
	String models_stat_output;
	
	t_index maxiter_num;	// maximum number of reestimation procedure iterations
	t_real conv_threshold;	// threshold for the convergence
	t_index num_states;		//number of model states
	t_index num_mix;		// number of gaussians for each state
	t_index vec_size;		// observation vector and statistics
							// dimension (cepstral vector dimension)
	VetDouble sequences_lprob;  // array[0..num_istance-1] of log-probabilities of
								// observation sequences
	// accumulator vectors & matrix's
	// vectors of cardinality=num_state
	VetDouble occ_beg;	// occupation count for state i at time i=0
	VetDouble occ_end;  // occupation count for state i at time i=numframe-1
	VetDouble occ;		// occupation count for state i at time 0<i<numframe-1

	//matrix  cardinality (numstates*numstates)
	MatrixOfDouble tran; // number of transition from state i to j

	//matrix  cardinality (numstates*num_mix*vec_size)
	Tri_D_Matrix mu; //vectors of mean sums for mixture m of state j

	//matrix  cardinality (numstates*num_mix*vec_size*vec_size)
	Quattuor_D_Matrix va; //  matrix of covariance square sums for mixture m
						  // of state j
	DiagQuattuor_D_Matrix diagva;

	// matrix  cardinality (num_states*num_mix)
	MatrixOfDouble c;  // new mixture weights

	// end accumulator vectors ---------------------------------------


	//class methods--------------------------------------

	// *  *  *  *  *  FIRST LEVEL *  *  *  *

	// Allocate_Mem_For_Parameters: dimensionates occupation counters(occ_beg,
	// occ_end, occ_i, tran, c), and accumulating statistics structures(mu, va)
	void Allocate_Mem_For_Parameters();

	// ReEstimate_Model:  iteratively trains the spectral model of phoneme act_phon
	// until the convergence threshold is reached or a max_iter_num of iterations
	// are performed
	void ReEstimate_Model(const t_index act_phon, ofstream & trai_statf);
	 
		
	// *  *  *  *  *  SECOND LEVEL *  *  *

	// Append_New_Frame_To_PhonIstance: add a new observation to observation sequence
	void Append_New_Frame_To_PhonIstance(t_mfcvet_list& phon_istance,
							const VetDouble& vetsmp);

	//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * //
	//                                                                      //
	//           OBSERVATIONS SEQUENCE PROBABILITIES MATRICES                //
	//                                                                      //
	//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * //

	//  Sequences_Total_LProb: computes global(over all sequences) log-probability
	t_real Sequences_Total_LProb();

	// Build_Observation: given the observation sequence(phon_istance) computes
	//   log-probability of each observation :
	//     			for each state, and put in b matrix
	//				for each mixture, and put in mix_out_lprob
	void Build_Observation(MatrixOfDouble& b, Tri_D_Matrix& mix_out_lprob,
					const t_mfcvet_list & phon_istance,	const t_index num_frames);

	// Calculate_b_By_Mixture_Observation: from observation obs_vect computs
	// the log-probabilities of which above
	void Calculate_b_By_Mixture_Observation(MatrixOfDouble & b,
						Tri_D_Matrix& mix_out_lprob, const VetDouble& obs_vect,
					    const t_index time);

	//  Build_Alfa: from a and b matrices computes alpha matrix of forward log-
	// probabilities  and observation sequence probability for given sequence
	void Build_Alfa(MatrixOfDouble& alfa, const t_index ist_idx,
						const t_index num_frames, const MatrixOfDouble& a, 
						const MatrixOfDouble& b);

	// Build_Beta: from a and b matrices computes beta matrix of backward
	// log_probabilities
	void Build_Beta(MatrixOfDouble& beta, const t_index ist_idx,
						const t_index num_frames, const MatrixOfDouble & a,
						const MatrixOfDouble & b );

	//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  //
	//                                                                       //
	//                   ESTIMATION OF PARAMETERS                            //
	//                                                                       //
	//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  //

	//  Reset_Parameters: sets to zero parameters (occupation counters
	// and statistics accumulators)
	void Reset_Parameters();

	// Update_Parameters: updates occupation counters and statistics accumulators
	void Update_Parameters(const MatrixOfDouble & alfa, const MatrixOfDouble & beta,
						const MatrixOfDouble & b, const Tri_D_Matrix& mix_out_lprob,
						const t_index ist_idx, const t_index num_frames,
						const t_mfcvet_list & phon_istance);

	// Up_Transition_Parameters: implements transition matrix reestimation
	void Up_Transition_Parameters(t_real Pr, const MatrixOfDouble & alfa,
						const MatrixOfDouble & beta, const MatrixOfDouble & b,
						const t_index num_frames);

	// Up_Multi_Mixture_Parameters: implements output statistics(means and covariance)
	// reestimation
	void Up_Multi_Mixture_Parameters(t_real Pr, const MatrixOfDouble & alfa,
						const MatrixOfDouble & beta, const Tri_D_Matrix& mix_out_lprob,
						const t_index num_frames, const t_mfcvet_list & phon_istance);

	// Update_Model: updates the model with accumulate parameters estimastes
	void Update_Model(const t_index phoneme_instance, t_index act_phon);

	// ***************    END ESTIMATION PARAMETERS    ******************

	// LogAdd: being lp1 and lp2 two log-probabilities returns logprobability
	// log(lp1+lp2)
	t_real LogAdd(t_real x, t_real y);

//-------------------------------------------------------------------------
public:
	void Configure(const String& config_file);

//  HIGHER LEVEL FUNCTION THAT PERFORMS THE TRAINING OVER ALL THE PHONEMES OF
//  THE ACTUAL DBASE BY TIED BAUM-WELCH PROCEDURE ( one phoneme at times )
	void Baum_Welch();

	};

#endif
