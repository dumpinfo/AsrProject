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
//  |   FILE:   hypolist.cpp                                            |
//  |   FUNCTIONALITY: hypolist for segmentation                        |
//  |   PROGRAM: graph and Hmm implementation                           |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: claudio  & massimo & antonio						|
//  |   ACTUAL REVISION: 4.0                                            |
//  |   DATA ACTUAL REVISION: 28/4/98                                   |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 14/7/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#include "../segment/hypolist.h"
#include "../segment/hypolist.hpp"

// *************************************************************
//
//					Class for HMM models
//
// *************************************************************

void RecogGaussian::operator= (const RecogGaussian & source)
	{
    weight=source.weight;
	mean=source.mean;
    gConst=source.gConst;
    if (source.full_cov)
		inv_cov=source.inv_cov;
	else diag_inv_cov=source.diag_inv_cov;

    return;
	}



//   *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *   //
//              GENERIC STATIC HMM FUNCTIONS IMPLEMENTATION          //
//   *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *   //

void GenericStaticHmm::Observed_Hypo_List_From_Silence(HypoList & ris_list,
					 											const HypoNode & silence)
	{
	t_index grammar_hilevel, i;
	
	// hypolist structure for grammar scanning
	HypoList  list_new;

	//verify that hypotesis cardinality is equal
	//to num of grammar level
	Assert (silence.state.Dim()==grammar.Dim());
	grammar_hilevel = grammar.Dim()-1;
	ris_list.Reset();
	list_new.Destroy_And_ReDim(1);
	list_new[0]	= silence;

	Append_Lowest_Level_Hypos_To(ris_list, list_new, grammar_hilevel); 
	// first model of the resulting list
	for(i=0;i<ris_list.Dim();i++)
		{
		ris_list[i].new_unit = (Boolean)TRUE; // first model of the resulting list
		}

	// in ris_list surely only valid hypoes (i.e. no starting either
	// ending nodes)
	return;
	};


void GenericStaticHmm::Observed_Hypo_List_From_This_Node(HypoList & ris_list,
					 const HypoNode & act_hyponode)
	{
	t_index i, l;
	t_index p_gramnode;
	
	// hypolist structures for grammar scanning
	HypoList  act_hyponode_list, list_old, list_new;

	//verify that hypotesis cardinality is equal
	//to num of grammar level
	Assert (act_hyponode.state.Dim()==grammar.Dim());
	// code with initial and final dummy states treatment
	// new iterative function version
	// initialisation pass: put actual hypothesis in list_old
	list_old.Destroy_And_ReDim(1);
	list_old[0] = act_hyponode;
	ris_list.Reset();
    //per ogni livello di grammar
	l=0;
	do	{
		list_new.Reset();
		for(i=0;i<list_old.Dim();i++)
			{
			act_hyponode_list.Reset();
			Append_Successors_To(act_hyponode_list, list_old[i], l);
			Append_Lowest_Level_Hypos_To(list_new, act_hyponode_list, l);
			}
		list_old.Reset();
		// scan list_new in order to locate valid hypotesis
		for(i=0;i<list_new.Dim();i++)
			{
			p_gramnode = list_new[i].state[0];
			if (grammar[0][p_gramnode].nexts.Dim() == 0)
			    // is an ending dummy state: it's necessary climbing
			    // the grammar up of one level to generate new hypotesis
				list_old.Append(list_new[i]);
			else{
				// the node has at least one successor, i.e.
				// an effective grammar zero level node has been
				// reached  update logprob_acc
				ris_list.Append(list_new[i]);
				}
			}

		l++;
		}
	
	while( l<grammar.Dim() AND list_old.Dim() != 0 );
	return;
	};

void GenericStaticHmm::Append_Lowest_Level_Hypos_To(HypoList & list,
					HypoList& act_level_list, t_index l)
	{
	t_index level, num_suxs, start, i, j;
	t_index lwr_num_suxs;
	t_ptr p_act_gramnode, p_init_lwr_level_gramnode;

	LinkedGraph::Branch next_branch;
	// terminal_hypolist: list for shurely terminal hyponodes
	// 	not to be reexamined at lower level
	HypoList terminal_hypolist, lwr_lev_list;

    terminal_hypolist.Reset();;
    // collect hypotesis till lower level
    for(level=l; level>=1;level--)
		{
		lwr_lev_list.Reset();
		// for each node in actual level list
		for(j=0; j<act_level_list.Dim(); j++)
			{
			start = lwr_lev_list.Dim();
			p_act_gramnode = act_level_list[j].state[level];
			num_suxs = grammar[level].Num_Successors(p_act_gramnode);
			if (num_suxs != 0)
				{
				// not a grammar l level final graph node
				p_init_lwr_level_gramnode=
							grammar[level][p_act_gramnode].info.link;
				//  append to list, grammar l-1 level, initial node successors
				lwr_num_suxs =
				grammar[level-1].Num_Successors(p_init_lwr_level_gramnode);

				for(i=0;i<lwr_num_suxs;i++)
					{
					next_branch =
						grammar[level-1][p_init_lwr_level_gramnode].nexts[i];
					// intialise fields of new hypotesis node copyng state and
					// log_prob_acc accumulated log_probability
					lwr_lev_list.Append(act_level_list[j]);
					// then update fields
					lwr_lev_list[start+i].state[level-1] = next_branch.p_node;
					// skip first dummy state
					lwr_lev_list[start+i].log_prob_acc += next_branch.label;
					}
				}
			else{
				// if a grammar l level final graph node is found simply
				// put in terminal_hypolist ( introduced for efficiency reasons
				// in order to not reexame a terminal node
				terminal_hypolist.Append(act_level_list[j]);
				}
			}  // for j
		act_level_list = lwr_lev_list;
		}  // for level
    
	act_level_list.Append(terminal_hypolist);
    list.Append(act_level_list);
    return;
	};

// Append_Successors_To: append same level actual hypo node successors
void GenericStaticHmm::Append_Successors_To(HypoList& list,
						const HypoNode & base_node,	const t_index l)
	{
	t_index i;
	t_index hyponode_suxs_num, higram_level;
	t_ptr act_gramnode_ptr;

	// *****************************************************************
	// initialisation pass: put actual hypothesis successors in list_old

	act_gramnode_ptr = base_node.state[l];
	higram_level = grammar.Dim()-1;

	// number of successors of actual hypotesis node
	hyponode_suxs_num = grammar[l].Num_Successors(act_gramnode_ptr);
	list.Destroy_And_ReDim(hyponode_suxs_num);
	for(i=0;i<hyponode_suxs_num;i++)
		{
		// initialize state vector and lprob_acc
		list[i] = base_node;
		list[i].state[l] = grammar[l][act_gramnode_ptr].nexts[i].p_node;
		list[i].log_prob_acc += grammar[l][act_gramnode_ptr].nexts[i].label;
		// if followers belong to grammar higher level
		if(l==higram_level)
		     list[i].new_unit =(Boolean)TRUE;
		else list[i].new_unit =(Boolean)FALSE;
		}
	return;
	};


void GenericStaticHmm::Set_To_Silence(HypoNode & act_node)
	{
	// hypotizing that silence is all zeros state, zero all the fields
	// but grammar higher level pointer. The latter would be initialised
	// at index: number of recognition units.
	// This grammar higher level node, is a dummy (strictly) rooth node
	// from wich a transition is possible towards all higher level grammar
	// states with uniform, till now, probability

	act_node.state.Destroy_And_ReDim(grammar.Dim());
	act_node.state[ grammar.Dim()-1 ] = 0;// 44; // fixed to silence
    act_node.new_unit=(Boolean)FALSE;
	act_node.log_prob_acc=0.0;
	return;
	}

// ***********************************************************************
//                   GENERICSTATICHMM::HYPONODE                          *
// ***********************************************************************

Boolean GenericStaticHmm::HypoNode::operator!=(const HypoNode & node) const
	{
	return(Boolean)(state!=node.state); 
	}

Boolean GenericStaticHmm::HypoNode::operator==(const HypoNode & node) const
	{
	return(Boolean)(state==node.state); 
	}

inline Boolean GenericStaticHmm::HypoNode::Is_Worse_Then(const HypoNode & node) const
	{
	return((Boolean)((t_real)log_prob_acc<(t_real)node.log_prob_acc));
	}

inline t_real GenericStaticHmm::HypoNode::Add_To_LProb_Acc(const t_lprob lprob)
	{
	log_prob_acc+=lprob;
	return log_prob_acc;
	};

// ***************************************************************  //
//
//             SPECTRALHMM FUNCTIONS IMPLEMENTATION                 //
//
// **************************************************************** //

void SpectralHmm::Initialise_Structures(const String& config_file, const SegmentOptions& options,
										const DbaseVoc& dbase)
	{
	RecogHmm single_phon_spc_model;
	LinkedGraph act_graph;
	ifstream recog_dataf;

	// parameters which refers to the kind of statistics in recog_dataf
	t_index models_stat_dim; // observation and statistics dimension
	t_index i, num_states=0, num_mix=0;
	t_index	old_cb_dim,	old_graph_dim;
	t_index act_phon, phons_card;
	t_lprob unif_lprob;

	Boolean full_covariance;
	features.Configure(config_file);
	
	// open file of initialisation and training dbase operative
	// parameters and trained phonemes spectral models
		
	recog_dataf.open(options.FileOfAcousticModels, ios::nocreate|ios::in);
	if (recog_dataf.fail())
		merr<<"Cannot open file :"<<options.FileOfAcousticModels;
	Read_Data_File_Header(recog_dataf, models_stat_dim, full_covariance);
	
	win_len=dbase.Window_Lenght();
	
	String pr;
	pr=dbase.Dbase_Name();
	
	frame_shift=win_len-dbase.Window_Overlap();

	phons_card =  dbase.Get_Num_Of_Symbols();
	phonemes_map.Destroy_And_ReDim(phons_card);
	// SpectralHmm is a two level grammar
	// 0 level: acoustic level
	// 1 level: phonemes level
	grammar.Destroy_And_ReDim(2);
	
	for(i=0;i<grammar.Dim();i++)
	    grammar[i].G.Reset();

	grammar[1].G.Destroy_And_ReDim(phons_card+1);
	spectral_model.Reset();

	// uniform grammar higher level transition log_probability
	unif_lprob = log( 1/(t_real)phons_card );
	for(act_phon=0;act_phon<phons_card;act_phon++)
		{
		old_cb_dim = spectral_model.Dim();
		single_phon_spc_model.Configure(act_phon, models_stat_dim, num_states, num_mix);
		single_phon_spc_model.Read(recog_dataf, full_covariance);
		
		num_states=single_phon_spc_model.Dim()+2;
		for(i=0;i<num_states-2;i++)
			{
			spectral_model.Append(single_phon_spc_model[i]);
			num_mix=single_phon_spc_model.num_gauss;
		    for (t_index k=0; k<num_mix; k++)
				spectral_model[old_cb_dim+i][k].full_cov=full_covariance;
			}
		old_graph_dim = grammar[0].G.Dim();
		// store a pointer to the act_phon position in grammar[0]
		phonemes_map[act_phon] =  old_graph_dim;
		act_graph.G.Destroy_And_ReDim(num_states);

		Set_Single_Phon_Graph(single_phon_spc_model.trans_mat, act_graph, 
							  old_cb_dim, old_graph_dim);
		grammar[0].G.Append(act_graph.G);
		// grammar level 1 is a unic completely linked graph of phonemes
		}
	
	recog_dataf.close();
	
	return;
	};

// grammar level 1 ( utterance level) must be reloaded for any new utterance to
// be segmented
void SpectralHmm::Build_Grammar_Over_Phonemes(const VetULong& utter_label_list,
											Boolean sil_opt, const DbaseVoc& dbase)
	{ 
	t_index l, num_labels;

	num_labels=utter_label_list.Dim();
	
	last_level_symbols.Destroy_And_ReDim(num_labels);
	// first and last labels are the phoneme sil (#h, h#)
	// dimensionate one state more than hiw many labels
	grammar[1].G.Destroy_And_ReDim(num_labels);
	dbase.Translate_Symbol(last_level_symbols[0], utter_label_list[0]);
	grammar[1][0].info.link = phonemes_map[utter_label_list[0]];
	// when segmenting only one successor for label
	grammar[1][0].nexts.Destroy_And_ReDim(1);
	grammar[1][0].nexts[0].p_node = 1;
	// transition with prob 1 to the following label
	grammar[1][0].nexts[0].label = 0;

	for(l=1;l<num_labels;l++)
		{
		dbase.Translate_Symbol(last_level_symbols[l], utter_label_list[l]);
		// link grammar level 1 label to the initial dummy node of the corresponding
		// level 0 HMM
		grammar[1][l].info.link = phonemes_map[utter_label_list[l]];

        if (sil_opt AND last_level_symbols[l]=="sil" AND l<num_labels-1)
			{
			grammar[1][l-1].nexts.Save_And_ReDim(2);
			grammar[1][l-1].nexts[1].p_node=l+1;
			grammar[1][l-1].nexts[1].label=0;
			}
		// when segmenting only one successor for label
		grammar[1][l].nexts.Destroy_And_ReDim(1);
		grammar[1][l].nexts[0].p_node = l+1;
		// transition with prob 1 to the following label
		grammar[1][l].nexts[0].label = 0;
		}

	// overwrite on final state branch in order to discard  shorter paths 
	grammar[1][num_labels-1].nexts[0].p_node = num_labels-1;
	// transition with very low prob  to the following label
	grammar[1][num_labels-1].nexts[0].label = -1.0e+010;

 	return;
	}
	  
	
void SpectralHmm::Set_Single_Phon_Graph(const MatrixOfDouble &trans_mat, LinkedGraph& act_graph,
						const t_index old_cb_dim, const t_index old_graph_dim)
	{
	String catchmsg;
	t_index i, j;
	t_index num_states;
	LinkedGraph::Branch act_branch;

	num_states =  act_graph.G.Dim();

	for(i=0;i<num_states;i++)
		{
		act_graph[i].info.obs = 1.0; // dummy field for test aims
		// link each state with its spectral representation
		if(i==0 OR i==(num_states-1))
			{
			// first and last dummy state has no spectrum associate
			act_graph[i].info.link = (t_index)(-1);
			}
		else act_graph[i].info.link = old_cb_dim+i-1;
		
		// read transition probabilities matrix and update partial phoneme graph
		act_graph[i].nexts.Reset();
		for(j=0;j<num_states;j++)
			{
			if(j>=i)
				// only foreward transitions
				if(trans_mat[i][j] > LOGSMALL)
					{
					act_branch.label = trans_mat[i][j];
					act_branch.p_node = old_graph_dim + j;
					act_graph[i].nexts.Append(act_branch);
					}
			} // end forj
		} // end fori
	return;
	};

void SpectralHmm::Dump_Utterance_Segmentation_Res(const GenericStaticHmm::HypoList& decoded_list,
															String& phn_fname)
	{
	t_index	i, tot_sample_num;
	ofstream segm_file;
	String new_phn_fname;
	
	i=0;
	new_phn_fname=phn_fname;
	new_phn_fname << ".sgm";
	segm_file.open(new_phn_fname);
	segm_file<< i <<'\t';

	for( i=1;i<decoded_list.Dim();i++)
		if(decoded_list[i].new_unit==TRUE)
			{
			segm_file<< (frame_shift *  (i+1)) <<'\t';
			segm_file<< last_level_symbols[decoded_list[i-1].state[1]] ;
			segm_file<<'\n';
			segm_file<< (frame_shift * (i+1)) <<'\t';
			// last frame of the previous label
			}
		
	tot_sample_num =   frame_shift * (decoded_list.Dim()-1) + win_len; 
	segm_file << tot_sample_num << '\t' << last_level_symbols[last_level_symbols.Dim()-1];
	segm_file.close();

	return;
	};

void SpectralHmm::Dump_Utterance_Segmentation_Res(const GenericStaticHmm::HypoList& decoded_list,
											  const String& phn_fname)
	{
	t_index	i, tot_sample_num;
	ofstream segm_file;
	
	i=0;
	segm_file.open(phn_fname);
	segm_file<< i <<'\t';
	for( i=1;i<decoded_list.Dim();i++)
		if(decoded_list[i].new_unit)
			{
			segm_file<< (frame_shift * (i+1)) <<'\t';
			segm_file<< last_level_symbols[decoded_list[i-1].state[1]] ;
			segm_file<<'\n';
			segm_file<< (frame_shift * (i+1)) <<'\t';
			// last frame of the previous label
			}

	tot_sample_num =   frame_shift * (decoded_list.Dim()-1) + win_len; 
	segm_file << tot_sample_num << '\t' << last_level_symbols[last_level_symbols.Dim()-1];
	segm_file.close();

	return;
	};

void SpectralHmm::Dump_Utterance_Segmentation_Ares(const GenericStaticHmm::HypoList& decoded_list,
											String& phn_fname, t_index sample_rate)
	{
	t_index	i;
	t_index tot_sample_num;
	t_index tot_label_num=decoded_list.Dim();
	t_index first_sample, mean_sample, last_sample;

	ofstream segm_file;
	String new_phn_fname;

	new_phn_fname=phn_fname;
	new_phn_fname<<".sgm";

	segm_file.open (new_phn_fname);
	if (segm_file.fail())
		merr<<"Cannot open file "<<new_phn_fname;

	tot_sample_num=frame_shift * (decoded_list.Dim()-1) + win_len;

	//write header of file
	segm_file<<"FIL: Label"<<endl;
	segm_file<<"SGM: "<<sample_rate<<endl;
	segm_file<<"BEG: 0"<<endl;
	segm_file<<"END: "<<tot_sample_num<<endl;
	segm_file<<"CMT: File con la segmentazione automatica"<<endl;

	first_sample=0;
	for (i=1; i<tot_label_num; i++)
		if (decoded_list[i].new_unit)
		    {
			segm_file<<"LBB: "<<first_sample<<',';
			last_sample=frame_shift * (i+1);
			mean_sample = (last_sample+first_sample)/2;
			segm_file<<mean_sample<<','<<last_sample<<','
		           <<last_level_symbols[decoded_list[i-1].state[1]]<<endl;
			first_sample=last_sample;
			} //end of if (decoded_list[i].new_unit)
		
    segm_file<<"LBB: "<<first_sample<<',';
	mean_sample = (tot_sample_num+first_sample)/2;
	segm_file<<mean_sample<<','<<tot_sample_num<<','
	          <<last_level_symbols[last_level_symbols.Dim()-1]<<endl;
	segm_file<<"ELF:"<<endl;

	segm_file.close();
	return;
	}

void SpectralHmm::Dump_Utterance_Segmentation_Ares(const GenericStaticHmm::HypoList& decoded_list,
											const String& phn_fname, t_index sample_rate)
	{
	t_index	i;
	t_index tot_sample_num;
	t_index tot_label_num=decoded_list.Dim();
	t_index first_sample, mean_sample, last_sample;

	ofstream segm_file;
	String new_phn_fname;

	segm_file.open (phn_fname);
	if (segm_file.fail())
		merr<<"Cannot open file "<<new_phn_fname;

	tot_sample_num=frame_shift * (decoded_list.Dim()-1) + win_len;

	//write header of file
	segm_file<<"FIL: Label"<<endl;
	segm_file<<"SAM: "<<sample_rate<<endl;
	segm_file<<"BEG: 0"<<endl;
	segm_file<<"END: "<<tot_sample_num<<endl;
	segm_file<<"CMT: File con la segmentazione automatica"<<endl;

	first_sample=0;
	for (i=1; i<tot_label_num; i++)
		if (decoded_list[i].new_unit)
			{
			segm_file<<"LBB: "<<first_sample<<',';
			last_sample=frame_shift * (i+1);
			mean_sample = (last_sample+first_sample)/2;
			segm_file<<mean_sample<<','<<last_sample<<','
		           <<last_level_symbols[decoded_list[i-1].state[1]]<<endl;
			first_sample=last_sample;
			} //end of if (decoded_list[i].new_unit)
		
	segm_file<<"LBB: "<<first_sample<<',';
	mean_sample = (tot_sample_num+first_sample)/2;
	segm_file<<mean_sample<<','<<tot_sample_num<<','
	          <<last_level_symbols[last_level_symbols.Dim()-1]<<endl;
	segm_file<<"ELF:"<<endl;

	segm_file.close();
	return;
	}


Boolean SpectralHmm::Get_Observation(Boolean& new_file, DbaseVoc& dbase)
	{
	t_signed i;
	VetDouble vetsmp;
	VetDoubleList pred_list;
	Boolean not_end_of_dbase;

	not_end_of_dbase = dbase.Get_Sequential_Vet(vetsmp, new_file);
	if(new_file)
		{
		pred_list.Destroy_And_ReDim(features.Max_Delta_Feature_Order());
		for(i=(pred_list.Dim()-1);i>=0;i--)
			pred_list[i]=vetsmp;
		features.Get_Previous_Frames_Info(pred_list, dbase.Smp_Rate());
  		}

	if(not_end_of_dbase)
  		features.Extract(observation, vetsmp, dbase.Smp_Rate());

    return (Boolean)not_end_of_dbase;
	};

//LogAdd: Return sum x + y on log scale, sum < LOGSMALL is floored to LZERO
t_lprob SpectralHmm:: LogAdd(t_lprob x, t_lprob y)const
	{
	t_lprob temp;
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

t_lprob SpectralHmm::Obs_LProb(const t_ptr p_node_spc) const
	{
	t_lprob obs_lprob, aux_obs_lprob;
	t_index i;

	// compute observation log_probability by mixture distribution
	obs_lprob = spectral_model[p_node_spc][0].Evaluate_Exp_Gauss(observation)
	 	+ spectral_model[p_node_spc][0].gConst
			+ spectral_model[p_node_spc][0].weight;

	for(i=1; i<spectral_model[p_node_spc].Dim(); i++)
		{
		// spectrum of num_cluster-th state and num_mix-th mixture
		aux_obs_lprob = spectral_model[p_node_spc][i].Evaluate_Exp_Gauss(observation)
			+ spectral_model[p_node_spc][i].gConst
			+ spectral_model[p_node_spc][i].weight;
		obs_lprob = LogAdd(obs_lprob, aux_obs_lprob);
		Assert(obs_lprob<=0);
		}
	return obs_lprob;
	};


// ***********************************************************************
//									*
//		       Dynamic structure				*
//									*
// ***********************************************************************




// **********************************************************************
//									                                    *
//							third layer                                 *
//										                                *
// **********************************************************************

//we hypotize that one node can't generate twice tha same node

t_index HypoTree::Actives_Dim() const
	{
	return tree.Leaves_Dim();
	}

t_index HypoTree::Candidates_Dim() const
	{
	return tree.Kids_Dim();
	}

const GenericStaticHmm::HypoNode & HypoTree::Get_Candidate(p_candidate num)
	{
	return(tree.Get_Kid_Info((p_kid) num));
	}

GenericStaticHmm::HypoNode& HypoTree::Get_Active(p_candidate num)
	{
	return(tree.Get_Leaf_Info((p_kid)num ));
	}

 void HypoTree::Prune_All_Dead_Active()
	{
	tree.Prune_All_Dead_Leaf();
	return;
	}

	//check if adds an already present node, if so select the best one
Boolean HypoTree::Check_Candidate_Presence_And_Get_Num(const GenericStaticHmm::HypoNode & info,
					p_candidate & num_candidate)
	{
	return tree.Check_Kid_Presence_And_Get_Num(info,
						      (p_kid&)num_candidate);
	}

Boolean HypoTree::Check_Candidate_Presence_And_Get_Num(VetInt& candidate_map,
			p_candidate & num_candidate,const GenericStaticHmm::HypoNode & info)
	{
    // if candidate is present get its index in candidates dir
	num_candidate=candidate_map[info.state[0]];
	return((Boolean)(num_candidate < unsigned(-1)));
	}


//following func calls WellTree.read_node_info(const & t_node node);
Boolean HypoTree::Node_Is_Worse_Then_Candidate(const GenericStaticHmm::HypoNode  & node,
					p_candidate  & num_candidate)
	{
	return node.Is_Worse_Then(Get_Candidate(num_candidate));
	}

void HypoTree::Create_First_Active_Of_Tree(const GenericStaticHmm::HypoNode & node)
	{
	tree.Create_First_Leaf_Of_Tree(node);
	return;
	};

void HypoTree::Add_Candidate_To_Active_Num(const GenericStaticHmm::HypoNode & node,
							p_active num_active)
	{
	tree.Add_Kid_To_Leaf(node,num_active);
	return;
	}

//following funcs call the same
//prune_branch_from_node in WellTree class
void HypoTree::Prune_Branch_From_Active(p_active num_active)
	{
	tree.Prune_Blind_Branch_From_Leaf((p_leaf) num_active);
	return;
	}

void HypoTree::Subst_Old_Cand_Destroy_Old_Branch_Ins_New(p_candidate old_kid,
		const  GenericStaticHmm::HypoNode & info,p_active new_father)
	{
	tree.Subst_Old_Kid_Destroy_Old_Branch_Ins_New((p_kid)old_kid,
					info,(p_leaf)new_father);
	return;
	}

void HypoTree::Transform_Next_Generation_In_Actives()
	{
	tree.Next_Gen();
	return;
	}

void HypoTree::UpDate_Actives_LProb_Acc(VetDouble& scores,
							const GenericStaticHmm& hmm)
	{
	t_index i, p_gramnode;
	t_lprob obs_lprob;

	// GenericStaticHmm::HypoNode active;
	scores.Destroy_And_ReDim(Actives_Dim());
	// since normally we would have a number of actives much greater
	// than window width we look for the best win_width actives
	// if(win_width>=rubbish)
	// find best win_width elements
	for(i=0;i<Actives_Dim();i++)
		{
		p_gramnode= Get_Active(i).state[0];
		obs_lprob = hmm.Obs_LProb(hmm.grammar[0][p_gramnode].info.link);
		scores[i] = Get_Active(i).Add_To_LProb_Acc(obs_lprob);
		}
	return;
	};

void HypoTree::Window_Search(VetDouble& scores,
					const t_index max_num_activate)
	{
	t_index i,j, best_idx, ord_elems;
	t_lprob max_score;

	// cycle to determine the max_num_activate best actives
	// and put it at the beginning of the actives list

	if(max_num_activate<Actives_Dim())
		ord_elems=max_num_activate;
	else ord_elems=Actives_Dim();

	for(i=0;i<ord_elems;i++)
		{
		best_idx=i;
		max_score = scores[i];
		for (j=i+1;j<scores.Dim();j++)
			{
			if(scores[j]>max_score)
				{
				best_idx = j;
				max_score = scores[j];
				}
			}
		scores[best_idx]= scores[i];
		// unusefull to save higher score;
		scores[i]=max_score;
		Exchange_Actives_Indexes(i, best_idx);
		}   // endfor i

	// at this point best max_num_activates hyponodes at
	// first max_num_activates  places of the list of actives
	return;
	};

void HypoTree::Exchange_Actives_Indexes(const t_index i, const t_index j)
	{
	tree.Exchange_Leaves_Indexes(i, j);
	return;
	}	

void HypoTree::ReDim_Tree(const t_index max_dim_tree,
						const t_index max_num_activate)
	{
	t_index ix;

	ix = Actives_Dim();
	while((tree.Num_Elements() > max_dim_tree
		OR ix > max_num_activate ) AND
		(ix > 1)) // don't prune if only one path
		// ix is the actual number of actives
		// actives are rwemoved all together asst the enfd
                // of the while cycle
		{                               
		ix--;
		Prune_Branch_From_Active(ix);
		}//Next pruning if necessary
	Reduce_Actives_To(ix);
	return;
	}

void HypoTree::Reduce_Actives_To(const t_index ix)
	{
	tree.ReDim_Leaves_Dir_To(ix);
	return;
	};

p_active HypoTree::Worse_Active()
	{
	p_active i;
	p_active worse;
	for(i=1,worse=0;i<Actives_Dim();i++)
		{
		if(!Get_Active(i).Is_Worse_Then(Get_Active(worse)))
			worse=i;
		}
	return worse;
	}


p_active HypoTree::Best_Active()
	{
	p_active i;
	p_active best;
	for(i=1,best=0;i<Actives_Dim();i++)
		{
		if(Get_Active(best).Is_Worse_Then(Get_Active(i)))
			best=i;
		}
	return best;
	}

//Backtrack_from active returns a new list sequence
//with every element
//containing nodes info of Viterbi best path sequence
void HypoTree::Backtrack_From(GenericStaticHmm::HypoList & sequence,
							p_active active)
	{
	tree.Backtrack_From( sequence,(p_leaf)active);
	return;
	}

// Viterbi implementation with faster search algorithm for candidate presence
// Known problems: at the moment we suppose that the grammar has not common
// state

Boolean HypoTree::Viterbi(GenericStaticHmm::HypoList & sequence,t_index max_num_activate,
												t_index max_dim_tree, Boolean& new_file, 
											GenericStaticHmm& hmm, DbaseVoc& dbase)
	{
    t_index ix, iy;
    t_index existing_candidate;

    GenericStaticHmm::HypoNode node_att;
    GenericStaticHmm::HypoList l_maybe_can;
    VetDouble scores;
    Boolean not_end_of_dbase=(Boolean)TRUE; 	// necessary when Viterby function
				 								// cycles, because it looses the file
												// not ended value shurely true; 

    new_file = (Boolean)FALSE; // to get rid of condition on while cycle
	// zero step: completely separate treatment for silence hypo
    if(tree.Num_Elements()==0)
	    {
	    hmm.Set_To_Silence(node_att);

		Create_First_Active_Of_Tree(node_att);
	    not_end_of_dbase = hmm.Get_Observation(new_file, dbase);

	    if(not_end_of_dbase)
			{
			hmm.Observed_Hypo_List_From_Silence(l_maybe_can, Get_Active(0));
			// here l_maybe_can wouldn't contain duplicated hyponodes
			for(iy=0;iy<l_maybe_can.Dim();iy++) // for each destination node
				// (possible candidate)
		    	Add_Candidate_To_Active_Num(l_maybe_can[iy], 0);
				// endfor iy end candidate construction of one active
			Transform_Next_Generation_In_Actives();
			UpDate_Actives_LProb_Acc(scores, hmm);
			Window_Search(scores, max_num_activate);
			// pruning of tree if excessive candidate or nodes
			ReDim_Tree(max_dim_tree, max_num_activate);
			not_end_of_dbase = hmm.Get_Observation(new_file, dbase);
			}
		}

    while ( not_end_of_dbase AND !new_file AND (tree.Num_Elements() < max_dim_tree))
		{
		for(ix=0;ix<Actives_Dim();ix++) // for each active hypo
			{
			// build the list of destination nodes
			hmm.Observed_Hypo_List_From_This_Node(l_maybe_can, Get_Active(ix));
			for(iy=0;iy<l_maybe_can.Dim();iy++) // for each destination node
			// (possible candidate)
				{
				if ( Check_Candidate_Presence_And_Get_Num(l_maybe_can[iy],existing_candidate))
					{
					//node_presence_handling();
					//if node is better then candidate
					if (!Node_Is_Worse_Then_Candidate(l_maybe_can[iy],existing_candidate))
						{
						Subst_Old_Cand_Destroy_Old_Branch_Ins_New(
							existing_candidate,	l_maybe_can[iy],(p_active)ix);
						}
					}
				else  Add_Candidate_To_Active_Num(l_maybe_can[iy],ix);
				}// endfor iy end candidate construction of one active
			}// endfor ix candidates list building

		Prune_All_Dead_Active();
		Transform_Next_Generation_In_Actives();
		// more efficient implementation of the path search in
		// a max_num_actives() window
		UpDate_Actives_LProb_Acc(scores, hmm);
		// tipically the most frequent action would be pruning for reduction of search
		// width (more than pruning in order to satisfy max_num_nodes condition) so
		// priority for efficient implemantation of window search
		Window_Search(scores, max_num_activate);
		// pruning of tree if excessive candidate or nodes
		ReDim_Tree(max_dim_tree, max_num_activate);
		//if real time required we can
		//1) backtrak every n seconds
		//2) do a new structure that memorize num of node a t certain tim k
		//if num=1 then immortal node
		not_end_of_dbase = hmm.Get_Observation(new_file, dbase);
		}  //  endwhile;  next time slot
    // FINAL STEP
    // The input sequence terminates and we recover the decoded state sequence
    // by navigating backward along the HypothesisList from the BestElement (which
    // has the highest value in likelyhood_field )
    Backtrack_From(sequence, Best_Active() );
    if (new_file)		// beginning of a new sentence
		{
        tree.Reset();
		}
    else{
    	node_att=Get_Active(Best_Active());
      	tree.Reset();
 		Create_First_Active_Of_Tree(node_att);
		}

    return(Boolean) not_end_of_dbase;
	};

void HypoTree::Beam_Search(VetDouble& scores, t_real& threshold,
					const t_real beam_coeff)
	{
	t_index j;
	t_index best_idx;
	t_index num_actives=Actives_Dim();
	t_lprob max_score;

	Assert(scores.Dim()==num_actives);

	// cycle to determine the best active
	best_idx=0;
	max_score = scores[0];
	for (j=1; j<num_actives; j++)
		if (scores[j]>max_score)
			{
			best_idx = j;
			max_score = scores[j];
			}

    //compute the beam threshold
	threshold = log(beam_coeff) + max_score;
	                                      
	return;
	}

//pruning for Viterbi beam-search
void HypoTree::ReDim_Tree(const t_real threshold)
	{
	p_active ix, i;
	t_index num_actives;
	t_index last_active_idx;
	
	num_actives=Actives_Dim();

    GenericStaticHmm::HypoNode act_node;

    // for each active hypothesis node
	for (i = num_actives;i>0; i--)
		{
		ix=i-1;
		// if the logprob accumulate is lower of beam threshold
		// prune the hypothesis node
		act_node=Get_Active(ix);
		if (act_node.Log_Prob_Acc()<threshold)
			{
			Prune_Branch_From_Active(ix);
			last_active_idx = Actives_Dim()-1;
			Exchange_Actives_Indexes(last_active_idx, ix);
			Reduce_Actives_To(last_active_idx);
			}
		} //end of or each hypothesis node

	return;
	}


Boolean HypoTree::Viterbi_Beam(GenericStaticHmm::HypoList & sequence,t_real beam_coeff,
					Boolean& new_file, GenericStaticHmm& hmm, DbaseVoc& dbase)
	{
    t_index ix, iy;
    t_index existing_candidate;
	t_real threshold;

    GenericStaticHmm::HypoNode node_att;
    GenericStaticHmm::HypoList l_maybe_can;

    VetDouble scores;

    Boolean not_end_of_dbase=(Boolean)TRUE; 	// necessary when Viterby function
				 								// cycles, because it looses the file
												// not ended value shurely true; 

    new_file = (Boolean)FALSE; // to get rid of condition on while cycle
    // zero step: completely separate treatment for silence hypo
    if(tree.Num_Elements()==0)
		{
	    hmm.Set_To_Silence(node_att);

		Create_First_Active_Of_Tree(node_att);
	    not_end_of_dbase = hmm.Get_Observation(new_file, dbase);

	    if(not_end_of_dbase)
			{
			hmm.Observed_Hypo_List_From_Silence(l_maybe_can, Get_Active(0));
			// here l_maybe_can wouldn't contain duplicated hyponodes
            
			for(iy=0;iy<l_maybe_can.Dim();iy++) // for each destination node
				// (possible candidate)
				Add_Candidate_To_Active_Num(l_maybe_can[iy], 0);
				// endfor iy end candidate construction of one active
			Transform_Next_Generation_In_Actives();
			UpDate_Actives_LProb_Acc(scores, hmm);
			Beam_Search(scores, threshold, beam_coeff);
			ReDim_Tree(threshold);
			not_end_of_dbase = hmm.Get_Observation(new_file, dbase);
			}
		}

    while ( not_end_of_dbase AND !new_file )
		{
		for(ix=0;ix<Actives_Dim();ix++) // for each active hypo
			{
			// build the list of destination nodes
			hmm.Observed_Hypo_List_From_This_Node(l_maybe_can, Get_Active(ix));
			for(iy=0;iy<l_maybe_can.Dim();iy++) // for each destination node
				// (possible candidate)
				{
				if ( Check_Candidate_Presence_And_Get_Num(l_maybe_can[iy],existing_candidate))
					{
					//node_presence_handling();
					//if node is better then candidate
					if (!Node_Is_Worse_Then_Candidate(l_maybe_can[iy],existing_candidate))
						{
						Subst_Old_Cand_Destroy_Old_Branch_Ins_New(
							existing_candidate, l_maybe_can[iy],(p_active)ix);
						}
					}
				else  Add_Candidate_To_Active_Num(l_maybe_can[iy],ix);
				}// endfor iy end candidate construction of one active
			}// endfor ix candidates list building

		Prune_All_Dead_Active();
		Transform_Next_Generation_In_Actives();

		// more efficient implementation of the path search in
		// a max_num_actives() window

		UpDate_Actives_LProb_Acc(scores, hmm);
		Beam_Search(scores, threshold, beam_coeff);
		ReDim_Tree(threshold);

		//if real time required we can
		//1) backtrak every n seconds
		//2) do a new structure that memorize num of node a t certain tim k
		//if num=1 then immortal node
		not_end_of_dbase = hmm.Get_Observation(new_file, dbase);
		}  //  endwhile;  next time slot

    // FINAL STEP
    // The input sequence terminates and we recover the decoded state sequence
    // by navigating backward along the HypothesisList from the BestElement (which
    // has the highest value in likelyhood_field )
    Backtrack_From(sequence, Best_Active() );
    if (new_file)		// beginning of a new sentence
		{
        tree.Reset();
		}
    else{
    	node_att=Get_Active(Best_Active());
     	tree.Reset();
 		Create_First_Active_Of_Tree(node_att);
		}

	return(Boolean) not_end_of_dbase;
	};

