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
//  |   FILE:   Lessico.h                                               |
//  |   FUNCTIONALITY: language models                                  |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico		                                    |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 23/1/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________


// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//28/05/98
//added class  OrderedList Domenico 

#ifndef __LESSICO_H_
#define __LESSICO_H_

#include "../vetclas/vetclas.h"
#include "../vetclas/vetclas.hpp"
#include "../vetclas/arraycla.h"
#include "../vetclas/arraycla.hpp"
#include "../baseclas/textclas.h"
#include "../lessico/lexopt.h"


#include <fstream.h>   

template <class T, class E>
class Couple
	{
		T elem_1;
		E elem_2;

    public:
       
		void operator= (const Couple & c) 
			{ 
			elem_1=c.elem_1;
			elem_2=c.elem_2;
			return;
			}

		void Create (const T & first, const E & second)
			{
			elem_1=first;
			elem_2=second;
			return;
			}        			 	 

		T First_Elem() { return elem_1; }
		E Second_Elem() { return elem_2; }

	};

    
typedef Couple <t_index, t_index> TIndexCouple;
typedef Couple <t_real, t_real> TRealCouple;
typedef Couple <Boolean, t_real> BooleanTRealCouple;

     
//Phonetic transcriptions of a single word
class PhnsList 
	{

	public:

		//phonetic transcription
		ImpObjectList<String> phn;

		//occurence of this pronounce
		t_index occur;
   
	};
 
//class for storing the information about a word
class WordInfo
	{
	public:

		//occurence of word
		t_index occur_word;
	 
		//unigram probability 
		t_real unigram_lprob;

		//phonetic transcriptions number
		t_index occur_pron; 

		//phonetic transcriptions list
		ImpObjectList<PhnsList> phns_transc; 
  
		//bigram_transitions vector
		VetULong bigram_transitions;

		//bigram's logprobability vector
		VetDouble bigram_lprob;

		//if automatic clustering
		TIndexCouple belonging_class;

	};
   
//each object of this class is a class of words result of automatic clustering
class AutomWordClass
	{

	public:

		//class transitions
		VetULong class_transitions;
     
		//bigram class probabilities 
		VetDouble class_prob;

		//mean vector of the class
		VetDouble mean_vector;

		//list of class's words 
		VetULong words;

		//prbability of the words of the classe
		VetDouble words_prob;

		//class unigram probabilities;
		t_real unig_prob;

	};     
	  

class Lexicon
	{
	protected:

   //********************************* data *********************************************

		//class of silence word
		t_index sil_class;
     
		//vocabulary
		OrderedList<String , WordInfo> vocab;

		//list of classes for automatic clustering
		ImpObjectList<AutomWordClass> autom_classes;

		//dimension of training corpus
		t_index corpus_dim;
  
		//list of files on training or compute perplexity
		ImpObjectList<String> file_list;

		//label cardinality
		t_index card_label;

		//bigram transition between phonemes
		MatrixOfULong bigram_trans;

		//occurrence of phonemes
		VetULong occur_phon;

		//phonemes list
		ImpObjectList<String> phon_list;

		//label of phonemes
		VetULong label;

	 
   //*************** general service functions ******************************************


		//read from file of vocabulary the logprobability of silence
		void Read_Silence_Logprob (const String & f_name);

		//create a list of files
		void Load_File_List (const String & name_file_list);

		//return the index of phonemes
		t_index Index_Of_Phoneme (const String & phoneme, const LabelType & label_t); 

		void Update_Phoneme_List (const t_index & card_label);

	//************** load phonetic transcriptons functions *******************************

		//load phonetic transcription of word
		virtual void Load_Phonetic_Transcriptions (const String & f_phon)=0;

	//************** functions for automatic clustering of words *************************

		//initialize word classes
		void Initialize_Word_Classes (t_index & sil_index);

		//compute the mean vector of the candidates classes
		void Compute_Mean_Vector (t_index first, t_index second, VetDouble & mean);

		//compute the square error of the new class
		t_real Compute_Error (t_index first, t_index second, const VetDouble & mean);
	  
		//merge first and second classes in a single class
		void Merge_Classes (ImpObjectList<TIndexCouple>& couples_to_merge, t_index & dim);

		//automatic clustering of word
		void Clustering_Words (t_index max_class_card, t_index sil_index);

		//at which class belong each word
		void Update_Word_To_Classes ();

		//compute the probability of the words in the class
		void Compute_Prob_Words ();

	
   //************** functions for calculation of probability ****************************

		//calculate the unigram logprobability
		void Update_Unigram_Probability ();

		//calculate the logprobability with smoothing technique
		void Calculate_Bigram_Log_Probability (const LexiconOptions & lexic_opt);

		//compute the maximun likelood classes bigram probabilities 
		void Compute_Class_Bigram_Probabilities ();

		//calculate the logprobability with smoothing techinque
		void Calculate_Class_Bigram_Log_Probability (const LexiconOptions & lexic_opt);

	//************** function for load language model ************************************

		//load from file lexic_options.filr_bigram the bigram language model 
		//void Load_Bigram_Model (const String & f_name_bigram); 

	//************** functions for writing file output ***********************************

		//write the file with vocabulary and phonetic transcription of each word
		virtual void Write_File_Of_Vocabulary (const String & f_name)=0;

		//write the file with the logprobability of unigram
		void Write_File_Unigram (const String & f_name);

		//write the file with the bigram transition
		void Write_File_Bigram (const LexiconOptions & lexic_opt);

		//write the file with bigram transition between classes
		void Write_File_Bigram_Classes (const String & f_bigram)const;

		//write the file with information on perplexity computation
		void Write_File_Perplex (t_real perplex, const LexiconOptions & lexic_opt);

		//write the file with classes member
		void Write_File_Classes (const String & file_classes);

		//write the file with bigram transitions between phonemes
		void Write_File_Phon_Bigram (const String & f_bigram)const;

		//write the file with information of phonemes perplexity
		void Write_File_Phon_Perplex (t_real perplex, const LexiconOptions & lexic_opt)const;

	//***** functions for calculation the number of observation on a only file ***********

  		//build unigram language model on a single file
		virtual void Build_Partial_Unigram_Language_Model(const String & file_name,
	                            t_index & sil_index, t_index & file_dim)=0;

		//build bigram language model on a single file
		virtual void Count_Partial_Bigram_Transitions(const String & file_name,
								t_index & file_dim)=0;

		void Count_Partial_Phonemes_Transitions (const String & file_name,
	                              const LabelType & label_t);

	//********* functions for count words transitions on files of file_list **************

		//build unigram language model
		virtual void Build_Unigram_Language_Model (const ImpObjectList<String> & f_list, 
	                            const LexiconOptions & lexic_options)=0;

		//build bigram language model
		virtual void Count_Bigram_Transitions (const ImpObjectList<String> & f_list, 
	                            const LexiconOptions & lexic_options)=0;

		void Count_Phonemes_Transitions (const ImpObjectList<String> & f_list,
	                             const LabelType & label_t);

		//compute the perplexity of bigram language model on a only file
		virtual t_real Compute_Partial_Perplexity_Bigram (const String & f_name, 
	                                 const LexiconOptions & lexicon_options)=0;

		//compute the perplexity of bigram classes language model on a only file
		virtual t_real Compute_Partial_Perplexity_Bigram_Classes(const String & f_name,
	                                 const LexiconOptions & lexicon_options)=0;

		//compute the perplexity of phoneme bigram on a only file
		virtual t_real Compute_Partial_Perplexity_Phon_Bigram (const String & f_name,
                                                     const LabelType & label_t)=0;

//*********** public functions **********************************************************

   public:

		//compute perplexity
		void Compute_Perplexity (const LexiconOptions & lexic_options);

		//build language model
		void Build_Language_Model (const LexiconOptions & lexic_options);

		//training phonemes grammar
		void Train_Phon_Grammar (const LexiconOptions & lexic_options);

		//compute perplexity between phoneme
		void Compute_Phon_Perplexity (const LexiconOptions & lexic_options);
	};


class LexiconAtis : public Lexicon
	{

		//load phonetic transcription of a single word
		void Get_Phon_Single_Word (const char * buffer, t_index buf_index, t_index wrd_index);

		//load phonetic transcription of word
		void Load_Phonetic_Transcriptions (const String & f_phon);

		//write the file with vocabulary and phonetic transcription of each word
		void Write_File_Of_Vocabulary (const String & f_name);

//************ functions for count words transitions on files of file_list ***********************

		//build unigram language model
		void Build_Unigram_Language_Model (const ImpObjectList<String> & f_list, 
	                            const LexiconOptions & lexic_options);

		//build bigram language model
		void Count_Bigram_Transitions (const ImpObjectList<String> & f_list, 
	                            const LexiconOptions & lexic_options);

		//compute the perplexity of phoneme bigram on a only file
		t_real Compute_Partial_Perplexity_Phon_Bigram (const String & f_name,
                                                     const LabelType & label_t){return 0;}

		//build unigram language model on a single file
		virtual void Build_Partial_Unigram_Language_Model(const String & file_name,
			         t_index & sil_index, t_index & file_dim){return;}

		//build bigram language model on a single file
		virtual void Count_Partial_Bigram_Transitions(const String & file_name, t_index & file_dim)
	                                                                        {return;}


//************ computation perplexity on a only file functions ********************************

		//compute the perplexity of bigram langage model om a only file
		t_real Compute_Partial_Perplexity_Bigram (const String & f_name, 
	                                 const LexiconOptions & lexicon_options){return 0;}

		//compute the perplexity of bigram classes language model on a only file
		t_real Compute_Partial_Perplexity_Bigram_Classes(const String & f_name, 
	                                 const LexiconOptions & lexicon_options){return 0;}
	public:

		//write file of solution
		void Write_Solution_File (const String & f_sol_name, const String & list_f_name);

	};



class LexiconAtis2 : public LexiconAtis
	{

		//build unigram language model on a single file
		void Build_Partial_Unigram_Language_Model(const String & file_name,t_index & sil_index,
								t_index & file_dim);

		//build bigram language model on a single file
		void Count_Partial_Bigram_Transitions(const String & file_name, t_index & file_dim);

//************ computation perplexity on a only file functions ********************************

		//compute the perplexity of bigram langage model om a only file
		t_real Compute_Partial_Perplexity_Bigram (const String & f_name, 
	                                 const LexiconOptions & lexicon_options);

		//compute the perplexity of bigram classes language model on a only file
		t_real Compute_Partial_Perplexity_Bigram_Classes(const String & f_name, 
	                                 const LexiconOptions & lexicon_options);
	};


class LexiconAtis3 : public LexiconAtis
	{
   		//count bigram transitions on a only file
		void Count_Partial_Bigram_Transitions(const String & file_name, t_index & file_dim);

   		//build unigram language model on a single file
		void Build_Partial_Unigram_Language_Model(const String & file_name, t_index & sil_index,
								t_index & file_dim);

//************ computation perplexity on a only file functions ********************************

		t_real Compute_Partial_Perplexity_Bigram (const String & f_name, 
	                                                 const LexiconOptions & lexicon_options);
		//compute the perplexity of bigram classes language model on a only file
		t_real Compute_Partial_Perplexity_Bigram_Classes(const String & f_name, 
	                                 const LexiconOptions & lexicon_options);

	};   

      
class LexiconTimit : public Lexicon
	{
		void Load_Phonetic_Transcriptions (const String & f_name){}

		void Load_Phonetic_Transcriptions (ifstream & phn_file, const String & first_phon, 
                   t_index initial_sample, t_index final_sample, Boolean & inserted, t_index pos);

		void Write_File_Of_Vocabulary (const String & f_name);

		void Build_Partial_Unigram_Language_Model (const String & file_name, 
                                   t_index & sil_index, t_index & file_dim);

		void Build_Unigram_Language_Model(const ImpObjectList<String> & f_list, 
                                const LexiconOptions& lexic_options);

		void Count_Partial_Bigram_Transitions(const String & file_name, t_index & file_dim);

		void Count_Bigram_Transitions(const ImpObjectList<String> & f_list, 
                                const LexiconOptions& lexic_options);

		t_real Compute_Partial_Perplexity_Bigram (const String & f_name, const LexiconOptions & lexicon_options);

		//compute the perplexity of bigram classes language model on a only file
		t_real Compute_Partial_Perplexity_Bigram_Classes(const String & f_name, 
	                                 const LexiconOptions & lexicon_options);
   
		t_real Compute_Partial_Perplexity_Phon_Bigram(const String & file_name,
                                                     const LabelType & label_t);
	};           


#endif	 


