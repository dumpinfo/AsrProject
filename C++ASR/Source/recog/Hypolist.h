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
//  |   FILE:   hypolist.h                                              |
//  |   FUNCTIONALITY: implementation graph and hmm                     |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 28/4/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

//implementazione grafo e hmm
//file principale
//version 28/4/95
//revised 5/5/1995
//by claudio  & massimo & antonio

// revised 12/7/95: compared with all existing version of the same file.
//       implemented new function Observed_Hypo_List_From... that is able
//       to manage with multilevel grammar skipping intial and final dummy state

// revised 1/8/95: adjusted GenericStaticHmm appending Hypoes to silence part

// revised 5/8/95: little problems in Viterbi: fixed silence treatment part
//			 and BackTrack From: more efficient code
//	        furthermore seems to be necessary to introduce a new flag to
//		hyponode structure to mark the change of the model (i.e. the transitio
//		between higher level nodes) otherwise it would be not possible
//		to distinguish cicles over the same phoneme.
//		It should be a Boolean initialized at false and turned true when appending
//		higher level hyponode successors.

// revised 3/9/95: flag of which at the previous point added to the hyponode structure
//              bug in Viterbi function: Check_Candidate_Presence... also when there
//		are no candidates in candidates list (solved putting ix!=0 condition)

// revised 10/9/95: lot of bugs in Viterbi
//              1) candidates comparison doesn't depend on obs_lprob so its sufficient
//		to add it to log_prob_acc when the candidates have been selectioned.
//		-> high complessity reduction.
//              2) fixed window search with exaustive algorithm for search of the best
//		max_num_actives actives (unefficient to keep actives ordered) and pruning
//		till conditions over dimensions are satisfied
//		3) in GenericStaticHmm::Hyponodes relational operators efficiency
// 		improvement: redefined comparing only the lower grammar level state
//		Attention: Prune_Branch_From_Active  does not remove the active from the
//		actives list
//version febbraio 96 routines to load bigram and trigram grammar
//version 6/6/96 routines to load word class grammar
// revised 11/05/98 by Domenico
#ifndef __HYPO_LIST
#define __HYPO_LIST

#include "../recog/recopt.h"
#include "../tspecmod/tspecbas.h"
#include "../tspecmod/tspecbas.hpp"
#include "../ioclass/soundlab.h"
#include "../features/feature.h"
#include "../lessico/lessico.h"



// *************************************************************
//
//					Class for HMM models
//
// *************************************************************

class RecogCluster : public Gaussian
	{ 

	public:
	 
		void operator=(const RecogCluster & source);
	  
	};

class RecogNodeSpecShape : public NodeSpecShape<RecogCluster> {};
class RecogCodebook : public Hmm<RecogNodeSpecShape> {};


// *************************************************************************************
//															                           *
//		      				 static structure				                           *
//															                           *
// *************************************************************************************


typedef t_index natural;
typedef t_index t_ptr;

typedef t_ptr p_spec_table;
typedef	t_ptr p_acustic_state;
typedef	t_ptr p_grammar_state;

//second level
typedef t_ptr p_leaf;
typedef t_ptr p_kid;

typedef t_real t_lprob;

template <class T,class LABEL>
class Graph
	{
	public:

		class Branch
			{
			public:
				LABEL label;
				t_ptr p_node;

				void operator = (const Branch& act_branch)
					{
					label = act_branch.label;
					p_node = act_branch.p_node;
					return;
					};
			};

		class Node
			{
			public:
	    
				T info;
				ImpObjectList<Branch> nexts;
			};

		ImpObjectList<Node> G;

		t_index Num_Successors(t_ptr p_node) const
			{
			return(G[p_node].nexts.Dim());
			}

		Node &  operator [] ( const t_index index )
			{
			return(G[index]);
			}

		const Node &  operator [] ( const t_index index ) const
			{
			return(G[index]);
			}

	};
	
	typedef  Graph<t_ptr,t_lprob> LinkedGraph;

	// DEFINITION OF A MULTILEVEL GRAMMAR RECOGNITION STRUCTURE.         //
	// pure virtual functions makes the class independent from           //
	// the particulart kind of observation and recognition unit          //

	//   *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *   //
	//                                                                   //
	//                        GENERIC STATIC HMM                         //
	//                                                                   //
	//   *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *   //


class GenericStaticHmm
	{
	public:
		ImpObjectList<BooleanTRealCouple> states;
		// conversion table from the node labels of the top level graph
		// ( = numerical pointers ) and recognition units
		ImpObjectList<String> top_level_symbols;

		//pay attention
		//grammar[l] select the linked graph at level l
		//grammar[l] [j]   select the jth node in the
		//linked graph at level l
		//base_node.state[l] gives the pointer to node of
		//the graphic at level l
		//if base_node is an HypoNode

		ImpObjectList<LinkedGraph> grammar;

		// single hypotesis unit
		class HypoNode
			{
			friend class GenericStaticHmm;
			protected:
				// accumulated path score
				t_lprob log_prob_acc;

				//hypotesis coordinate
			public:
		
				// beginning of a new recognition unit necessary
				// in order to disciminate recognition unit node self-cycle
				Boolean new_unit;
				ImpMatVector<t_ptr> state;
				Boolean operator!=(const HypoNode& node) const;
				Boolean operator==(const HypoNode& node) const;
				Boolean Is_Worse_Then(const HypoNode& node) const;

				t_real Add_To_LProb_Acc(const t_lprob lprob);
				t_real Log_Prob_Acc()
					{
					return log_prob_acc;
					}

			}; //end of class HypoNode

		class HypoList: public ImpObjectList<HypoNode>{};

		// put in ris_list the list of successors of
		// silence updated with observation probability
		void Observed_Hypo_List_From_Silence(HypoList & ris_list, const HypoNode & silence);
		void Observed_Hypo_List_From_This_Node(HypoList & ris_list, const HypoNode & act_node);

		// Append_Successors_To: given a node (base node) valid till level
		// "level_to_change-1"returns a list of nodes and calculates
		// the log_prob_acc i.e the log probability accumulated
		// an empty list returned means that there are no more observations
		void Append_Successors_To(HypoList& list, const HypoNode & base_node, const t_index l);

		// Append_Lowest_Level_Hypos_To:
		void Append_Lowest_Level_Hypos_To(HypoList & list, HypoList& act_level_list, t_index l);
		// Set_To_Silence: as above but assigning the index of the 
		// node of the silence at the highest level of the grammar
		void Set_To_Silence(HypoNode & act_node,const t_index sil_index);

		// Get_Observation: gets sequentially observation units,
		// returns TRUE while input is not ended, FALSE on the end of the input
		virtual Boolean Get_Observation(Boolean& new_file, DbaseVoc& dbase)=0;

		// Obs_LProb: computes observation log-probabilities
		virtual t_lprob Obs_LProb(const t_ptr pnode_stat)const =0 ;

	};




//************************************************************************************
//                                                                                   *
//                           SPECTRALHMM                                             *
//                                                                                   *
//        (derived class for recognition from acoustic information)                  *
//************************************************************************************

class SpectralHmm: public GenericStaticHmm
	{

	public:
						
		t_real preemphasis;
		Boolean is_full;

		FeatureExtraction feature;

		// spectral description of one 1-level states(nodes)
		RecogCodebook spectral_model;

		VetDouble observation ;

		// Initialise_Structures: opens trained phonemes models file and
		// initialises dbase voc and phonemes statistical representation
		// codebook

		void Configure(const RecogOptions& choice, DbaseVoc& dbase, t_index& sil_index,
	                     const String & config_f_name);

		//load phonemes acoustic models
		void Load_Acoustic_Level(ifstream & symb_stat_dataf, const t_index & stat_dim, 
	                             VetULong & phons_map, t_index & phons_card,  
								 DbaseVoc & dbase, Boolean is_full, t_real skip_th);

 		void Load_Word_Level(const String & vocab_fname, const VetULong & phons_map,
				    t_index & words_card,VetULong & words_map, DbaseVoc & dbase, 
					t_index & sil_index);

		void Bigram_Model_Wrd_Level (const String & matrix_f_name,
				    const t_index words_card, const VetULong& words_map, 
					const t_index& sil_index, const t_real penalty,
					const t_real penalty_sil, const t_real bigr_th,
					const t_real desc_gr_const);

		void Uniform_Grammar(const VetULong & units_map,const t_index level,
				   	const t_index units_card, const t_real penalty);

		void Phoneme_Level_Grammar(const String & trans_f_name,
					const VetULong & phons_map, DbaseVoc & dbase,
					const t_real penalty, const t_real penalty_sil, t_index sil_index);

		void Load_Word_Classes (const String & classes_f_name, t_index & classif_card,
						const VetULong & words_map, t_index & sil_index);

		void Load_Class_Transitions (const String & class_matrix, const t_index classif_card,
	                                  const t_index class_card, const t_real penalty,
									  const t_real penalty_sil);

		void Set_Silence_Index(t_index& sil_index, 
									const StaticGrammarLevel& grammar,
									const StaticGrammarType& grammar_type);

		Boolean Get_Observation(Boolean& new_file, DbaseVoc& dbase);

		t_lprob LogAdd(t_lprob x, t_lprob y)const;

		t_lprob Obs_LProb(const t_ptr pcluster)const;

	};


// ************************************************************************************
//									                                                  *
//       		            DYNAMIC STRUCTURE				                          *
//									                                                  *
// ************************************************************************************


//we hypotize that one node can't generate twice tha same node


//FIRST LEVEL LINKED BUFFERED LIST WITH EMBEDDED FREE LIST

//linked sparse list con lista libera NUll pointer is zero index
//zero index poinnter non accessible
template <class T>
class SparseList
	{
		t_ptr free_list;
		t_ptr start_list;
		natural dim_free;
		natural chunk_size;

		class Node
			{
				friend SparseList<T>;
				T info;
				t_ptr link;
				t_index num_kids;
			public:

				static t_index Kids_Of_Free_Node()
					{
					return t_index(-1);
					}

				Node()
					{
					num_kids=this->Kids_Of_Free_Node();
					}  
			}; // class Node


		ImpObjectList<Node> List;

	public:

		void Reset();
	
		//when freelist finishes chunk size elements allocated in free list
		SparseList(natural chunk_size=100);

		void Restart();

		//return ptr to node created
		t_ptr  Create(const T & info,t_ptr parent);

		//error message if referenced node
		void Destroy_Node(t_ptr node);

		//error message if referenced node
		//Destroy Branc nodes until referenced node
		void Destroy_Branch(t_ptr node);

		// retrieves list of info of connected nodes starting from node
		// assumes that T has an operator = that duplicates the classes
		void Backtrack_From(ImpObjectList<T> & sequence,t_ptr node);

		t_ptr Next(t_ptr node) const;
		T & operator[](const t_ptr node);
		const T & operator[](const t_ptr node)const;
	
		t_index Num_Node() const;
		Boolean Has_No_Kids(t_ptr t_node) const;

	protected:

       void Allocate_Mem();

	};


//SECOND LEVEL
template<class T>
class WellTree
	{

    friend class HypoTree;

		SparseList<T> l_list;
		ImpSimpleTypeList<t_ptr> leaves_dir; //start of last level nodes
		ImpSimpleTypeList<t_ptr> kid_dir;    //start of new level (or end of tree)


   public:

		~WellTree();
		void Reset();

		//needed by Viterbi.num_attivi
		t_index Num_Elements() const;
		t_index Kids_Dim() const;
		t_index Leaves_Dim() const;

		void Exchange_Leaves_Indexes(const t_index i, const t_index j);
		void ReDim_Leaves_Dir_To(const t_index ix);

		//Check_Kid_Presence_And_Get_Num: checks for the presence of act_kid in kid_dir
		Boolean Check_Kid_Presence_And_Get_Num(const T & act_kid, p_kid & num_kid);
		const T& Get_Leaf_Info(const p_leaf leaf)const;
		T& Get_Leaf_Info(const p_leaf leaf);
		const T& Get_Kid_Info(const p_kid kid)const;
		T& Get_Kid_Info(const p_kid kid);
		void Create_First_Leaf_Of_Tree(const  T& info);
		void Add_Kid_To_Leaf(const T & info,p_leaf leaf);
		void Prune_All_Dead_Leaf();
		void Prune_Blind_Branch_From_Leaf(p_leaf leaf);
		//Backtrack_from(a_node) returns a new list with every element
		//containing address of every nodes along path sequence
		void Backtrack_From(ImpObjectList<T> & sequence,p_leaf leaf);
		//start the next generation
		//transform kid_dir leaves_dir;
		void Next_Gen(); // leaves=kid

		void Subst_Old_Kid_Destroy_Old_Branch_Ins_New(p_kid old_kid,
					const  T & info,p_leaf new_father);

	};

// **********************************************************************
//                                                                      *
//                          third level                                 *
//                                                                      *
// **********************************************************************

//we hypotize that one node can't generate twice tha same node

//class header

typedef t_index p_candidate;
typedef t_index p_active;

class HypoTree
	{
	
		WellTree<GenericStaticHmm::HypoNode> tree;

	//actives are actives-node at step K
	//candidates are actives-node at step K+1
	//p_active p_candidate are pointers to active and candidate respectively
	//t_index are unsigned long numbers

	public:

		t_index Candidates_Dim() const;

		t_index Actives_Dim() const;

		const GenericStaticHmm::HypoNode & Get_Candidate(p_candidate num);
 		// GenericStaticHmm::HypoNode & Get_Candidate(p_candidate num);

		GenericStaticHmm::HypoNode & Get_Active(p_candidate num);
	
		void Prune_All_Dead_Active();

		// check if adds an already present node, if so select the best one
		Boolean Check_Candidate_Presence_And_Get_Num(const GenericStaticHmm::HypoNode & info,
															p_candidate& num_candidate);
		Boolean Check_Candidate_Presence_And_Get_Num(VetULong & candidate_map,
				 	p_candidate & num_candidate, const GenericStaticHmm::HypoNode & info);
		// the following function calls WellTree.read_node_info(const & t_node node);
		Boolean Node_Is_Worse_Then_Candidate(const GenericStaticHmm::HypoNode& node,
																p_candidate & num_candidate);
		void Create_First_Active_Of_Tree(const GenericStaticHmm::HypoNode & node);

		void Add_Candidate_To_Active_Num(const GenericStaticHmm::HypoNode & node,
							p_active num_active);
		void Prune_Branch_From_Active (p_active num_active);

		void Subst_Old_Cand_Destroy_Old_Branch_Ins_New(p_candidate old_kid,
								const  GenericStaticHmm::HypoNode & info,p_active new_father);

		void Transform_Next_Generation_In_Actives();

		void UpDate_Actives_LProb_Acc(VetDouble& scores, GenericStaticHmm & hmm);

		//void UpDate_Actives_LProb_Acc(VetDouble& scores, const GenericStaticHmm & hmm);
		void Beam_Search(VetDouble & scores, t_real & threshold_init, t_real & threshold_internal,
										const t_real beam_init,	const t_real beam_internal);
		void Window_Search(VetDouble& scores, const t_index max_num_activate);

		void Exchange_Actives_Indexes(const t_index i, const t_index j);

		void ReDim_Tree(const t_real threshold_init, const t_real threshold_internal);

		void ReDim_Tree(const t_index max_dim_tree, const t_index max_num_activate);

		void Reduce_Actives_To(const t_index ix);

		p_active Worse_Active();

		p_active Best_Active();

		//Backtrack_from active returns a new list sequence
		//with every element
		//containing nodes info of Viterbi best path sequence
		void Backtrack_From(GenericStaticHmm::HypoList & sequence, p_active active);

		Boolean Viterbi_Window(GenericStaticHmm::HypoList & sequence,t_index max_num_activate,
			t_index max_dim_tree,Boolean & new_file, GenericStaticHmm& hmm,
									const t_index sil_index, DbaseVoc& dbase);

		Boolean Viterbi_Beam(GenericStaticHmm::HypoList & sequence, const t_real beam_init,
								const t_real beam_internal,	t_index max_dim_tree, Boolean & new_file,
								GenericStaticHmm & hmm,	const t_index sil_index, DbaseVoc & dbase);
	};

     
#endif
