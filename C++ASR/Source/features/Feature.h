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
//  |   FILE:   feature.h                                               |
//  |   FUNCTIONALITY: low level C-like procedure for signal processin  |
//  |   PROGRAM: required to feature extraction                         |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio                                          |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   1.0     (old newiofile.h)                      |
//  |   DATA FIRST VERSION: 17/01/98                                    |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//23/4/98 introduced ImpPolimorphic in FeatureExtraction

//13/2/98 eliminated const char *,
//        inserted	static  const String dsp_block_name;

// Fabrizio 
// 4/6/1998 corrected bug in dsdderivative class, inserted feature_dim 
// arguments in order to avoid possible memory crash when used as first block

// *******************   USER INFORMATION   ******************************
/*
available signal processing blocks (classes)
1) GenericDspOperation (abstract)
2) CommonMfccAndFft    (abstract)
3) MfccAndEnergy
4) FftAndEnergy
5) NullBlock
6) DspDerivative
7) DspDerivativeLT
8) MeanSubraction
9) PreemphasisAndHammingWindow
*/

//to add a signal processing new block
//1) derive the class form GenericDspOperation
//2) implement at least the pure virtual functions
//3) if block returns ( through apply ) a vector size
//   different form the input, overload Size_Of_Output_Vector
//4) if block has memory (use past vectors) overload Memory_Order
//5) declare a public static const string dsp_block_name an initialize to the name 
//		of the cblock as found in config file
//6) add the proper instruction in FeatureExtraction::Configure
// *******************   Implementation ISSUES   ******************************

// *******************   to be implemented   ******************************


//Functionality:


#ifndef __FEATURE_H_
#define __FEATURE_H_
#include <fstream.h>
#include "../baseclas/textclas.h"
#include "../vetclas/vetclas.h"
#include "../resconf/resconf.h"
#include "../baseclas/polimorf.h"

#define RES_DSP_FEATURE_CLASS_VERSION "V. 6.0 of 19/11/98  " ## __FILE__ " c." ## __DATE__


//*****************  signal processing useful functions ****************
void Mfcc_Call(VetDouble & mfc_vet, VetDouble & fft_module,t_real d_sam);

void Fft_Call(VetDouble & vet, VetDouble & vet_im);

// ifft: inverse FFT transform. Gets a vector of DFT samples in vet_re
	// and returns the inverse transform in vet_re and vet_im
	// Attention: the form of the input array must be specified
void Ifft_Call(VetDouble & vet_re, VetDouble & vet_im);

typedef ImpObjectList<VetDouble> VetDoubleList ;

//***********************************************
//*												*
//*			GenericDspOperation					*
//*												*
//***********************************************
//abstract signal processing block
//derive always your block from this one

class GenericDspOperation
	{
	protected:

	public:

	//derive this data and initialize with the same name
	//of the block found in the config file
	static  const String dsp_block_name;

		//get options from configuration file
	virtual Boolean Initialize(const String & filename, const String & section)=0;

		//perform operation
	virtual Boolean Apply(VetDouble & data,t_real  smp_rate)=0;

		//Reset eventual memory
	virtual void Reset_Memory(){};

	//informative functions
		//return actual configuration
	virtual void Actual_Configuration(String & string) const =0;

		//return how many past samples are used by the operation
		//if no past samples used (zero-memory order) return 0.
	virtual t_index Memory_Order(t_index memory_order)
		{
		return memory_order;
		};

	//return 0 if output vector returns the same size
	virtual t_index Size_Of_Output_Vector(t_index size_of_input_vector)
		{
		return size_of_input_vector;
		};

	};

//***********************************************
//*												*
//*	PreemphasisAndHammingWindow					*
//*												*
//***********************************************

class PreemphasisAndHammingWindow:public GenericDspOperation
	{
	t_real preemphasis;
	public:

	//derive this data and initialize with the same name
	//of the block found in the config file
	static  const String dsp_block_name;

		//get options from configuration file
	Boolean Initialize(const String &  filename, const String &  section);
		//apply operation
	Boolean Apply(VetDouble & data,t_real  smp_rate);


		//return actual configuration
	void Actual_Configuration(String & string) const;

	};

//***********************************************
//*												*
//*			CommonMfccAndFft					*
//*												*
//***********************************************

class CommonMfccAndFft: public GenericDspOperation
	{
	protected:
	t_index feature_vet_dim;
	Boolean compute_energy;
	//log is base based on C log
	Boolean compute_log_of_energy;

	//return module of samples stored in vet_re and energy
	void Perform_FftModule_And_Energy(VetDouble & vet_re, t_real & energy);

	public:
	CommonMfccAndFft();

	//return 0 if output vector returns the same size
	t_index Size_Of_Output_Vector(t_index size_of_input_vector);

	private:
	void Magnitude_And_Energy_Of_Real_Vectors(
		VetDouble & v_mag, const VetDouble & v_imm,t_real &energy);
	};

//***********************************************
//*												*
//*			MfccAndEnergy						*
//*												*
//***********************************************

class MfccAndEnergy: public CommonMfccAndFft
	{
	public:
	static  const String dsp_block_name;

	Boolean Initialize(const String &  filename, const String &  section);

		//apply operation
	Boolean Apply(VetDouble & data,t_real  smp_rate);

		//return actual configuration
	void Actual_Configuration(String & string) const;

	};

//***********************************************
//*												*
//*			FftAndEnergy						*
//*												*
//***********************************************

class FftAndEnergy: public CommonMfccAndFft
	{
	Boolean compute_log_of_fft;

	public:
	static  const String dsp_block_name;

	Boolean Initialize(const String &  filename, const String &  section);
		//apply operation
	Boolean Apply(VetDouble & data,t_real  smp_rate);

		//return actual configuration
	void Actual_Configuration(String & string) const;

	};

//***********************************************
//*												*
//*			NullBlock							*
//*												*
//***********************************************

class NullBlock:public GenericDspOperation
	{
	t_index feature_dim;

	public:
	static  const String dsp_block_name;

		//get options from configuration file
	inline Boolean Initialize(const String &  filename, const String &  section);
	t_index Size_Of_Output_Vector(t_index size_of_input_vector);

	inline Boolean Apply(VetDouble & data,t_real  smp_rate)
		{
		return TRUE;
		};

	void Actual_Configuration(String & string) const;

	};

//***********************************************
//*												*
//*			DspDerivative						*
//*												*
//***********************************************

class DspDerivative:public GenericDspOperation
	{
protected:
	Boolean shuffle_output;
	ImpObjectList<VetDouble> history;
	Boolean never_used;
	t_index feature_dim;

	public:
	static  const String dsp_block_name;

	DspDerivative();
	~DspDerivative();
		//get options from configuration file
	Boolean Initialize(const String &  filename, const String &  section);
		//apply operation
	virtual Boolean Apply(VetDouble & data,t_real  smp_rate);


	void Reset_Memory();

	void Actual_Configuration(String & string) const;

	virtual t_index Memory_Order(t_index memory_order);


	t_index Size_Of_Output_Vector(t_index size_of_input_vector);

	//return the type of block as written in the config file


	protected:
   //performs vectorial y[start_y.. start_y+ num_elems-1] =x[start_x..]-s
	void Subtract(	VetDouble & y,t_index start_x,VetDouble & s,
					t_index start_y,t_index num_elems);
	void Assign(VetDouble & out, const VetDouble & y,t_index start,
			t_index num_elems);

	//1) y[0]=data;
	void First_Assign(VetDouble & data);

	};

//***********************************************
//*												*
//*			DspDerivativeLT						*
//*												*
//***********************************************

class DspDerivativeLT:public DspDerivative
	{
	
	// samples and derivatives at step t=-2
	ImpObjectList<VetDouble> history_old;

	public:
	static  const String dsp_block_name;

	Boolean Initialize(const String &  filename,
									const String &  section);
	
	t_index Memory_Order(t_index memory_order);

	Boolean Apply(VetDouble & data,t_real  smp_rate);

	};



//***********************************************
//*												*
//*			MeanSubtraction						*
//*												*
//***********************************************

class MeanSubtraction :public GenericDspOperation
	{
	VetDouble mean_vet;
	
	void Load_Mean_Value(const String & mean_file);

	public:
	static  const String dsp_block_name;

		//get options from configuration file
	Boolean Initialize(const String &  filename, const String &  	section);
		//apply operation
	Boolean Apply(VetDouble & data,t_real  smp_rate);


		//return actual configuration
	void Actual_Configuration(String & string) const;

	t_index Memory_Order(t_index memory_order);

	};



//***********************************************
//*												*
//*			FeatureExtraction					*
//*												*
//***********************************************

class FeatureExtraction
	{

	protected:
	ImpPolimorphicVectorPtr<GenericDspOperation> operations;
	t_index size_of_output_vector;
	t_index num_of_past_samples_required;

	public:

	FeatureExtraction();
	~ FeatureExtraction();

	//retrieve configuration from configuration file
	void Configure(const String &  config_fname,
											const String &  section  =	"FeatureExtraction");

	//apply all the required transformations
	void Extract(VetDouble& output, const VetDouble &input,t_real  smp_rate=0);
	//perform sequential transformations over prev_vetsmp_list to
	//the memory of all the modules
	void FeatureExtraction::Get_Previous_Frames_Info(
		const VetDoubleList & prev_vetsmp_list, t_real smp_rate=0);

	//Reset eventual memory of the blocks
	void Reset();

	t_index Feature_Vet_Dim() const;

	void Actual_Configuration_String(String & config_string);

	//preenfasi va sostituito in TSPECBASE e Baumwelch con la funzione
	//Actual_Configuration
	//t_real Preemphasis()

	t_index Max_Delta_Feature_Order() const;

	};


#endif

