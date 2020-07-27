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
//  |   FILE:   Lessico.cpp                                             |
//  |   FUNCTIONALITY:                                                  |
//  |   PROGRAM:                                                        |
//  |   COMMENTS:                                                       |
//  |   CONTRIBUTIONS: Domenico			                                |
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 23/1/96                                     |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

// *******************    VERSION HISTORY  *******************************

// <<< version 4.0 >>>
//old version 03/12/97, 24/4/98 added Unix compatibility
//claudio fixed bug with linux 19/11/98

#include "../lessico/lessico.h"

//max number of phonemes of a atis word
#define MAX_NUM_PHON 15   

//max number of digits (decimal point included) of the silence log prob
#define DIM_BUFFER_SILENCE 12


//read from file of vocabulary the logprobability of silence
void Lexicon::Read_Silence_Logprob (const String & f_name)
	{
    ifstream f_vocab;
    t_index i, j=0;
	t_index index=0;
	char buffer[__MAX_LEN_LINE], buf [DIM_BUFFER_SILENCE];
	Boolean esc, found=FALSE;
	String str;

	f_vocab.open(f_name, ios::in|ios::nocreate);

	if (f_vocab.fail())
		merr<<"Cannot open file : "<<f_name;

	//jump header of file of vocabulary
    for (i=0; i<6; i++)
		f_vocab.getline(buffer, __MAX_LEN_LINE);

	do	{
		index++;
		f_vocab.getline(buffer, __MAX_LEN_LINE);
		}
	while (buffer[0]!='s');

	do	{
		esc=FALSE;
		for (i=0; i<7 AND !esc; i++)
			if (buffer[i]!=' ')
				str<<buffer[i];
			else esc=TRUE; 
		if (!esc AND str=="silence")
			{	
			//reading the unigram logprobability of silence 
			while (buffer[i]==(char)9)
				i++;
			while (buffer[i]!=(char)9)
				{
				buf[j]=buffer[i];
				i++;
				j++;
				}
			while (j<DIM_BUFFER_SILENCE)
				{
				buf[j]='0';
				j++;
				}
			vocab[index].unigram_lprob=(t_real)atof(buf);
			found=TRUE;
			}
		str="";
		f_vocab.getline (buffer, __MAX_LEN_LINE);
		index++; 
		}
	while (!found);
	return;
	}


//create a list of files
void Lexicon::Load_File_List (const String & name_file_list)
	{
    ifstream file_in;
	String dir, temp;
	t_index dim;

    file_in.open(name_file_list, ios::in|ios::nocreate);
	if (file_in.fail())
		merr<<"Cannot open file : "<<name_file_list;  

    //to avoid continuous memary allocations
    file_list.Set_Data_Chunk(100);
  
	file_in>>dir;
	if (dir[dir.Dim()-1]=='\\'OR dir[dir.Dim()-1]=='/')
		{
		while (!file_in.eof())
			{ 
			file_in>>temp;
			if (temp[0]!=EOF)
				{
				file_list.Append(dir);
				file_list[file_list.Dim()-1]<<temp;
				}
			}
		}
	else{
		file_list.Append(dir);
		while (!file_in.eof())
			{
			file_in>>temp;
			if (temp[0]!=EOF)
				file_list.Append(temp);
			}
		}

    file_in.close();
    //check that file_list contain at least a only file
    dim=file_list.Dim();
    if (dim==0 OR (dim==1 AND file_list[0][file_list[0].Dim()-1]=='\\'))
		merr<< "The file of files list "<<name_file_list<<" is empty."<<endl;  

	return;
    
	}//end of Load_File_List function


void Lexicon::Update_Phoneme_List (const t_index & card_label)
	{
    phon_list.Destroy_And_ReDim(63);

    phon_list[0] ="#h"; phon_list[1] ="aa"; phon_list[2] ="ae"; phon_list[3] ="ah";
    phon_list[4] ="ao"; phon_list[5] ="aw"; phon_list[6] ="ax"; phon_list[7] ="ax-h";
	phon_list[8] ="axr";phon_list[9] ="ay"; phon_list[10]="b";  phon_list[11]="bcl"; 
	phon_list[12]="ch"; phon_list[13]="d";  phon_list[14]="dcl";phon_list[15]="dh"; 
	phon_list[16]="dx"; phon_list[17]="eh"; phon_list[18]="el"; phon_list[19]="em";
	phon_list[20]="en"; phon_list[21]="eng";phon_list[22]="epi";phon_list[23]="er";
	phon_list[24]="ey"; phon_list[25]="f";  phon_list[26]="g";  phon_list[27]="gcl"; 
	phon_list[28]="h#"; phon_list[29]="hh"; phon_list[30]="hv"; phon_list[31]="ih"; 
	phon_list[32]="ix"; phon_list[33]="iy"; phon_list[34]="jh"; phon_list[35]="k"; 
	phon_list[36]="kcl";phon_list[37]="l";  phon_list[38]="m";  phon_list[39]="n"; 
	phon_list[40]="ng"; phon_list[41]="nx"; phon_list[42]="ow"; phon_list[43]="oy"; 
	phon_list[44]="p";  phon_list[45]="pau";phon_list[46]="pcl";phon_list[47]="q"; 
	phon_list[48]="qcl";phon_list[49]="r";  phon_list[50]="s";  phon_list[51]="sh"; 
	phon_list[52]="t";  phon_list[53]="tcl";phon_list[54]="th"; phon_list[55]="uh"; 
	phon_list[56]="uw"; phon_list[57]="ux"; phon_list[58]="v";  phon_list[59]="w"; 
	phon_list[60]="y";  phon_list[61]="z";  phon_list[62]="zh";
	
	label.Destroy_And_ReDim(63);
   
	if (card_label==46)
		{
		label[0] =44; label[1] =10; label[2] =3;  label[3] =6;  label[4] =9;  label[5] =14;
		label[6] =5;  label[7] =5;  label[8] =20; label[9] =12; label[10]=27; label[11]=42;
		label[12]=24; label[13]=28; label[14]=42; label[15]=26; label[16]=45; label[17]=2;
		label[18]=16; label[19]=21; label[20]=22; label[21]=23; label[22]=43; label[23]=20;
		label[24]=11; label[25]=36; label[26]=29; label[27]=42; label[28]=44; label[29]=40;
		label[30]=40; label[31]=1;  label[32]=4;  label[33]=0;  label[34]=25; label[35]=32;
		label[36]=41; label[37]=16; label[38]=21; label[39]=22; label[40]=23; label[41]=22;
		label[42]=15; label[43]=13; label[44]=30; label[45]=44; label[46]=41; label[47]=32;
		label[48]=41; label[49]=17; label[50]=38; label[51]=39; label[52]=31; label[53]=41;
		label[54]=37; label[55]=8;  label[56]=7;  label[57]=7;  label[58]=35; label[59]=19;
		label[60]=18; label[61]=33; label[62]=34;
		}
	else if (card_label==63)
			{ 
			label[0] =60; label[1] =11; label[2] =3;  label[3] =7;  label[4] =10; 
			label[5] =15; label[6] =6;  label[7] =62; label[8] =22; label[9] =13; 
			label[10]=33; label[11]=55; label[12]=30; label[13]=34; label[14]=56; 
			label[15]=32; label[16]=35; label[17]=2;  label[18]=23; label[19]=24;  
			label[20]=25; label[21]=26; label[22]=58; label[23]=21; label[24]=12; 
			label[25]=45; label[26]=37; label[27]=57; label[28]=59; label[29]=49;
			label[30]=50; label[31]=1;  label[32]=5;  label[33]=0;  label[34]=31; 
			label[35]=40; label[36]=53; label[37]=17; label[38]=27; label[39]=28; 
			label[40]=29; label[41]=36; label[42]=16; label[43]=14; label[44]=38; 
			label[45]=61; label[46]=51; label[47]=41; label[48]=54; label[49]=18;  
			label[50]=47; label[51]=48; label[52]=39; label[53]=52; label[54]=46; 
			label[55]=9;  label[56]=8;  label[57]=4;  label[58]=44; label[59]=20;
			label[60]=19; label[61]=42; label[62]=43;
			}
    else if (card_label==39)
			{
			label[0] =37; label[1] =7;  label[2] =3;  label[3] =4;  label[4] =7; 
		    label[5] =11; label[6] =4;  label[7] =4;  label[8] =17; label[9] =9; 
		    label[10]=24; label[11]=37; label[12]=21; label[13]=25; label[14]=37; 
		    label[15]=23; label[16]=38; label[17]=2;  label[18]=13; label[19]=18;  
		    label[20]=19; label[21]=20; label[22]=37; label[23]=17; label[24]=8; 
		    label[25]=32; label[26]=26; label[27]=37; label[28]=37; label[29]=36;
            label[30]=36; label[31]=1;  label[32]=1;  label[33]=0;  label[34]=22; 
		    label[35]=29; label[36]=37; label[37]=13; label[38]=18; label[39]=19; 
		    label[40]=20; label[41]=19; label[42]=12; label[43]=10; label[44]=27; 
		    label[45]=37; label[46]=37; label[47]=29; label[48]=37; label[49]=14;  
		    label[50]=34; label[51]=35; label[52]=28; label[53]=37; label[54]=33; 
		    label[55]=6;  label[56]=5;  label[57]=5;  label[58]=31; label[59]=16;
	        label[60]=15; label[61]=30; label[62]=35;
	        }
	else if (card_label==36)
	        {
            label[1] =7;  label[2] =3;  label[3] =4;  label[4] =7; 
	        label[5] =11; label[6] =4;  label[7] =4;  label[9] =9;  label[10]=22;
			label[12]=19; label[13]=23; label[15]=21; label[17]=2;  label[18]=13;
			label[19]=17; label[20]=18; label[24]=8;  label[25]=30; label[26]=24;
			label[29]=34; label[30]=34; label[31]=1;  label[32]=1;  label[33]=0;  
			label[34]=20; label[35]=27; label[37]=13; label[38]=17; label[39]=18; 
		    label[41]=18; label[42]=12; label[43]=10; label[44]=25; label[49]=14;  
		    label[50]=32; label[51]=33; label[52]=26; label[54]=31; label[55]=6;  
			label[56]=5;  label[57]=5;  label[58]=29; label[59]=16; label[60]=15; 
			label[61]=28; label[62]=33;

			//phonemes out of atis
			label[0] =(t_index) -1; //#h 
			label[8] =(t_index) -1; //axr
            label[11]=(t_index) -1; //bcl
			label[14]=(t_index) -1; //dcl
			label[16]=(t_index) -1; //dx 
            label[21]=(t_index) -1; //eng 
			label[22]=(t_index) -1; //epi 
			label[23]=(t_index) -1; //er 
            label[27]=(t_index) -1; //gcl 
			label[28]=(t_index) -1; //h# 
			label[36]=(t_index) -1; //kcl 
            label[40]=(t_index) -1; //ng 
			label[45]=(t_index) -1; //pau 
			label[46]=(t_index) -1; //pcl 
			label[47]=(t_index) -1; //q
			label[48]=(t_index) -1; //qcl 
			label[53]=(t_index) -1; //tcl
			}
	return;
	}
	
//return the index of phoneme
t_index Lexicon::Index_Of_Phoneme (const String & phoneme, const LabelType & label_t)
	{
    Boolean found;
 
    found=FALSE;
    t_signed inf=0;
    t_signed sup=62;
    t_signed mid;

    if (phoneme=="sil")
		if (label_t==NTimitReduced) 
			return 44;
	else if (label_t==NTimit39)
		   return 37;
	else if (label_t==AtisReducedLabels)
	       return 35;
	else return 60;

    while (inf<=sup AND !found)
		{
		mid=(inf+sup)/2;
	    if (phoneme<phon_list[mid])
			sup=mid-1;
		else if (phoneme>phon_list[mid])
			     inf=mid+1;
        else found=(Boolean)TRUE;
		}
 
    Check(found,"Not found phoneme :"<<phoneme);

    return label[mid];
	}


//initialize word classes : each class is a single word of vocabulary
void Lexicon::Initialize_Word_Classes (t_index & sil_index)
	{
	t_index dim=vocab.Dim();
	t_index i;
	Boolean found;
  
    sil_index=vocab.Binary_Search("silence", found);
	Check(found,"silence word not found in the vocabulary";);

    //the number of word classes is equal to the number of word of the vocabulary
	autom_classes.Destroy_And_ReDim(dim);

    //update the unigram class probabilities
	for (i=0; i<dim; i++)
		autom_classes[i].unig_prob=exp(vocab[i].unigram_lprob);

    //update the class transitions probabilities
    Compute_Class_Bigram_Probabilities ();

    for (i=0; i<dim; i++)
		{
		autom_classes[i].words.Set_Data_Chunk(10);
		autom_classes[i].words.Destroy_And_ReDim(1);
		autom_classes[i].words[0]=i;
		}

	return;
   
	} //end of Lexicon::Initialize_Word_Classes function


//compute the mean vector of candidates classes
void Lexicon::Compute_Mean_Vector (t_index first, t_index second, VetDouble & mean)
	{
    t_index num_words_first=autom_classes[first].words.Dim();
	t_index num_words_second=autom_classes[second].words.Dim();

    mean.Reset();
	mean=autom_classes[first].mean_vector*(t_real)num_words_first;
	mean+=autom_classes[second].mean_vector*(t_real)num_words_second;
	mean/=(t_real)(num_words_first+num_words_second);

    return;
	}


t_real Lexicon::Compute_Error (t_index first, t_index second, const VetDouble & mean)
	{
	t_index i, j;
	t_index dim=vocab.Dim();
	t_index num_words_first=autom_classes[first].words.Dim();
	t_index num_words_second=autom_classes[second].words.Dim();
	t_real error;
	VetDouble temp;

	t_index w_index;
	t_real big_lp;

	temp.Destroy_And_ReDim(dim);
	error=0.0;
	for (i=0; i<num_words_first; i++)
		{
		w_index =autom_classes[first].words[i];
		for (j=0; j<dim; j++)
			{
			big_lp=vocab[w_index].bigram_lprob[j];
			if (big_lp!=0)
				temp[j]=pow(big_lp-mean[j],2.0)*((t_real)vocab[w_index].occur_word/big_lp);
			else temp[j]=0.0;
			error+=temp[j];
			}
   		}
   
	for (i=0; i<num_words_second; i++)
		{
		w_index=autom_classes[second].words[i];
		for (j=0; j<dim; j++)
			{
			big_lp=vocab[w_index].bigram_lprob[j];
			if (big_lp!=0)
				temp[j]=pow(big_lp-mean[j],2.0)*((t_real)vocab[w_index].occur_word/big_lp);
			else temp[j]=0.0;
			error+=temp[j];
			}
		}
	return error;
	}
   
void Lexicon::Merge_Classes (ImpObjectList<TIndexCouple> & couples, t_index & actual_card)
	{
	t_index i, j, k;
    t_index num_couples=couples.Dim();
	ImpObjectList<VetULong> class_to_merge;
	VetULong class_to_eliminate;
	VetULong aux;
	VetULong new_class;
	t_index old_classes=0;
	t_index num_class_to_elim;
	t_index dim;
	t_index den;
	t_index num_words;
	t_index vect_dim;
	t_index index_new_class;
	VetULong num_classes;
	Boolean found_first;
	Boolean found_second;
	t_index first_class;
	t_index second_class;

	//check if num_couples is a valid number
	Check (num_couples>0,"Number of couples to merge isn't valid : "<<num_couples;);   

    mstat<<"Number of couples to merge is "<<num_couples<<endl;

    //check if any class are present in two or more couples
	//and construct the list of the new classes

	//for each couple candidate 
	for (i=0; i<num_couples; i++)
		{
		found_first=FALSE;
		found_second=FALSE;
		dim=class_to_merge.Dim();
		//check if the first element of the couple is present in a class to merge
		for (j=0; j<dim AND !found_first; j++)
			{
			vect_dim=class_to_merge[j].Dim();
			for (k=0; k<vect_dim AND !found_first; k++)
				if (class_to_merge[j][k]==couples[i].First_Elem())
					{
					found_first=TRUE;
					first_class=j;
					}
			}
		//check if the second element of the couple is present in a class to merge
		for (j=0; j<dim AND !found_second; j++)
			{
			vect_dim=class_to_merge[j].Dim();
			for (k=0; k<vect_dim AND !found_second; k++)
				if (class_to_merge[j][k]==couples[i].Second_Elem())
				{
				found_second=TRUE;
				second_class=j;
				}	
			}
		if (found_first AND found_second)
			{
			if (first_class!=second_class)
				{
				class_to_merge[first_class].Append(class_to_merge[second_class]);
				class_to_eliminate.Append(couples[i].Second_Elem());
				class_to_eliminate.Append(class_to_merge[second_class][0]);
				for (k=second_class; k<dim-1; k++)
					class_to_merge[k]=class_to_merge[k+1];
				class_to_merge.Save_And_ReDim(dim-1);
				}
			}
		else if (found_first)
				{
				class_to_merge[first_class].Append(couples[i].Second_Elem());
				class_to_eliminate.Append(couples[i].Second_Elem());
				}
		else if (found_second)
				{
		        class_to_merge[second_class].Append(couples[i].First_Elem());
				class_to_eliminate.Append(couples[i].First_Elem());
				}	
		else //if (!found_first AND !found_second)
				{
			    new_class.Reset();
		        new_class.Append(couples[i].First_Elem());
		        new_class.Append(couples[i].Second_Elem());
	            class_to_merge.Append(new_class);
				class_to_eliminate.Append(couples[i].Second_Elem());
				}
		}

    num_class_to_elim=class_to_eliminate.Dim();
    for (i=0; i<num_class_to_elim-1; i++)
		for (j=i+1; j<num_class_to_elim; j++)
			if (class_to_eliminate[i]==class_to_eliminate[j])
				{
				for (k=j; k<num_class_to_elim-1; k++)
					class_to_eliminate[k]=class_to_eliminate[k+1];
				class_to_eliminate.Save_And_ReDim(num_class_to_elim-1);
				num_class_to_elim=class_to_eliminate.Dim();
				}

	num_classes.Reset();
	dim=class_to_merge.Dim();
	for (i=0; i<dim; i++)
		num_classes.Append(class_to_merge[i].Dim());

    mstat<<"Merge of the words"<<endl;

    //for each new classes
	for (i=0; i<dim; i++)
		{
		index_new_class=class_to_merge[i][0];
		for (j=1; j<num_classes[i]; j++)
			{
			//merge of the words of the classes to merge
			autom_classes[index_new_class].words.Append(autom_classes[class_to_merge[i][j]].words);
			//update unigram class probabilities
			autom_classes[index_new_class].unig_prob+=autom_classes[class_to_merge[i][j]].unig_prob;
			} 

		//compute mean_vector
		den=0;
		for (j=0; j<num_classes[i]; j++)
			{
			num_words=autom_classes[class_to_merge[i][j]].words.Dim();
			den+=num_words;
			autom_classes[index_new_class].mean_vector+=
		    autom_classes[class_to_merge[i][j]].mean_vector*(t_real)(num_words);
			}
		autom_classes[index_new_class].mean_vector/=(t_real)den;
		}

	
    num_class_to_elim=class_to_eliminate.Dim();
    for (i=0; i<num_class_to_elim; i++)
		{
		for (j=class_to_eliminate[i]; j<actual_card-1; j++)
			autom_classes[j]=autom_classes[j+1];
		for (k=i+1; k<num_class_to_elim; k++)
			if (class_to_eliminate[k]>class_to_eliminate[i])
				class_to_eliminate[k]--;
		}
    autom_classes.Save_And_ReDim(actual_card-num_class_to_elim);
	actual_card-=num_class_to_elim;

	return;
	   
	} //end of Lexicon::Merge_Classes function


//automatic clustering of words
void Lexicon::Clustering_Words (t_index max_class_card, t_index sil_index)
	{
    t_index i, j;
	t_index actual_class_card=vocab.Dim();
    t_real error, min_error;
	VetDouble mean;
	ImpObjectList<TIndexCouple> couples_to_merge;
	TIndexCouple classes_to_merge;
    Boolean found=FALSE;

    mean.Destroy_And_ReDim(vocab.Dim());

    while (actual_class_card>max_class_card)
		{
		found=FALSE;
		//search the couple of classes to merge
		for (i=0; i<actual_class_card AND !found; i++)
			//only "silence word" belong to the "silence class" 
			if (autom_classes[i].words.Dim()==1)
				if (autom_classes[i].words[0]==sil_index)
					{
					sil_class=i;
					found=TRUE;
					}
    
		ofstream f_out;
		f_out.open("sil_index.txt", ios::app);
		f_out<<"class cardinality "<<actual_class_card<<endl;
		f_out<<"sil_class "<<sil_class<<endl;
		f_out<<"numero di parole "<<autom_classes[sil_class].words.Dim()<<endl;
		String wor;
		vocab.Get_Key(autom_classes[sil_class].words[0], wor);
		f_out<<"Parola della classe "<<wor<<endl<<endl;
		f_out.close();
	
		mstat<<"Actual number of classes is "<<actual_class_card<<endl;
	   
		//for each couple of classes except the class of silence
		for (i=0; i<actual_class_card; i++)
			{
			if (i!=sil_class)
				for (j=i+1; j<actual_class_card; j++)
					{
					if (j!=sil_class)
						{
						//compute the mean vector of the candidates classes
						Compute_Mean_Vector (i, j, mean);
    					//compute the square error of the candidates classes
						error=Compute_Error (i, j, mean);
						if (i==0 AND j==1)
							{
							min_error=error; 
							classes_to_merge.Create(i,j);
							couples_to_merge.Reset();
							couples_to_merge.Append(classes_to_merge);
							}
						else if (error<min_error)
								{
								min_error=error;
								classes_to_merge.Create(i,j);
								couples_to_merge.Reset();
								couples_to_merge.Append(classes_to_merge);
								}
						else if (error == min_error)
								{
			 					classes_to_merge.Create(i,j);
			                    couples_to_merge.Append(classes_to_merge);
	 							}
						}//end of if
					}//end of for each couple of classes
			}

			//merge the best couple of classes
		Merge_Classes (couples_to_merge, actual_class_card);
		}  //end of while (actual_class_card>max_class_card)

	found=FALSE;
	//search the couple of classes to merge
	for (i=0; i<actual_class_card AND !found; i++)
		//only "silence word" belong to the "silence class" 
		if (autom_classes[i].words.Dim()==1)
			if (autom_classes[i].words[0]==sil_index)
				{
				sil_class=i;
				found=TRUE;
				}
	return;
	}  //end of Lexicon::Clustering_Words function


//at wich class belong each word
void Lexicon::Update_Word_To_Classes ()
	{
    t_index num_classes=autom_classes.Dim();
	t_index num_words;
	t_index i, j;

	for (i=0; i<num_classes; i++)
		{
		num_words=autom_classes[i].words.Dim();
		for (j=0; j<num_words; j++)
			vocab[autom_classes[i].words[j]].belonging_class.Create(i,j);
		}

	return;

	} //end of Lexicon::Update_Word_To_Class

//compute the probability of the words in the class
void Lexicon::Compute_Prob_Words ()
	{
    t_index i, j;
    t_index class_card=autom_classes.Dim();
    t_real acc;
	t_index num_words;
	t_index occurence;
  
	for (i=0; i<class_card; i++)
		{
		acc=0.0;
		num_words=autom_classes[i].words.Dim();
		autom_classes[i].words_prob.Destroy_And_ReDim(num_words);
		for (j=0; j<num_words; j++)
			{
			occurence=vocab[autom_classes[i].words[j]].occur_word;
			autom_classes[i].words_prob[j]=occurence;
			acc+=occurence;
			}
		autom_classes[i].words_prob/=acc;
		autom_classes[i].words_prob.Log();
		}
	
	return;
	}

//calculate the logprobability of unigram
void Lexicon::Update_Unigram_Probability ()
	{
    for (t_index i=0; i<vocab.Dim(); i++)
		vocab[i].unigram_lprob=log((t_real)(vocab[i].occur_word)/(t_real)(corpus_dim));

	return;
	} //end of Update_Unigram_Probability function


//calculate the logprobability (smoothing)
void Lexicon::Calculate_Bigram_Log_Probability (const LexiconOptions & lexicon_opt)
	{
    //general variables
    SmoothingTechn technique=lexicon_opt.smooth_techn;
    t_index i, j;
	t_index dim=vocab.Dim();
    t_real acc;
    t_index count_ij;

    //variables of nonlinear interpolation	
    t_index n1=0, n2=0;
	t_real b;
	t_real D;
	t_real unig_c;
	t_real bigr;
	t_real unigr;
	VetULong n0_vet;

    //variables of linear interpolation	
	t_real lambda;

    //variables of Good-Turing smoothing technique
	t_index	max_r;
	t_index N=0;
	VetULong n_vet;
	VetDouble prob;
	t_index k;

    switch (technique)
		{
		case 0: //nonlinear interpolation model

   				mstat <<"Calculate logprobability of bigram interpolarted nonlinear model"<<endl;
	            n0_vet.Destroy_And_ReDim(dim);
	            //for each word of vocabulary
	            for (i=0; i<dim; i++)
					{
					//calculate n1 and n2 for the i-th word of vocabulary
					for (j=0; j<dim; j++)
						switch (vocab[i].bigram_transitions[j])
							{
							case 0: n0_vet[i]++;
								    break;
			   				case 1: n1++;
									break;
							case 2: n2++;
				                    break;
							default: break;
							};

					}  //end of for each word of vocabulary

                Check (n1!=0 AND n2!=0,"The values of n1 and n2 is out of range n1="<<n1<<" n2="<<n2<<endl);
				
                //calculate b constant
                b=((t_real)(n1)/(t_real)(n1+2*n2));
                for (i=0; i<dim; i++)
					{ 
					acc=0.0;

					//calcxulate D constant   
					D=((t_real)dim*b)/((t_real)n0_vet[i]);

			        //calculate coeff of unigram probability 
					unig_c=D*((t_real)(dim-n0_vet[i])/(t_real)(vocab[i].occur_word));

			  		//allocate the memory for the bigram logprobability
					vocab[i].bigram_lprob.Destroy_And_ReDim(dim); 
	                   
 					//calculate the conditional probability of bigram 
					for (j=0; j<dim; j++)
						{
						bigr=vocab[i].bigram_transitions[vocab.tbl_index[j]]-D;
						unigr=unig_c*(exp(vocab[j].unigram_lprob));
						if (bigr > 0 )
				            vocab[i].bigram_lprob[vocab.tbl_index[j]]=(bigr/((t_real)vocab[i].occur_word))+unigr;
						else vocab[i].bigram_lprob[vocab.tbl_index[j]]=unigr;
						acc+=vocab[i].bigram_lprob[vocab.tbl_index[j]];
						}

					//free memory
					vocab[i].bigram_transitions.Destroy_And_ReDim(0);
					//normalization of conditional bigram probabilities
					vocab[i].bigram_lprob/=acc;
					vocab[i].bigram_lprob.Log();
		            }//end of for (i=0; i<dim; i++)

                break; //end of case 0
		
		case 1: //linear interpolation model
				mstat <<"Calculate logprobability of bigram interpolarted linear model";

				for (i=0; i<dim; i++)
					{
					acc=0.0;
	
					//allocate memory for the bigram logprobabilities
					vocab[i].bigram_lprob.Destroy_And_ReDim(dim);
	
					for (j=0; j<dim; j++)
	                    {
						count_ij=vocab[i].bigram_transitions[vocab.tbl_index[j]];
						lambda=exp(-(t_real)count_ij);
	
						vocab[i].bigram_lprob[vocab.tbl_index[j]]=
								(1-lambda)*(count_ij/((t_real)vocab[i].occur_word))+
								lambda*((t_real)vocab[j].occur_word)/((t_real)corpus_dim);
	
						acc+=vocab[i].bigram_lprob[vocab.tbl_index[j]];
						}
				
					//free memory
					vocab[i].bigram_transitions.Destroy_And_ReDim(0);
	
					//normalization
					vocab[i].bigram_lprob/=acc;
					vocab[i].bigram_lprob.Log();
	  		          
					}                                                
				break; //end of case 1

        case 2: // Good-Turing smoothing technique
				mstat <<"Calculate logprobability of bigram with Good-Turing smoothing technique"<<endl;

				//update n_vet 
				N=0;
				max_r=0;
				n_vet.Destroy_And_ReDim(1);
				for (i=0; i<dim; i++)
					for (j=0; j<dim; j++)
						{
						count_ij=vocab[i].bigram_transitions[vocab.tbl_index[j]];
						N+=count_ij;
						if (count_ij>max_r)
							{
							max_r=count_ij;
							n_vet.Save_And_ReDim(max_r+1);
							}
						n_vet[count_ij]++;
						} 

				//check if Good-Turing techinique can applied
				k=0;
				while (n_vet[k]!=0 AND k<=MAX_R)
					k++;
				if (k!=MAX_R+1)
					merr<<"Cannot apply the Good-Turing smoothing technique, k="<<k<<endl;

				prob.Destroy_And_ReDim(max_r+1);

				//computation of bigram probabilities
				for (i=0; i<MAX_R; i++)
					prob[i]=((t_real)(n_vet[i+1]*(i+1))/(t_real)(n_vet[i]*N));
				for (;i<=max_r;i++)
					prob[i]=(((t_real)(i))/((t_real)N));
             
				//computation of normalized conditional bigram probabilities
				for (i=0; i<dim;i++)
					{
					acc=0.0;
					vocab[i].bigram_lprob.Destroy_And_ReDim(dim);
					for (j=0; j<dim; j++)
	                    {
						vocab[i].bigram_lprob[vocab.tbl_index[j]]=
					    prob[vocab[i].bigram_transitions[vocab.tbl_index[j]]]/exp(vocab[i].unigram_lprob);
						acc+=vocab[i].bigram_lprob[vocab.tbl_index[j]];
						}
	
		            //free memory
					vocab[i].bigram_transitions.Destroy_And_ReDim(0);

					//normalization
					vocab[i].bigram_lprob/=acc;
					vocab[i].bigram_lprob.Log();
	
					}
				     
				break; //end of case 2

 		case 3: //NoSmoothing
				for (i=0; i<dim; i++)
					{
					vocab[i].bigram_lprob.Destroy_And_ReDim(dim);
					for (j=0; j<dim; j++)
					    {
						if (vocab[i].bigram_transitions[j]!=0)
							vocab[i].bigram_lprob[j]=log((t_real)vocab[i].bigram_transitions[j]/vocab[i].occur_word);
						else vocab[i].bigram_lprob[j]=1.0;
						}
					}
				break;  //end of case 3

		}; //end of switch 	            
	 
    return;  
	}//end of Calculate_Bigram_Log_Probability function


//Compute the maximum likelood class bigram probabilities 
void Lexicon::Compute_Class_Bigram_Probabilities ()
	{
    t_index i, j;
	t_index voc_dim=vocab.Dim();

	for (i=0; i<voc_dim; i++)
		{ 
		vocab[i].bigram_lprob.Destroy_And_ReDim(voc_dim);
		for (j=0; j<voc_dim; j++)
			vocab[i].bigram_lprob[j]=
		           vocab[i].bigram_transitions[vocab.tbl_index[j]]/(t_real)vocab[i].occur_word;
		autom_classes[i].mean_vector=vocab[i].bigram_lprob;
		}

	return;

	} //end of Lexicon::Compute_Class_Bigram_Probabilities function


//calculate the logprobability with smoothing tecnique for classes automatic
void Lexicon::Calculate_Class_Bigram_Log_Probability (const LexiconOptions & lexicon_opt)
	{
    //general variables
    SmoothingTechn technique=lexicon_opt.smooth_techn;
    t_index i, j, k, h;
	t_index class_card=autom_classes.Dim();
	t_index dim=vocab.Dim();
    t_real acc;
    t_index count_ij;
	t_index num_words;
	t_index num_wrd_k;
	VetULong vet_trans;
	VetDouble vet_occur;

    //variables of nonlinear interpolation	
    t_index n1=0, n2=0;
	t_real b;
	t_real D;
	t_real unig_c;
	t_real bigr;
	t_real unigr;
	VetULong n0_vet;

    //variables of linear interpolation	
	t_real lambda;

    //variables of Good-Turing smoothing technique
	t_index	max_r;
	t_index N=0;
	VetULong n_vet;
	VetDouble prob;

	
    for (i=0; i<class_card; i++)
		{
		num_words=autom_classes[i].words.Dim();
		vet_trans.Destroy_And_ReDim(dim);
		autom_classes[i].class_transitions.Destroy_And_ReDim(class_card);
		for (j=0; j<num_words; j++)
			{
			//vet_trans+=vocab[autom_classes[i].words[j]].bigram_transitions;
			for (k=0; k<dim; k++)
			    vet_trans[k]+=vocab[autom_classes[i].words[j]].bigram_transitions[vocab.tbl_index[k]];
			vocab[autom_classes[i].words[j]].bigram_transitions.Destroy_And_ReDim(0);
			}
		for (k=0; k<class_card; k++)
			{
			num_wrd_k=autom_classes[k].words.Dim();
			for (h=0; h<num_wrd_k; h++)
				autom_classes[i].class_transitions[k]+=vet_trans[autom_classes[k].words[h]];
			}
		}
	
		vet_occur.Destroy_And_ReDim(class_card);
		for (i=0; i<class_card; i++)
			vet_occur[i]=autom_classes[i].unig_prob*corpus_dim;

		switch (technique)
			{
			case 0: //nonlinear interpolation model
			        mstat <<"Calculate logprobability of bigram interpolarted nonlinear model"<<endl;
					n0_vet.Destroy_And_ReDim(dim);
					//for each word of vocabulary
					for (i=0; i<class_card; i++)
						{                   
  						//calculate n1 and n2 for the i-th word of vocabulary
						for (j=0; j<class_card; j++)
							switch (autom_classes[i].class_transitions[j])
								{
								case 0: n0_vet[i]++;
								        break;
			   					case 1: n1++;
										break;
								case 2: n2++;
										break;
								default : break;
								};
						}  //end of for each word of vocabulary

					Check (n1!=0 AND n2!=0, 
						"The values of n1 and n2 is out of range n1="<<n1<<" n2="<<n2<<endl);
				
					//calculate b constant
					b=((t_real)(n1)/(t_real)(n1+2*n2));


					for (i=0; i<class_card; i++)
						{ 
							autom_classes[i].class_prob.Destroy_And_ReDim(class_card);
							acc=0.0;
							if (n0_vet[i]!=0)
								{
								//calcxulate D constant   
								D=((t_real)class_card*b)/((t_real)n0_vet[i]);
					            //calculate coeff of unigram probability 
								unig_c=D*((t_real)(class_card-n0_vet[i])/vet_occur[i]);

 								//calculate the conditional probability of bigram (wi,wj)
								for (j=0; j<class_card; j++)
									{
									bigr=autom_classes[i].class_transitions[j]-D;
									unigr=unig_c*(autom_classes[j].unig_prob);
	
									if (bigr > 0 )
										autom_classes[i].class_prob[j]=(bigr/vet_occur[i])+unigr;
									else autom_classes[i].class_prob[j]=unigr;
									acc+=autom_classes[i].class_prob[j];
									}

						//normalization of conditional bigram probabilities
						autom_classes[i].class_prob/=acc;
						}
					else{
      					for (j=0; j<class_card; j++)
						    autom_classes[i].class_prob[j]=(t_real)autom_classes[i].class_transitions[j];
						autom_classes[i].class_prob/=vet_occur[i];
						acc=0.0;
						for (j=0; j<class_card; j++)
						    acc+=autom_classes[i].class_prob[j];
						}
					autom_classes[i].class_prob.Log();

					}//end of for (i=0; i<dim; i++)

				break; //end of case 0

			case 1: //linear interpolation model
					mstat <<"Calculate logprobability of bigram interpolarted linear model";

       				for (i=0; i<class_card; i++)
						{
						autom_classes[i].class_prob.Destroy_And_ReDim(class_card);
						acc=0.0;
						for (j=0; j<class_card; j++)
							{
							count_ij=autom_classes[i].class_transitions[j];
							lambda=exp(-(t_real)count_ij);
							autom_classes[i].class_prob[j]=
								   (1-lambda)*(count_ij/((t_real)vet_occur[i]))+
									lambda*(autom_classes[j].unig_prob);
							acc+=autom_classes[i].class_prob[j];
							}
			
						//normalization
						autom_classes[i].class_prob/=acc;
						autom_classes[i].class_prob.Log();
						}                                                
					break; //end of case 1

			case 2: // Good-Turing smoothing technique
					mstat <<"Calculate logprobability of bigram with Good-Turing smoothing technique"<<endl;

					//update n_vet 
					N=0;
					max_r=0;
					n_vet.Destroy_And_ReDim(1);
					for (i=0; i<class_card; i++)
						for (j=0; j<class_card; j++)
							{
							count_ij=autom_classes[i].class_transitions[j];
							N+=count_ij;
							if (count_ij>max_r)
								{
								max_r=count_ij;
								n_vet.Save_And_ReDim(max_r+1);
								}
							n_vet[count_ij]++;
							} 

					//check if Good-Turing techinique can applied
					k=0;
					while (n_vet[k]!=0 AND k<=MAX_R)
						k++;
					if (k!=MAX_R+1)
						merr<<"Cannot apply the Good-Turing smoothing technique, k="<<k<<endl;

					prob.Destroy_And_ReDim(max_r+1);

					//computation of bigram probabilities
					for (i=0; i<MAX_R; i++)
						prob[i]=((t_real)(n_vet[i+1]*(i+1))/(t_real)(n_vet[i]*N));
					for (;i<=max_r;i++)
						prob[i]=(((t_real)(i))/((t_real)N));

					//computation of normalized conditional bigram probabilities
					for (i=0; i<class_card;i++)
						{
						autom_classes[i].class_prob.Destroy_And_ReDim(class_card);
						acc=0.0;
						for (j=0; j<class_card; j++)
							{
							autom_classes[i].class_prob[j]=
							prob[autom_classes[i].class_transitions[j]]/autom_classes[i].unig_prob;
							acc+=autom_classes[i].class_prob[j];
							}
						//normalization
						autom_classes[i].class_prob/=acc;
						acc=0.0;
						for (j=0; j<class_card; j++)
							acc+=autom_classes[i].class_prob[j];
						autom_classes[i].class_prob.Log();
						}
				    break; //end of case 2

			case 3: //NoSmoothing
	           		for (i=0; i<class_card; i++)
						{
						autom_classes[i].class_prob.Destroy_And_ReDim(class_card);
						for (j=0; j<class_card; j++)
			     			if (autom_classes[i].class_transitions[j]!=0)
								autom_classes[i].class_prob[j]=log ((t_real)autom_classes[i].class_transitions[j]/vet_occur[i]);
							else autom_classes[i].class_prob[j]=1.0;
						}

					break; //end of case 3  
			}; //end of switch 	            
    //disallocate memory
	for (i=0; i<class_card; i++)
		autom_classes[i].class_transitions.Destroy_And_ReDim(0);
	return;  
	}  //end of Calculate_Bigram_Log_Probability function


//write the file with the logprobability of unigram
void Lexicon::Write_File_Unigram (const String & f_name)
	{
    ofstream f_unigr;
	t_index i;
	t_index voc_dim=vocab.Dim();

    mstat<<"Writing file unigram"<<endl;
	
	f_unigr.open(f_name);
	if (f_unigr.fail())
		merr<<"Cannot open file : "<<f_name;

    f_unigr<<"Unigram logprobability file"<<endl;
	f_unigr<<"corpus dimension : "<<corpus_dim<<endl;
	f_unigr<<"vocabulary dimension : "<<voc_dim<<endl;
    for (i=0; i<voc_dim; i++)
		{
		String word;
		vocab.Get_Key(i, word);
		f_unigr<<word<<'\t'<<vocab[i].unigram_lprob<<endl;
		}

	f_unigr.close();

	return;
	}


//write the file with the bigram transition
void Lexicon::Write_File_Bigram (const LexiconOptions & lexic_opt)
	{
    ofstream f_bigr;
	t_index i, j;
	t_index voc_dim=vocab.Dim();
	
    mstat<<"Writing file bigram"<<endl;
	
	f_bigr.open(lexic_opt.file_bigram);
	if (f_bigr.fail())
		merr<<"Cannot open file : "<<lexic_opt.file_bigram;

    f_bigr<<"Bigram_transitions_file"<<endl;
	f_bigr<<"vocabulary_dimension: "<<voc_dim<<endl;

    for (i=0; i<voc_dim; i++)
		{
		String word;
		vocab.Get_Key(i, word);
		f_bigr<<word<<'\t';

		for (j=0; j<voc_dim; j++)
			{
			f_bigr<<vocab[i].bigram_lprob[vocab.tbl_index[j]]<<" ";
			}
		f_bigr<<endl;
		}

	f_bigr.close();

	return;
	}

//write the file with bigram transition between classes
void Lexicon::Write_File_Bigram_Classes (const String & f_bigram)const
	{
    ofstream f_bigr;
	t_index i, j;
	t_index num_classes=autom_classes.Dim();
	
	mstat<<"Writing file bigram of classes"<<endl;

	f_bigr.open(f_bigram);
	if (f_bigr.fail())
		merr<<"Cannot open file : "<<f_bigram;

    f_bigr<<"Bigram_classes_transitions_file"<<endl;
	f_bigr<<"number_of_classes: "<<num_classes<<endl;

    for (i=0; i<num_classes; i++)
		{
		f_bigr<<i<<")\t";
		for (j=0; j<num_classes; j++)
			f_bigr<<autom_classes[i].class_prob[j]<<" ";
		f_bigr<<endl;
		}

	f_bigr.close();

	return;
	}   //end of Lexicon::Write_File_Bigram_Classes function  


//write the file with information on perplexity computation
void Lexicon::Write_File_Perplex (t_real perplex, const LexiconOptions & lexic_opt)
	{
    ofstream f_perplex;

	f_perplex.open (lexic_opt.file_perplex);
	if (f_perplex.fail())
		merr<<"Cannot open the output file : "<<lexic_opt.file_perplex;

    f_perplex << "File with the result of perplexity computation"<<endl;
    f_perplex << "Vocabulary dimensions :"<<vocab.Dim()<<endl;

    if (lexic_opt.class_type==NoClass)
		{
		switch (lexic_opt.smooth_techn)
		    {
			case 0: f_perplex << "Nonlinear interpolated model applied"<<endl;
					break;
			case 1: f_perplex << "Linear interpolated model applied"<<endl;
					break;
		    case 2: f_perplex << "Good-Turing smoothing technique applied"<<endl;
					break;
			};
		}
    else if (lexic_opt.class_type==AutomClass)
			{
			f_perplex << "Automatic clustering applied"<<endl;
			f_perplex << "Number of classes : "<<autom_classes.Dim()<<endl;
			}
    f_perplex << "Perplexity computed on "<<file_list.Dim()<< " files"<<endl;
	f_perplex << "Value of the perplexity : "<<perplex<<endl;

	f_perplex.close();

	return;

	} //end of Write_File_Perplex function


//write the file with classes member
void Lexicon::Write_File_Classes (const String & file_classes)
	{
    ofstream f_class;
    t_index i, j;
	t_index num_words;
	t_index num_classes=autom_classes.Dim();

    f_class.open(file_classes);
	if (f_class.fail())
	  merr<<"Cannot open file of classes :"<<file_classes;

	f_class<<"Classification_Cardinality: "<<num_classes<<endl<<endl;

    for (i=0; i<num_classes; i++)
		{
		f_class<<"Class_Number: "<<i<<endl;

		num_words=autom_classes[i].words.Dim();
		f_class<<"Class_Cardinality: "<<num_words<<endl;
		for (j=0; j<num_words; j++)
			{
			String word;
			vocab.Get_Key(autom_classes[i].words[j], word);
			f_class<<word;
			f_class<<'\t'<<autom_classes[i].words_prob[j]<<endl;
			}
		f_class<<endl;
		}
	f_class.close();
	return;
	}  //end of Lexicon::Write_File_Classes function


//write file of phonemes bigram
void Lexicon::Write_File_Phon_Bigram (const String & file_bigr)const
	{
    ofstream f_bigr;

    mstat<<"Write file of phoneme bigram";

	f_bigr.open (file_bigr);

	f_bigr<<"Phonemes_occurrences_and_transitions"<<endl;

	for (t_index i=0; i<card_label; i++)
		{
		f_bigr<<"phoneme_"<<i<<'\t'<<occur_phon[i]<<'\t';
		for (t_index j=0; j<card_label; j++)
			f_bigr<<bigram_trans[i][j]<<'\t';
		f_bigr<<endl;
		}
	f_bigr.close();

	return;
	}


//write the file with information on perplexity computation
void Lexicon::Write_File_Phon_Perplex (t_real perplex, const LexiconOptions & lexic_opt)const
	{
    ofstream f_perplex;

	f_perplex.open (lexic_opt.file_perplex);
	if (f_perplex.fail())
	 merr<<"Cannot open the output file : "<<lexic_opt.file_perplex;

    f_perplex << "File with the result of perplexity computation"<<endl;
    f_perplex << "Label type : ";

    switch (lexic_opt.label_type)
		{
	    case 0: f_perplex << "NTimitReduced"<<endl;
	            break;
		case 1: f_perplex << "NTimit"<<endl;
	            break;
	    case 2: f_perplex << "AtisLabel"<<endl;
	            break;
	    };

    f_perplex << "Perplexity computed on "<<file_list.Dim()<< " files"<<endl;
	f_perplex << "Value of the perplexity : "<<perplex<<endl;

	f_perplex.close();

	return;
	} //end of Write_File_Perplex function

//Build language model
void Lexicon::Build_Language_Model(const LexiconOptions& lexic_options)
	{
	String name_file_list;
	t_index num_file;
	t_index sil_index;

    //build list of files to train grammar
	name_file_list=lexic_options.file_train_list;
    Load_File_List(name_file_list);
	num_file=file_list.Dim(); 

    vocab.list.Set_Data_Chunk (10);

	switch(lexic_options.class_type)
		{
		case 0: //lexic_options.class_type==HandClass
				merr<<"Not implemented hand_class";
		        break;

		case 1: //lexic_options.class_type==AutomClass
				Count_Bigram_Transitions(file_list,lexic_options);

				//calculate the logprobability of unigram
                Update_Unigram_Probability ();

				//initialize the word classes 
				Initialize_Word_Classes (sil_index);

                //automatic clustering of words
				Clustering_Words (lexic_options.max_num_classes, sil_index);

				//calculate the logprobability with smoothing techniques of bigram classes
                Calculate_Class_Bigram_Log_Probability (lexic_options);

				//compute the probability of the words in the class
				Compute_Prob_Words ();

                //write the file with vocabulary and phonetic transcription of each word
	            Write_File_Of_Vocabulary (lexic_options.file_vocabulary);

                //write the file with bigram transition between classes
                Write_File_Bigram_Classes (lexic_options.file_bigram);
               
			    //write the file with classes member
				Write_File_Classes (lexic_options.file_autom_classes);
	  	        break;

		case 2: //lexic_options.class_type==NoClass
                switch (lexic_options.grammar)
					{
					case 0:	//lexic_options.grammar==Unigram
						    Build_Unigram_Language_Model(file_list,lexic_options);
					
			   				//write the file with unigram logprobability
							Write_File_Unigram (lexic_options.file_unigram);

							//write the file with vocabulary and phonetic transcription of each word
							Write_File_Of_Vocabulary (lexic_options.file_vocabulary);

							break;

					case 1: //lexic_options.grammar==Bigram        
							Count_Bigram_Transitions(file_list,lexic_options);

							//calculate the logprobability of unigram
							Update_Unigram_Probability ();

							//calculate the logprobability 
							Calculate_Bigram_Log_Probability (lexic_options); 

							//write the file with vocabulary and phonetic transcription of each word
							Write_File_Of_Vocabulary (lexic_options.file_vocabulary);

							//write the file with bigram transition
							Write_File_Bigram(lexic_options);

							break;

					};
		};   
 
	return;
	}//end of Build_Bigram_Language_Model function


//Compute perplexity
void Lexicon::Compute_Perplexity (const LexiconOptions & lexic_options)
	{
    String file_list_name;
	t_index num_files;
	t_index i;
	t_real perplex=0.0;

    mstat<<"Computation perplexity"<<endl;
	file_list.Reset();

	file_list_name=lexic_options.file_perplex_list;
	Load_File_List (file_list_name);
	num_files=file_list.Dim();

    mstat<<"Number of test files is "<<num_files<<endl;
		  
	switch(lexic_options.class_type)
		{
		case 0: //lexic_options.class_type==HandClass
				merr<<"Not implemented hand_class";
		        break;

		case 1: Update_Word_To_Classes ();
				for (i=0; i<num_files; i++)
				   perplex+=Compute_Partial_Perplexity_Bigram_Classes(file_list[i], lexic_options);
			    perplex/=(t_real)num_files;
	            Write_File_Perplex (perplex, lexic_options);
				break;

		case 2: //lexic_options.class_type==NoClass
				switch (lexic_options.grammar)
					{
					case 0: //lexic_options.grammar==Unigram
							merr<<"Not implemented unigram";
							break;
					case 1:	//lexic_options.grammar==Bigram 
							if (!lexic_options.trainingLM)
								merr<<"Cannot calculate the perplexity without training the language model";
							for (i=0; i<num_files; i++)
								perplex+=(*this).Compute_Partial_Perplexity_Bigram(file_list[i], 
							                                                    lexic_options);
							perplex/=(t_real)num_files;
							Write_File_Perplex (perplex, lexic_options);
							break;
						  
					};
		};   

	return;
	}


//Training of phonemes grammar
void Lexicon::Train_Phon_Grammar (const LexiconOptions & lexic_options)
	{
	String name_file_list;
	t_index num_file;

    //build list of files to train grammar
	name_file_list=lexic_options.file_train_list;
    Load_File_List(name_file_list);
    num_file=file_list.Dim(); 

    switch (lexic_options.label_type)
		{
		case 0: //lexic_options.label_type==NTimitReduced
				card_label=46;
				break;
	    case 1: //lexic_options.label==NTimit
				card_label=63;
				break;
		case 2: //lexic_options.label==AtisLabel
				card_label=46;
				break;
		case 3: //lexic_options.label==NTimit39Label
				card_label=39;
				break;
	    case 4: //lexic_options.label==AtisReducedLabel
				card_label=36;
				break;
		};

    Update_Phoneme_List (card_label);
    bigram_trans.Destroy_And_ReDim(card_label, card_label);
    occur_phon.Destroy_And_ReDim(card_label);

    switch (lexic_options.grammar)
		{
		case 0: //lexic_options.grammar==Unigram
				merr<<"Not implemented unigram for phonemes";
				break;
		case 1: //lexic_options.grammar==Bigram        
				Count_Phonemes_Transitions (file_list, lexic_options.label_type);
	          	Write_File_Phon_Bigram (lexic_options.file_bigram);
				break;
		};   
 
    return;

	}//end of Build_Bigram_Language_Model function


void Lexicon::Compute_Phon_Perplexity (const LexiconOptions & lexic_options)
	{
    String file_list_name;
	t_index num_files;
	t_index i;
	t_real perplex=0.0;

    mstat<<"Computation perplexity"<<endl;
	
    file_list.Reset();

	file_list_name=lexic_options.file_perplex_list;
	Load_File_List (file_list_name);
	num_files=file_list.Dim();
	  
	switch (lexic_options.grammar)
		{
		case 0: //lexic_options.grammar==Unigram
				merr<<"Not implemented unigram";
				break;
		case 1: //lexic_options.grammar==Bigram 
				for (i=0; i<num_files; i++)
					perplex+=(*this).Compute_Partial_Perplexity_Phon_Bigram(file_list[i], 
				                                   lexic_options.label_type);

				perplex/=(t_real)num_files;
	
				Write_File_Phon_Perplex (perplex, lexic_options);
				break;
		};   

	return;
	}

//************************************************************************************//
//                                                                                    //
//                         class LexiconAtis                                          //
//                                                                                    //
//************************************************************************************//


//load phonetic transcription of a single atis word from buffer

void LexiconAtis::Get_Phon_Single_Word (const char * buffer, t_index buf_index, 
                                                                t_index wrd_index) 
	{
    String phoneme;
    t_index dim;

    while (buffer[buf_index]!='\0')
		{
		while (buffer[buf_index]<'a' OR buffer[buf_index]>'z')
			buf_index++;

		//load phoneme in phoneme String
		while (buffer[buf_index]!=' ' AND buffer[buf_index]!='\0')
			{
			phoneme<<buffer[buf_index];
			buf_index++;
			}

		//copy phoneme String in the vocabulary
		dim=vocab[wrd_index].phns_transc[0].phn.Dim();
		vocab[wrd_index].phns_transc[0].phn.Save_And_ReDim(dim+1);
		vocab[wrd_index].phns_transc[0].phn[dim]=phoneme;
		phoneme.Reset();
		}//end of while(buffer[buf_index]!='\0')
	}  //end of Get_Phon_Single_Word function


//load phonetic transcription of atis word from the atis_phon_file
void LexiconAtis::Load_Phonetic_Transcriptions (const String & f_phon)
	{
    ifstream f_ph_tr;    
	char buffer[__MAX_LEN_LINE];
	t_index i, dim;
    String wrd;
	Boolean found;

    mstat <<"load phonetic transcription ";
	
    //open file of phonetic transcriptions
	f_ph_tr.open(f_phon, ios::in|ios::nocreate);
	if (f_ph_tr.fail())
		merr<<"Cannot open file : "<<f_phon;

    //cycle for jump commentary lines
	do 
		f_ph_tr.getline(buffer, __MAX_LEN_LINE);
    while ((buffer[0]==';') OR (buffer[0]=='*') OR (buffer[0]=='\''));
	  
    //for each word of lessico load the phonetic transcription
	dim=vocab.Dim();
    for (i=0; i<dim; i++)
		{
		String searched_word;
		vocab.Get_Key(i,searched_word);
		//allocate memory and set data chunk for actual word
		vocab[i].phns_transc.Destroy_And_ReDim(1);
		vocab[i].phns_transc[0].phn.Set_Data_Chunk(MAX_NUM_PHON);

		//number of phonetic transcriptions of actual word
		vocab[i].occur_pron=1;
	   
		//number of occurence of this pronounce is the same of 
		//number of occurence of the actual word
		vocab[i].phns_transc[0].occur=vocab[i].occur_word;

	    //search the actual word phonetic transcription in the file
		do	{
			found = FALSE;
			while (buffer[wrd.Dim()]!=' ')
				wrd<<(char)buffer[wrd.Dim()];
            if (wrd==searched_word)
				{
				//get phonetic transcriptionof a single word
				Get_Phon_Single_Word (buffer, wrd.Dim(), i); 
				found=TRUE;
				}//end of if(wrd==vocab[i].word)
			f_ph_tr.getline(buffer, __MAX_LEN_LINE);
			wrd.Reset();
			}//end of do 
		while (!found AND !f_ph_tr.eof());
	     
		Check(found, "Unknown phonetic transcription of word "<<searched_word;);
	   
		}//end of for cycle
	f_ph_tr.close();
    return;
	} //end of Load_Phonetic_Transcription function


//write the file with vocabulary and phonetic transcription of each word
void LexiconAtis::Write_File_Of_Vocabulary (const String & f_name)
	{
	ofstream f_vocab;
    t_index dim=vocab.Dim();
	t_index i, j;

	f_vocab.open(f_name);
	if (!f_vocab.is_open())
		merr<<"Cannot open file : "<<f_name;

    mstat<<"Writing file of vocabulary"<<endl;
	
    f_vocab<<"Corpus_dimension: "<<corpus_dim<<endl;
	f_vocab<<"Vocabulary_cardinality: "<<dim<<endl<<endl;

    //each word has a only phonetic transcription
    f_vocab<<"Phonetic_transcriptions_of_words:"<<endl;
    f_vocab<<"word  #pronounces  #phonemes prob_pronounce phonemes"<<endl;
    for (i=0; i<dim; i++)
		{
		//write the word ...
		String word;
		vocab.Get_Key(i,word);
		f_vocab<<word<<'\t';

	    //for atis database there is only a pronounce of each word 
		f_vocab<<1<<'\t';
		//... number of phonemes of the pronounce ...
		t_index num_phons=vocab[i].phns_transc[0].phn.Dim();
		f_vocab<<num_phons<<'\t';
		//probability of pronounce
		f_vocab<<0<<'\t';
		//... phonemes of phonetic trascription 
		for (j=0; j<num_phons; j++)
			f_vocab<<'/'<<vocab[i].phns_transc[0].phn[j];  
		f_vocab<<'/'<<endl;  
		}
	f_vocab<<endl;
	f_vocab.close();
    
	return;
	}  //end of LexiconAtis0::Write_File_Of_Vocabulary function


//gives back the unigram language model
void LexiconAtis::Build_Unigram_Language_Model(const ImpObjectList<String> & f_list, 
                                const LexiconOptions& lexic_options) 
	{
    t_index num_files=f_list.Dim();
	t_index i, file_dim=0;
    t_index sil_index;

    corpus_dim=0;

    //update vocabulary on all files of training files list
    for (i=0; i<num_files; i++)
		{
		Build_Partial_Unigram_Language_Model(f_list[i], sil_index, file_dim);
		corpus_dim+=file_dim;
		}
 
    //if database is ATIS load all phonetic transcriptions from a single file
	//in the which is the list of all words and, for each of these, 
	//the phonetic transcription
    Load_Phonetic_Transcriptions (lexic_options.atis_phon_file);
  
    //calculate the logprobability of unigram
    Update_Unigram_Probability ();
    
    return;
	} //end of Build_Unigram_Language_Model function


//gives back the bigram langauge model
void LexiconAtis::Count_Bigram_Transitions(const ImpObjectList<String> & f_list, 
                                const LexiconOptions& lexic_options)
	{
    t_index num_files = f_list.Dim();
	t_index i, file_dim=0;
	corpus_dim=0;
    
	mstat<<"Processing "<<num_files<<" files"<<endl;  
    //up_date vocabulary on all files of training files list
	for (i=0; i<num_files; i++)
		{
  		Count_Partial_Bigram_Transitions(f_list[i], file_dim);
		corpus_dim+=file_dim;
		}
    //if database is ATIS load all phonetic transcriptions from a single file
	//in the which is the list of all words and, for each of these, 
	//the phonetic transcription
    Load_Phonetic_Transcriptions(lexic_options.atis_phon_file);

    return;
	} //end of Build_Bigram_Language_Model function

void LexiconAtis::Write_Solution_File (const String & f_sol_name, const String & list_f_name)
	{
	String complete_file_name_sro;
	String actual_word;
	t_index num_file;
    ifstream f_sro;
	ofstream f_sol;
     
    //build list of files to train grammar
	Load_File_List(list_f_name);
    num_file=file_list.Dim(); 

    f_sol.open(f_sol_name);
	if (f_sol.fail())
		merr<<"Cannot open file of solutions: "<<f_sol_name;

    for (t_index i=0; i<num_file; i++)
		{
		complete_file_name_sro.Reset();
		complete_file_name_sro<<file_list[i]<<".sro";

		f_sro.open(complete_file_name_sro, ios::in|ios::nocreate);    
		if (f_sro.fail()) 
	        merr<<"Cannot open input file : "<<complete_file_name_sro;

		f_sol<<"silence ";

		while (!f_sro.eof())
			{
			f_sro >> actual_word;
            if (!f_sro.eof())
				{ 
				actual_word.Make_Lower();
				actual_word.Cut_Char('!');
				actual_word.Cut_Char('<');
				actual_word.Cut_Char('>');
				actual_word.Cut_Char(':');
				actual_word.Cut_Char('~');
				actual_word.Cut_Char(',');
				actual_word.Cut_Char('.');

				if (actual_word!="")
					if (actual_word==".")
						f_sol<<"silence ";
					else f_sol<<actual_word<<" ";
				}
		   
			}//end of while(!f_wrd.eof())

		f_sol<<"silence "<<endl;
		f_sro.close();
		}  //end of for i
    f_sol.close();
    return;
	} //end of traduci function


//************************************************************************************//
//                                                                                    //
//                         class LexiconAtis2                                         //
//                                                                                    //
//************************************************************************************//

//gives back the vocabulary up-to-date just on single file
//build vocabulary with dynamic algorithm 
void LexiconAtis2::Build_Partial_Unigram_Language_Model(const String & file_name,
                      t_index & sil_index, t_index & file_dim)
	{
    String complete_file_name_wrd;
    String actual_word;
	t_index position;
	Boolean found;
	WordInfo new_elem;
    ifstream f_wrd;
    ifstream f_phn;

    complete_file_name_wrd << file_name<<".lsn";

    f_wrd.open(complete_file_name_wrd, ios::in|ios::nocreate);    
    if(f_wrd.fail()) 
		merr<<"Cannot open input file : "<<complete_file_name_wrd;

    //suppose that each sentence start with silence
    actual_word="silence";

    new_elem.occur_word=1;
	new_elem.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
	new_elem.bigram_transitions.Set(0);
	new_elem.bigram_transitions.Set_Data_Chunk(10);
	    
    found=vocab.Insertion(actual_word, new_elem, position);

	if (found)
		vocab[position].occur_word++;
	
    file_dim=1;  
    while (!f_wrd.eof())
		{
     	f_wrd >> actual_word;
        if (!f_wrd.eof())
			{ 
			actual_word.Make_Lower();
   			file_dim++;
			new_elem.occur_word=1;
			new_elem.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
			new_elem.bigram_transitions.Set(0);
			new_elem.bigram_transitions.Set_Data_Chunk(10);
			found=vocab.Insertion(actual_word, new_elem, position);
 			if (found)
				vocab[position].occur_word++;
			}
		}//end of while(!f_wrd.eof())

    //suppose that each sentence end is silence 
    actual_word="silence";
    file_dim++;

    new_elem.occur_word=1;
	new_elem.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
	new_elem.bigram_transitions.Set(0);
	new_elem.bigram_transitions.Set_Data_Chunk(10);
	    
    found=vocab.Insertion(actual_word, new_elem, position);
	if (found)
		vocab[position].occur_word++;
    f_wrd.close();
    return;
	}//end of Build_Partial_Unigram_Language_Model function

//gives back the vocabulary up-to-date just on single file
//build vocabulary with dynamic algorithm and update statistical 
//informations about transitions between words	
void LexiconAtis2::Count_Partial_Bigram_Transitions(const String & file_name, t_index & file_dim)
	{
    t_index dim;
    t_index i;
    t_index prec_word_index;
    t_index next_word_index;
	WordInfo new_elem;
    String complete_file_name_wrd;
    String actual_word;
    String next_word;
	String second_word;
	Boolean present;
    ifstream f_wrd;
    ifstream f_phn;

    complete_file_name_wrd << file_name<<".lsn";

    f_wrd.open(complete_file_name_wrd, ios::in|ios::nocreate);    
    if(f_wrd.fail()) 
      merr<<"Cannot open input file : "<<complete_file_name_wrd;

    //suppose that each sentence start with silence
    actual_word="silence";
   
    new_elem.occur_word=1;
    new_elem.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
	new_elem.bigram_transitions.Set(0);
	new_elem.bigram_transitions.Set_Data_Chunk(10);

    //insert the word and give back the position index
    present=vocab.Insertion(actual_word,new_elem,prec_word_index);
    
	if (present)
		vocab[prec_word_index].occur_word++;

    file_dim=1;  
    while (!f_wrd.eof())
		{
		f_wrd >> next_word;
       	next_word.Make_Lower();

		if (!f_wrd.eof())   
			{
			file_dim++;
			new_elem.occur_word=1;
			new_elem.bigram_transitions.Destroy_And_ReDim(vocab.Dim());
			new_elem.bigram_transitions.Set(0);
			new_elem.bigram_transitions.Set_Data_Chunk(10);

			present=vocab.Insertion(next_word,new_elem,next_word_index);
         
			if (present)
				vocab[next_word_index].occur_word++;
			else{
				dim=vocab.Dim();
				for (i=0; i<dim; i++)
					vocab[i].bigram_transitions.Append(0);
				}	  
			//update the count of the bigram transition
			if (!present AND next_word_index<=prec_word_index)
				vocab[prec_word_index+1].bigram_transitions[vocab.tbl_index[next_word_index]]++;
			else vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]++;
          
			prec_word_index=next_word_index;
			}// end of if (!f_wrd.eof())
		}//end of while(!f_wrd.eof())
    //suppose that each sentence end is silence 
    next_word="silence";
    file_dim++;
    new_elem.occur_word=1;
    new_elem.bigram_transitions.Destroy_And_ReDim(vocab.Dim());
	new_elem.bigram_transitions.Set(0);
	new_elem.bigram_transitions.Set_Data_Chunk(10);

    present=vocab.Insertion(next_word,new_elem,next_word_index);
    
    if (present)
		vocab[next_word_index].occur_word++;
	else{
		dim=vocab.Dim();
		for (i=0; i<dim; i++)
			vocab[i].bigram_transitions.Append(0);
		}
    vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]++;
	f_wrd.close();
	return;
	}//end of Build_Partial_Bigram_Language_Model function



//Compute logperplexity on a only file for bigram language model
t_real LexiconAtis2::Compute_Partial_Perplexity_Bigram (const String & f_name, const LexiconOptions & lexicon_options)
	{
    String complete_file_name;
	String prec_word;
	String actual_word;
	String temp;
	String second_word;
	ifstream f_wrd;
	t_real res;
	t_index prec_word_index;
	t_index actual_word_index;
	t_index sil_index;
	t_index file_dim;
	Boolean found;
	
	complete_file_name<<f_name<<".lsn";

  	f_wrd.open(complete_file_name, ios::in|ios::nocreate);
	if (f_wrd.fail())
		merr<<"Cannot open input file : "<<complete_file_name;

	//suppose that each sentence start with a silence word
    prec_word="silence";
	prec_word_index=vocab.Binary_Search(prec_word, found);
    
	sil_index=prec_word_index;
	res = vocab[prec_word_index].unigram_lprob;
    file_dim=1;
	while (!f_wrd.eof())
		{
		f_wrd >> actual_word;
		actual_word.Make_Lower();
        if (!f_wrd.eof())   
			{
			file_dim++;
			actual_word_index=vocab.Binary_Search(actual_word, found);

			if (!found)
				merr<<"Not found in the vocabulary the word "<<actual_word
		           <<" of file "<<complete_file_name;
		   
			res+=vocab[prec_word_index].bigram_lprob[vocab.tbl_index[actual_word_index]];
			prec_word_index=actual_word_index;
			} //end of if(!f_wrd.eof())
		}  //end of while (!f_wrd.eof())
     
	//suppose that each sentence finish with a silence 
	res+=vocab[prec_word_index].bigram_lprob[vocab.tbl_index[sil_index]];
	file_dim++;
	return exp(-(res/(t_real)file_dim));
	} //end of Compute_Partial_Perplexity_Bigram function


//compute the perplexity of bigram classes language model on a single file
t_real LexiconAtis2::Compute_Partial_Perplexity_Bigram_Classes (const String & f_name,
	                                 const LexiconOptions & lexicon_options) 
	{
    String complete_file_name;
	String prec_word;
	String actual_word;
	ifstream f_wrd;
	t_real res;
	t_index actual_word_index;
	t_index prec_class_index;
	t_index actual_class_index;
	t_index index_word_class;
    t_index file_dim;
	Boolean found;
    
    complete_file_name=f_name;
	complete_file_name<<".lsn";

	f_wrd.open(complete_file_name, ios::in|ios::nocreate);
	if (f_wrd.fail())
		merr<<"Cannot open input file : "<<complete_file_name;

    res = log(autom_classes[sil_class].unig_prob);
    prec_class_index=sil_class;
    file_dim=1;
	while (!f_wrd.eof())
		{
		f_wrd >> actual_word;
		actual_word.Cut_Char('?');
		actual_word.Cut_Char('.');
		actual_word.Cut_Char(',');
        actual_word.Make_Lower();
                     
		if (!f_wrd.eof())   
			{
			file_dim++;
			actual_word_index=vocab.Binary_Search(actual_word, found);
			if (!found)
				merr<<"Not found in the vocabulary the word : "<<actual_word
			                                 <<" of file "<<complete_file_name<<endl;
			actual_class_index=vocab[actual_word_index].belonging_class.First_Elem();
			index_word_class=vocab[actual_word_index].belonging_class.Second_Elem();
			res+=autom_classes[prec_class_index].class_prob[actual_class_index]+
		         (autom_classes[actual_class_index].words_prob[index_word_class]);	   
			prec_class_index=actual_class_index;
		    } //end of if(!f_wrd.eof())
		}  //end of while (!f_wrd.eof())
     
	//suppose that each sentence finish with a silence 
	res+=autom_classes[prec_class_index].class_prob[sil_class];
	file_dim++;

	return exp(-(res/(t_real)file_dim));
   
	} //end of Compute_Partial_Perplexity_Bigram_Classes function
    

//***********************************************************************************//
//                                                                                   //
//                         class LexiconAtis3                                        //
//                                                                                   //
//***********************************************************************************//

//gives back the vocabulary up-to-date just on single file
//build vocabulary with dynamic algorithm and update statistical 
//informations about transitions between words	

void LexiconAtis3::Count_Partial_Bigram_Transitions(const String & file_name, t_index & file_dim)
	{
    t_index prec_word_index;
    t_index next_word_index;
	t_index dim;
	t_index i;
    String complete_file_name_wrd;
    String actual_word;
    String next_word;
	String temp;
	WordInfo new_word;
    Boolean present;
	Boolean fin_sil=TRUE;
    ifstream f_wrd;
    ifstream f_phn;
    t_index pos;

    if (file_name.Is_SubString_Inside("Atis2", pos))
		complete_file_name_wrd << file_name<<".lsn";
	else complete_file_name_wrd<<file_name<<".sro";

    f_wrd.open(complete_file_name_wrd, ios::in|ios::nocreate);    
    if(f_wrd.fail()) 
		merr<<"Cannot open input file : "<<complete_file_name_wrd;

    f_wrd >> next_word;
	next_word.Make_Lower();
	if (next_word!="~")
		{
		//suppose that each sentence start with silence
		actual_word="silence";
		new_word.occur_word=1;
		new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
		new_word.bigram_transitions.Set(0);
		new_word.bigram_transitions.Set_Data_Chunk(10);
		present=vocab.Insertion(actual_word,new_word, prec_word_index);

		if (present)
			vocab[prec_word_index].occur_word++;
		else{
			dim=vocab.Dim();
			for (i=0; i<dim; i++)
				vocab[i].bigram_transitions.Append(0);
			}

		file_dim=2;  
		new_word.occur_word=1;
		new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
		new_word.bigram_transitions.Set(0);
		new_word.bigram_transitions.Set_Data_Chunk(10);

		present=vocab.Insertion(next_word,new_word, next_word_index);

		if (present)
			vocab[next_word_index].occur_word++;
		else{
			dim=vocab.Dim();
			for (i=0; i<dim; i++)
				vocab[i].bigram_transitions.Append(0);
			}

		//update the count of the bigram transition
		if (!present AND next_word_index<=prec_word_index)
			vocab[prec_word_index+1].bigram_transitions[vocab.tbl_index[next_word_index]]++;
		else vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]++;

		prec_word_index=next_word_index;
		}
    else{
		f_wrd>>actual_word;

		//insert the word and give back the position index
		new_word.occur_word=1;
		new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
		new_word.bigram_transitions.Set(0);
		new_word.bigram_transitions.Set_Data_Chunk(10);
		present=vocab.Insertion(actual_word,new_word, prec_word_index);

		if (present)
			vocab[prec_word_index].occur_word++;
		else{
			dim=vocab.Dim();
			for (i=0; i<dim; i++)
				vocab[i].bigram_transitions.Append(0);
			}

		file_dim=1;  
		}
    while (!f_wrd.eof())
		{
		f_wrd >> next_word;

		next_word.Make_Lower();
	       
		if (next_word=="~")
			fin_sil=FALSE;
		else if(!f_wrd.eof())
				{
				file_dim++;
				new_word.occur_word=1;
    			new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
				new_word.bigram_transitions.Set(0);
				new_word.bigram_transitions.Set_Data_Chunk(10);

				present=vocab.Insertion(next_word,new_word, next_word_index);
	
				if (present)
					vocab[next_word_index].occur_word++;
				else{
					dim=vocab.Dim();
					for (i=0; i<dim; i++)
						vocab[i].bigram_transitions.Append(0);
					}	

				//update the count of the bigram transition
				if (!present AND next_word_index<=prec_word_index)
					vocab[prec_word_index+1].bigram_transitions[vocab.tbl_index[next_word_index]]++;
				else vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]++;
          
				prec_word_index=next_word_index;
				}
      
		}//end of while(!f_wrd.eof())

	if (fin_sil)
		{
		//suppose that each sentence end is silence 
		next_word="silence";
      
		new_word.occur_word=1;
		new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
		new_word.bigram_transitions.Set(0);
		new_word.bigram_transitions.Set_Data_Chunk(10);

		present=vocab.Insertion(actual_word,new_word, next_word_index);

		if (present)
			vocab[next_word_index].occur_word++;
		else{
			dim=vocab.Dim();
			for (i=0; i<dim; i++)
				vocab[i].bigram_transitions.Append(0);
			}

		//update the count of the bigram transition
		if (!present AND next_word_index<=prec_word_index)
			vocab[prec_word_index+1].bigram_transitions[vocab.tbl_index[next_word_index]]++;
		else vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]++;
      
		file_dim++;
		}
	f_wrd.close();
	return;
	}//end of Count_Partial_Bigram_Transitions function



//gives back the vocabulary up-to-date just on single file
//build vocabulary with dynamic algorithm 
void LexiconAtis3::Build_Partial_Unigram_Language_Model(const String & file_name,
                      t_index & sil_index, t_index & file_dim)
	{
    t_index index;
    String complete_file_name_wrd;
    String actual_word;
	String sil;
	WordInfo new_word;
    Boolean present;
	Boolean fin_sil=TRUE;
    ifstream f_wrd;
    ifstream f_phn;

    sil="silence";
    complete_file_name_wrd << file_name<<".sro";

    f_wrd.open(complete_file_name_wrd, ios::in|ios::nocreate);    
    if(f_wrd.fail()) 
		merr<<"Cannot open input file : "<<complete_file_name_wrd;

    f_wrd>>actual_word;

	if (actual_word!="~")
		{
		new_word.occur_word=1;
		new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
		new_word.bigram_transitions.Set(0);
		new_word.bigram_transitions.Set_Data_Chunk(10);

		//insert the word in the vocabulary
		present=vocab.Insertion(sil, new_word, sil_index);
  	   
		if (present)
			vocab[sil_index].occur_word++;
		  
		new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
		new_word.bigram_transitions.Set(0);
		  	  
		present=vocab.Insertion (actual_word, new_word, index);

		if (present)
			vocab[index].occur_word++;
	
		file_dim=2;
		}  

	while (!f_wrd.eof())
		{
     	f_wrd >> actual_word;
       	if (actual_word=="~")
			fin_sil=FALSE;
		else if (!f_wrd.eof())
				{ 
				actual_word.Make_Lower();
   				file_dim++;
				new_word.occur_word=1;
				new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
				new_word.bigram_transitions.Set(0);
				new_word.bigram_transitions.Set_Data_Chunk(10);

				present=vocab.Insertion(actual_word,new_word, index);
				if (present)
					vocab[index].occur_word++;
				}
		   
		}//end of while(!f_wrd.eof())

	if (fin_sil)
		{
		//suppose that each sentence end is silence if not ends with ~
		file_dim++;

		new_word.occur_word=1;
		new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
		new_word.bigram_transitions.Set(0);
		new_word.bigram_transitions.Set_Data_Chunk(10);

		present=vocab.Insertion(sil, new_word, sil_index);
		if (present)
			vocab[sil_index].occur_word++;
		}

    f_wrd.close();

    return;

	}//end of Build_Partial_Unigram_Language_Model function


//Compute logperplexity on a only file for bigram language model
t_real LexiconAtis3::Compute_Partial_Perplexity_Bigram (const String & f_name, 
                                                   const LexiconOptions & lexicon_options)
	{
    String complete_file_name;
	String prec_word;
	String actual_word;
	ifstream f_wrd;
	t_real res;
	t_index prec_word_index;
	t_index actual_word_index;
	t_index file_dim;
	t_index sil_index;
	Boolean found;
	Boolean fin_sil=TRUE;
    t_index pos;

    if (f_name.Is_SubString_Inside("Atis3", pos))
		complete_file_name << f_name<<".sro";
	else complete_file_name<<f_name<<".lsn";

  	f_wrd.open(complete_file_name, ios::in|ios::nocreate);
	if (f_wrd.fail())
	 merr<<"Cannot open input file : "<<complete_file_name;

    f_wrd>>actual_word;

	if (actual_word!="~")
		{
		sil_index=vocab.Binary_Search("silence", found);
		Check(found, "silence word not found in the vocabulary";);

		res=vocab[sil_index].unigram_lprob;
		actual_word.Make_Lower();
		actual_word_index=vocab.Binary_Search(actual_word, found);
		if (!found)
			merr<<"Not found in the vocabulary the word : "<<actual_word
			                                 <<" of file "<<complete_file_name<<endl;

		res+=vocab[sil_index].bigram_lprob[vocab.tbl_index[actual_word_index]];
		file_dim=2;
		prec_word_index=actual_word_index;
		}

    else{
		f_wrd>>prec_word;
		prec_word.Make_Lower();

		prec_word_index=vocab.Binary_Search(prec_word, found);

		if (!found)
			merr<<"Not found in the vocabulary the word : "<<prec_word
			                                 <<" of file "<<complete_file_name<<endl;
		res=vocab[prec_word_index].unigram_lprob;
		}
	while (!f_wrd.eof())
		{
		f_wrd >> actual_word;
		actual_word.Make_Lower();
		if (actual_word=="~")
			fin_sil=FALSE;
		else if (!f_wrd.eof())   
				{
		        file_dim++;
				actual_word_index=vocab.Binary_Search(actual_word, found);
				if (!found)
					merr<<"Not found in the vocabulary the word : "<<actual_word
			                                 <<" of file "<<complete_file_name<<endl;
				res+=vocab[prec_word_index].bigram_lprob[vocab.tbl_index[actual_word_index]];
				prec_word_index=actual_word_index;
				} //end of if(!f_wrd.eof())
		}  //end of while (!f_wrd.eof())
     
	if (fin_sil)
		{
		//suppose that each sentence finish with a silence 
		res+=vocab[prec_word_index].bigram_lprob[vocab.tbl_index[sil_index]];
		file_dim++;
		}

	return exp(-(res/(t_real)file_dim));
	} //end of Compute_Partial_Perplexity_Bigram function


//compute the perplexity of bigram classes language model on a single file
t_real LexiconAtis3::Compute_Partial_Perplexity_Bigram_Classes (const String & f_name,
	                                 const LexiconOptions & lexicon_options) 
	{
    String complete_file_name;
	String prec_word;
	String actual_word;
	ifstream f_wrd;
	t_real res;
	t_index actual_word_index;
	t_index prec_class_index;
	t_index actual_class_index;
	t_index index_word_class;
    t_index file_dim;
	Boolean found;
	Boolean fin_sil=TRUE;
    
	t_index pos;
    if (f_name.Is_SubString_Inside("Atis3", pos))
		complete_file_name << f_name<<".sro";
	else complete_file_name<<f_name<<".lsn";

	f_wrd.open(complete_file_name, ios::in|ios::nocreate);
	if (f_wrd.fail())
		merr<<"Cannot open input file : "<<complete_file_name;

    f_wrd>>actual_word;

	if (actual_word!="~")
		{
		res=log(autom_classes[sil_class].unig_prob);
		actual_word.Make_Lower();
		actual_word_index=vocab.Binary_Search(actual_word, found);
		if (!found)
			merr<<"Not found in the vocabulary the word "<<actual_word
							<<" of file "<<complete_file_name<<endl;
		prec_class_index=vocab[actual_word_index].belonging_class.First_Elem();
		index_word_class=vocab[actual_word_index].belonging_class.Second_Elem();
		res+=autom_classes[sil_class].class_prob[prec_class_index]+
			   (autom_classes[prec_class_index].words_prob[index_word_class]);
		file_dim=2;
		}
    else{
		actual_word.Make_Lower();
		actual_word_index=vocab.Binary_Search(actual_word, found);
		if (!found)
			merr<<"Not found in the vocabulary the word "<<actual_word
	                    <<" of file "<<complete_file_name<<endl;
     
		prec_class_index=vocab[actual_word_index].belonging_class.First_Elem();
		index_word_class=vocab[actual_word_index].belonging_class.Second_Elem();
		res=log(autom_classes[prec_class_index].unig_prob);
		file_dim=1;
		}

    while (!f_wrd.eof())
		{
		f_wrd >> actual_word;
		actual_word.Make_Lower();
		if (actual_word=="~")
			fin_sil=FALSE;
		else if (!f_wrd.eof())   
			{
			file_dim++;
			actual_word_index=vocab.Binary_Search(actual_word, found);
			if (!found)
				merr<<"Not found in the vocabulary the word : "<<actual_word
			                                 <<" of file "<<complete_file_name<<endl;
			actual_class_index=vocab[actual_word_index].belonging_class.First_Elem();
			index_word_class=vocab[actual_word_index].belonging_class.Second_Elem();
			res+=autom_classes[prec_class_index].class_prob[actual_class_index]+
		         (autom_classes[actual_class_index].words_prob[index_word_class]);	   
			prec_class_index=actual_class_index;
			} //end of if(!f_wrd.eof())
		}  //end of while (!f_wrd.eof())
     
	if (fin_sil)
		{
		//suppose that each sentence finish with a silence 
		res+=autom_classes[prec_class_index].class_prob[sil_class];
		file_dim++;
		}

	return exp(-(res/(t_real)file_dim));
   
	} //end of Compute_Partial_Perplexity_Bigram_Classes function



//***********************************************************************************//
//                                                                                   //
//                         class LexiconTimit                                        //
//                                                                                   //
//***********************************************************************************//


void LexiconTimit::Load_Phonetic_Transcriptions (ifstream & phn_file, 
                                     const String & first_phon, t_index initial_sample, 
								 t_index final_sample, Boolean & inserted, t_index pos)
	{
    t_index temp1;
	t_index temp2;
	t_index contr_dim;
	t_index num_pron;
	t_index k,i;
	String none;
    ImpObjectList<String> control;       
    Boolean found=FALSE;

	//copy phonetic transcription on control

	if (first_phon!="")
		{
		//the last phoneme of last word coincides with the first phoneme of actual word
		control.Destroy_And_ReDim(1);
		control[0]=first_phon;
		phn_file>>temp1;
		phn_file>>temp2;
		}
	else{
		//search first sample of word
		//necessary for evental presence of silence between last and actual word
		phn_file>>temp1;
		while(temp1!=initial_sample)
	        {
			phn_file>>temp2;
			phn_file>>none;
			phn_file>>temp1;
			}
		phn_file>>temp2;
		}
	while(temp2!=final_sample)  
		{                         
		//load phonetic transcription on control
		control.Save_And_ReDim(control.Dim()+1);
		phn_file>>control[control.Dim()-1];

		phn_file>>temp1;
		phn_file>>temp2;
		}
	  
    control.Save_And_ReDim(control.Dim()+1);
    phn_file>>control[control.Dim()-1];

    if (inserted==TRUE)
		{
		//this is the first proununce of actual word
		vocab[pos].phns_transc.Destroy_And_ReDim(1);
		vocab[pos].occur_pron=1;
		vocab[pos].phns_transc[0].phn=control; 
		vocab[pos].phns_transc[0].occur=1;
		}
	else //if (inserted==FALSE)
		{
		//control cycle for pronounce already inserted
		contr_dim=control.Dim();
		num_pron=vocab[pos].occur_pron;

		for (k=0;k<num_pron;k++)
	        {
			if (contr_dim==vocab[pos].phns_transc[k].phn.Dim())
				{
				i=0;
				while(i<contr_dim AND control[i]==vocab[pos].phns_transc[k].phn[i])
					i++;
				if (i==contr_dim)
					{		     
					vocab[pos].phns_transc[k].occur++;
					found=TRUE;
					}
				}
			}//end of for cycle

		if (!found)
			{
			vocab[pos].phns_transc.Save_And_ReDim(num_pron+1);
			vocab[pos].phns_transc[num_pron].phn=control; 
			vocab[pos].phns_transc[num_pron].occur=1; 
			vocab[pos].occur_pron++;
			}

		}//end of else //if (inserted==FALSE)
    return ;
	}//end of Load_Timit_Phon function


void LexiconTimit::Write_File_Of_Vocabulary (const String & f_name) 
	{
    ofstream f_vocab;
	t_index dim=vocab.Dim();
	t_index i, j;

	f_vocab.open(f_name);
	if (f_vocab.fail())
	  merr<<"Cannot open file : "<<f_name;

	mstat <<"Writing file of vocabulary"<<endl;
	
	f_vocab<<"Corpus_dimension: "<<corpus_dim<<endl;
	f_vocab<<"Vocabulary_cardinality: "<<dim<<endl<<endl;

    f_vocab<<"Phonetic_transcriptions_of_words:"<<endl;
    f_vocab<<"word  #pronounces  #phonemes prob_pronounce phonemes"<<endl;
	for (i=0; i<dim; i++)
		{
		//write the word ...
		String word;
		vocab.Get_Key(i, word);
		f_vocab<<word<<'\t';
	 
        //... different pronounces number ...
		f_vocab<<vocab[i].occur_pron;
		for (t_index k=0; k<vocab[i].occur_pron; k++)
			{
			t_index num_phon=vocab[i].phns_transc[k].phn.Dim();
			//... number of phonemes of the pronounce of index [k] ...
			f_vocab<<'\t'<<num_phon<<'\t';
			//... logarithmic probability of pronounce of index [k] ...
			f_vocab<<log((t_real)(vocab[i].phns_transc[k].occur)/(vocab[i].occur_word))<<'\t';
			//... phonemes of phonetic trascription of index [k]
			for (j=0; j<num_phon; j++)
				f_vocab<<'/'<<vocab[i].phns_transc[k].phn[j];
			f_vocab<<'/'<<endl;
			}  //end of for (t_index k=0; k<vocab[ind].occur_pron; k++)
		f_vocab<<endl;
		}  //end of for (i=0; i<dim; i++)
	f_vocab<<endl;
	f_vocab.close();

	return;
	}	 //end of LexiconTimit::Write_File_Of_Vocabulary function


void LexiconTimit::Build_Partial_Unigram_Language_Model (const String & file_name, 
                                   t_index & sil_index, t_index & file_dim)
	{
    String complete_file_name_wrd;
    String actual_word;
    Boolean not_present, present;
    ifstream f_wrd;
    ifstream f_phn;
    t_index prec_word_index;
	t_index actual_word_index; 
    t_index first_sample, last_sample;
	t_index last_sample_old=0;
	t_index dim;
	t_index i;
	String first_phon;
    String complete_file_name_phn;
    WordInfo new_word;

    complete_file_name_phn=file_name;
    complete_file_name_wrd=file_name;

	complete_file_name_wrd << ".wrd";
    complete_file_name_phn << ".phn";
	   
	f_wrd.open(complete_file_name_wrd, ios::in|ios::nocreate);    
    if (f_wrd.fail()) 
		merr<<"Cannot open input file : "<<complete_file_name_wrd;

    f_phn.open(complete_file_name_phn, ios::in|ios::nocreate);
    if (f_phn.fail()) 
		merr<<"Cannot open input file : "<<complete_file_name_phn;

    //suppose that each sentence start with silence
    actual_word="silence";
    
	new_word.occur_word=1;
	new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
    new_word.bigram_transitions.Set(0);
	new_word.bigram_transitions.Set_Data_Chunk(10);

    //insert the word in the vocabulary
    present=vocab.Insertion(actual_word, new_word, prec_word_index);
  	   
	if (present)
		vocab[prec_word_index].occur_word++;
	else{
		dim=vocab.Dim();
		for (i=0; i<dim; i++)
			vocab[i].bigram_transitions.Append(0);
		}
	file_dim=1;

    while (!f_wrd.eof())
		{
     	f_wrd>>first_sample;
		f_wrd>>last_sample;
	    f_wrd>>actual_word;  

		if (!f_wrd.eof()) 
			{
			file_dim++; 
			new_word.occur_word=1;
			new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
			new_word.bigram_transitions.Set(0);
			new_word.bigram_transitions.Set_Data_Chunk(10);
			present=vocab.Insertion(actual_word,new_word,actual_word_index);
		
			if (present)
				vocab[actual_word_index].occur_word++;
			else{
				dim=vocab.Dim();
				for (i=0; i<dim; i++)
					vocab[i].bigram_transitions.Append(0);
				}
		 
			//load phonetic transcription 
			// if (firts_sample<last_sample_old) then the first phoneme of next_word
			// coincides with the last phoneme of actual_word
			if (first_sample<last_sample_old)
				{
				t_index oc_pr = vocab[prec_word_index].occur_pron;
				t_index last_phn_index=vocab[prec_word_index].phns_transc[oc_pr-1].phn.Dim()-1;
				first_phon=vocab[prec_word_index].phns_transc[oc_pr-1].phn[last_phn_index];
				}
			else //the first phoneme of actual word must be read
			    first_phon = ""; 

			not_present=!present;
			//if (last_sampe==last_sample_old) then the phonetic transcription of
			//actual word composed only by the last phoneme of last word
			if (last_sample > last_sample_old)
				Load_Phonetic_Transcriptions(f_phn, first_phon, first_sample, last_sample, not_present, actual_word_index); 
		      
			last_sample_old=last_sample;
			prec_word_index=actual_word_index;

			}//end of if (!f_wrd.eof())
		}//end of while(!f_wrd.eof())

    f_phn.close();

    //suppose that each sentence end is silence 
    actual_word="silence";
    file_dim++;

    new_word.occur_word=1;
	new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
    new_word.bigram_transitions.Set(0);
	new_word.bigram_transitions.Set_Data_Chunk(10);

    //insert the word in the vocabulary
    present=vocab.Insertion(actual_word, new_word, actual_word_index);
  	   
	if (present)
		vocab[actual_word_index].occur_word++;
	else{
		dim=vocab.Dim();
		for (i=0; i<dim; i++)
			vocab[i].bigram_transitions.Append(0);
		}
	f_wrd.close();
    return;
	}//end of LexiconTimit::Build_Partial_Unigram_Language_Model function


//gives back the unigram language model
void LexiconTimit::Build_Unigram_Language_Model(const ImpObjectList<String> & f_list, 
                                const LexiconOptions& lexic_options) 
	{
    t_index num_files=f_list.Dim();
	t_index i, file_dim=0;
    t_index sil_index;

    corpus_dim=0;

    //update vocabulary on all files of training files list
    for (i=0; i<num_files; i++)
		{
        Build_Partial_Unigram_Language_Model(f_list[i], sil_index, file_dim);
		corpus_dim+=file_dim;
		}
 
    //update silence field
	vocab[sil_index].phns_transc.Destroy_And_ReDim(1);
	vocab[sil_index].phns_transc[0].phn.Destroy_And_ReDim(1);
	vocab[sil_index].phns_transc[0].phn[0]="sil";
	vocab[sil_index].phns_transc[0].occur=vocab[sil_index].occur_word;
	vocab[sil_index].occur_pron=1; 
  
    //calculate the logprobability of unigram
    Update_Unigram_Probability ();
    
    return;
	} //end of Build_Unigram_Language_Model function


//gives back the vocabulary up-to-date just on single file
//build vocabulary with dynamic algorithm and update statistical 
//informations about transitions between words	

void LexiconTimit::Count_Partial_Bigram_Transitions(const String & file_name, t_index & file_dim)

	{
    t_index prec_word_index;
    t_index next_word_index;
	t_index dim;
	t_index i;
    String complete_file_name_wrd;
    String actual_word;
    String next_word;
	WordInfo new_word;
    Boolean not_present, present;
    ifstream f_wrd;
    ifstream f_phn;

    complete_file_name_wrd = file_name;

    t_index first_sample, last_sample;
	t_index last_sample_old=0;
	String first_phon;
    String complete_file_name_phn;

    complete_file_name_phn=file_name;

	complete_file_name_wrd << ".wrd";
    complete_file_name_phn << ".phn";
	  
	f_wrd.open(complete_file_name_wrd, ios::in|ios::nocreate);    
    if (f_wrd.fail()) 
		merr<<"Cannot open input file : "<<complete_file_name_wrd;

    f_phn.open(complete_file_name_phn, ios::in|ios::nocreate);
    if (f_phn.fail()) 
		merr<<"Cannot open input file : "<<complete_file_name_phn;

    //suppose that each sentence start with silence
    actual_word="silence";

    new_word.occur_word=1;
	new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
    new_word.bigram_transitions.Set(0);
	new_word.bigram_transitions.Set_Data_Chunk(10);

    //insert the word in the vocabulary
    present=vocab.Insertion(actual_word, new_word, prec_word_index);
  	   
	if (present)
		vocab[prec_word_index].occur_word++;
	else{
		dim=vocab.Dim();
		for (i=0; i<dim; i++)
			vocab[i].bigram_transitions.Append(0);
		}
       
	file_dim=1;

    while (!f_wrd.eof())
		{
       	f_wrd>>first_sample;
		f_wrd>>last_sample;
	   	f_wrd>>next_word;  

		if (!f_wrd.eof()) 
			{
			file_dim++; 

		    new_word.occur_word=1;
			new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
			new_word.bigram_transitions.Set(0);
			new_word.bigram_transitions.Set_Data_Chunk(10);

			//insert the word in the vocabulary
			present=vocab.Insertion(next_word, new_word, next_word_index);
  	   
			if (present)
				vocab[prec_word_index].occur_word++;
			else{
				dim=vocab.Dim();
				for (i=0; i<dim; i++)
					vocab[i].bigram_transitions.Append(0);
				}
			//update the count of the transitions between the last two words
			if (!present AND next_word_index<=prec_word_index)
				{
				prec_word_index++;
				vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]++;
				}
			else vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]++;
         
			//load phonetic transcription 
			// if (firts_sample<last_sample_old) then the first phoneme of next_word
			// coincides with the last phoneme of actual_word
			if (first_sample<last_sample_old)
				{
				t_index oc_pr = vocab[prec_word_index].occur_pron;
				t_index last_phn_index=vocab[prec_word_index].phns_transc[oc_pr-1].phn.Dim()-1;
				first_phon=vocab[prec_word_index].phns_transc[oc_pr-1].phn[last_phn_index];
				}
			else //the first phoneme of actual word must be read
				first_phon = ""; 

			not_present=!present;
			//if (last_sampe==last_sample_old) then the phonetic transcription of
			//actual word composed only by the last phoneme of last word
			if (last_sample > last_sample_old)
				Load_Phonetic_Transcriptions(f_phn, first_phon, first_sample, last_sample, not_present, next_word_index); 
			     
			prec_word_index=next_word_index;
			last_sample_old=last_sample;

			}//end of if (!f_wrd.eof())
		}//end of while(!f_wrd.eof())

    f_phn.close();

    //suppose that each sentence end is silence 
    next_word="silence";
    file_dim++;

    new_word.occur_word=1;
	new_word.bigram_transitions.Destroy_And_ReDim(vocab.Dim()+1);
    new_word.bigram_transitions.Set(0);
	new_word.bigram_transitions.Set_Data_Chunk(10);

    //insert the word in the vocabulary
    present=vocab.Insertion(next_word, new_word, next_word_index);
  	   
	if (present)
		vocab[next_word_index].occur_word++;
	else{
		dim=vocab.Dim();
		for (i=0; i<dim; i++)
			vocab[i].bigram_transitions.Append(0);
		}
    
    vocab[prec_word_index].bigram_transitions[vocab.tbl_index[next_word_index]]+=1;

	f_wrd.close();

	return;

	}//end of Build_Partial_Bigram_Language_Model function


//gives back the bigram langauge model
void LexiconTimit::Count_Bigram_Transitions(const ImpObjectList<String> & f_list, 
                                const LexiconOptions& lexic_options)
	{
    t_index num_files = f_list.Dim();
	t_index i, file_dim=0;
    t_index sil_index;
    Boolean found;
    corpus_dim=0;
  
    //up_date vocabulary on all files of training files list
	for (i=0; i<num_files; i++)
		{
		mstat<<"Procassing file "<<i<<" di "<<num_files<<" files"<<endl;

		Count_Partial_Bigram_Transitions(f_list[i], file_dim);
		corpus_dim+=file_dim;
		}

    sil_index=vocab.Binary_Search("silence", found);

	Check(found, "silence word not found";);

	vocab[sil_index].phns_transc.Destroy_And_ReDim(1);
	vocab[sil_index].phns_transc[0].phn.Destroy_And_ReDim(1);
	vocab[sil_index].phns_transc[0].phn[0]="sil";
	vocab[sil_index].phns_transc[0].occur=vocab[sil_index].occur_word;
	vocab[sil_index].occur_pron=1;   
   
    return;

	} //end of Build_Bigram_Language_Model function
    

//Compute logperplexity on a only file for bigram language model
t_real LexiconTimit::Compute_Partial_Perplexity_Bigram (const String & f_name, const LexiconOptions & lexicon_options)
	{
    t_real res;
	t_index file_dim;
    String complete_file_name;
	String prec_word;
	String actual_word;
	ifstream f_wrd;
	t_index prec_word_index;
	t_index actual_word_index;
	t_index sil_index;
	Boolean found;

	complete_file_name<<f_name<<".wrd";
    
	//suppose that each sentence start with a silence word
    prec_word="silence";
    
	prec_word_index=vocab.Binary_Search(prec_word, found);
    sil_index=prec_word_index;

    if (!found)
	 merr<<"Not found in the vocabulary the word : silence"<<endl;

    res = vocab[prec_word_index].unigram_lprob;
    
    file_dim=1;
    t_index num_sample;
    
	f_wrd.open(complete_file_name, ios::in|ios::nocreate);
	if (f_wrd.fail())
		merr<<"Cannot open input file : "<<complete_file_name;

	while (!f_wrd.eof())
		{
        f_wrd>>num_sample;
		f_wrd>>num_sample;
	   	f_wrd>>actual_word;  

		if (!f_wrd.eof()) 
			{
			file_dim++; 
			actual_word_index=vocab.Binary_Search(actual_word, found);
			if (!found)
				merr<<"Not found the word "<<actual_word<<" in the vocabulary";
		   
			res+=vocab[prec_word_index].bigram_lprob[vocab.tbl_index[actual_word_index]];
            prec_word_index=actual_word_index;
			}
		}
      
    //suppose that each sentence finish with a silence
	res+=vocab[prec_word_index].bigram_lprob[vocab.tbl_index[sil_index]];
	file_dim++;

	return exp(-(res/(t_real)file_dim));
   
	} //end of Compute_Partial_Perplexity_Bigram function
 

//Compute logperplexity on a only file for bigram language model
t_real LexiconTimit::Compute_Partial_Perplexity_Bigram_Classes (const String & f_name, const LexiconOptions & lexicon_options)
	{
    String complete_file_name;
	String prec_word;
	String actual_word;
	ifstream f_wrd;
	t_real res;
	t_index num_sample;
	t_index actual_word_index;
	t_index prec_class_index;
	t_index actual_class_index;
	t_index index_word_class;
    t_index file_dim;
	Boolean found;
    
    complete_file_name<<f_name<<".wrd";

	f_wrd.open(complete_file_name, ios::in|ios::nocreate);
	if (f_wrd.fail())
		merr<<"Cannot open input file : "<<complete_file_name;

    res = log (autom_classes[sil_class].unig_prob);
    prec_class_index=sil_class;
    file_dim=1;

	while (!f_wrd.eof())
		{
		f_wrd>>num_sample;
		f_wrd>>num_sample;
		f_wrd >> actual_word;
                           
		if (!f_wrd.eof())   
			{
			file_dim++;
			actual_word_index=vocab.Binary_Search(actual_word, found);
			if (!found)
				merr<<"Not found in the vocabulary the word : "<<actual_word
			                                 <<" of file "<<complete_file_name<<endl;
			actual_class_index=vocab[actual_word_index].belonging_class.First_Elem();
			index_word_class=vocab[actual_word_index].belonging_class.Second_Elem();
			res+=autom_classes[prec_class_index].class_prob[actual_class_index]+
		         (autom_classes[actual_class_index].words_prob[index_word_class]);
			prec_class_index=actual_class_index;
			} //end of if(!f_wrd.eof())
		}  //end of while (!f_wrd.eof())
     
	//suppose that each sentence finish with a silence 
	res+=autom_classes[prec_class_index].class_prob[sil_class];
	file_dim++;
	return exp(-(res/(t_real)file_dim));
   	} //end of LexiconTimit::Compute_Partial_Perplexity_Bigram_Classes function


void Lexicon::Count_Partial_Phonemes_Transitions(const String & file_name,
                                                      const LabelType & label_t)
	{
    String complete_file_name;
	String phon;
    ifstream f_phn;
	t_index first_sample;
	t_index second_sample;
	t_index prec_phon_index;
	t_index actual_phon_index;

	complete_file_name<<file_name<<".phn";

    f_phn.open(complete_file_name, ios::in|ios::nocreate);
	if (f_phn.fail())
		merr<<"Cannot open file : "<<complete_file_name;
    
	f_phn>>first_sample;
    f_phn>>second_sample;
    f_phn>>phon;

    prec_phon_index=Index_Of_Phoneme (phon, label_t);
    occur_phon[prec_phon_index]++;
    corpus_dim++;
	while (!f_phn.eof())
		{
		f_phn>>first_sample;
		f_phn>>second_sample;
		f_phn>>phon;

		if (!f_phn.eof())
			{
			actual_phon_index=Index_Of_Phoneme (phon, label_t);
			occur_phon[actual_phon_index]++;
			corpus_dim++;
			bigram_trans[prec_phon_index][actual_phon_index]++;
			prec_phon_index=actual_phon_index;
			}	
		}
    f_phn.close();
    return;
	}


//count bigram transitions between phonemes
void Lexicon::Count_Phonemes_Transitions (const ImpObjectList<String> & file_list,
                           const LabelType & label_t)
	{
    t_index num_files=file_list.Dim();

    corpus_dim=0;

    mstat<<"Processing "<<num_files<<" files";
		
	for (t_index i=0; i<num_files; i++)
	  Count_Partial_Phonemes_Transitions(file_list[i], label_t);

	return;
	}
    

t_real LexiconTimit::Compute_Partial_Perplexity_Phon_Bigram(const String & file_name,
                                                     const LabelType & label_t)
	{
    t_real res;
 	t_index file_dim;
    String complete_file_name;
	String phon;
	ifstream f_phn;
	t_index prec_phon_index;
	t_index actual_phon_index;
	t_index first_sample;
	t_index second_sample;
	
	complete_file_name<<file_name<<".phn";
    f_phn.open(complete_file_name, ios::in|ios::nocreate);
	if (f_phn.fail())
		merr<<"Cannot open input file : "<<complete_file_name;

    f_phn>>first_sample;
	f_phn>>second_sample;
    f_phn>>phon;

	prec_phon_index=Index_Of_Phoneme (phon, label_t);

	if (occur_phon[prec_phon_index]==0)
		merr<<"Unseen phoneme in test corpus"; 

    res = log(((t_real)occur_phon[prec_phon_index])/(t_real)corpus_dim);
    file_dim=1;
    while (!f_phn.eof())
		{
        f_phn>>first_sample;
	    f_phn>>second_sample;
	    f_phn>>phon;  

	    if (!f_phn.eof()) 
			{
			file_dim++; 
			actual_phon_index=Index_Of_Phoneme (phon, label_t);
			res+=log((t_real)bigram_trans[prec_phon_index][actual_phon_index]/
		                                   (t_real)(occur_phon[prec_phon_index]));
            prec_phon_index=actual_phon_index;
			}
		}
      
	return exp(-(res/(t_real)file_dim));
   
	} //end of Compute_Partial_Perplexity_Bigram function
	 

