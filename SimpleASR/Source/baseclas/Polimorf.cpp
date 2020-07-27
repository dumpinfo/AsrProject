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
//  |   FILE:   polimorf.cpp                                            |
//  |   FUNCTIONALITY: safe use of pointers for polimorphosm            |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio	                                        |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 23/2/98                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________



#include "../baseclas/polimorf.h"




// *******************    VERSION HISTORY  *******************************

// *******************     Implementation  *******************************
//this class are implemented only for testing

class AbstractBasePolimorphic
	{
	protected:
	int ma;
	String my_string;
	public:

	AbstractBasePolimorphic()
		{
		ma=0;
		mstat<<"invoking AbstractBasePolimorphic constructor";
		return;
		}

	virtual String & Print(String & input_string )=0;
	const String & Name() const
		{
		return my_string;
		}
	virtual ~AbstractBasePolimorphic()
		{
		mstat<<"invoking AbstractBasePolimorphic destructor";
		return;
		};
	};

class DerivedPolimorphic:public AbstractBasePolimorphic
	{
	public:

	DerivedPolimorphic()
		{
		my_string="Derived Pol";
		mstat<<"invoking DerivedPolimorphic constructor";
		return;
		}
	virtual String & Print(String & input_string )
		{
		input_string <<"Print of DerivedPolimorphic:";
		input_string <<"data is "<<  my_string;
		return input_string;
		}
	virtual ~DerivedPolimorphic()
		{
		mstat<<"invoking DerivedPolimorphic destructor";
		my_string.Reset();
		return;
		}
	};

class TwoDerivedPolimorphic:public AbstractBasePolimorphic
	{
	public:

	TwoDerivedPolimorphic()
		{
		my_string="Two Derived Pol";
		mstat<<"invoking TwoDerivedPolimorphic constructor";
		return;
		}
	virtual String & Print(String & input_string )
		{
		input_string <<"Print of TwoDerivedPolimorphic:";
		input_string <<"data is "<<  my_string;
		return input_string;
		}
	virtual ~TwoDerivedPolimorphic()
		{
		mstat<<"invoking TwoDerivedPolimorphic destructor";
		my_string.Reset();
		return;
		}
	};

Boolean Test_Polimorphism()
	{
	String temp;
	mstat<<endl<<"1: ImpPolimorphicPointer<AbstractBasePolimorphic> my_clas;";
	ImpPolimorphicPointer<AbstractBasePolimorphic> my_clas;


	mstat<<endl<<"2: New_For_Polimorphic_Pointer_With_Allocated_Class(TwoDerivedPolimorphic);";
	my_clas.New_For_Polimorphic_Pointer_With_Allocated_Class(TwoDerivedPolimorphic);

	mstat<<endl<<"3: my_clas->Print(temp)";
	mstat<<my_clas->Print(temp);

	if(my_clas->Name()!="Two Derived Pol")
		merr<<"my_clas->Name()=="<<my_clas->Name();

	mstat<<endl<<"4: my_clas.Delete_Pointer();";
	my_clas.Delete_Pointer();


	mstat<<endl<<"5: New_For_Polimorphic_Pointer_With_Allocated_Class(DerivedPolimorphic);";
	my_clas.New_For_Polimorphic_Pointer_With_Allocated_Class(DerivedPolimorphic);

	mstat<<endl<<"6: my_clas->Print(temp)";
	mstat<<my_clas->Print(temp);

	if(my_clas->Name()!="Derived Pol")
		merr<<"my_clas->Name()=="<<my_clas->Name();


	//check vector of pointers

	ImpPolimorphicVectorPtr<AbstractBasePolimorphic> my_vet;
	t_index i;
	t_index dim=400;

	my_vet.Destroy_And_ReDim(dim);

	for(i=0;i<dim;i++)
		{
		if(i%2==0)
			my_vet[i].New_For_Polimorphic_Pointer_With_Allocated_Class(TwoDerivedPolimorphic);
			else my_vet[i].New_For_Polimorphic_Pointer_With_Allocated_Class(DerivedPolimorphic);
		}
	for(i=0;i<dim;i++)
		{
		if(i%2==0)
			{
			if(my_vet[i]->Name()!="Two Derived Pol")
				merr<<"my_vet->Name()=="<<my_vet[i]->Name();
			}
			else 	{
					if(my_vet[i]->Name()!="Derived Pol")
						merr<<"my_vet->Name()=="<<my_vet[i]->Name();
					}
		}
	mwarn<<"test polimorphism passed"<<endl<<
	 "Version "<<POLIMORPH_VERSION<<endl;
	return TRUE;
	}


//define the labels below to get the correspondent errors
//#define TEST_ERRORS_NOT_PERMITTED_BY_LINKER
//#define TEST_ERRORS_NOT_PERMITTED_BY_COMPILER

int Test_Linker_Compiler_Errors()
	{
	String temp;
	ImpPolimorphicPointer<AbstractBasePolimorphic> my_clas,clas2;

#ifdef TEST_ERRORS_NOT_PERMITTED_BY_LINKER
	//this should originate a linker error
	//this operation must !! not be performed
	// the only operation allowed with assignmentis my_clas = new(DerivedClass)

	void * ptr=NULL
	my_clas = ptr;     //undefined symbol
	clas2   = my_clas; //undefined symbol
	//this provoke a compiler error
#ifdef TEST_ERRORS_NOT_PERMITTED_BY_COMPILER
	//this is not allowed because Unknown_Function does not exists
	my_clas->Unknown_Function();
	my_clas = NULL; //compiler error Ambiguity between =
#endif

#endif

	return 1;
	}


