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
//  |   FILE:   Emb_b_w.h                                               |
//  |   FUNCTIONALITY: class Embedded Training definition               |
//  |   PROGRAM:														|
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Antonio & Fabrizio  & Massimo					|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                	|
//  |   FIRST VERSION:                                                  |
//  |   DATA FIRST VERSION: 9/11/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________
// *******************    VERSION HISTORY  *******************************

//24/4/98 changed some class names

#ifndef _EMBEDDED_TRAINING_
#define _EMBEDDED_TRAINING_

#include <iostream.h>
#include "../baseclas/baseclas.h"
#include "../baseclas/textclas.h"
#include "../features/feature.h"
#include "../ioclass/soundlab.h"
#include "../vetclas/arraycla.h"
#include "../tspecmod/tspecbas.h"
#include "../tspecmod/tspecbas.hpp"


typedef ImpObjectList<MatrixOfDouble> Tri_D_Array;
typedef ImpObjectList<Tri_D_Array> Quad_D_Array;

typedef ImpObjectList<VetDouble> Bi_D_List;
typedef ImpObjectList<Bi_D_List> Tri_D_List;

typedef ImpObjectList<String> t_string_list;


const t_real LMINMIX = log(MINMIX);



// **************************************************************************
//                                                                          *
//  	     		  CODEBOOK MODEL FOR EMBEDDED TRAINING                  *
//                                                                          *
// **************************************************************************

class EmbNodeSpecShape : public NodeSpecShape<Gaussian>
	{
    public:

		// MOutP: Returns prob of vector x for given state & given mixture
		t_real Mix_Obs_LProb(const VetDouble& obs, const t_index m);
		// observation log-probability
		t_real Obs_LProb(const VetDouble& x);
	
	};
 
class EmbCodebook : public Hmm<EmbNodeSpecShape>{};


// **************************************************************************
//                                                                          *
//		     		  STATISTICS ACCUMULATORS                               *
//                                                                          *
// **************************************************************************

// class StatisticsAccumulators:
// set of accumulators for the parameters of a  single model
class StatisticsAccumulators
	{

	public:
		t_index num_states;
		t_index num_mixes;
		t_index num_istances;	// number of istances for the model

		// For the generic h-th label, being Nh the number of its model states
		// and Mh the order of mixtures
		MatrixOfDouble tran;	// array (Nh * Nh) of transition counters
		VetDouble occ;			// array Nq of occupation counters
		Tri_D_List mu;			// array ((Nh-2) * Mh * obs_size) mu[j][m] = mean
								// vector of j-th state mixture m-th component

		Quad_D_Array full_cov; // array ((Nh-2) * Mh * obs_size* obs_size)
							// full_cov[j][m] = covariance matrix of							// j-th state mixture m-th component
		Tri_D_List diag_va; 	// array ((Nh-2) * Mh * obs_size)
							// full_cov[j][m] = covariance matrix of
							// j-th state mixture m-th component
		MatrixOfDouble c;     	// array (Nh * Mh) of mixtures weights

		void Configure(const t_index num_states_of_symbol,const t_index num_mixes_of_symbol,
				   const t_index obs_size, const Boolean full_covs);

		void Reset_Parameters();
	};



// ***************************************************************************
//
//                     CLASS  EmbeddedTraining
//
// ***************************************************************************


class ModelsSimultaneousTraining
	{
	// list of the H EmbCodebook's definitions (one for each label) 
	// (the class is specified above)
	ImpObjectList<EmbCodebook> HMM_defs; 
		
	// array (H)-dim of accumulators for computing
	// the new estimates of the parameters
	ImpObjectList<StatisticsAccumulators> HMM_accs; 
							
	FeatureExtraction features;	  	// feature extraction
	DbaseVoc dbase;	
	
	Boolean load_accs;
	Boolean store_accs;
	String accs_file;
	String list_file;
	
	t_index max_num_iteration;
	t_real pruning_threshold;
	
	// about what reestimate
	Boolean reest_means;
	Boolean reest_variances;
	Boolean reest_weights;
	Boolean reest_transitions;
	t_real min_var_value;
	t_real const_to_add_min_var;
	t_index min_istance_number;

	// NAME OF THE FILES FOR ... 		
	String models_file_input; 			// ... initialised EmbCodebook's parameters
	String models_file_output;			// ... trained EmbCodebook's parameters
	//String train_parms_fname; 			

	// FOR THE CURRENT UTTERANCE:
	t_index T; 					// number of frames in the utterance
	t_index Nq;    				// number of states of the q-th model
	t_index M;    				// order of mixture
	t_index Q;     				// number of EmbCodebook's (Labels) (get local??)

	// VetULong label_idxes_list; 		// array Q-dim of indexes of one 								// utterance labels
	
	// STRUCTURES FOR FORWARD-BACKWARD COMPUTATION.
	Bi_D_List act_alfa;	   // array(Q * Nq) of forward log-probabilities at time t
	Bi_D_List prev_alfa;   // array(Q * Nq) of forward log-probabilities at time t-1
	Tri_D_List beta;	   // array(Q*T*Nq) of backward log-prob
	Tri_D_List obs_lprob;  // array(Q*T*Nq) of observation log-							// prob
	t_real utter_lprob;    // log probability of current utterance according to 
						   // actual models
	
	t_real maxBeamWidth;
	t_real maxAlphaBeta;
	t_real minAlphaBeta;
	

	//	**********************************************************************
	//
	//					CONFIGURING TRAINING PROCEDURE
	//
	//
	//	**********************************************************************

	void Load_Models_Parameters();
	
	// ************************************************************************
	//                                                                        *
	//						REESTIMATE PARAMETERS                             *
	//                                                                        *
	// ************************************************************************
	
	t_real Update_Parameters_With_New_Utterance(const Bi_D_List& utterance,
														const VetULong& label_idxes_list);

	// InitPruneStats: initialise pruning stats
	void Init_Prune_Stats();

	// CheckPruning: record peak alfa.beta product and position
	void CheckPruning(t_index t, const t_index beam_top, const t_index beam_bottom,
					  EmbCodebook& act_hmm);

	// CreateInsts: create array of hmm instance indexes
	void Create_Insts(VetULong& label_idxes_list, 
									const t_string_list& label_list, DbaseVoc& dbase);

	// ****** BETA MATRIX  & OBS LOG PROBABILITIES *************

	// SetBeta: allocate and calculate beta and obs_lprob matrices
	t_real Compute_Beta(VetULong& qHi, VetULong& qLo,
		const Bi_D_List& whole_utter, const VetULong& label_idxes_list);

	// Setotprob: allocate and calculate otprob matrix at time t
	void Compute_Obs_LProbs(const VetDouble& obs,
				 const t_index t, const t_index beam_top,
				 const t_index beam_bottom, const VetULong& label_idxes_list);

	t_real Sequences_Total_LProb(const VetDouble& sequences_lprob);


	// **** ALFA MATRIX  ************

	// InitAlpha: allocate and initialise alfa columns for time t=0
	void Init_Alpha(const VetULong& label_idxes_list);

	// StepAlpha: calculate alfat column for time t and return
	//   forward beam limits in startq and endq
	//   for first opbservation startq = endq=0
	void Compute_Alpha_At_Time_t(t_index& startq, t_index& endq, const t_real pr,
					   const VetULong& qHi, const VetULong& qLo, const t_index t,
				 								const VetULong& label_idxes_list);

	// ZeroAlpha: zero alpha's of given models
	void Zero_Alpha(const t_index qlo, const t_index qhi,
											    const VetULong& label_idxes_list);


	// **** END ALPHA MATRIX  ************

	// GetOutVec: Get observation vector at time t: obs
	void Get_Observation(VetDouble& obs, const Bi_D_List& whole_utter, 
																 const t_index t);


	// ****************    	  END REESTIMATE PARAMETERS       *******************

	// UpMMixParms: update mu/va of given acc for M>1 mix case
	void Up_Mix_Parms(EmbCodebook& hmm, StatisticsAccumulators& acc, const t_index q,
							 const t_index t, const VetDouble& obs, const t_real pr);

	// UpTranParms: update the transition counters of given acc
	void Up_Tran_Parms(const EmbCodebook& hmm, StatisticsAccumulators& acc, const t_real pr,
					const t_index q, const t_index Q, const t_index t, const t_index T);

	void Update_Occourrence_Counter(StatisticsAccumulators& acc, const t_index t,
								                   const t_index q, const t_real pr);

	// Update_Model: use accumulator values to compute new estimate for EmbCodebook
	void Update_Model(EmbCodebook& hmm, StatisticsAccumulators& acc);
													       
	t_real MaxModelProb(const t_index q, const t_index t, const VetULong& label_idxes_list);

	void ReEstimate_Parameters( t_real& training_set_likelyhood, DbaseVoc& dbase);
	void Update_Models();

	void Store_Statistic_Accs(const String& accs_file);

    void Load_Statistic_Accs(const String& accs_file);

public:
	//  Init_Emb_Train: initialise dbase, training structures and load models.
	void Configure(const String& config_file);

	// ReEstimate_Parameters: apply forward-backward algorithm compute alfa and beta matrixes
	// to all training utterances updating parameters when requested

	// Update_Models: updates all models and save them in newDir if set;
	// new files have newExt if set
	
	Boolean Symbol_Models_Optimisation();

	void Store_HMM_Models();

	void Store_Iteration_Model(t_index iteration);

	};


#endif
