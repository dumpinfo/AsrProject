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
//  |   FILE:   textclas.h                                              |
//  |   FUNCTIONALITY: class String definition                          |
//  |   PROGRAM: required to  codes which employ text                   |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Claudio, Domenico					            |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   4.0                                            |
//  |   DATA FIRST VERSION: 09/9/97                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************
//added Linux compatibility
//12/02/98 added String(const char *) eliminated operator +=
//NULL and const char *
//are NO LONGER necessary they must never be used !!
//null can be replaced by ""
//05/02/98 stringadder eliminated//16/9/97 read code revision by claudio
//02/10/97 corrected operator >> by Domenico
//25/9/97 added Delete_Non_Print_Space_Tab_Chars
//	added void operator += ( StringAdder & pstring);
//pay attention a syntax like string=const char * +const char * is misinterpreted by the system
//the previous problem has been fixed
// string concatenation is now with << operator and not + this
//is because string = const char * + char was allowed
//even if it gave wrong results
//string = const char * << char does not work//16/9/97 added  operator + (int or long or t_real )
//added operator >>






// *******************   USER INFORMATION   ******************************
//pay attention comparison of two const char * remains a pointer comparison !!
// the system checks if strings are greater then MAX_LEN_STRING if so check error

//NULL and const char * are NO LONGER necessary they must never be used !!
//NULL can be replaced by ""

// *******************   Implementation ISSUES   ******************************
//1) at the end of any String a zero is added for compatibility with char *
//2) all String objects are converted to const char * to be used instead of C char pointers

//3) concat is implemented through operator <<
//4) a const char * ptr=NULL is considered a "" ( in some compilers this is automatic)

// *******************   to be implemented   ******************************



//Functionality:

#ifndef __TEXTCLAS_
#define __TEXTCLAS_


#include"../baseclas/baseclas.h"


//************************************************************************
//                                                                      **
//                      class String                                    **
//                                                                      **
//************************************************************************

#define OPERATORPLUS_EQ_NOT_DEFINED

class String:public ImpSimpleTypeList<char>
{
public:


	//return dim as strlen does, ('\0' is not counted)
	t_index Dim() const;
	
	//cast String -> const char * required for old style C-code
	operator const char _HUGEPTR_ *() const;

	//cast const char *  -> String 
	String (const char * arg) ;
	String ();

	//used for string concatenation
	String & operator<<  (const char _HUGEPTR_ *pstring) ;
	String & operator<<  (const String & string) ;

	String & operator<<  (unsigned long num)  ;
	String & operator<<  (long num) ;
	String & operator<<  (double num)  ;
	String & operator<<  (int num)  ;
	String & operator<<  (unsigned int num)  ;
	String & operator<<  (char num)  ;
	String & operator<<  (short num)  ;
	String & operator<<  (unsigned short num) ;

//it should be eliminated because 
//operator << has the same behaviour 

#ifndef OPERATORPLUS_EQ_NOT_DEFINED
	//used for string concatenation
	void operator += (const char _HUGEPTR_ *pstring);
	void operator += (const String & string) ;

	void operator += (unsigned long num);
	void operator += (long num);
	void operator += (double num);
	void operator += (int num);
	void operator += (unsigned int num);
	void operator += (char num);
	void operator += (short num);
	void operator += (unsigned short num);
#endif
	//added for linux
    void operator =   (const String _HUGEPTR_ & string) ;
	//used for assignments
	void operator =   (const char _HUGEPTR_ *pstring) ;
	void operator =   (unsigned long num) ;
	void operator =   (long num) ;
	void operator =   (double num) ;
	void operator =   (int num) ;
	void operator =   (unsigned int num) ;
	void operator =   (char num) ;
	void operator =   (short num) ;
	void operator =   (unsigned short num) ;


	//relation operators

	Boolean operator == (const String & mystr) const;
	Boolean operator != (const String & mystr) const;
	Boolean operator <	(const String & mystr) const;
	Boolean operator >	(const String & mystr) const;
	Boolean operator <= (const String & mystr) const;
	Boolean operator >= (const String & mystr) const;

	String & Set(char car);

	String &  Make_Upper(); //convertion to uppercase
	String &  Make_Lower(); //convertion to lowercase

	 //return False if substring not present,
	 //if true  position is set to the start of the substring
	Boolean Is_SubString_Inside(const String & str,t_index & position)const;

	//form a substring from a string from start to end comprised
	//if end of string return number of char in string otherwise end-start+1 char returned
	t_index Take(const String & str, t_index start, t_index end);

	//prune all ch char from a string
	void Cut_Char ( char ch);

	//prune all non print char and space and tabs
	void Delete_Non_Print_Space_Tab_Chars();
	
	static Boolean Test_Class();


};



	//all combination of relation operators
	Boolean operator== (const String & mystr,const char _HUGEPTR_ * pstring);
	Boolean operator== (const char _HUGEPTR_ * pstring,const String & mystr);

	Boolean operator!= (const String & mystr,const char _HUGEPTR_ * pstring);
	Boolean operator!= (const char _HUGEPTR_ * pstring,const String & mystr);

	Boolean operator< (const String & mystr,const char _HUGEPTR_ * pstring);
	Boolean operator< (const char _HUGEPTR_ * pstring,const String & mystr);

	Boolean operator> (const String & mystr,const char _HUGEPTR_ * pstring);
	Boolean operator> (const char _HUGEPTR_ * pstring,const String & mystr);

	Boolean operator>= (const String & mystr,const char _HUGEPTR_ * pstring);
	Boolean operator>= (const char _HUGEPTR_ * pstring,const String & mystr);

	Boolean operator>= (const String & mystr,const char _HUGEPTR_ * pstring);
	Boolean operator>= (const char _HUGEPTR_ * pstring,const String & mystr);


	//this operator works exactly as the operator>> for strings
	istream& operator>>(istream& s, String &string);




#endif
