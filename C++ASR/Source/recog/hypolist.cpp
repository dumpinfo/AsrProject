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
//  |   FUNCTIONALITY: implementation graph and hmm                     |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 11/05/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 28/4/95                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// 24/5/98 added gpp compatibility

//
// 
// ********************************************************************
// hypolist.cpp
// graph and hmm implementation
// vers 14/7/95
// by Claudio  & Massimo & Antonio

// vers. 22/4/1996
// introduced third and fourth grammar level in order to perform 
// word recognition with trigram grammar
// both decoding algorithm versions allowed: window search and beam
// search

#include "../recog/hypolist.h"
#include "../recog/hypolist.hpp"

// *************************************************************
//
//					Class for HMM models
//
// *************************************************************

void RecogCluster::operator= (const RecogCluster & source)
	{
    weight=source.weight;
	mean=source.mean;
    gConst=source.gConst;
    if (source.full_cov)
	 inv_cov=source.inv_cov;
	else diag_inv_cov=source.diag_inv_cov;

    return;
	}

//  *******************************************************************
//                                                                    *
//              GENERIC STATIC HMM FUNCTIONS IMPLEMENTATION           *
//                                                                    *
//  *******************************************************************


void GenericStaticHmm::Observed_Hypo_List_From_Silence (HypoList & ris_list,
					 								const HypoNode & silence)
	{
    t_index i;
	t_index grammar_hilevel;
	t_index ris_list_dim;

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
	
	ris_list_dim=ris_list.Dim();

	// first model of the resulting list
	for (i=0; i<ris_list_dim; i++)
		ris_list[i].new_unit = (Boolean)TRUE; 
	  

	// in ris_list surely only valid hypoes 
	// (i.e. no starting either ending nodes)
	return;
	}


void GenericStaticHmm::Observed_Hypo_List_From_This_Node(
			   HypoList & ris_list, const HypoNode & act_hyponode)
	{
	t_index i;
	t_index p_gramnode;
	t_index l;
	t_index list_old_dim;
	t_index list_new_dim;

	// hypolist structures for grammar scanning
	HypoList  act_hyponode_list;
	HypoList list_old;
	HypoList list_new;

	// verify that hypotesis cardinality is equal
	//to num of grammar level
	Assert (act_hyponode.state.Dim()==grammar.Dim());

	// code with initial and final dummy states treatment
	// new iterative function version

	// initialisation pass: put actual hypothesis in list_old
	list_old.Destroy_And_ReDim(1);
	list_old[0] = act_hyponode;
	ris_list.Reset();
	l=0;
	do	{
		list_old_dim=list_old.Dim();
		list_new.Reset();

		for (i=0; i<list_old_dim; i++)
			{
			// hypolist for level l list_old[i] followers
			act_hyponode_list.Reset();

			Append_Successors_To(act_hyponode_list, list_old[i], l);
			Append_Lowest_Level_Hypos_To(list_new, act_hyponode_list, l);
		    } // end of for i
		
		list_old.Reset();
		list_new_dim=list_new.Dim();

		// scan list_new in order to locate valid hypotesis
		for (i=0; i<list_new_dim; i++)
			{
			p_gramnode = list_new[i].state[0];
			if (grammar[0][p_gramnode].nexts.Dim() == 0)
			// is an ending dummy state: it's necessary climbing
			// the grammar up of one level to generate new hypotesis
				list_old.Append(list_new[i]);

			else
		
				// the node has at least one successor, i.e.
				// an effective grammar zero level node has been
			   // reached  update logprob_acc
		       ris_list.Append(list_new[i]);
		
			} // end of for i

			l++;
		}
	while( l<grammar.Dim() AND list_old.Dim() != 0 );
	return;
	}


void GenericStaticHmm::Append_Lowest_Level_Hypos_To(HypoList & list,
					                  HypoList & act_level_list, t_index l)
	{
    t_index i, j;
	t_index level;
	t_index num_suxs;
	t_index start;
	t_index dim;
	t_index lwr_num_suxs;
	t_ptr p_act_gramnode;
	t_ptr p_init_lwr_level_gramnode;

	LinkedGraph::Branch next_branch;

	// terminal_hypolist: list for shurely terminal hyponodes
	// 	not to be reexamined at lower level
	HypoList terminal_hypolist;
	HypoList lwr_lev_list;

	//pay attention
	//grammar[l] select the linked graph at level l
	//grammar[l] [j]   select the jth node in the
	//			linked graph at level l
	//base_node.state[l] gives the pointer to node of
	// 			the graphic at level l

    terminal_hypolist.Reset();

    // collect hypotesis till lower level
    for (level=l; level>=1;level--)
		{
		lwr_lev_list.Reset();
		dim=act_level_list.Dim();

		// for each node in actual level list
		for (j=0; j<dim; j++)
			{
			start = lwr_lev_list.Dim();
			p_act_gramnode = act_level_list[j].state[level];

			num_suxs = grammar[level].Num_Successors(p_act_gramnode);

			if (num_suxs != 0)
				{
				// not a grammar l level final graph node
				p_init_lwr_level_gramnode = grammar[level][p_act_gramnode].info;

				//	append to list, grammar l-1 level, initial node successors
				lwr_num_suxs = grammar[level-1].Num_Successors(p_init_lwr_level_gramnode);

				for (i=0; i<lwr_num_suxs; i++)
					{
					next_branch = grammar[level-1][p_init_lwr_level_gramnode].nexts[i];

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
	
			}  // for each node in actual level list

		act_level_list = lwr_lev_list;

		}  // for each level

    act_level_list.Append(terminal_hypolist);

    list.Append(act_level_list);

    return;
	}


// Append_Successors_To: append same level actual hypo node successors
void GenericStaticHmm::Append_Successors_To(HypoList& list,
						const HypoNode & base_node,	const t_index l)
	{
	t_index i;
	t_index hyponode_suxs_num, higram_level;
	t_ptr act_gramnode_ptr;

	//pay attention
	//grammar[l] select the linked graph at level l
	//grammar[l] [j]   select the jth node in the
	//linked graph at level l
	//base_node.state[l] gives the pointer to node of
	//the graphic at level l

    // initialisation pass: put actual hypothesis successors in list_old

	act_gramnode_ptr = base_node.state[l];
	higram_level = grammar.Dim()-1;

	// number of successors of actual hypothesis node
	hyponode_suxs_num = grammar[l].Num_Successors(act_gramnode_ptr);
	list.Destroy_And_ReDim(hyponode_suxs_num);

    // for each successor of actual hypothesis node
	for (i=0; i<hyponode_suxs_num; i++)
		{
		// initialize state vector and update lprob_acc
		list[i] = base_node;
		list[i].state[l] = grammar[l][act_gramnode_ptr].nexts[i].p_node;
		list[i].log_prob_acc += grammar[l][act_gramnode_ptr].nexts[i].label;
		// if followers belong to grammar higher level
		if (l==higram_level)
		    list[i].new_unit =(Boolean)TRUE;
		else list[i].new_unit =(Boolean)FALSE;
		} // end of for each successor of actual hypothesis node

	return;
	}

void GenericStaticHmm::Set_To_Silence(HypoNode & act_node, const t_index sil_index)
	{
	act_node.state.Destroy_And_ReDim(grammar.Dim());
	act_node.state[ grammar.Dim()-1 ] =sil_index;
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
	}

// ************************************************************************  
//                                                                        *
//             class SPECTRALHMM: FUNCTIONS IMPLEMENTATION                * 
//                                                                        *
// ************************************************************************

void SpectralHmm::Configure(const RecogOptions & choice, DbaseVoc & dbase, 
                                             t_index & sil_index, const String & config_f_name)
	{
  	t_index stat_dim;
	t_index phons_card;
	t_index words_card;
	t_index classif_card;
	t_index i;
  	VetULong phons_map;
	VetULong words_map;
	Boolean is_full;
	ifstream symb_stat_dataf;

    //read the header of acoustic models file    
	symb_stat_dataf.open(choice.file_of_acoustic_models, ios::in|ios::nocreate);
	if (symb_stat_dataf.fail())
	  merr<<"Cannot open file :"<<choice.file_of_acoustic_models;
    Read_Data_File_Header(symb_stat_dataf, stat_dim, is_full);

    feature.Configure(config_f_name);

    //load acoustic models of phonemes
  	Load_Acoustic_Level(symb_stat_dataf, stat_dim, phons_map, phons_card, dbase, is_full, choice.skip_th);

	if (choice.grammar == WordGrammar)
   		{
		Load_Word_Level(choice.file_of_vocabulary, phons_map, words_card, words_map, dbase, sil_index);
		if (choice.word_class==NoClassif)
			{
			if (choice.grammar_type == Bigram)
			Bigram_Model_Wrd_Level (choice.file_of_bigram, words_card, words_map, sil_index, choice.penalty, choice.penalty_sil, choice.bigr_th, choice.desc_gr_const);
			else // if(choice.grammar_type == NoGrammar)
				 Uniform_Grammar (words_map, 2, words_card, choice.penalty);
			}  // end of if (choice.word_class==NoClassif)
		else // if (choice.word_class==YesClassif)
			{ 
		    // load the word classes
			Load_Word_Classes (choice.file_of_classes, classif_card, words_map, sil_index);
        
			// load the matrix of bigram classes transitions
			Load_Class_Transitions (choice.file_of_bigram_classes, classif_card, words_map.Dim(), choice.penalty, choice.penalty_sil);
			}
   		} //end of if (choice.grammar == WordGrammar)

    else //if(choice.grammar == PhonGrammar)
   		{
		if (choice.grammar_type == NoGrammar)
			Uniform_Grammar (phons_map, 1, phons_card, choice.penalty);
		else //if(choice.grammar_type == BiGram)
			{
			Set_Silence_Index(sil_index, choice.grammar, choice.grammar_type);
			Phoneme_Level_Grammar (choice.file_of_phonemes_bigram, phons_map, dbase, choice.penalty, choice.penalty_sil, sil_index);
			}
		} 

    if (choice.word_class==NoClassif OR choice.grammar==PhonGrammar )
		Set_Silence_Index(sil_index, choice.grammar, choice.grammar_type);

    states.Destroy_And_ReDim(grammar[0].G.Dim());
	for ( i=0; i<states.Dim(); i++)
		states[i].Create(FALSE, 0.0);
	
	return ;
	}


//load grammar of level 0 : phonemes acoustic models
void SpectralHmm::Load_Acoustic_Level(ifstream & symb_stat_dataf, const t_index & stat_dim, 
							 VetULong & phons_map, t_index & phons_card,  DbaseVoc & dbase,
							 Boolean is_full, t_real skip_th)
	{
	RecogCodebook single_phon_spc_model;
    LinkedGraph act_graph;
    LinkedGraph::Branch act_branch;

	t_index i, j;
	t_index num_states=0;
	t_index	num_mix=0;
	t_index	old_cb_dim;
	t_index	old_graph_dim;
	t_index act_phon;
	String trans_fname;

	// open file containing initialisation and training dbase operative
	// features and trained phonemes spectral models

	phons_card =  dbase.Get_Num_Of_Symbols();
	top_level_symbols.Destroy_And_ReDim(phons_card);

	// SpectralHmm is a two level grammar
	// 0 level: acoustic level
	// 1 level: phonemes level

	grammar.Destroy_And_ReDim(1);
    grammar[0].G.Reset();

	spectral_model.Reset();
	phons_map.Destroy_And_ReDim(phons_card);

	for (act_phon=0; act_phon<phons_card; act_phon++)
		{
		dbase.Translate_Symbol (top_level_symbols[act_phon], act_phon);
		old_cb_dim = spectral_model.Dim();

		//read the acoustic model of actual phoneme
		single_phon_spc_model.Configure(act_phon, stat_dim, num_states, num_mix);
		single_phon_spc_model.Read(symb_stat_dataf, is_full);
	   
		//first and last states aren't present in single_phon_spc_model
		num_states=single_phon_spc_model.Dim()+2;

		//append the acoustic states (not dummy) of actual phoneme on static strusture
		for (i=0; i<num_states-2; i++)
			{
			spectral_model.Append(single_phon_spc_model[i]);
			num_mix=single_phon_spc_model.num_gauss;
			for (t_index k=0; k<num_mix; k++)
				spectral_model[old_cb_dim+i][k].full_cov=is_full;
			}

		//update index of first acoustic state of actual phoneme
		old_graph_dim = grammar[0].G.Dim();
		phons_map[act_phon]=old_graph_dim;

		//append the acoustic states of actual phoneme on grammar[0]
		act_graph.G.Destroy_And_ReDim(num_states);
       
		//set single phon graph
		for (i=0; i<num_states; i++)
			{
			//act_graph[i].info.obs = 1.0;//dummy field for test aim
			//link each state with its spectral representation
			//first and last dummy states haven't spectrum associate
			if (i==0 OR i==num_states-1)
				act_graph[i].info = t_index (-1);
			else act_graph[i].info=old_cb_dim+i-1;

			//update matrix of transitions probabilities
			act_graph[i].nexts.Reset();
			//only foward transitions
			for (j=i; j<num_states; j++)
				{
				if (j>i+1)
					{ // skipping arcs
					t_real pr=single_phon_spc_model.trans_mat[i][j];
					if (single_phon_spc_model.trans_mat[i][j]>skip_th)
						{
						act_branch.label = single_phon_spc_model.trans_mat[i][j];
						act_branch.p_node = old_graph_dim+j;
						act_graph[i].nexts.Append(act_branch);
						}   
					}
				else if (single_phon_spc_model.trans_mat[i][j]>LOGSMALL)
						{
						act_branch.label = single_phon_spc_model.trans_mat[i][j];
						act_branch.p_node = old_graph_dim+j;
						act_graph[i].nexts.Append(act_branch);
						}
				}
			}  //end of for each state

		grammar[0].G.Append(act_graph.G);
		}

    symb_stat_dataf.close();
    return;
	}

 
 //Load_Word_Level load grammar of level 1 : the phonemes
 void SpectralHmm::Load_Word_Level(const String & vocab_f_name, const VetULong & phons_map,
				    t_index & words_card,VetULong & words_map,DbaseVoc & dbase, t_index & sil_index)
	{
	t_index k,i;
	t_index num_phons;
	t_index index;
	t_index act_word;
	t_index old_word_graph_dim;
	t_index num_pronounce;

	String new_phon;
	String str_in, str_aux;
	String str_space;
	String catchmsg;
	String phn;

  	VetULong init_map,fin_map;
  	VetDouble prob_pronounce;

  	char ch;
	char line[__MAX_LEN_LINE];

  	LinkedGraph::Branch act_branch;
  	LinkedGraph word_graph;

   	ifstream file_in;

	//**********************************************************************
	// ONLY FOR DEBUG
	ImpObjectList<String> phoneme_symb;
	phoneme_symb=top_level_symbols;
	//**********************************************************************/

   	// allocate mem for word level
   	grammar.Save_And_ReDim(2);
	// reset level of phonetic transcription of words
  	grammar[1].G.Reset();

	// open file of vocabulary 
  	file_in.open(vocab_f_name, ios::in|ios::nocreate);
	if (file_in.fail())
		merr<<"Cannot open input file of vocabulary : "<<vocab_f_name;

    //check if is a correct file
   	file_in>>str_in;
   	if (str_in!="Corpus_dimension:")
		merr<<"Wrong input file of vocabulary : "<<vocab_f_name;	
     
    //read the corpus dimension
	file_in>>k;

	file_in>>str_in;
	if (str_in!="Vocabulary_cardinality:")
		merr<<"Wrong input file of vocabulary : "<<vocab_f_name;	

    //read the vocabulary cardinality
   	file_in>>words_card;

   	top_level_symbols.Destroy_And_ReDim(words_card);
	 
	// list of pointers to initial dummy states of the words
   	words_map.Destroy_And_ReDim(words_card);

    do	{
		Check(!file_in.eof(),"Wrong input file of vocabulary");
   		file_in>>str_in;
		}
   	while (str_in!="Phonetic_transcriptions_of_words:");

    //read the commentary line
	file_in.getline(line, __MAX_LEN_LINE);
	  
    init_map.Set_Data_Chunk(20);
	fin_map.Set_Data_Chunk(20);
	prob_pronounce.Set_Data_Chunk(20);
	word_graph.G.Set_Data_Chunk(20);
	grammar[1].G.Set_Data_Chunk(50);
	
	//for each word of vocabulary
	for (act_word=0; act_word<words_card; act_word++)
		{
	 	old_word_graph_dim=grammar[1].G.Dim();
		// read the actual word ...
	 	file_in>>top_level_symbols[act_word];
        // ... the number of actual word pronounces
	 	file_in>>num_pronounce;
		// first effective state of each different pronunciation
		init_map.Save_And_ReDim(num_pronounce);
	 	// last effective state of each different pronunciation
	 	fin_map.Save_And_ReDim(num_pronounce);
	 	// array of probabilities of each different pronunciation 
	 	prob_pronounce.Save_And_ReDim(num_pronounce);
		//allocate memory for the first dummy state of the actual word
	 	grammar[1].G.Save_And_ReDim(old_word_graph_dim+1);
	 	grammar[1][old_word_graph_dim].info=(t_index(-1));
		
		// for each different pronounce
		for (i=0; i<num_pronounce; i++)
			{
			init_map[i]=grammar[1].G.Dim();
			//read the number of phonemes of actual pronounce
			file_in>>num_phons;
			//read the log probability of actual pronounce
			file_in>>prob_pronounce[i];

			word_graph.G.Save_And_ReDim(num_phons);
			// read the first slash of phonetic transcription	
			file_in>>ch;
           
			Check(ch=='/',"Wrong input file of vocabulary");

			// for each phonemes of actual pronounce
	 		for (k=0; k<num_phons; k++)
				{
				phn.Reset();
				file_in>>ch;
				// phoneme reading while cycle
				while(ch!='/')
					{
					phn.Append(ch);
					file_in>>ch;
					}
				phn.Append('\0');
				new_phon=phn;
			
				index=dbase.Translate_Symbol(new_phon);
				word_graph[k].info=phons_map[index];
				word_graph[k].nexts.Reset();

				// transition towards successive state with 0 log probability
				act_branch.label=0;
	  			act_branch.p_node=init_map[i]+k+1;
				word_graph[k].nexts.Append(act_branch);
		 		} //end of for each phonemes

	 		//append the graph of each pronunciation
			//of the actual word act_word
			grammar[1].G.Append(word_graph.G);
			fin_map[i]=grammar[1].G.Dim()-1;

	 		} //end of for each different pronounce

		//update the list of pointers to initial dummy states of the words
		words_map[act_word]=old_word_graph_dim;
	  	grammar[1][old_word_graph_dim].nexts.Reset();

		// update the field 'nexts' of the initial dummy states of the words
	  	for (k=0; k<num_pronounce; k++)
	  		{
			act_branch.p_node=init_map[k];
			act_branch.label=prob_pronounce[k];
			grammar[1][old_word_graph_dim].nexts.Append(act_branch);
	  		}

		//allocate mem for the last dummy state of the word act_word
	  	grammar[1].G.Save_And_ReDim(grammar[1].G.Dim()+1);
	  	grammar[1][grammar[1].G.Dim()-1].info=(t_index(-1));

		// up-to-date the field 'nexts' of the last effective state of
		// each different pronunciation
	  	for (k=0; k<num_pronounce; k++)
	  		{
			grammar[1][fin_map[k]].nexts.Destroy_And_ReDim(1);
			grammar[1][fin_map[k]].nexts[0].p_node=grammar[1].G.Dim()-1;
			grammar[1][fin_map[k]].nexts[0].label=0;
			}

		if (top_level_symbols[act_word]=="silence")
			sil_index=act_word;
		} // end of for each word of vocabulary

    return;
	}

//load grammar of level 2 : the words
void SpectralHmm::Bigram_Model_Wrd_Level(const String & f_bigram, const t_index words_card,
				           const VetULong& words_map, const t_index& sil_index, 
						   const t_real penalty, const t_real penalty_sil, const t_real bigr_th,
						   const t_real desc_gr_const)
	{
	t_index temp;
	String str_in;
	t_index j;
	t_index act_word;
	
	t_real bigr_lprob;
	LinkedGraph::Branch act_branch;

	ifstream file_in;

	// open file of grammar to read
	//  transition probabilities matrix
	file_in.open(f_bigram, ios::in|ios::nocreate);
	if (file_in.fail())
		merr<<"Cannot open input file : "<<f_bigram;

	//allocate mem for grammar between words
	grammar.Save_And_ReDim(3);
	grammar[2].G.Destroy_And_ReDim(words_card);

	file_in>>str_in;
	if (str_in!="Bigram_transitions_file")
		merr<<"Wrong input file of bigram transitions : "<<f_bigram;

	file_in>>str_in;
	if (str_in!="vocabulary_dimension:")
		merr<<"Wrong file of bigram transitions : "<<f_bigram;

	file_in>>temp;
	if (temp!=words_card)
		merr<<"Wrong file of bigram transitions : "<<f_bigram;

    //for each word of vocabulary
	for (act_word=0; act_word<words_card; act_word++)
		{
		//pointer to lower level
		grammar[2][act_word].info=words_map[act_word];

		grammar[2][act_word].nexts.Reset();

		file_in>>str_in;
		if (str_in!=top_level_symbols[act_word])
			merr<<"Worng input bigram transitions file : "<<f_bigram;

		//load the transition probabilities between words
		//reading matrix
		for (j=0; j<words_card; j++)
			{
			file_in>>bigr_lprob;

			if (bigr_lprob>bigr_th)
				{
				t_index dim=top_level_symbols[j].Dim();
				if (j==sil_index)
					act_branch.label=bigr_lprob*penalty_sil;
				else act_branch.label=bigr_lprob*penalty/log((t_real)dim+desc_gr_const);
				act_branch.p_node=j;
				grammar[2][act_word].nexts.Append(act_branch);
				}
			}
		} // end of for each word of vocabulary
 	file_in.close();
 	return;
	}


void SpectralHmm::Uniform_Grammar(const VetULong & units_map, const t_index level,
				                          const t_index units_card, const t_real penalty)
	{
	LinkedGraph::Branch act_branch;

   	t_real unif_lprob;
   	t_index act_unit,i;

	// allocate mem for uniform grammar level
	// between words (if index_level is 2)
	// or between phonemes (if index_level is 1)
   	grammar.Save_And_ReDim(level+1);
   	grammar[level].G.Destroy_And_ReDim(units_card);

   	for (act_unit=0; act_unit<units_card; act_unit++)
   		{  
		//pointer to lower level
		grammar[level][act_unit].info=units_map[act_unit];
		grammar[level][act_unit].nexts.Reset();
		unif_lprob=log(1.0/(t_real)units_card)*penalty;
		for (i=0; i<units_card; i++)
	 		{
			act_branch.p_node=i;
			act_branch.label=unif_lprob;
			grammar[level][act_unit].nexts.Append(act_branch);
	 		}
   		}
    
	return;
   }


//Phoneme_Level_Grammar loads grammar of level 1 (between phonemes)
//if there is not word level
void SpectralHmm::Phoneme_Level_Grammar(const String & trans_f_name,
					const VetULong & phons_map, DbaseVoc & dbase, 
					const t_real penalty, const t_real penalty_sil, t_index sil_index)
	{
	t_index i;
	t_index act_phon;
	t_index phons_card;
    t_index ist_num;
	t_index trans_num;
	t_real trans_lprob;
	t_real trans_prob;

    String str_in;
	LinkedGraph::Branch act_branch;
    ifstream trans_file;
	
	//open phoneme level grammar file
	trans_file.open(trans_f_name, ios::in|ios::nocreate);
	if (trans_file.fail())
		merr<<"Cannot open file of phonemes bigram : "<<trans_f_name;

	phons_card=dbase.Get_Num_Of_Symbols();

	//allocate mem for phoneme level
	grammar.Save_And_ReDim(2);
	grammar[1].G.Destroy_And_ReDim(phons_card);

    // reading first row
    trans_file>>str_in;
	if (str_in=="Phonemes_occurrences_and_transitions")
		{
		// for each possible starting phoneme
		for (act_phon=0; act_phon<phons_card; act_phon++)
			{
			// reading symbol
			trans_file>>str_in;
	
			// number of symbol istances
			trans_file>>ist_num;

			grammar[1][act_phon].info=phons_map[act_phon];
			grammar[1][act_phon].nexts.Reset();
	
			// for each possible target phoneme
  			for (i=0; i<phons_card; i++)
				{
				trans_file>>trans_num;

				if (trans_num!=0)
					{
					if (i==sil_index)
						trans_lprob = (log((t_real)trans_num/(t_real)ist_num))*penalty_sil;
					else trans_lprob = (log ((t_real)trans_num/(t_real)ist_num))*penalty;

					act_branch.label = trans_lprob;
					act_branch.p_node = i;
					grammar[1][act_phon].nexts.Append(act_branch);
					}
				} // end of for each possible target phoneme
			} // end of for each possible starting phoneme
		}
	else if (str_in!="Phonemes_occurrences_and_transitions_prob")
				merr<<"Error in the file this phonetic grammar";
		 else	{
				// for each possible starting phoneme
				for (act_phon=0; act_phon<phons_card; act_phon++)
					{
					// reading symbol
					trans_file>>str_in;
					// number of symbol istances
					trans_file>>ist_num;

					grammar[1][act_phon].info=phons_map[act_phon];
					grammar[1][act_phon].nexts.Reset();
	
    				// for each possible target phoneme
     				for (i=0; i<phons_card; i++)
						{
    					trans_file>>trans_prob;
						if (i==sil_index)
							trans_lprob = (log(trans_prob))*penalty_sil;
						else trans_lprob = (log (trans_prob))*penalty;

						act_branch.label = trans_lprob;
						act_branch.p_node = i;

						grammar[1][act_phon].nexts.Append(act_branch);
						}
					} // end of for each possible target phoneme
				} // end of for each possible starting phoneme
	trans_file.close();
	return;
	} 

void SpectralHmm::Load_Word_Classes (const String & classes_f_name, t_index & classif_card,
										const VetULong & words_map, t_index & sil_index)
	{ 		
	t_index i,k;
	t_index class_number;
	t_index class_card;
	t_index act_word;
	t_index	word_card=words_map.Dim();

	String new_word;
	String str_in;

	ifstream file_in;

    Assert (word_card==top_level_symbols.Dim());

	grammar.Save_And_ReDim(3);
	grammar[2].G.Destroy_And_ReDim(word_card);

	//for each word of vocabulary
	for (act_word=0; act_word<word_card; act_word++)
		{
		//pointer to lower level:level of phonetic transcription
		grammar[2][act_word].info=words_map[act_word];
		//each word is linked to a node that represent the class of the word
		grammar[2][act_word].nexts.Destroy_And_ReDim(1);
		}
		
	//open file of grammar to read classification
	file_in.open(classes_f_name, ios::in|ios::nocreate);
	if (file_in.fail())
		merr<<"Cannot open input file of classification :"<<classes_f_name;

	file_in>>str_in;
	if (str_in!="Classification_Cardinality:")
		merr<<"Wrong input file of classification";

	//load classification cardinality
	file_in>>classif_card;
	grammar[2].G.Save_And_ReDim(word_card+classif_card*2);

    //for each class
	for (i=0; i<classif_card; i++)
		{	
		do
			file_in>>str_in;
		while (str_in!="Class_Number:" AND !file_in.eof());

		if (file_in.eof())
			merr<<"Wrong input file of classification";

		file_in>>class_number;
			
		file_in>>str_in;
		if (str_in!="Class_Cardinality:")
			merr<<"Wrong input file of classification";

		file_in>>class_card;
		
		// classification first class number is 0	
		grammar[2][word_card+2*class_number].nexts.Destroy_And_ReDim(class_card);

		//for each word of the actual class
		for (k=0; k<class_card; k++)
			{
			file_in>>new_word;
		  
			act_word=0;
			while (top_level_symbols[act_word]!=new_word AND act_word<word_card)
		  		act_word++;	

			Check(act_word<word_card, "Word "<<new_word<<" isn't present in top level symbol");
						 
			grammar[2][word_card+2*class_number].nexts[k].p_node=act_word;
			
			t_real temp;
			file_in>>temp;
			grammar[2][word_card+2*class_number].nexts[k].label=0.0;
			grammar[2][act_word].nexts[0].p_node=word_card+2*class_number+1;
			grammar[2][act_word].nexts[0].label=0;
			} // end of for each word of the actual class

		grammar[2][word_card+2*class_number].info=(t_index(-1));
		grammar[2][word_card+2*class_number+1].info=(t_index(-1));
			
		} // end of for each class
	 
	//the first class is a class for the "silence" word
	sil_index=0;
    return;
	}

 
void SpectralHmm::Load_Class_Transitions (const String & bigram_class_file,
								const t_index classif_card, const t_index word_card,
								const t_real penalty, const t_real penalty_sil)
	{		
    String str_in;
	t_index j;
	t_index act_class;
	t_index c_card;

    t_real bigr_lprob;

	LinkedGraph::Branch act_branch;

	ifstream file_in;
		
	// open file of grammar to read transition probabilities matrix
	file_in.open(bigram_class_file, ios::in|ios::nocreate);
	if (file_in.fail())
		merr<<"Cannot open file of bigram classes transitions : "<<bigram_class_file;

	//allocate mem for grammar between words
	grammar.Save_And_ReDim(4);
	grammar[3].G.Destroy_And_ReDim(classif_card);

    //read the header of the file of bigram classes
	file_in>>str_in;
    if (str_in!="Bigram_classes_transitions_file")
		merr<<"Wrong input file of bigram classes transitions : "<<bigram_class_file;
	file_in>>str_in;
	if (str_in!="number_of_classes:")
		merr<<"Wrong input file of bigram classes transitions : "<<bigram_class_file;
	file_in>>c_card;
	if (c_card!=classif_card)
		merr<<"Wrong input file of bigram classes transitions : "<<bigram_class_file;

    //for each class
	for (act_class=0; act_class<classif_card; act_class++)
		{
		String check;
		check<<act_class<<')';
		file_in>>str_in;
		if (str_in!=check)
			merr<<"Wrong input file of bigram classes transitions : "<<bigram_class_file<<" at class number "<<act_class;
		//pointer to lower level
		grammar[3][act_class].info=word_card+2*act_class;
		grammar[3][act_class].nexts.Reset();
	   
		//load the transition probabilities between words reading matrix
		for (j=0; j<classif_card; j++)
			{
			file_in>>bigr_lprob;				 
			if (bigr_lprob!=1.0)
				{
			    if (j==0)
					act_branch.label=bigr_lprob*penalty_sil;
				else act_branch.label=bigr_lprob*penalty;

				act_branch.p_node=j;
				grammar[3][act_class].nexts.Append(act_branch);
				}
			}
		} // end of for each class
			
	file_in.close();
	return;
	}


void SpectralHmm::Set_Silence_Index(t_index & sil_index, 
                                    const StaticGrammarLevel & grammar_level,
									const StaticGrammarType & grammar_type)
	{
	String temp;
	t_index j;
	t_index num_sym;

	num_sym=top_level_symbols.Dim();

	if (grammar_level == PhonGrammar)
		temp = "sil";
	else if(grammar_level == WordGrammar)
			temp = "silence";
	
	j=0;
	while (j<num_sym AND top_level_symbols[j] != temp)
		j++;

	if (j==num_sym)
		merr<<"Not found symbol silence phoneme";

	sil_index=j;
	
	return;
	}		

Boolean SpectralHmm::Get_Observation (Boolean& new_file, DbaseVoc& dbase)
	{ 
	t_index i;
	VetDouble vetsmp;
	VetDoubleList pred_list;
	Boolean not_end_of_dbase;

	not_end_of_dbase = dbase.Get_Sequential_Vet(vetsmp, new_file);

	if (new_file)
	    {
		pred_list.Destroy_And_ReDim(feature.Max_Delta_Feature_Order());
	 
		for (i=(pred_list.Dim()-1);i!=(t_index)(-1);i--)
			pred_list[i]=vetsmp;

		feature.Get_Previous_Frames_Info(pred_list, dbase.Smp_Rate());
  		}

	if (not_end_of_dbase)
		feature.Extract(observation, vetsmp, dbase.Smp_Rate());

	return (Boolean)not_end_of_dbase;
	}


//LogAdd: Return sum x + y on log scale, sum < LOGSMALL is floored to LZERO
t_lprob SpectralHmm:: LogAdd(t_lprob x, t_lprob y)const
	{
	t_lprob temp;

	if (x<y)
		{
		temp = x;
		x = y;
		y = temp;
		}

	temp = y-x;

	if (temp<MINLOGEXP)
		return (x<LOGSMALL) ? LOGZERO : x;
	else
		return x+log((double)1.0+ exp(temp));
   }


t_lprob SpectralHmm::Obs_LProb(const t_ptr p_node_spc) const
	{ 
	t_lprob obs_lprob;
	t_lprob aux_obs_lprob;
	t_index i, lim;

	// compute observation log_probability by mixture distribution
	obs_lprob = spectral_model[p_node_spc][0].Evaluate_Exp_Gauss (observation)
	 	+ spectral_model[p_node_spc][0].gConst
			+ spectral_model[p_node_spc][0].weight;

    lim=spectral_model[p_node_spc].Dim();

	for (i=1; i<lim; i++)
		{
		// spectrum of num_cluster-th state and num_mix-th mixture
		aux_obs_lprob = spectral_model[p_node_spc][i].Evaluate_Exp_Gauss(observation)
			+ spectral_model[p_node_spc][i].gConst
			+ spectral_model[p_node_spc][i].weight;
	   
		obs_lprob = LogAdd(obs_lprob, aux_obs_lprob);
		//Assert(obs_lprob<=0);

		}
	return obs_lprob;
	}


// **************************************************************
//																*
//		       			struttura DINAMICA						*
//																*
// **************************************************************

// *********************************************************************
//                                                                     *
//                terzo livello                                        *
//                                                                     *
// *********************************************************************

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
	return (tree.Get_Leaf_Info((p_kid)num ));
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
	return tree.Check_Kid_Presence_And_Get_Num(info, (p_kid &) num_candidate);
	}


Boolean HypoTree::Check_Candidate_Presence_And_Get_Num(VetULong& candidate_map,
				 	p_candidate & num_candidate,
					const GenericStaticHmm::HypoNode & info)
	{
	// if candidate is present get its index in candidates dir
	num_candidate=candidate_map[info.state[0]];

	return((Boolean)(num_candidate < t_index(-1)));
	}


Boolean HypoTree::Node_Is_Worse_Then_Candidate(const GenericStaticHmm::HypoNode  & node,
						p_candidate  & num_candidate)
	{
	return node.Is_Worse_Then(Get_Candidate(num_candidate));
	}


void HypoTree::Create_First_Active_Of_Tree(const GenericStaticHmm::HypoNode & node)
	{
	tree.Create_First_Leaf_Of_Tree(node);
	return;
	}


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

void HypoTree::UpDate_Actives_LProb_Acc (VetDouble & scores, GenericStaticHmm & hmm)
	{
    t_index i;
	t_index p_gramnode;
	t_lprob obs_lprob;
	t_index num_actives=Actives_Dim();
    
	scores.Destroy_And_ReDim(num_actives);

	for (i=0; i<num_actives; i++)
		{
		p_gramnode= Get_Active(i).state[0];

		if (hmm.states[p_gramnode].First_Elem())
			{
			Assert (hmm.grammar[0][p_gramnode].info!=(t_index) -1);
			obs_lprob = hmm.states[p_gramnode].Second_Elem();
			}
		else {
			obs_lprob = hmm.Obs_LProb(hmm.grammar[0][p_gramnode].info);
			hmm.states[p_gramnode].Create(TRUE, obs_lprob);
			}
		scores[i] = Get_Active(i).Add_To_LProb_Acc(obs_lprob);
		}

	for (i=0; i<hmm.states.Dim(); i++)
		hmm.states[i].Create (FALSE, 0.0);

    return;
	}


void HypoTree::Window_Search(VetDouble& scores, const t_index max_num_activate)
	{
	t_index i,j;
	t_index best_idx;
	t_index ord_elems;
	t_index num_actives=Actives_Dim();

	t_lprob max_score;

	if (max_num_activate<num_actives)
		ord_elems=max_num_activate;
	else
		ord_elems=num_actives;

    // cycle to determine the max_num_activate best actives
	// and put it at the beginning of the actives list
	for (i=0; i<ord_elems; i++)
		{	
		best_idx=i;
		max_score = scores[i];
		for (j=i+1;j<scores.Dim();j++)
	 		{
			if (scores[j]>max_score)
				{
				best_idx = j;
				max_score = scores[j];
				}	
			} // end of for j
	   
		scores[best_idx]= scores[i];
		// unusefull to save higher score;
		scores[i]=max_score;

		Exchange_Actives_Indexes(i, best_idx);

		}   // end of for i

	// at this point best max_num_activates hyponodes at
	// first max_num_activates  places of the list of actives
	return;
	}


void HypoTree::Beam_Search(VetDouble& scores, t_real& threshold_init,
					t_real & threshold_internal, const t_real beam_init,
					const t_real beam_internal)
	{
	t_index j;
	t_index best_idx;
	t_index num_actives=Actives_Dim();
	t_lprob max_score;

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
	threshold_init = log(beam_init) + max_score;
	threshold_internal=log(beam_internal) + max_score;
                                      
	return;

	}


void HypoTree::Exchange_Actives_Indexes(const t_index i, const t_index j)
	{
	tree.Exchange_Leaves_Indexes(i, j);
	return;
	} 


// pruning for Viterbi window-search
void HypoTree::ReDim_Tree(const t_index max_dim_tree,
						const t_index max_num_activate)
	{
	t_index ix;

	ix = Actives_Dim();
	while((tree.Num_Elements() > max_dim_tree 
			OR ix > max_num_activate ) AND (ix > 1)) 
		// don't prune if only one path
		// ix is the actual number of actives
		// actives are rwemoved all together asst the enfd
        // of the while cycle
		{                               
		ix--;
		Prune_Branch_From_Active(ix);
		}  //end of while

	Reduce_Actives_To(ix);

	return;
	} 


//pruning for Viterbi beam-search
void HypoTree::ReDim_Tree(const t_real threshold_init, const t_real threshold_internal)
	{
	p_active ix;
	t_index num_actives;
	t_index last_active_idx;
	
	num_actives=Actives_Dim();

    GenericStaticHmm::HypoNode act_node;

    // for each active hypothesis node
	for (ix = num_actives-1;ix!=t_index(-1); ix--)
		{
		// if the logprob accumulate is lower of beam threshold
		// prune the hypothesis node
		act_node=Get_Active(ix);
		if ((act_node.new_unit AND act_node.Log_Prob_Acc()<threshold_init)
	       OR (NOT(act_node.new_unit) AND act_node.Log_Prob_Acc()<threshold_internal))
			{
			Prune_Branch_From_Active(ix);
			last_active_idx = Actives_Dim()-1;

			Exchange_Actives_Indexes(last_active_idx, ix);

			Reduce_Actives_To(last_active_idx);
			}
		} //end of or each hypothesis node

	return;
	}


void HypoTree::Reduce_Actives_To(const t_index ix)
	{
	tree.ReDim_Leaves_Dir_To(ix);
	return;
	}

p_active HypoTree::Worse_Active()
	{
	p_active i;
	p_active worse;
	
	for (i=1, worse=0; i<Actives_Dim(); i++)
		{
		if (!Get_Active(i).Is_Worse_Then(Get_Active(worse)))
			worse=i;
		}

	return worse;
	}	


p_active HypoTree::Best_Active()
	{
	p_active i;
	p_active best;

	for (i=1, best=0; i<Actives_Dim(); i++)
		{
		if(Get_Active(best).Is_Worse_Then(Get_Active(i)))
			best=i;
		}
	
	return best;
	}


//Backtrack_from active returns a new list sequence
//with every element containing nodes info of Viterbi best path sequence
void HypoTree::Backtrack_From(GenericStaticHmm::HypoList & sequence,
							p_active active)
	{
	tree.Backtrack_From( sequence,(p_leaf)active);

	return;
	}


// Viterbi with exaustive search for candidate presence and window search
Boolean HypoTree::Viterbi_Window(GenericStaticHmm::HypoList & sequence,
              t_index max_num_activate, t_index max_dim_tree,Boolean & new_file, 
			  GenericStaticHmm& hmm,const t_index sil_index, DbaseVoc& dbase)
	{
    t_index ix, iy;
    t_index existing_candidate;
	t_index pos_cand;
	t_index num_actives;
	static count_file=1;

    // necessary when Viterby function cycles, 
	// because it looses the file not ended value shurely true; 
    Boolean not_end_of_dbase=(Boolean)TRUE; 
	
    GenericStaticHmm::HypoNode node_att;
    GenericStaticHmm::HypoList l_maybe_can;

    VetDouble scores;

 
    // to get rid of condition on while cycle
    new_file = (Boolean)FALSE; 

    // zero step: completely separate treatment for silence hypo
    if (tree.Num_Elements()==0)
		{
		hmm.Set_To_Silence(node_att, sil_index);

		Create_First_Active_Of_Tree(node_att);
		
		not_end_of_dbase = hmm.Get_Observation(new_file, dbase);

		if (not_end_of_dbase)
			{												
			hmm.Observed_Hypo_List_From_Silence(l_maybe_can, Get_Active(0));
		
			// here l_maybe_can wouldn't contain duplicated hyponodes

			// for each destination node (possible candidate)
			for (iy=0; iy<l_maybe_can.Dim(); iy++) 
				Add_Candidate_To_Active_Num(l_maybe_can[iy], 0);
		
			Transform_Next_Generation_In_Actives();
			UpDate_Actives_LProb_Acc(scores, hmm);
			Window_Search(scores, max_num_activate);
			// pruning of tree if excessive candidate or nodes
			ReDim_Tree(max_dim_tree, max_num_activate);
			not_end_of_dbase = hmm.Get_Observation(new_file, dbase);
			}  // end of if (not_end_dbase)  
		}  // end of if (tree.Num_Elements()==0)
    

    // from each actives node build the canddates list
    while ( not_end_of_dbase AND !new_file AND
		tree.Num_Elements() < max_dim_tree )
		{
		num_actives=Actives_Dim();
		// for each active hypo
		for (ix=0; ix<num_actives; ix++) 
			{
			// build the list of destination nodes
			hmm.Observed_Hypo_List_From_This_Node(l_maybe_can,
		         			Get_Active(ix));
	    
			pos_cand=l_maybe_can.Dim();

     		// for each destination node (possible candidate)
			for (iy=0; iy<pos_cand; iy++) 
				{
	    		if( Check_Candidate_Presence_And_Get_Num(
			 		l_maybe_can[iy], existing_candidate))
					{
					//node presence handling
					//if node is better then candidate
					if (!Node_Is_Worse_Then_Candidate(l_maybe_can[iy],
							existing_candidate))		      
					Subst_Old_Cand_Destroy_Old_Branch_Ins_New(existing_candidate,
							l_maybe_can[iy],(p_active)ix);
					} // end of if (Check_Candidate_Presence_And_Get_Num())
				else Add_Candidate_To_Active_Num(l_maybe_can[iy],ix);

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

		not_end_of_dbase = hmm.Get_Observation(new_file, dbase);

		}  //  end of while;  next time slot

    // FINAL STEP

    // The input sequence terminates and we recover the decoded state sequence
    // by navigating backward along the HypothesisList from the BestElement (which
    // has the highest value in likelyhood_field )

    Backtrack_From(sequence, Best_Active() );

    //eliminates last node because is part of the next
    //sequence.Save_And_ReDim(sequence.Dim()-1);

    if (new_file)
		{//reset tree so that at the next iteration the Viterbi tree
		// will start from silence
		tree.Reset();
		mstat<<"End of processing file number "<<count_file;
		count_file++;
		}
    else{
		node_att=Get_Active(Best_Active());
		tree.Reset();
		Add_Candidate_To_Active_Num(node_att, 0);
		}

    return(Boolean) not_end_of_dbase;
   	}


//Viterbi with exaustive search for candidate presence and beam search
Boolean HypoTree::Viterbi_Beam(GenericStaticHmm::HypoList & sequence, const t_real beam_init,
            const t_real beam_internal,
			t_index max_dim_tree, Boolean& new_file, GenericStaticHmm& hmm,
			const t_index sil_index, DbaseVoc& dbase)
	{
    t_index ix, iy;
    t_index existing_candidate;
	t_index pos_cand;
	t_index num_actives;
   
	static count_file=1;

    t_real threshold_init;
	t_real threshold_internal;

    // necessary when Viterby function cycles, 
	// because it looses the file not ended value shurely true; 
    Boolean not_end_of_dbase=(Boolean)TRUE; 
	
    GenericStaticHmm::HypoNode node_att;
    GenericStaticHmm::HypoList l_maybe_can;

    VetDouble scores;

 
    // to get rid of condition on while cycle
    new_file = (Boolean)FALSE; 

    // zero step: completely separate treatment for silence hypo
    if (tree.Num_Elements()==0)
		{
		hmm.Set_To_Silence(node_att,sil_index);

		Create_First_Active_Of_Tree(node_att);
		not_end_of_dbase = hmm.Get_Observation(new_file, dbase);

		if (not_end_of_dbase)
			{												
			hmm.Observed_Hypo_List_From_Silence(l_maybe_can, Get_Active(0));
		
			// here l_maybe_can wouldn't contain duplicated hyponodes
		    // for each destination node (possible candidate)
		for (iy=0; iy<l_maybe_can.Dim(); iy++) 
		  Add_Candidate_To_Active_Num(l_maybe_can[iy], 0);
		
		Transform_Next_Generation_In_Actives();

		UpDate_Actives_LProb_Acc(scores, hmm);

		Beam_Search(scores, threshold_init, threshold_internal, beam_init, beam_internal);

		// pruning of tree if excessive candidate or nodes

		ReDim_Tree(threshold_init, threshold_internal);

		not_end_of_dbase = hmm.Get_Observation(new_file, dbase);

		}  // end of if (not_end_dbase)  
    }  // end of if (tree.Num_Elements()==0)
    
    t_index oss=1;
    // from each actives node build the canddates list
    while ( not_end_of_dbase AND !new_file /*AND
		tree.Num_Elements() < max_dim_tree */)
		{
		oss++;
		num_actives=Actives_Dim();
	 
		// for each active hypo
		for (ix=0; ix<num_actives; ix++) 
			{
			// build the list of destination nodes
			hmm.Observed_Hypo_List_From_This_Node(l_maybe_can,
		         			Get_Active(ix));
	    
			pos_cand=l_maybe_can.Dim();
	
			// for each destination node (possible candidate)
			for (iy=0; iy<pos_cand; iy++) 
				{
	    		if ( Check_Candidate_Presence_And_Get_Num(
			 		l_maybe_can[iy], existing_candidate))
					{
					//node presence handling
					//if node is better then candidate
					if (!Node_Is_Worse_Then_Candidate(l_maybe_can[iy],
							existing_candidate))		      
						Subst_Old_Cand_Destroy_Old_Branch_Ins_New(existing_candidate,
									l_maybe_can[iy],(p_active)ix);
					} // end of if (Check_Candidate_Presence_And_Get_Num())
				else Add_Candidate_To_Active_Num(l_maybe_can[iy],ix);

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
		Beam_Search(scores, threshold_init, threshold_internal, beam_init, beam_internal);

		// pruning of tree if excessive candidate or nodes
		ReDim_Tree(threshold_init, threshold_internal);

		not_end_of_dbase = hmm.Get_Observation(new_file, dbase);

		}  //  end of while;  next time slot

    // FINAL STEP

    // The input sequence terminates and we recover the decoded state sequence
    // by navigating backward along the HypothesisList from the BestElement (which
    // has the highest value in likelyhood_field )

    Backtrack_From(sequence, Best_Active() );

    //eliminates last node because is part of the next
    //sequence.Save_And_ReDim(sequence.Dim()-1);

    if (new_file)
		{
		//reset tree so that at the next iteration the Viterbi tree
		// will start from silence
		tree.Reset();
		mstat<<"End of processing file number "<<count_file;
		count_file++;
		}
    else{
		node_att=Get_Active(Best_Active());
		tree.Reset();
		Add_Candidate_To_Active_Num(node_att, 0);
		}

    return(Boolean) not_end_of_dbase;
   
	}

