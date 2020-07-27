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
//  |   FILE:   feature.cpp                                             |
//  |   FUNCTIONALITY: low level C-like procedure for signal processing |
//  |   PROGRAM: required to feature extraction                         |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio , Fabrizio					   			|
//  |   ACTUAL REVISION: 6.0     (completely renewed)                   |
//  |   DATA ACTUAL REVISION: 19//11/98                                 |
//  |   FIRST VERSION:   1.0     (newiofile.cpp)                        |
//  |   DATA FIRST VERSION: 25/2/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// <<< version 4.0 >>>
//24/4/98 added Unix compatibility
//23/3/98 introduced ImpPolimorphic in FeatureExtraction
//12/2/98 eliminated const char *  and operator += with strings

//inserted	static  const String dsp_block_name;

// Fabrizio 
// 09/03/98 bug corrected in function Max_Delta_Feature_Order now return
//          num_of_past_samples_required instead of size_of_output_vector
// 10/03/98 bug corrected in Magnitude_And_Energy_Of_Real_Vectors energy passed by value
//          instead of by reference, corrected shuffle_optput in DspDerivative
//          class, in DspDerivative function Apply deal with case that the feature
//          is the first in the utterance
// 02/04/98 added function Size_Of_Output_Vector in NullBlock class
// 2/8/98	fixed bug on feature dim on Null Block and derivative
#include "../features/feature.h"


//add here your custom block::dsp_block_name

const String PreemphasisAndHammingWindow::dsp_block_name
		="preemphasis_and_hamming_window";


const String MfccAndEnergy::dsp_block_name
		="mfcc_with_energy";

const String FftAndEnergy::dsp_block_name
		="fft_with_energy";

const String NullBlock::dsp_block_name
		="null_block";

const String  DspDerivative::dsp_block_name =
				"add_derivatives";

const String  DspDerivativeLT::dsp_block_name =
				"add_derivatives_lt";


const String  MeanSubtraction::dsp_block_name
		= "subtract_mean";


//***********************************************
//*												*
//*			FeatureExtraction					*
//*												*
//***********************************************

// ADD in this function a NEW BLOCK !!!

void FeatureExtraction::Configure(const String & config_fname,const String &  section)
	{
	ConfigFile conf;
	t_index number_of_blocks,i;
	String section_name,option_label,block_type,value;

	conf.Open_File(config_fname);

	//get first parameters
	number_of_blocks =conf.Get_Unsigned_Opt(section,
									"Number_Of_Blocks");

	if(number_of_blocks==0)
		merr<<"zero blocks requested in Feature Configuration";

	conf.Get_String_Opt(section, "Label_Of_Blocks",option_label);


	Assert(option_label.Dim()>0);
	//delete previous configuration
	if(operations.Dim()>0)
		{
		//save configuration in diagnostic
		Actual_Configuration_String(value);
		mwarn<<"reconfiguration of feature extraction requested"<<
													"previous configuration was: "<<value;
		operations.Reset();
		}

	//allocate blocks
	operations.Destroy_And_ReDim(number_of_blocks);
	for(i=0;i< number_of_blocks;i++)
		{
		section_name.Reset();
		section_name<<option_label<<'_'<<(i+1)<<"_";

		conf.Get_String_Opt(section_name, "Type_Of_Block",value,
				"NO_OPTION_FILE_CHECK");


		// ADD HERE ANY NEW BLOCK !!!

		if (value== MfccAndEnergy::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(MfccAndEnergy);
		else
		if (value== FftAndEnergy::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(FftAndEnergy);
		else
		if (value== NullBlock::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(NullBlock);
		else
		if (value== DspDerivative::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(DspDerivative);
		else
		if (value== DspDerivativeLT::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(DspDerivativeLT);
		else
		if (value== MeanSubtraction::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(MeanSubtraction);
		else
		if (value== PreemphasisAndHammingWindow::dsp_block_name)
			operations[i].New_For_Polimorphic_Pointer_With_Allocated_Class(PreemphasisAndHammingWindow);
		else
		merr<<"the block type: "<< value<<" section:"<< section_name
			<<" of the file: "<< config_fname<<" does not exist";

		//initialize blocks
		operations[i]->Initialize(config_fname, section_name);
		};

	//Compute memory_order
	num_of_past_samples_required=0;
	for(i=0;i< number_of_blocks;i++)
		num_of_past_samples_required=
		operations[i]->Memory_Order(num_of_past_samples_required);

	//Compute Size_Of_Output_Vector
	size_of_output_vector =0;
	for(i=0;i< number_of_blocks;i++)
		size_of_output_vector =
		operations[i]->Size_Of_Output_Vector (size_of_output_vector);

	return;
	}



FeatureExtraction::FeatureExtraction()
	{
	size_of_output_vector=0;
	num_of_past_samples_required=0;
	return;
	}

FeatureExtraction::~FeatureExtraction()
	{
	size_of_output_vector=0;
	num_of_past_samples_required=0;
	operations.Reset();
	return;
	}


//retrieve configuration from configuration file
//apply all the required transformations
void FeatureExtraction::Extract(VetDouble& output,
			const VetDouble &input, t_real smp_rate)
	{
	t_index i,number_of_blocks;

	number_of_blocks = operations.Dim();

	Assert(number_of_blocks >0);
	output=input;

	for(i=0;i< number_of_blocks;i++)
		operations[i]->Apply(output,smp_rate);

	return;
	}

	//perform sequential transformations over prev_vetsmp_list to
	//the memory of all the modules
void FeatureExtraction::Get_Previous_Frames_Info(
		const VetDoubleList & prev_vetsmp_list, t_real smp_rate)
	{
	t_signed i,dim;
	static VetDouble feature;
	dim= (t_signed) prev_vetsmp_list.Dim();

	Reset();

	for(i=dim-1;i>=0;i--)
		Extract(feature, prev_vetsmp_list[i],smp_rate);

	return;
	}
//reset memory
void FeatureExtraction::Reset()
	{
	t_index i;
	for(i=0;i<operations.Dim();i++)
		operations[i]->Reset_Memory();
	return;
	}

void FeatureExtraction::Actual_Configuration_String(String & string)
	{
	String temp;
	t_index i;
	string.Reset();
	for(i=0;i<operations.Dim();i++)
		{
		temp.Reset();
		operations[i]->Actual_Configuration(temp);
		string<<temp;
		if (i<operations.Dim()-1)
			string<<">>";
		}
	return;
	}
		
	//preenfasi va sostituito in TSPECBASE e Baumwelch con la funzione 
	//Actual_Configuration 
	//t_real Preemphasis()
    
t_index FeatureExtraction::Max_Delta_Feature_Order() const
	{
	return num_of_past_samples_required;
	}

t_index FeatureExtraction::Feature_Vet_Dim() const
	{
	return size_of_output_vector;
	}
	

//***********************************************
//*															*
//*	PreemphasisAndHammingWindow				*
//*															*
//***********************************************

	//get options from configuration file
Boolean PreemphasisAndHammingWindow::Initialize(const String &  filename,
						const String &   section)
	{
	ConfigFile conf;

	String value, option_label,block_type;

	conf.Open_File(filename);
	conf.Get_String_Opt(section, "Type_Of_Block",value,
													dsp_block_name );
	if (value!= dsp_block_name)
		merr<<"Wrong Block Requested:"<<value<<" instead of "
			<< dsp_block_name<<" in section "<<section;

	preemphasis =conf.Get_Real_Opt(section, "Preemphasis",
													dsp_block_name );
	return TRUE;
	}


// Premp_And_Hamming: preemphasizes and hamming
// weighting the time signal in smp_vet
Boolean PreemphasisAndHammingWindow::Apply(VetDouble & smp_vet,t_real  smp_rate)
	{

	static VetDouble vhamm;

	if (vhamm.Dim()!=smp_vet.Dim())
		{
		vhamm.Destroy_And_ReDim(smp_vet.Dim());
		/**********************************************************/
		/*    CREATE HAMMING WINDOW VECTOR                        */
		/**********************************************************/
		const t_real a = 25.0/46.0;
		const t_real b = 1.0-a;
		t_real theta,arg;
		t_index i;

		theta = acos(-1.0);
		theta = 2.0*theta/(t_real)(smp_vet.Dim()-1L);
		for (i=1;i<=vhamm.Dim();i++)
			{
			arg = (t_real)(i-1L);
			vhamm[i-1] = a - b*cos(theta*arg);
			}
		}
	/**********************************************************/
	/*    COMPUTE CEPSTRUM FOR ALL THE FRAMES                 */
	/**********************************************************/
	t_real mem=0;
	t_real oldmem=0;
	t_index j;

	//  PREENPHATISES AND HAMMING WINDOWING 
	for(j=0;j<smp_vet.Dim();j++)
		{
		mem=smp_vet[j];
		smp_vet[j] = vhamm[j] *	(smp_vet[j] - oldmem* preemphasis);
		oldmem=mem;

		}
	return TRUE;
	}

void PreemphasisAndHammingWindow::Actual_Configuration(String & string) const
	{
	string.Reset();
	string<<dsp_block_name<<":"<<" preemphasis="<< preemphasis;
	return;
	}

//***********************************************
//*															*
//*			CommonMfccAndFft							*
//*															*
//***********************************************

//assume a real vector then energy is multiplied for two
//nyquist frequency is discarded
//magnitude returned on the first N/2 samples v_mag
void CommonMfccAndFft::Magnitude_And_Energy_Of_Real_Vectors(
		VetDouble & v_mag, const VetDouble & v_imm,t_real &energy)
	{
	t_index len = v_mag.Dim()/2;
	t_index j;
	energy = 0;

	for(j=0;j< len;j++)
		{
		v_mag[j] = v_mag[j]*v_mag[j] + v_imm[j]*v_imm[j];
		energy += v_mag[j];
		v_mag[j] = sqrt( v_mag[j] );
		}

	energy = sqrt( energy )*2.0;
	return;
	};

//return module of samples stored in vet_re and energy
void CommonMfccAndFft::Perform_FftModule_And_Energy(VetDouble & vet_re,	
															t_real & energy)
	{
	static VetDouble vet_im;
	t_index vet_dim;

	Assert(feature_vet_dim>0);
	Fft_Call(vet_re, vet_im);
	vet_dim = vet_re.Dim()/2;

	// save only one of the mirrored parts of the fft
	//nyquist frequency is discarded
	Magnitude_And_Energy_Of_Real_Vectors(vet_re, vet_im, energy);
	//add energy if required
	vet_re.Save_And_ReDim(vet_dim);
	if (compute_log_of_energy)
		energy=log(energy );

	return;
	}

CommonMfccAndFft::CommonMfccAndFft()
	{
	feature_vet_dim=0;
	compute_energy=FALSE;
	compute_log_of_energy=FALSE;
	return;
	}

	//return 0 if output vector returns the same size
t_index CommonMfccAndFft::Size_Of_Output_Vector(t_index size_of_input_vector)
	{
	if (compute_energy)
		return feature_vet_dim+1;
		else return feature_vet_dim;
	}


//***********************************************
//*															*
//*			MfccAndEnergy								*
//*															*
//***********************************************
		//apply operation


Boolean MfccAndEnergy::Initialize(const String & config_fname,
									const String &  section)
	{
	ConfigFile conf;

	String value, option_label,block_type;

	conf.Open_File(config_fname);
	feature_vet_dim=conf.Get_Unsigned_Opt(section,
									"num_features",
									dsp_block_name );
	Assert(feature_vet_dim>0);
	compute_energy =conf.Get_Boolean_Opt(section, "Compute_Energy");
	compute_log_of_energy=conf.Get_Boolean_Opt(section,"Compute_Log_Of_Energy");
	return TRUE;
	}

Boolean MfccAndEnergy::Apply(VetDouble & data,t_real smp_rate)
	{
	static VetDouble temp;
	t_real energy=0;

	Perform_FftModule_And_Energy(data, energy);
	temp=data;
	data.Destroy_And_ReDim(feature_vet_dim);
	Mfcc_Call(data,temp, smp_rate);
	if(compute_energy)
		data.Append(energy);
	return TRUE;
	}


		//return actual configuration
void MfccAndEnergy::Actual_Configuration(String & string)  const
	{
	String buffer="no";

	string.Reset();
	string<<dsp_block_name<<": "<<"num_features="<< feature_vet_dim
		<<" compute_energy=";
	if(compute_energy)
		buffer="yes";
	
	string << buffer <<" compute_log_of_energy=";
	
	if(compute_log_of_energy)
		buffer="yes";
	else buffer="no";
	
	string<<buffer;
	return;
	}


//***********************************************
//*															*
//*			FftAndEnergy								*
//*															*
//***********************************************


Boolean FftAndEnergy::Initialize(const String & config_fname,
									const String &  section)
	{
	ConfigFile conf;

	String value, option_label,block_type;

	conf.Open_File(config_fname);
	feature_vet_dim=conf.Get_Unsigned_Opt(section,
									"num_features",
									dsp_block_name );
	Assert(feature_vet_dim>0);
	compute_energy =conf.Get_Boolean_Opt(section, "Compute_Energy");
	compute_log_of_energy=conf.Get_Boolean_Opt(section,"Compute_Log_Of_Energy");

	compute_log_of_fft=conf.Get_Boolean_Opt(section,"Compute_Log_Of_FFT");

	return TRUE;
	}


		//apply operation
Boolean FftAndEnergy::Apply(VetDouble & data,t_real smp_rate)
	{
	static VetDouble temp;
	t_real energy;

	Perform_FftModule_And_Energy(data, energy);
	temp=data;
	data.Destroy_And_ReDim(feature_vet_dim);
	if (compute_log_of_fft)
		data.Log();
	if(compute_energy)
		data.Append(energy);
	return TRUE;
	}
	
		//return actual configuration
void FftAndEnergy::Actual_Configuration(String & string)  const 
	{
	string.Reset();
	string<<dsp_block_name<<": "<<"num_features="<< feature_vet_dim
		<<" compute_energy=" << compute_energy <<" compute_log_of_energy="
		<<compute_log_of_energy<<
		" compute_log_of_fft:"<< compute_log_of_fft;
	return;
	}


//***********************************************
//*															*
//*			NullBlock									*
//*															*
//***********************************************
void NullBlock::Actual_Configuration(String & string)  const
	{
	string.Reset();
	string<<dsp_block_name;
	return;
	}

Boolean NullBlock::Initialize(const String &  filename,
									const String &  section)
	{
	ConfigFile conf;
			
	conf.Open_File(filename);
	feature_dim=conf.Get_Unsigned_Opt(section,	"Feature_Dim",	dsp_block_name );

	return TRUE;
	}

t_index NullBlock::Size_Of_Output_Vector(t_index size_of_input_vector)
	{
	return feature_dim;
	}





//***********************************************
//*															*
//*			DspDerivative								*
//*															*
//***********************************************

DspDerivative::DspDerivative()
	{
	shuffle_output=FALSE;
	//check if clients modify on the fly
	// the dimension of history[i]
	never_used=TRUE;
	}

DspDerivative::~DspDerivative()
	{
	history.Reset();
	}

	//get options from configuration file
Boolean DspDerivative::Initialize(const String &  filename,
									const String &  section)
	{
	ConfigFile conf;
	t_index memory_order;
		
	conf.Open_File(filename);
	memory_order=conf.Get_Unsigned_Opt(section,
								"Number_Of_Derivatives",
													dsp_block_name );

	history.Destroy_And_ReDim(memory_order);

	shuffle_output =conf.Get_Boolean_Opt(section, "Shuffle_Output");
	feature_dim=conf.Get_Unsigned_Opt(section,	"Feature_Dim",	dsp_block_name );

	never_used=TRUE;
	return TRUE;
	}

		//apply operation
Boolean DspDerivative::Apply(VetDouble & data,t_real  smp_rate)
	{
	t_index i,num_features;

	//check for no derivatives
	if(history.Dim()==0)
		{
		if (never_used)
			{
			mwarn<<"DspDerivative: derivate equal to zero"<<
			 "no operation performed";
			never_used=FALSE;
			}
		return TRUE;
		}
	//check for equal vector length
		num_features=data.Dim();
	if (history[0].Dim()!= num_features)
		{
		if (NOT never_used)
			mwarn<<"input vector has changed dimensions from: "<<
			history[0].Dim()<<" to: "<< num_features
			<<"without prior initialization check your program";

		for(i=0;i< history.Dim();i++)
		{
			history[i].Destroy_And_ReDim(num_features);
		}

		for(i=0;i<num_features;i++)
				history[0][i]=data[i];

		never_used=FALSE;
		}

	//compute n-order finite differences
	
	First_Assign(data);

	for(i=1;i<=history.Dim();i++)
		{
		//	y[i]=y[i-1]-history[i-1];
		Subtract(data, (i-1)* num_features,history[i-1],
			(i)* num_features, num_features);

		//	history[i-1]=y[i-1];
		Assign(history[i-1],data,(i-1)* num_features, num_features);
		}
	
	if(shuffle_output)
	{
		t_index z=0,k;
		VetDouble temp;

		temp.Destroy_And_ReDim(data.Dim());
		for(k=0;k<=history.Dim();k++)
		{
			for(i=0;i<num_features-1;i++)
				temp[z+i]=data[i+k+z];
			z=i*(k+1);
			temp[temp.Dim()-history.Dim()-1+k]=data[i+k*num_features];
		}

		data=temp;
	}

	return TRUE;
	}
//	y[i]=y[i-1]-history[i-1];
//performs vectorial y[start_y.. start_y+ num_elems-1] =y[start_x..]-s
void DspDerivative::Subtract(
			VetDouble & y,/*-*/ 	t_index start_x,
			VetDouble & s,/*=*/t_index start_y,t_index num_elems)
	{
	t_index i;
	Assert(num_elems==s.Dim());
	Assert(y.Dim()>=start_y+ num_elems);
	for(i=0;i< num_elems;i++)
		y[start_y+i]=y[start_x+i]- s[i];

	return;
	}

//	history[i-1]=y[i-1];
//out=in[start…start-1+ num_elems]
void DspDerivative::Assign(VetDouble & out, const VetDouble & y,t_index start,
		t_index num_elems)
	{
	t_index i;

	for(i=0;i< num_elems;i++)
		out[i]=y[start +i];

	return;
	}

void DspDerivative::First_Assign(VetDouble & data)
	{
	t_index num_features= data.Dim();
	data.Save_And_ReDim(Size_Of_Output_Vector(num_features));

	return;
	}
void DspDerivative::Reset_Memory()
	{
	t_index i;
	for(i=0;i<history.Dim();i++)
		history[i].Set(0.0);
	return;
	}

void DspDerivative::Actual_Configuration(String & string)  const
	{
	String buffer="no";

	string.Reset();
	string<<dsp_block_name<<": "<<"num_of_derivatives="
			<< history.Dim();
	if(shuffle_output)
		buffer="yes";

	string<<" shuffle_output=" <<buffer;
	return;
	}

t_index DspDerivative::Memory_Order(t_index memory_order) 
	{
	return memory_order + (history.Dim());
	};

t_index DspDerivative::Size_Of_Output_Vector(t_index size_of_input_vector)
	{
	// only if dsp_derivative is the first block
	if(size_of_input_vector==0)
		size_of_input_vector=feature_dim;

	return size_of_input_vector * (history.Dim()+1);
	};


//***********************************************
//*												*
//*			DspDerivativeLT						*
//*												*
//***********************************************

Boolean DspDerivativeLT::Initialize(const String &  filename,
									const String &  section)
	{

	DspDerivative::Initialize(filename, section);
	history_old.Destroy_And_ReDim(Memory_Order(0));
	
	return TRUE;
	}

t_index DspDerivativeLT::Memory_Order(t_index memory_order) 
	{
	return memory_order + (history.Dim()*2);
	};

		//apply operation
Boolean DspDerivativeLT::Apply(VetDouble & data,t_real  smp_rate)
	{
	t_index i,num_features;

	//check for no derivatives
	if(history.Dim()==0)
		{
		if (never_used)
			{
			mwarn<<"DspDerivative: derivate equal to zero"<<
			 "no operation performed";
			never_used=FALSE;
			}
		return TRUE;
		}
	//check for equal vector length
		num_features=data.Dim();
	if (history[0].Dim()!= num_features)
		{
		if (NOT never_used)
			mwarn<<"input vector has changed dimensions from: "<<
			history[0].Dim()<<" to: "<< num_features
			<<"without prior initialization check your program";

		for(i=0;i< history.Dim();i++)
		{
			history[i].Destroy_And_ReDim(num_features);
			history_old[i].Destroy_And_ReDim(num_features);
		}

		history[0]=data;
		history_old[0]=data;

		never_used=FALSE;
		}

	First_Assign(data);
	
	for(i=1;i<=history.Dim();i++)
		{
		//	y[i]=y[i-1]-history[i-1];
		Subtract(data, (i-1)* num_features,history_old[i-1],
			(i)* num_features, num_features);

		history_old[i-1]=history[i-1];

		//	history[i-1]=y[i-1];
		Assign(history[i-1],data,(i-1)* num_features, num_features);
		}

	
	if(shuffle_output)
		{
		t_index z=0,k;
		VetDouble temp;

		temp.Destroy_And_ReDim(data.Dim());
		for(k=0;k<=history.Dim();k++)
			{
			for(i=0;i<num_features-1;i++)
				temp[z+i]=data[i+k+z];
			z=i*(k+1);
			temp[temp.Dim()-history.Dim()-1+k]=data[i+k*num_features];
			}

		data=temp;
		}

	return TRUE;
	}

//***********************************************
//*												*
//*			MeanSubtraction						*
//*												*
//***********************************************


		//get options from configuration file
Boolean MeanSubtraction::Initialize(const String &  filename, const String & 	section)
	{ 
	ConfigFile conf;
	String file_mean_value,value;
	conf.Open_File(filename);

	conf.Get_String_Opt(section, "Type_Of_Block",value,	dsp_block_name );
	
	if (value!= dsp_block_name)
		merr<<"Wrong Block Requested:"<<value<<" instead of "
			<< dsp_block_name<<" in section "<<section;

	conf.Get_String_Opt(section, "File_of_mean_value", file_mean_value,
													dsp_block_name );
	Load_Mean_Value(file_mean_value);

	return TRUE;
	}

		//apply operation
Boolean MeanSubtraction::Apply(VetDouble & data,t_real  smp_rate)
	{
	data-=mean_vet;
	return TRUE;
	}

	//return actual configuration
void MeanSubtraction::Actual_Configuration(String & string) const
	{
	string.Reset();
	string<<dsp_block_name;
	return;
	}

void MeanSubtraction::Load_Mean_Value(const String &  mean_file)
	{
	ifstream file;
	String buffer;
	t_index dim;

	file.open(mean_file, ios::in|ios::nocreate);
	if(file.fail())
		merr<<"Cannot open mean file:"<<mean_file;

	file>>buffer;
	if(buffer!="[mean_value]")
		merr<<"Bad file of feature mean value";

	file>>buffer;
	if(buffer!="vet_dim:")
		merr<<"Bad file of feature mean value";

	file>>dim;
	mean_vet.Destroy_And_ReDim(dim);

	for(t_index i=0;i<dim;i++)
		file>>mean_vet[i];

	file.close();

	return;
	};

t_index MeanSubtraction::Memory_Order(t_index memory_order)
	{
	return memory_order;
	};

