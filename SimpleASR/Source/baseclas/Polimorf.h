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
//  |   FILE:   polimorf.h                                              |
//  |   FUNCTIONALITY: safe use of pointers for polimorphosm            |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio                                          |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 21/2/98                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#define POLIMORPH_VERSION "6.0 of 19/11/98 " ## __FILE__ ##" compiled "##__DATE__

#ifndef _POLIMORPHIC_VECTORS_
#define _POLIMORPHIC_VECTORS_

#include "../baseclas/textclas.h"




// *******************    VERSION HISTORY  *******************************

// *******************  user information    ******************************
// to use the class to the following

//	1) ImpPolimorphicPointer<AbstractClass> my_clas;
//	2) my_clas.New_For_Polimorphic_Pointer_With_Allocated_Class(TwoDerivedPolimorphic);
//	3) mstat<<my_clas->Some_Virtual_Function();
//  4) my_clas.Delete_Pointer();      //delete if you need to recall New_For_Polimorphic_Pointer_With_Allocated_Class
//     otherwise he pointer is disallocated automatically

//note that:
//a:New_For_Polimorphic_Pointer_With_Allocated_Class
//				cannot be called twice without first invoking Delete_Pointer
// 			an run-time error is generated
//b: Delete_Pointer cannot be called twice without invoking new


// *******************     Implementation  *******************************
//pointer = NULL  means pointer not alllocated i.e. new not invoked
//pointer != NULL means pointer allocated
//pointer cannot be reallocated twice without first invoking Delete_Pointer
// 			a warn is emitted
//pointer cannot be deleted twice through Delete_Pointer

//************************************************************************
//                                                                      **
//                      class String functions                          **
//                                                                      **
//************************************************************************

//all the code is implemented here because it is more clear
template <class T>
class ImpPolimorphicPointer
	{
	T * pointer;


	public:
	ImpPolimorphicPointer()
		{
		pointer=NULL;
		return;
		}

	~ImpPolimorphicPointer()
		{
		//delete only allocated pointers
		if (pointer!=NULL)
			Delete_Pointer();
		return;
		}
#define New_For_Polimorphic_Pointer_With_Allocated_Class(a) \
		__Instantiate_Pointer_With___Allocated_Class__(new (a) )

	void __Instantiate_Pointer_With___Allocated_Class__(T * pippo)
		{
		Check(pippo!=NULL,"memory not allocated by new");
		Check(pointer==NULL,"cannot reinitialize before Delete_Pointer() call");
		pointer=(T *)pippo;
		return;
		};


	Boolean Is_Allocated() const
		{
		return (pointer!=NULL);
		}

	//return the pointer
	const T * operator->() const
		{
		Check(pointer!=NULL,"calling a function of no initialized object");
		return pointer;
		}

	T * operator->()
		{
		Check(pointer!=NULL,"calling a function of non initialized object");
		return pointer;
		}

	void Delete_Pointer()
		{
		Check(pointer!=NULL,"Deleting non allocated pointer");
		delete(pointer);
		pointer=NULL;
		return;
		}

	//this function is declared and implemented only because required
	//the vector of pointers any call to this function provokes
	//a run time error

	void operator=(ImpPolimorphicPointer pippo)
		{
		merr<<"don't invoke me"; //requested by vector of pointers
		}

	//not implemented; save from unwanted calls to operator=
	//the only allowed assignment is pippo = new DerivedClass;
	void operator=(void * pippo);
	void operator=(const void * pippo);
	void operator=(int pippo);
	ImpPolimorphicPointer(T*);
	void operator=(T * pippo);
	};

template <class T>
class ImpPolimorphicVectorPtr:public ImpObjectList<ImpPolimorphicPointer<T> >
	{
	};
Boolean Test_Polimorphism();
#endif




