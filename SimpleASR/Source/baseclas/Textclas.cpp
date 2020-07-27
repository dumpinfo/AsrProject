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
//  |   FILE:   textclas.cpp                                            |
//  |   FUNCTIONALITY: code for class String                            |
//  |   PROGRAM: required to  codes which employ text                   |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio , Domenico				                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 09/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#define TEXT_CLASS_VERSION "V. 6.0 of 19/11/98 " ## __FILE__ " c." ## __DATE__

// *******************    VERSION HISTORY  *******************************
//17/4/98 added linux compatibility

//12/02/98 fixed error with concat string eliminated string adder  concatenation
// is now correct with <<
//
//16/9/97 read code revision by claudio
//25/5/97 added Delete_Non_Print_Space_Tab_Chars
// string concatenation is now with << operator and not + this
//is because string = const char * + char was allowed
//even if it gave wrong results
//string = const char * << char does not work

//07/05/98 Domenico
//         corrected bug on operator<< overloading

// *******************   Implementation ISSUES   ******************************
//it should be better to use template functions
//but not on all the compilers support explicit instantiation 
//of function templates. compilers generally 
//support only the explicit instantiation of class templates



#include "../baseclas/textclas.h"
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <ctype.h>



//************************************************************************
//                                                                      **
//                      class String functions                          **
//                                                                      **
//************************************************************************

	 String::String() {};

	 String::String(const char * ptr)
		{
		if (ptr==NULL)
			{
			Reset();
			return;
			}
		//assignment without allocation of memory
		lis=(char _HUGEPTR_ *)ptr;
		dim =strlen(ptr)+1;
		return;
		};



	t_index String::Dim() const
		{
		t_index temp;

		temp=ImpSimpleTypeList<char>::Dim();
		if (temp==0)
			return 0;

		return temp-1;
		}

	String & String::Set(char car)
		{
		ImpSimpleTypeList<char>::Set(car);
		//this is not  ImpSimpleTypeList<char>::Dim()
		lis[Dim()]='\0';
		return (*this);
		}


	String::operator const char _HUGEPTR_ *() const
		{
		static const char * empty_string="";

		if(Dim()==0)
			return empty_string;
		return (const char _HUGEPTR_ *)lis;
		}

	
//****************** operator <<**************
//it should be better to use template functions
//but not on all the compilers support explicit instantiation 
//of function templates. compilers generally 
//support only the explicit instantiation of class templates


String & String::operator<<(const String & pstring)
	{	
	if(pstring.Dim()==0)
		return(*this);
	if(ImpSimpleTypeList<char>::Dim()>0)
		Save_And_ReDim(ImpSimpleTypeList<char>::Dim()-1);//delete '\0'
	Append(pstring);
	return(*this);
	}


String & String::operator<< (const char _HUGEPTR_ * pstring)
		{
		t_index temp, len_char;
			
		if (pstring==NULL)
				return(*this);
			
		len_char= strlen(pstring);
			
		if (len_char==0)
				return(*this);
			
		temp=Dim();
			
		Check(len_char<MAX_LEN_STRING,"string capacity: "<<MAX_LEN_STRING<<" exceeded, requested:"<<len_char);
			
			
		if (temp==0)//empty string
			{
			Save_And_ReDim(1+len_char);
			lis[0]='\0';
			}
			else	{
					Save_And_ReDim(temp+len_char+1);
					}
		strcat(( char _HUGEPTR_ *)lis,pstring);
			return(*this);
		}



 #define OPERATOR_DOUBLE_ARROW(type,format)	\
											\
String & String::operator << (type num)   	\
		{                                   \
		char buf[256];                      \
											\
		sprintf(buf,format,num);            \
		(*this)<<buf;                       \
											\
		return(*this);                  \
		}

	 OPERATOR_DOUBLE_ARROW(unsigned long,"%lu")
	 OPERATOR_DOUBLE_ARROW(long,"%ld")
	 OPERATOR_DOUBLE_ARROW(double,"%lg")
	 OPERATOR_DOUBLE_ARROW(int,"%i")
	 OPERATOR_DOUBLE_ARROW(unsigned int,"%u")
	 OPERATOR_DOUBLE_ARROW(char,"%c")
	 OPERATOR_DOUBLE_ARROW(short,"%hd")
	 OPERATOR_DOUBLE_ARROW(unsigned short,"%hu")

#undef OPERATOR_DOUBLE_ARROW


//****************** operator +=**************
#ifndef OPERATORPLUS_EQ_NOT_DEFINED


#define OPERATOR_PLUS_STRING(type)                      \
														\
	void String::operator+= (type num)    	 		\
		{                                               \
		operator<<(num);       							\
		return (*this);                                 \
		}
	
		OPERATOR_PLUS_STRING(const char _HUGEPTR_ *)
		OPERATOR_PLUS_STRING(const String & )
		OPERATOR_PLUS_STRING(unsigned long)
		OPERATOR_PLUS_STRING(long)
		OPERATOR_PLUS_STRING(double)
		OPERATOR_PLUS_STRING(int)
		OPERATOR_PLUS_STRING(unsigned int)
		OPERATOR_PLUS_STRING(char)
		OPERATOR_PLUS_STRING(short)
		OPERATOR_PLUS_STRING(unsigned short)
		
//prevent form side-effect
#undef  OPERATOR_PLUS_STRING

#endif

//********************** operator = *****************
void String::operator =   (const String _HUGEPTR_ & string)
		 {
		 if (string.Dim()==0)
			{
			Destroy_And_ReDim(1);
			lis[0]='\0';
			return;
			}
		Destroy_And_ReDim(string.Dim()+1);
		strcpy(( char _HUGEPTR_ *)lis,string.lis);
		return;
		}

void String::operator= (const char _HUGEPTR_ * pstring)
		{
		t_index len;
		//pstring ==NULL is not an error because compiler returns null
		//the empty string ""
		if (pstring==NULL OR strlen(pstring)==0)
			{
			Destroy_And_ReDim(1);
			lis[0]='\0';
			return;
			}
		len=(strlen(pstring)+1);
		Check(len<MAX_LEN_STRING,"string capacity: "<<MAX_LEN_STRING<<" exceeded, requested:"<<len);
		Destroy_And_ReDim(len);
		strcpy(( char _HUGEPTR_ *)lis,pstring);
		return;
		}



 #define OPERATOR_EQ(type,format)  			\
											\
	void String::operator = (type num)   	\
		{                                   \
		char buf[256];                      \
		sprintf(buf,format,num);            \
		(*this)=buf;						\
											\
		return;                             \
		}

	 OPERATOR_EQ(unsigned long,"%lu")
	 OPERATOR_EQ(long,"%ld")
	 OPERATOR_EQ(double,"%lg")
	 OPERATOR_EQ(int,"%i")
	 OPERATOR_EQ(unsigned int,"%u")
	 OPERATOR_EQ(char,"%c")
	 OPERATOR_EQ(short,"%hd")
	 OPERATOR_EQ(unsigned short,"%hu")

#undef OPERATOR_EQ

	Boolean  String::Is_SubString_Inside (const String & sub, t_index &position)const
		{

		t_index i=0, j=0;

		position=0;
		if(sub.Dim()==0)
			return FALSE;
		if (Dim()==0)
			return FALSE;

		
		while ((*this)[i]!='\0' AND sub[j]!='\0')
			if((*this)[i] == sub[j])
				{
				j++;
				i++;
				}
				else  {
						i++;
						position=i;
						j=0;
						}
		if(sub[j]=='\0')
				return TRUE;
			else return FALSE;
		}

	 void String::Cut_Char (char ch)
		{
		t_index i=0,j=0;
		if(Dim()==0)
			return;
		while ((*this)[i]!='\0')
			{
			if ((*this)[i]!=ch)
			  {
			  (*this)[j]=(*this)[i];
			  j++;
			  }
			i++;
			}

		 (*this)[j]='\0';
		 Save_And_ReDim(j+1);
		 return;
		 }

	 void String::Delete_Non_Print_Space_Tab_Chars()
		{
		t_index i=0,j=0;
		int car;

		if (Dim()==0)
			return;
		
		while ((*this)[i]!='\0')
			{
			car=(int) (*this)[i];
			if ( isprint(car)AND NOT isspace(car))
			  {
			  (*this)[j]=(*this)[i];
			  j++;
			  }
			i++;
			}

		 (*this)[j]='\0';
		 Save_And_ReDim(j+1);
		 return;
		 }

	t_index String::Take(const String & str, t_index start, t_index end)
		{
		t_index i;
		Assert(str.Dim()>0);

		Check(start<=end,"bound not valid in take, start:"<<start<<" end: "<<end);
		Check(start<str.Dim(),"bound not valid in take, start:"<<start<<" end: "<<end<<" str.Dim()"<<str.Dim());

		if (end >= str.Dim())
			end=str.Dim()-1;

		Destroy_And_ReDim(end-start+2);

		for(i=0;i<end-start+1 ;i++)
			 (*this)[i]=str[i+start];

		(*this)[end-start+1]='\0';

		return end-start+1;
		}

	Boolean operator== (const String & mystr,const char _HUGEPTR_ * pstring)
		{
		//in C ""==""
		if (pstring==NULL)
			{
			if (mystr.Dim()==0)
				return TRUE;
				else return FALSE;
			}

		 t_index len_char= strlen(pstring);
		 Check(len_char<MAX_LEN_STRING,"string capacity: "<<MAX_LEN_STRING<<" exceeded, requested:"<<len_char);

		 if( mystr.Dim()!=len_char)
			 return (Boolean)FALSE;
			 else return (strcmp(mystr,pstring)==0);
		}

	Boolean operator== (const char _HUGEPTR_ * pstring,const String & mystr)
		{
		return(mystr==pstring);
		}

	Boolean String::operator == (const String & mystr) const
		{
		return (mystr== (const char _HUGEPTR_ *) (*this));;
		}

	Boolean operator!= (const String & mystr,const char _HUGEPTR_ * pstring)
		{
		return(NOT(mystr==pstring));
		}

	Boolean operator!= (const char _HUGEPTR_ * pstring,const String & mystr)
		{
		return(mystr!=pstring);
		}

	Boolean String::operator != (const String & mystr) const
		{
		 return (mystr!= (const char _HUGEPTR_ *) (*this));;
		}

	Boolean operator< (const String & mystr,const char _HUGEPTR_ * pstring)
		{
		//in C string comparison  "a">"" and "ab">"a"
		if (pstring==NULL OR strlen(pstring)==0)
			return FALSE;
		
		if(mystr.Dim()==0)
			return TRUE;

		t_index len_char= strlen(pstring);
		Check(len_char<MAX_LEN_STRING,"string capacity: "<<MAX_LEN_STRING<<" exceeded, requested:"<<len_char);
		return (strcmp(mystr,pstring)<0);
		}

	Boolean operator< (const char _HUGEPTR_ * pstring,const String & mystr)
		{
		return(mystr>pstring);
		}

	Boolean String::operator < (const String & mystr) const
		{
		return (mystr> (const char _HUGEPTR_ *) (*this));;
		}

	Boolean operator>(const String & mystr,const char _HUGEPTR_ * pstring)
		{
		//in C string comparison  "a">"" and "ab">"a"
		if (pstring==NULL OR strlen(pstring)==0)
			if(mystr.Dim()>0)
			return TRUE;
			else return FALSE;
		
		if(mystr.Dim()==0)
			return FALSE;
		
		t_index len_char= strlen(pstring);
		
		Check(len_char<MAX_LEN_STRING,"string capacity: "<<MAX_LEN_STRING<<" exceeded, requested:"<<len_char);
		return (strcmp(mystr,pstring)>0);
		}

	Boolean operator> (const char _HUGEPTR_ * pstring,const String & mystr)
		{
		return(mystr<pstring);
		}

	Boolean String::operator > (const String & mystr) const
		{
		 return (mystr< (const char _HUGEPTR_ *) (*this));;
		}


	Boolean operator>=(const String & mystr,const char _HUGEPTR_ * pstring)
		{
		 return(NOT(mystr<pstring));
		}

	Boolean operator>= (const char _HUGEPTR_ * pstring,const String & mystr)
		 {
		 return(NOT(pstring<mystr));
		}

	Boolean String::operator >= (const String & mystr) const
		{
		 return (NOT( (const char _HUGEPTR_ *) (*this)<mystr ));;
		}


	Boolean operator<=(const String & mystr,const char _HUGEPTR_ * pstring)
		{
		 return(NOT(mystr>pstring));
		}

	Boolean operator<= (const char _HUGEPTR_ * pstring,const String & mystr)
		{
		 return(NOT(pstring>mystr));
		}

	Boolean String::operator <= (const String & mystr) const
		{
		return (NOT((const char _HUGEPTR_ *) (*this)>mystr ));
		}

	String & String::Make_Lower()
		 {
		 t_index i,lim;
		 lim=Dim();
		 for (i=0;i<lim;i++)
			if ( ((*this)[i]>='A')AND((*this)[i]<='Z') )
				(*this)[i]+=(char)('a'-'A');
		 return (*this);
		}

	String & String::Make_Upper()
		 {
		 t_index i,lim;
		 lim=Dim();

		 for (i=0;i<lim;i++)
			 if ( ((*this)[i]>='a')AND((*this)[i]<='z') )
				 (*this)[i]+=(char)('A'-'a');

		 return (*this);
		 }

	istream& operator>>(istream& s, String &string)
		 {
		 t_signed c;

		 string.Reset();

		while((c = s.get(), isspace(c)));

		string.Append((char)c);

		while((c = s.get(), ! isspace(c))  &&  c != EOF )
			string.Append((char)c);

		string.Append('\0');

		return s;
		}





	Boolean String::Test_Class()
		{
		String bip,null_string;
		String mat="123";
		int num=12345;
		mwarn<<"Text class  Version: "<<TEXT_CLASS_VERSION;
		
		mwarn<<"testing Text Classes";
		
		
		
		bip=mat;
		mat.Reset();
		mat<<bip<<456<<(short)7890<<bip<<"456"<<num;

		//test empty string concatenation
		mat<<null_string<<(const char *)(null_string)<<""<<'\0';
		
		Check(mat=="123456789012345612345","concatenation doesn't work "<<mat<<"!="<<"123456789012345612345");
		mat="test stream output ok";
		mat=5.8;
		Assert(mat=="5.8");
		mstat<<mat<<" len string "<<mat.Dim();
		mat.Destroy_And_ReDim(60000UL);
		mat.Set('1');

		for(t_index i=0;i<mat.Dim();i++)
			Check(mat[i]=='1',"failed Set");
		
		mat="adesso";
		if(mat=="adesso" AND "adesso" ==mat)
			mstat<<"test == ok";
		else mstat<<"test == false";
		
		mat<<bip;
		if(mat!=bip)
			mstat<<"test != ok"<<mat;
		mstat<<mat.Make_Upper();
		mstat<<mat.Make_Lower();
		
		if(mat> bip)
			mstat<<"test != ok"<<mat;
		
		
		String pr;
		pr="number";
		pr<<num;
		if (pr=="number12345")
			mstat<<pr<<" test ok";
		else mstat<<pr<<"test failed";
		
		long num2=34567L;
		pr<<num2;
		if (pr=="number1234534567")
			mstat<<pr<<" test ok";
		else mstat<<pr<<"test failed";
		mat.Take(pr,1,6);
		Assert(mat== "umber1");
		mat.Take(pr,14,18);
		Assert(mat== "67");
		
		double numd=45.345;
		String st;
		st="number";
		st<<numd;
		mstat<<st<<" test double";
		st="";
		mstat<<"Write two strings:"<<endl;
		cin>>st>>mat;
		
		mstat<<"The string is : "<<st;
		mstat<<"second string: "<<mat;
		//check inequalities
		mat="";
		bip="";
		Assert(mat==bip);
		Assert(mat=="" AND ""==bip);
		mat="a";
		Assert(mat>bip);
		Assert("a">bip);
		bip=mat;
		mat<<"b";
		Assert(mat>bip AND "ab">bip);
	
	
		mwarn<<"Text Classes OK";
		return 1;
		};



