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
//  |   FILE:   Evaluate.cpp                                            |
//  |   FUNCTIONALITY: evaluate recognition                             |
//  |   PROGRAM: required to all codes                                  |
//  |   COMMENTS:          /                                             |
//  |   CONTRIBUTIONS: Domenico											|
//  |   ACTUAL REVISION: 6.0                                            |
//  |   DATA ACTUAL REVISION: 19/11/98                                  |
//  |   FIRST VERSION:   2.0                                            |
//  |   DATA FIRST VERSION: 1996                                        |
//  |                                                                   |
//  |-------------------------------------------------------------------|
//  _____________________________________________________________________

#include "../eval_rec/evaluate.h"

// *******************    VERSION HISTORY  *******************************
//linux compatibility added 19/11/98


void Evaluate::Build_Symbol_Table (const String & file_name)
	{
    ifstream f_in;
    String temp;
	Boolean present=FALSE;
	t_index position;
  
	f_in.open(file_name, ios::in|ios::nocreate);
	if (f_in.fail())
		merr<<"Cannot open file: "<<file_name;

    f_in>>temp;
    while (NOT(f_in.eof() AND temp[0]==EOF))
		{
		present=symbol_table.Insertion(temp, temp, position);
		if (present)
			{
			mwarn<<"Symbol "<<temp<<" is present twice in the file "<<file_name;
			present=FALSE;
			}
		f_in>>temp;
		}
    if (temp[0]!=-1)
		{
		present=symbol_table.Insertion(temp, temp, position);
		if (present)
		    {
			mwarn<<"Symbol "<<temp<<" is present twice in the file "<<file_name;
			present=FALSE;
			}
		}
    
	return;
	}

void Evaluate::Read_Sentences (const String & f_name, ImpObjectList<VetULong> & sentences,
                  VetULong & sym_list, Boolean sil_err, const String & sil_sym)
	{
    ifstream f_in;
	char buffer[MAX_LEN_INPUT_LINE];
	String temp;
	t_index i=0, j, k;
	Boolean found;
	

	f_in.open(f_name, ios::in|ios::nocreate);
	if (f_in.fail())
		merr<<"Cannot open file "<<f_name;

    f_in.getline(buffer, MAX_LEN_INPUT_LINE);
    do	{
		j=sentences.Dim(); //number of sentences already present in the list
		sentences.Save_And_ReDim(j+1);
		while (buffer[i]!='\0'AND buffer[i]!='\r'AND buffer[i]!='\n')
			{
			temp.Reset();
			while (buffer[i]!=' 'AND buffer[i]!='\0'
			      AND buffer[i]!='\r'AND buffer[i]!='\n')
				{
				temp<<buffer[i];
				i++;	
				}
			i++;
			if (sil_err OR temp!=sil_sym)
				{
				k=sentences[j].Dim(); //number of symbol already present in the sentence sentences[j]
				sentences[j].Save_And_ReDim(k+1);
				sentences[j][k]=symbol_table.Binary_Search(temp, found);
				if (!found)
					merr<<"Symbol "<<temp<<" of sentence "<<j+1<<" of file "<<f_name<<" isn't admissible";
				sym_list[sentences[j][k]]++;
				}
			}
		f_in.getline(buffer, MAX_LEN_INPUT_LINE);
		i=0;
		}	
	while (buffer[0]!='\0');
		f_in.close();
	return;
	}

void Evaluate::Build_Cost_Matrix (const VetULong & sol, const VetULong & res)
	{
    t_index i, j;
	t_index rows, culomns;
    t_index sym_sol_number;
	t_index sym_res_number;
	t_index m1, m2, m3;
    
    sym_sol_number=sol.Dim();
	sym_res_number=res.Dim();

    rows=sym_sol_number+1;
	culomns=sym_res_number+1;
    cost.Destroy_And_ReDim(rows, culomns);

    cost[0][0]=0;
	for (i=1; i<rows; i++)
		cost[i][0]=cost[i-1][0]+1;
	for (j=1; j<culomns; j++)
		cost[0][j]=cost[0][j-1]+1;

	for (i=1; i<rows; i++)
		for (j=1; j<culomns; j++)
			{
			if (sol[i-1]==res[j-1])
				m1=cost[i-1][j-1];
			else m1=cost[i-1][j-1]+1;
			m2=cost[i-1][j]+1;
			m3=cost[i][j-1]+1;
		 
			//assign to cost[i][j] min value among m1, m2 and m3
			if (m1<m2)
				if (m1<m3)
					cost[i][j]=m1;
				else cost[i][j]=m3;
			else if (m2<m3)
					cost[i][j]=m2;
			else cost[i][j]=m3;
			}

	return;
	}

void Evaluate::Compute_Error_On_One_Sentence (const VetULong & sol, const VetULong & res,
                     Boolean each_sentence, const String & f_name, t_index sent)
	{
    t_index i, j;
	t_index n_om=0;
	t_index n_ins=0;
	t_index n_sost=0;
    
    Build_Cost_Matrix (sol, res);
	
	i=cost.Dim_Row()-1;
	j=cost.Dim_Col()-1;

    if (each_sentence)
		{
		VetULong sol_all;
		VetULong res_all;
		ofstream f_out;
		t_index temp;

		while (i!=0 || j!=0)
			{
			sol_all.Save_And_ReDim(sol_all.Dim()+1);
			res_all.Save_And_ReDim(res_all.Dim()+1); 
			if (i!=0 AND cost[i][j]==(cost[i-1][j]+1))
				{
				i--;
				sol_all[sol_all.Dim()-1]=sol[i];
				res_all[res_all.Dim()-1]=(t_index)(-1);
				omiss[sol[i]]++;
				n_om++;
	  			}
			else if (j!=0 AND cost[i][j]==(cost[i][j-1]+1))
					{ 
					j--;
					sol_all[sol_all.Dim()-1]=(t_index)(-1);
					res_all[res_all.Dim()-1]=res[j];
					insert[res[j]]++;
					n_ins++;
					}
			else{
			    j--;
				i--;
			    sol_all[sol_all.Dim()-1]=sol[i];
		        res_all[res_all.Dim()-1]=res[j];
                sost[sol[i]][res[j]]++;
			    if (sol[i]!=res[j])
					n_sost++;
				}
			} //end of while (i!=0 || j!=0)
      
		f_out.open(f_name, ios::out|ios::app);
		if (f_out.fail())
			merr<<"Cannot open file: "<<f_name;

		f_out.precision(4);
		f_out<<"Sentence number "<<sent+1<<endl;
		f_out<<"Solution: ";
		for (i=sol_all.Dim(); i>0; i--)
			{
			temp=sol_all[i-1];
			if (temp==(t_index)(-1))
				f_out<<"* ";
			else f_out<<symbol_table[sol_all[i-1]]<<' ';
			}
		f_out<<endl<<"Result:    ";
		for (i=res_all.Dim(); i>0; i--)
			{
			temp=res_all[i-1];
			if (temp==(t_index)(-1))
				f_out<<"* ";
			else f_out<<symbol_table[res_all[i-1]]<<' ';
			}
		f_out<<endl<<"Symbol in the solution: "<<sol.Dim()<<endl;
		f_out<<"symbol in the result: "<<res.Dim()<<endl;
		f_out<<"insertion: "<<n_ins<<"; substitution: "<<n_sost<<"; omission: "<<n_om<<";"<<endl;
		f_out<<"percent correct: "<<(1-((t_real)(n_om+n_sost)/sol.Dim()))*100.0<<"%\t";
		f_out<<"accuracy: "<<(1-((t_real)(n_om+n_sost+n_ins)/sol.Dim()))*100.0<<"%";
		f_out<<endl<<endl;
		f_out.close();
		}
	else{
		while (i!=0 || j!=0)
			{
			if (i!=0 AND cost[i][j]==(cost[i-1][j]+1))
				{
				i--;
				omiss[sol[i]]++;
				n_om++;
	  			}
			else if (j!=0 AND cost[i][j]==(cost[i][j-1]+1))
					{ 
					j--;
					insert[res[j]]++;
					n_ins++;
					}
			else{
			    j--;
				i--;
	            sost[sol[i]][res[j]]++;
			    if (sol[i]!=res[j])
					n_sost++;
				}
			} //end of while (i!=0 || j!=0)
		}
    insertion+=n_ins;
	omission+=n_om;
	substitution+=n_sost;

    return;
	}

void Evaluate::Compute_Error (const EvalOption & option)
	{
    t_index i, j;
	t_index num_sentences=solution.Dim();
	t_index symb_sol=0; //numero di simboli veri
	t_index symb_res=0; //numero di simboli trovati
    ofstream f_out;
	ofstream f_matr;
	ofstream f_sent;
	t_index num_symbols;
	t_index sym_subst;

    insertion=0;
	substitution=0;
	omission=0;

    if (option.each_sentence)
		{
		f_sent.open(option.report_each_sent_file_name, ios::out);
		f_sent.close();
		}
    
    for (i=0; i<num_sentences; i++)
		{
		Compute_Error_On_One_Sentence (solution[i], result[i], option.each_sentence,
	                                              option.report_each_sent_file_name, i);
		symb_sol+=solution[i].Dim();
		symb_res+=result[i].Dim();
		}

    f_out.open(option.report_file_name, ios::out);
	if (f_out.fail())
		merr<<"Cannot open file: "<<option.report_file_name;
    f_out.precision(4);
	f_out<<endl;
	f_out<<" Number of sentences: "<<num_sentences<<endl<<endl;
	f_out<<" Insertion: "<<insertion<<" Substitution: "<<substitution<<" Omission: "<<omission<<endl;
	f_out<<" Number of symbols in the solution: "<<symb_sol<<endl;
    f_out<<" Number of symbols in the result: "<<symb_res<<endl;
	f_out<<" Percent correct: "<<(1-((t_real)(substitution+omission)/(t_real)symb_sol))*100.0<<"%\t";
	f_out<<" Accuracy: "<<(1-((t_real)(insertion+substitution+omission)/(t_real)symb_sol))*100.0<<"%"<<endl;
	
	num_symbols=symbol_table.Dim();
	if (option.each_symbol)
		{
		f_out<<endl<<endl;
		f_out<<"Symbols\t\tIns  Omiss  Subst     P. Correct     Accuracy"<<endl<<endl;
		for (i=0; i<num_symbols; i++)
			{
			f_out<<" "<<symbol_table[i]<<" ("<<sym_sol[i]<<")\t\t";
			f_out<<insert[i]<<"\t"<<omiss[i]<<"\t";
			sym_subst=0;
			for (j=0; j<num_symbols; j++)
				if (j!=i)
					sym_subst+=sost[i][j];
			f_out<<sym_subst<<"\t";
			if (sym_sol[i]>0)
				{
				f_out<<(1-((t_real)(omiss[i]+sym_subst)/sym_sol[i]))*100.0<<"%";
				f_out<<"\t\t"<<(1-((t_real)(insert[i]+omiss[i]+sym_subst)/sym_sol[i]))*100.0<<"%"<<endl;
				}
			else f_out<<"Symbol non present in the solution "<<endl;
			}
		}
	f_out.close();

	if (option.conf_matrix)
		{
		f_matr.open(option.report_conf_matrix_file_name, ios::out);
		if (f_matr.fail())
			merr<<"Cannot open file: "<<option.report_conf_matrix_file_name;

		for (i=0; i<num_symbols; i++)
			{
			f_matr<<endl<<" "<<symbol_table[i]<<"\t";
			for (j=0; j<num_symbols; j++)
				f_matr<<sost[i][j]<<' ';
			}
		f_matr.close();   	
		}	
	
	return;
	}
	
void Evaluate::Evaluate_Recog_Performance (const EvalOption & option)
	{
	Build_Symbol_Table (option.symb_file_name);
    sym_sol.Destroy_And_ReDim(symbol_table.Dim());

    //read all the sentence in the solution file
    Read_Sentences (option.solution_file_name, solution, sym_sol, option.sil_err, 
	                                                       option.sil_sym);

    sym_res.Destroy_And_ReDim(symbol_table.Dim());

    //read all the sentence in the result file
	Read_Sentences (option.result_file_name, result, sym_res, option.sil_err, 
	                                                       option.sil_sym);

    if (solution.Dim()!=result.Dim())
		merr<<"Number of sentence in solution file is "<<solution.Dim()
	         <<" while in result file is "<<result.Dim();

    omiss.Destroy_And_ReDim(symbol_table.Dim());
    insert.Destroy_And_ReDim(symbol_table.Dim());
    sost.Destroy_And_ReDim(symbol_table.Dim(), symbol_table.Dim());
    
	Compute_Error (option);
    return;
	}