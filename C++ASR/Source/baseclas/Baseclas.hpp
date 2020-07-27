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
//  |   FILE:   baseclas.hpp                                            |
//  |   FUNCTIONALITY: code in a header file for baseclas.h             |
//  |   PROGRAM: required to  all codes                                 |
//  |   COMMENTS:header template classes                                |
//  |   CONTRIBUTIONS: Claudio  (revision Antonio )						|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 07/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************

// <<< version 4.0 >>>
//24/4/98 added unix eliminate a cast (T) seems not good on new compilers
//10/2/98 added more verbose checks for array over bound
//Save and ReDim now handles the case where object
//is not empty (dim>0 and lis!=NULL) and no memory is allocated
//allocated_memory =0 this allows implementing non allocated objects//15/9/97 read code revision by claudio
//copy contructor of impbase generate a linker error if called
//21/9/97 eliminated new[] which was not accepted by some compilers

//<< version 4.0>>
//bug fixed impSimpleList Destroy and ReDim
//the routine void ImpObjectList<T>::Init_List_ElemS(t_index start)
//may be changed for 32 bit platforms
//to do inizialization in one shot using the array new
//and speed up inizialization

//<< version 4.2>
//8/10/98 eliminated time consuming Dim() from code (the ones in cycles
//*******************   CLASS INDEX   *******************************

//class ImpBaseList<T> it doesn't inizialize/destroy objects don't use !!!!
//ImpSimpleTypeList<T> it inizializes elems. ONLY FOR SIMPLE TYPE
//ImpSimpleTypeArray<T> array of simple type vectors calls allowed





#ifndef _BASECLAS_HPP_
#define _BASECLAS_HPP_

/*
#ifdef  _MSC_VER
#include<memory.h>

#else
#include<mem.h>
#endif
*/

#include<string.h>

#define ITERATE_ALL(operation) 			\
		t_index _limit =Dim(); 	  		\
		for(t_index i=0;i<_limit;i++)	\
			{         					\
			operation ;					\
			}


//************************************************************************
//                                                                      **
//                      class ImpBaseList                               **
//                                                                      **
//************************************************************************

#ifdef __CHECK_MEMORY_ALLOC

	template <class T>
	void ImpBaseList<T>::Check_Mem(int check_type)
		{
		int res;
		Check(farheapfillfree(1) < 0,"Heap corrupted");

		res=farheapcheckfree(1);
		if (res < 0)
			{
			Check((res!=_HEAPCORRUPT),"Heap corrupted.");
			Check((res!=_BADVALUE),"Bad value in free space.");
			Check(NOT(res!=_BADVALUE AND res!=_HEAPCORRUPT),"Unknown error");
			}
		Check(NOT(available_mem==0 AND dim==0 AND lis==0));


		if(farheapchecknode(lis)==tipo)
			return;
		Assert(farheapchecknode(lis)!=_USEDENTRY);
		Assert(farheapchecknode(lis)!=_HEAPEMPTY);
		Assert(farheapchecknode(lis)!=_HEAPCORRUPT);
		Assert(farheapchecknode(lis)!=_BADNODE);
		Assert(farheapchecknode(lis)!=_FREEENTRY);
		merr<<"unknown return code";


		 return;
		 }
#endif


//dummy new operator used to get rid of compiler
//this operator inizializes the class doing nothing on memory allocation
//N:B: as it uses size_t  only size < 64k of classes can be inizialized
enum t_dummy_enum {dummy_flag};

#ifdef __32BIT_CODE__
/*
//this is not accepted by some compilers

inline void *operator new[](size_t size,void* where,enum t_dummy_enum temp)
	{
	return  where;
	}
*/
inline void *operator new(size_t size,void* where,enum t_dummy_enum temp)
	{
	return  where;
	}

#else

inline void *operator new(size_t size,void huge * where,enum t_dummy_enum temp)
	{
	return  where;
	}
#endif

//type list

	template <class T>
	inline ImpBaseList<T>::ImpBaseList():
		dim(0UL),
		lis(NULL),
		available_mem(0UL),
		//default data_chunk is 4
		data_chunk(_DEFAULT_DATA_CHUNK){};
/*
	//not defining copy contructor prevents from
	//calling it 
	template <class T>
	ImpBaseList<T>::ImpBaseList(const ImpBaseList<T> & obj)
		{
		//merr<<"the copy constructir of this class must never be called "<<obj.Dim();
		return;
		}
*/		

	template <class T>
	ImpBaseList<T>::ImpBaseList(t_index requested_data_chunk):
		dim(0),
		lis(0),
		available_mem(0),
		data_chunk(requested_data_chunk){};

	template <class T>
	void ImpBaseList<T>::Set_Data_Chunk(t_index requested_data_chunk)
		{
		data_chunk=requested_data_chunk;
		return;
		}


	template <class T>
	void ImpBaseList<T>::Disallocate_Mem()
		{
		#ifdef __CHECK_MEMORY_ALLOC
		Check_Mem( _USEDENTRY);
		#endif

		if(available_mem>0)
			{
			Assert(dim<= available_mem);
			#ifdef __32BIT_CODE__
				free(lis);
			#else
				farfree(lis);
			#endif

			available_mem=0UL;
			dim=0UL;
			lis= (T _HUGEPTR_ * ) 0;
			}
		#ifdef __CHECK_MEMORY_ALLOC
		Check_Mem( _FREEENTRY);
		#endif
		return;
		}

	template <class T>
	inline ImpBaseList<T>::~ImpBaseList()
		{
		Disallocate_Mem();
		return;
		}


	template <class T>
	inline void ImpBaseList<T>::Destroy_And_ReDim(t_index dimension)
		{
		//the last lines are elimiminated because requesting
		//a zero size memory is not an error
		//  if(dimension==0)
		//  	errmsg("ERROR: zero size dimension requested");

		if(dimension<=available_mem)
			{
			dim=dimension;
			return;
			}
			else	{
					Disallocate_Mem();

					#ifdef __32BIT_CODE__
						lis= (T*) malloc((dimension+data_chunk)*sizeof(T));

						if(lis==(T*)(NULL))
							merr<<"not enough allocable mem: num_elem "<<(dimension+data_chunk)<<" size "<<sizeof(T)<<" tot. mem. :"<<(dimension+data_chunk)*(unsigned long)sizeof(T);

					#else
						lis= (T huge *) farmalloc((dimension+data_chunk)*sizeof(T));

						if(lis==(T huge *)(NULL))
							merr<<"not enough allocable mem: num_elem "<<(dimension+data_chunk)<<" size "<<sizeof(T)<<" tot. mem. :"<<(dimension+data_chunk)*(unsigned long)sizeof(T);

					#endif

#ifdef __CHECK_MEMORY_ALLOC
				Check_Mem( _USEDENTRY);
#endif
				dim=dimension;
				available_mem=(dimension+data_chunk);
		}
		return;
	}

	template<class T>
	inline void ImpBaseList<T>::Reset()
		{
		dim=0;
		if (available_mem==0)
			lis=NULL;
		return;
		}

	template<class T>
	void ImpBaseList<T>::Save_And_ReDim(t_index dimension)
		{
		Check(dimension>0,"zero size dimension requested");

		//no memory allocated
		if (available_mem==0)
			{
			//non empty object but no memory allocated
			if (dim!=0)
				{
				T _HUGEPTR_ * temp;
				t_index i,act_dim;
				Assert(lis!=NULL);
				temp=lis;
				act_dim=min(dim,dimension);
				Destroy_And_ReDim(dimension);
				//copy the old elements
				for(i=0;i<act_dim;i++)
					//lis[i]=(T)temp[i];does not work on MSV4.0 
					lis[i]=temp[i];
				}
				//empty object
				else 	{
						Destroy_And_ReDim(dimension);
						}
			return;
			}

		//memory allocated
		if (dimension>available_mem)
			{
#ifdef __32BIT_CODE__
			lis= (T*) realloc(lis,(dimension+data_chunk)*sizeof(T));
			if(lis==(T*)(NULL))
				merr<<"not enough Reallocable mem:: num_elem "<<(dimension+data_chunk)<<" size "<<sizeof(T)<<" tot. mem. :"<<(dimension+data_chunk)*(unsigned long)sizeof(T);

#else
			lis= (T huge *) farrealloc(lis,(dimension+data_chunk)*sizeof(T));
			if(lis==(T huge *)(NULL))
				merr<<"not enough Reallocable mem: num_elem "<<(dimension+data_chunk)<<" size "<<sizeof(T)<<" tot. mem. :"<<(dimension+data_chunk)*(unsigned long)sizeof(T);

#endif

#ifdef __CHECK_MEMORY_ALLOC
			Check_Mem( _USEDENTRY);
#endif
			available_mem=(dimension+data_chunk);
			}
		dim=dimension;
		return;
		}

	template <class T>
	void ImpBaseList<T>::operator= (const ImpBaseList<T> & source)
		{
		t_index len;
		len = source.Dim();

		if (len>0)
			Destroy_And_ReDim(len);
			else Reset();
		ITERATE_ALL(lis[i]=source[i])

		return;
		}

#ifdef _OPERATOR_SQUARE_BRACKET_TO_BE_DECLARED_ALSO_FOR_INT	

	template <class T>
	inline T& ImpBaseList<T>::operator [] ( int index )
		{
		Check( index < (int)dim AND index>=0,"index = "
			<<index<<", dim = "<<dim<<" mem = "<<available_mem );
		return (T&)lis[index];
		}

	template <class T>
	inline const T& ImpBaseList<T>::operator [] ( int index ) const
		{
		Check( index < (int)dim AND index>=0,"index = "
			<<index<<", dim = "<<dim<<" mem = "<<available_mem );
		return (T&)lis[index];
		};


#endif

	template <class T>
	inline T& ImpBaseList<T>::operator [] ( t_index index )
		{
		Check( index < dim ,"index = "
			<<index<<", dim = "<<dim<<" mem = "<<available_mem );

		return (T&)lis[index];
		}

	template <class T>
	inline const T& ImpBaseList<T>::operator [] ( t_index index ) const
		{
		Check( index < dim ,"index = "
			<<index<<", dim ="<<dim<<" mem"<<available_mem );

		return (T&)lis[index];
		};
	template <class T>
	inline t_index ImpBaseList<T>::Dim() const
		{
		return dim;
		}

//append one element
	template <class T>
	void ImpBaseList<T>::Append(const T & obj)
		{
		Save_And_ReDim(dim+1);
		(*this)[dim-1]=obj;
		return;
		}

//append a list
	template <class T>
	void ImpBaseList<T>::Append(const ImpBaseList<T> & obj)
		{
		t_index old_dim,i;
		old_dim=dim;
		Save_And_ReDim(old_dim+obj.Dim());
		for(i=old_dim;i<dim;i++)
			(*this)[i]=obj[i-old_dim];
		return;
		}


//************************************************************************
//                                                                      **
//                      List implementation of Simple Type              **
//                                                                      **
//************************************************************************

#ifdef __32BIT_CODE__
	template <class T>
	void ImpSimpleTypeList<T>::operator=(const ImpSimpleTypeList<T> & source)
		{
		t_index lim=source.Dim();
		Destroy_And_ReDim(lim);
		memcpy(( void * )lis,( void *  ) source.lis,
			lim*((char *)&(source.lis[1])-(char *)source.lis));

		return;
		}
#endif


	template <class T>
	void ImpSimpleTypeList<T>::Set(T num)
		{
		ITERATE_ALL(lis[i]=(T)(num))

		return;
		}

	template <class T>
	void ImpSimpleTypeList<T>::Destroy_And_ReDim(t_index dimension)
		{
		ImpBaseList<T>::Destroy_And_ReDim( dimension);
		ITERATE_ALL(lis[i]=(T)(0))
		return;
		}

	template<class T>
	void ImpSimpleTypeList<T>::Save_And_ReDim(t_index dimension)
		{
		t_index i=Dim(),lim;

		ImpBaseList<T>::Save_And_ReDim(dimension);
		//equivalent of Init_List_ElemS
		lim=Dim();
		for(;i<lim;i++)
			lis[i]=(T)(0);

		return;
		}

	template <class T>
	Boolean ImpSimpleTypeList<T>::operator== (const ImpSimpleTypeList<T> &string1) const
		{
		t_index i=0UL,lim=Dim();
		if( lim!=string1.Dim())
			return FALSE;
		else  {

				while (i<lim AND lis[i]==string1[i])
					i++;
				if (i==lim)
					return TRUE;
					else return FALSE;
				 }
		}

	template<class T>
	Boolean ImpSimpleTypeList<T>::operator != (const ImpSimpleTypeList<T> &string1) const
		{
		t_index i=0UL,lim=Dim();

		if( lim!=string1.Dim())
			return TRUE;
		else  {

				while (i<lim AND lis[i]==string1[i])
					i++;
				if (i==lim)
					return FALSE;
				else return TRUE;
				 }
		}

//************************************************************************
//                                                                                                                                              **
//                      List implementation of objects                                                          **
//                                                                                                                                              **
//************************************************************************

	template <class T>
	ImpObjectList<T>::~ImpObjectList()
		{
		ITERATE_ALL(lis[i].T::~T());
		return;
		}
	template <class T>
	void ImpObjectList<T>::Reset()
		{
		ITERATE_ALL(lis[i].T::~T());

		ImpBaseList<T>::Reset();

		return;
		}


	template <class T>
	inline void ImpObjectList<T>::Destroy_And_ReDim(t_index dimension)
		{
		//this-> added for GNU
		this->ImpObjectList<T>::~ImpObjectList();
		ImpBaseList<T>::Destroy_And_ReDim(dimension);
		Init_List_ElemS(0);
		return;
		}

	template<class T>
	void ImpObjectList<T>::Save_And_ReDim(t_index dimension)
		{
		t_index lastdim=Dim();
		ImpBaseList<T>::Save_And_ReDim(dimension);
		Init_List_ElemS(lastdim);
		return;
		}

//this routine may be changed for 32 bit platforms
//to do inizialization in one shot using the array new
//and speed up inizialization

	template <class T>
	void ImpObjectList<T>::Init_List_ElemS(t_index start)
		{
		t_index num,i;
		enum t_dummy_enum temp=dummy_flag;

		if(start>=Dim())
			return;
		num=Dim()-start;

#ifdef __32BIT_CODE__
		void* pdummy;

		T* pdummy2;
		Check(lis!=(T*)0,"Null pointer passed");
		pdummy =(void  *) &(lis[start]);
//		pdummy2= new (pdummy,temp) T[num];

#else
		void huge * pdummy;
		T huge * pdummy2;

		Check(lis!=(T huge * )0,"Null pointer passed");
		pdummy =(void  huge *) &(lis[start]);
#endif




		//new handles only unsigned int
		//pdummy2= new (pdummy) T[n>1] doesn't work
		//on Borland c++3.1 even if it should work
		//as far as I know this is the only method to invoke
		//standard constructorsof objects
		for(i=0;i<num;i++)
			{
			//new is a dummy routine that forces compiler
			//to initialize objects

			pdummy2= new (pdummy,temp) T;

			Check(pdummy2==pdummy,"ERROR: Compiler doesn't work properly");

			pdummy2++;
			pdummy=pdummy2;
			}


		return;
		}

	template <class T>
	void ImpObjectList<T>::operator= (const ImpObjectList<T> & source)
		{
		t_index len;
		len = source.Dim();
		if (len>0)
			Destroy_And_ReDim(len);
			else Reset();
		ITERATE_ALL((*this)[i]=source[i]);

		return;
		}
//append one element
	template <class T>
	void ImpObjectList<T>::Append(const T & obj)
		{
		Save_And_ReDim(Dim()+1);
		(*this)[Dim()-1].T::operator=(obj);
		return;
		}

//append a list
	template <class T>
	void ImpObjectList<T>::Append(const ImpObjectList<T> & obj)
		{
		t_index old_dim,i, dim_new;

		old_dim=Dim();
		Save_And_ReDim(old_dim+obj.Dim());
		dim_new=Dim();
		for(i=old_dim;i<dim_new;i++)
			(*this)[i]=obj[i-old_dim];

		return;
		}

//************************************************************************************//
//                                                                                    //
//                         class OrderedList                                          //
//                                                                                    //
//************************************************************************************//

template <class T, class E>
void OrderedList<T,E>::Get_Key (const t_index position, T & key_elem)
	{
    key_elem=list[tbl_index[position]].key;
    return; 
	}


//Binary_Search gives back the index (position in the ordered list)
//of the word if the word is present (found ==TRUE),
//else returns the index where must be inserted the new word (found==FALSE).
template <class T, class E>
t_index OrderedList<T,E>::Binary_Search (const T &search_key, Boolean & found)const
	{
    found=FALSE;
    t_signed inf=0;
    t_signed sup=list.Dim()-1;
    t_signed mid;

    while(inf<=sup)
		{
		mid=(inf+sup)/2;
		if (search_key<list[tbl_index[mid]].key)
	   		sup=mid-1;
		else if (search_key>list[tbl_index[mid]].key)
				inf=mid+1;
		else{
			found=(Boolean)TRUE;
			return mid;
			}
		}

    return (t_index)inf;   
	} //end of Binary_Search function

//return true if the key is already present in the ordered list
//in position return the position in the ordered list of the new_key (also if already present)
template <class T, class E>
Boolean OrderedList<T,E>::Insertion(const T & new_key, const E & new_info, t_index & position)

	{
	t_index i;
	t_index dim=list.Dim()+1;
	Boolean present;
	
    position=Binary_Search(new_key, present);

	if(!present)
		{
		OrderedListElem new_elem;
		new_elem.key=new_key;
		new_elem.info=new_info;

		list.Append(new_elem);

		tbl_index.Save_And_ReDim(dim);
      
		Assert (position<=dim-1);

		if (position<dim-1)
			{
			for (i=dim-1; i>position; i--)
				tbl_index [i]=tbl_index[i-1];
			tbl_index[position]=dim-1;
			}
		else tbl_index[position]=position;
		}
    return present;

	}//end of Insertion function


template <class T, class E>
t_index OrderedList<T,E>::Dim()
	{
	return list.Dim();
	}
	

#endif
