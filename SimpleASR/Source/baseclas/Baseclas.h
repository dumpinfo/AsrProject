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
//  |   FILE:   baseclas.h (old iperclas.h)                             |
//  |   FUNCTIONALITY: general data vector management                   |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes                                |
//  |   CONTRIBUTIONS: Claudio  (revision Antonio )						|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   1.0                                            |
//  |   DATA FIRST VERSION: 1/11/94                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________




// *******************    VERSION HISTORY  *******************************
// <<< version 4.0 >>>
//25/4/98 added unix compatibility
//10/2/98 copy contructor of impbase generate a linker error if called

//25/9/97 added operator[](int index) for 32 bit code only this is to be more compliant to ansi c++
//16/9/97 read code revision by claudio

//<< version 4.0>>

//tested on bc 4.51 win 16 , bc 3.1 dos
//1) added new diagnostic system

//3) when size of objects are not multiple of 16 an error
// occurs when a new is called acrossing the 64k frame bound
// thus any class used in ImpObjectList must be sizeof 16 bytes multiple
//4)	copy constructor may improve for ImpSimpleList with memcopy
//only for 32bit platform
//5) adding void *operator new[](size_t size,void* where) may be
	//to speed up inizialization on 32 platform
//6) added t_signed and t_real replacing long and double

//7) 18/9/97deleted new[] on some compilers it gave problems and it was not used


//<< version 2.1>>
//1) added multiplatform borland 3.1 (dos win )
//2) compatibility with 4.51 easy win dos TDocument SDI win16 win32s
//3) added new with enum dummy used   because
		//void *operator new(size_t size,void* where) may be
		//already defined on some platform
// <<< version 2.0 >>>
// 1) OK mathematical Matrix eliminated 15/09/96
// 2) OK vector functions eliminated 15/09/96
// 3) OK added check memory for borland dos 15/09/96
// 4) OK 32 bit treatment added 15/09/96
// 5) OK _HUGEPTR_ inserted to simiplify code

// <<< version 1.0 >>>

//TERRIBLE BUG HAS BEEN CORRECTED (init_s_elem in impobject List)
//don't use previous versions

//1) baseList copy constructor eliminated
//2) buffered impobject list

//3) inserted operator = in impbaseList
//4) inserted operator Reset() that destroy all (call to destructor);

//5) virtual call to Save_aND_ReDim() eliminated and fixed
//6) functions Append Inserted

//7) IO function inserted
//8) Matrix implemented
//9) for mfc computation mfc.cpp is required
//11 Append corrected
//12 wrnmsg for windows
//13) vector functions inserted ( declaration in mfc.cpp)
//14 fixed save and redim for impbase
//15 errmsg adapted for windows
//16 antonio: assert modified in order to easier climbing up to causes


//*******************   USER INFORMATION   *******************************

//to create lists or array of classes the classes must have:
//1)a copy constructor that if it has pointers to run-time allocated
// memory it DUPLICATES the allocated memory (non different pointers
//pointing on the same memory area allowed
//2) a valid operator = (that  duplicates!!!!!!)!!!
//class ImpBaseList<T> it doesn't inizialize/destroy objects don't use
//ImpSimpleTypeList<T> it inizializes elems. ONLY FOR SIMPLE TYPE

//3) when size of objects are not multiple of 16 an error
// occurs on 16 byte Borland 4.51 (not on B3.1 )
// this is because compiler has a bug when a new is called acrossing the 64k frame bound
// thus any class used in ImpObjectList must be sizeof 16 bytes multiple
// when possible always do data alignment with 16 byte

//*******************   IMPLEMENTATION INFORMATION   *******************************

//these routines are based on farmalloc
//standard new is useless because based on malloc
//i.e. max 64k memory and no more than 64k element
//and new based on farmalloc doesn't inizialize
// pointers are needed to avoid wrap around

//*******************   CLASS INDEX   *******************************

//class ImpBaseList<T> it doesn't inizialize/destroy objects don't use !!!!
//ImpSimpleTypeList<T> it inizializes elems. ONLY FOR SIMPLE TYPE
//ImpSimpleTypeArray<T> array of simple type vectors calls allowed



#ifndef __IPER_CLASS
#define __IPER_CLASS


//if def __32BIT_CODE__ flat memory and 32 bit pointer and 32 bit int expected
//if def __32BIT_CODE__ short is always 16 bit char always 8 bit
// otherwise define TWO_BYTE_SHORT
//if def __CHECK_MEMORY_ALLOC and __MSDOS__ and __BORLANDC__


#include "../baseclas/compatib.h"
#include "../baseclas/boolean.h"
#include "../baseclas/diagnost.h"


#if  defined(__CHECK_MEMORY_ALLOC) && defined(__MSDOS__ )
	#include <alloc.h>
#endif

//#define _DEFAULT_DATA_CHUNK 4L

typedef unsigned long t_index;
typedef double t_real;
typedef long t_signed;

// list type
// if no buffering requested call Set_Data_Chunk(0)
// or call constructor(0);

//************************************************************************
//                                                                      **
//                      class ImpBaseList                               **
//                                                                      **
//************************************************************************



template <class T>
class ImpBaseList
	{

	//memory allocated may be zero even if dim>0 in this case
	//lis!= NULL and object is not empty even if it is not allocated
	t_index available_mem;
	//memory increment
	t_index data_chunk;

	//this prevent from calling it explicitely

	protected:
	//dimension of vector
	t_index dim;
	T _HUGEPTR_ * lis;

	public:

	ImpBaseList();

//	ImpBaseList(const ImpBaseList<T> & obj);

	ImpBaseList(t_index requested_data_chunk);
	void Set_Data_Chunk(t_index requested_data_chunk);
	//destroy vector and deallocate mem
	~ImpBaseList();
	//destroy vector but does not deallocate mem
	void Reset();
	//the previous functions are equivalents from user point of view


//append one element
	void Append(const T & obj);
//append a list
	void Append(const ImpBaseList<T> & obj);

//duplicate
	void operator=(const ImpBaseList<T> & source);

	T & operator [] ( t_index index );
	const T & operator [] ( t_index index ) const;

#ifdef _OPERATOR_SQUARE_BRACKET_TO_BE_DECLARED_ALSO_FOR_INT	

	T & operator [] ( int index );
	const T & operator [] ( int index ) const;
#endif

	t_index Dim() const;

protected:
	void Destroy_And_ReDim(t_index dimension);
	void Save_And_ReDim(t_index dimension);
private:
	void Disallocate_Mem();
	#ifdef __CHECK_MEMORY_ALLOC
	void Check_Mem(int check_type);
	#endif
	};

//************************************************************************
//                                                                      **
//                      class ImpSimpleTypeList                         **
//                                                                      **
//************************************************************************


//tipo lista
template <class T>
class ImpSimpleTypeList:public ImpBaseList<T>
	{
	public:
#ifdef __32BIT_CODE__
	//duplicate not sure for 16 bit platforms
	void operator=(const ImpSimpleTypeList<T> & source);
#endif
	Boolean operator==(const ImpSimpleTypeList<T> & sub) const;
	Boolean operator!=(const ImpSimpleTypeList<T> & sub) const;

	//Set all the number to num
	void Set(T num);

	void Destroy_And_ReDim(t_index dimension);
	void Save_And_ReDim(t_index dimension);

	};

//************************************************************************
//                                                                      **
//                      class ImpObjectList                             **
//                                                                      **
//************************************************************************


//List implementation of a generic object
template <class T>
class ImpObjectList:public ImpBaseList<T>
	{
	public:

	//destroy vector and deallocate mem
	~ImpObjectList();

	//destroy vector but does not deallocate mem
	void Reset();

	//the previous functions are equivalents from user point of view

	//append one element
	void Append(const T & obj);
	//append a list
	void Append(const ImpObjectList<T> & obj);

	//duplicate
	void operator=(const ImpObjectList<T> & source);


	void Destroy_And_ReDim(t_index dimension);
	void Save_And_ReDim(t_index dimension);

	private:
	void Init_List_ElemS(t_index start);
	};

//class for implementation of a ordered list
template <class T, class E>
class OrderedList 
	{
     
    class OrderedListElem
		{
	    
        public:

			T key;
			E info;
		           
		};
   
	public:

		ImpSimpleTypeList<t_index> tbl_index;

		t_index Dim();

		ImpObjectList<OrderedListElem> list; 

		void Get_Key (const t_index position, T & key_elem);
	 
		//return the index of key if this is already present in the ordered list
		t_index Binary_Search (const T & search_key, Boolean & found)const;

		Boolean Insertion(const T & new_key, const E & new_info, t_index & position);

	    E& operator[] (t_index index)
			{
			return list[tbl_index[index]].info;
			}	

		const E& operator[] (t_index index)const
			{
			return list[tbl_index[index]].info;
			}

	};

//template code file
#include "../baseclas/baseclas.hpp"
Boolean Test_Base_Class(t_index max_num=200000UL);


#endif

