/*
 * =====================================================================================
 *
 *       Filename:  CExperiment.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24/02/1014 18:100:109
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuji Hao (), hao.shuji@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  exit_with_help
 *  Description:  
 * =====================================================================================
 */
#define _CRT_SECURE_NO_DEPRECATE
#include "Experiment.h"
#include <iomanip>
#include "Global.h"
void CExperiment::ParseCommandLine(int argc, char **argv){
	std::cout << "Parse argv" << std::endl;
	int i = 0;
	for (i = 1; i<argc; i++)
	{
		if (argv[i][0] != '-') break;
		if (++i >= argc)
			this->ExitWithHelp();

		switch (argv[i - 1][1])
		{
			case 's':
				setting_file_full_name = argv[i];
				break;
			case 'l':
				data_file_location = argv[i];
				break;
			case 'd':
				data_file_name = argv[i];
				break;
			case 'o':
				output_dir = argv[i];
				break;
			default:
				cerr << "Unknown option:" << argv[i - 1][1] << endl;
				ExitWithHelp();
		}
	}
}

void CExperiment::ExitWithHelp( )
{
    std::cout << "Usage: Model [options] data_set_file" << endl;
    std::cout << "options:" << endl;
    std::cout << "-s setting_file" << endl;
    std::cout << "-d data_set_file" << endl;
    std::cout << "-o output_dir" << endl;       
    exit(EXIT_FAILURE);        
}       /* -----  end of function exit_with_help  ----- */

void CExperiment::AOS(int argc, char **argv)
{		
	ParseCommandLine(argc, argv);
	data_file_full_name = data_file_location + data_file_name;	
	par.ImportSetting(this->setting_file_full_name);
	data.Import_data(data_file_full_name);	
	if (par.Norm2One == 1)	data.Norm2One();	

	trainID_file_name = data_file_full_name + ".trainID";
	testID_file_name = data_file_full_name + ".testID";
	triplets_file_name = data_file_full_name + ".triplets";
	data.ImportTriplets(triplets_file_name);
	data.ImportTestID(testID_file_name);
	data.ImportTrainID(trainID_file_name);

	//par.MallocM(data.d);
	par.time_ticks = data.num_triplets / (par.num_fold*par.num_ticks);
	data.num_triplets_each_fold = data.num_triplets / par.num_fold;	
	data.num_trainID_each_fold = data.num_trainID / par.num_fold;
	data.num_testID_each_fold = data.num_testID / par.num_fold;	
	
	par.Initialize(data.d, data.n);

	std::cout << setw(10) << left << "Data" 
		<< setw(10) << left << "# Classes"
		<< setw(10) << left << "# Features"
		<< setw(15) << left << "# Instances"
		<< setw(10) << left << "# Triplet"
		<< endl;
	std::cout << setw(10) << left << this->data_file_name
		<< setw(10) << left << data.num_classes
		<< setw(10) << left << data.d
		<< setw(15) << left << data.n
		<< setw(10) << left << data.num_triplets_each_fold
		<< endl;

	if (par.Par_Loop==1){
		std::cout << "Parameters (W_Ini,Norm2One,AvgM, Loop" << endl;
		for (int i = 0; i < 2; i++){
			par.Norm2One = i;
			if (par.Norm2One == 1)
				data.Norm2One();
			for (int j = 0; j < 2; j++){
				par.M_Ini = j;
				for (int k = 0; k < 2; k++){
					par.AvgM = k;
					this->FindBestParameters();
					std::cout << "data.Norm2One=" << par.Norm2One << "\t par.M_Ini=" << par.M_Ini << "\t par.AvgM=" << par.AvgM << endl;
					this->ActiveLearn();
				}
			}
		}
	}
	else{
		if (par.Norm2One == 1)		
			data.Norm2One();
		else
			std::cout << "No Norm instance to length one" << endl;
		if(this->data_file_name.compare("letter_scale_all") == 0){
			this->par.OASIS_C = 1;		this->par.CWS_r = 0.01;
			this->par.CWS_eta = 100;	this->par.AROMA_r = 0.00001;
		}else if(this->data_file_name.compare("pendigits_all") == 0){
			this->par.OASIS_C = 1;		this->par.CWS_r = 0.1;
			this->par.CWS_eta = 10;	this->par.AROMA_r = 0.00001;
		}
		else if(this->data_file_name.compare("satimage_scale_all") == 0){
			this->par.OASIS_C = 100;	this->par.CWS_r = 0.001;
			this->par.CWS_eta = 10000;	this->par.AROMA_r = 0.001;
		}
		else if(this->data_file_name.compare("aloi50") == 0){
			this->par.OASIS_C = 1;		this->par.CWS_r = 1;
			this->par.CWS_eta = 1;		this->par.find_OASIS_C = 0;
			this->par.find_CWS_r = 0;	this->par.find_CWS_eta = 0;
			this->par.find_AROMA_r = 0;	this->FindBestParameters();
		}
		else if(this->data_file_name.compare("segment_scale") == 0){
			this->par.OASIS_C = 1;		this->par.CWS_r = 0.001;
			this->par.CWS_eta = 10000;	this->par.AROMA_r = 0.0001;
		}
		else if(this->data_file_name.compare("shuttle_scale_all") == 0){
			this->par.OASIS_C = 10;		this->par.CWS_r = 100000;
			this->par.CWS_eta = 1000;	this->par.AROMA_r = 0.01;
		}
		else{
			std::cout << "Parameters not manually set for dataset: " << this->data_file_name << endl;
		}
		this->FindBestParameters();
		
		std::cout << "data.Norm2One=" << par.Norm2One << "\t par.M_Ini=" << par.M_Ini << "\t par.AvgM=" << par.AvgM << endl;
		this->ActiveLearn();
	}

	par.Release(data.d);
	std::cout << "/******************************************/" << endl << endl;
}//end of function

void CExperiment::ActiveLearn(){
	
	int num_ticks = par.num_ticks;

	CModel** models = new CModel*[3];
	vector<string> alg_names;

	models[0] = new CModelOASIS; 
	alg_names.push_back("OASIS");
	alg_names.push_back("PAAS");
	alg_names.push_back("PARS");

	if (par.Full_Matrix == 1){
		models[1] = new CModelCWS;
		models[2] = new CModelAROMA;
		alg_names.push_back("CWS");
		alg_names.push_back("CWAS");
		alg_names.push_back("CWAS_Adap");
		alg_names.push_back("CWRS");
		alg_names.push_back("AROMA");
	}
	else{
		models[1] = new CModelCWSD;
		models[2] = new CModelAROMAD;
		alg_names.push_back("CWS-d");
		alg_names.push_back("CWAS-d");
		alg_names.push_back("CWAS_Adap-d");
		alg_names.push_back("CWRS-d");
		alg_names.push_back("AROMA-d");
	}
	par.num_alg = alg_names.size();

	CResult result_tmp(par.num_ticks);
	vector<vector<vector<CResult> > > result;
	result.resize(par.num_que);
	for (int i = 0; i<par.num_que; i++){
		result[i].resize(par.num_alg);
		for (int j = 0; j<par.num_alg; j++){
			result[i][j].resize(par.num_fold);
			for (int k = 0; k<par.num_fold; k++)
				result[i][j][k].Initialize(par.num_ticks);
		}
	}

	std::cout << std::fixed << endl;
	std::cout.precision(3);
	std::cout << setw(10) << "algorithm" << setw(10) << "que" << setw(10) << "pre_train" << setw(10) << "pre_test" << setw(10) << "avg_pre_test" << endl;

	string out_tmp_name = output_dir + data_file_name;// +"_M_Ini_" + NumberToString(par.M_Ini) + "_Norm2One_" + NumberToString(par.Norm2One) + "_AvgM_" + NumberToString(par.AvgM);
	out_tmp_name += "_tmp.txt";
	ofstream out_tmp;
	out_tmp.open(out_tmp_name.c_str());

	for (int i = par.i_query_start; i < par.i_query_end; i++){
	// for (int i = 0; i < 2; i++){

		std::cout << i << "-th query" << endl;

		double b_first = pow(2.0, (par.b_start_first + i * par.b_step_first));
		double b_second = pow(2.0, (par.b_start_second + i * par.b_step_second));
		// if (i==1){
		// 	b_first = pow(2.0, (par.b_start_first + 19 * par.b_step_first));
		// 	b_second = pow(2.0, (par.b_start_second + 19 * par.b_step_second));
		// }
		if (par.debug == 2)
			std::cout << b_first << endl;

		for (int j = 0; j<par.num_fold; j++){			
			if (par.debug >= 1)
				std::cout << "\t" << j << "-th fold" << endl;
			
			par.ind_fold = j;			

			int m = 0;
			// OASIS
			for (m = 0; m <= 2; m ++) {
				if (m==0 && i>par.i_query_start){
					result[i][m][j] = result[0][m][j];
					continue;
				}else{
					if (m == 0)
						par.b = -100; // useless
					else if (m == 1) 
						par.b = b_first; 	// active
					else if (m == 2) 		
						par.b = result[i][m - 1][j].que; //random
					else{
						std::cout << "m should be in {0,1,2}" << std::endl;
						exit(1);
					}

					models[0]->SetAlgName(alg_names[m]);
					result_tmp.Reset(); par.Reset(data.d, data.n);
					models[0]->Learning(&result_tmp, &data, &par);
					if (par.AvgM == 1){
						par.AverageSumM(data.d, data.num_triplets_each_fold);				
					}
					models[0]->TestOnTestID(&result_tmp, &data, &par);
					models[0]->TestOnTrainID(&result_tmp, &data, &par);
					result[i][m][j] = result_tmp;
				}
			}
			// CWS
			for (m = 3; m <= 6; m++) {
				if (m == 3 && i > par.i_query_start) {
					result[i][m][j] = result[0][m][j];
					continue;
				}else{
					if (m==3){
						par.b = -100; // passive, useless						
					}
					else if (m==4){ 
						par.b = b_first;  // CWAS
					}else if (m==5){ 
						par.b = b_second; //CWAS-Adap
					}else if (m==6){  //random
						par.b = result[i][m-2][j].que;
					}else{
						std::cout << "m should be in {3,4,5,6}" << std::endl;
						exit(1);
					}	
					models[1]->SetAlgName(alg_names[m]);
					result_tmp.Reset(); par.Reset(data.d, data.n);
					models[1]->Learning(&result_tmp, &data, &par);
					if (par.AvgM == 1){
						par.AverageSumM(data.d, data.num_triplets_each_fold);
					}
					models[1]->TestOnTestID(&result_tmp, &data, &par);
					models[1]->TestOnTrainID(&result_tmp, &data, &par);
					result[i][m][j] = result_tmp;				
				}
			}					

			//AROMA
			m = 7;
			if (i > 0){
				result[i][m][j] = result[0][m][j];
			}
			else{
				models[2]->SetAlgName(alg_names[m]);
				result_tmp.Reset(); par.Reset(data.d, data.n);
				models[2]->Learning(&result_tmp, &data, &par);
				if (par.AvgM == 1)
					par.AverageSumM(data.d, data.num_triplets_each_fold);
				models[2]->TestOnTestID(&result_tmp, &data, &par);
				models[2]->TestOnTrainID(&result_tmp, &data, &par);
				result[i][m][j] = result_tmp;
			}			
			

			for (int k = 0; k < par.num_alg; k++)
			{
				std::cout << setw(10) << alg_names[k]
					<< setw(10) << setprecision(4) << result[i][k][j].que
					<< setw(10) << setprecision(4) << result[i][k][j].AP_train
					<< setw(10) << setprecision(4) << result[i][k][j].AP_test
					<< setw(10) << setprecision(4) << result[i][k][j].mAP_test
					<< endl;
			}
		}//fold
		
		out_tmp << i << " query" << endl;

		for (int mm = 0; mm < par.num_alg; mm++)
		{
			out_tmp << alg_names[mm] << " algorithm" << endl;
			
			for (int jj = 0; jj < par.num_fold; jj++)
			{
				out_tmp << result[i][mm][jj].que << " ";
			}
			out_tmp << endl;
			for (int jj = 0; jj < par.num_fold; jj++)
			{
				out_tmp << result[i][mm][jj].AP_train << " ";
			}
			out_tmp << endl;
			for (int jj = 0; jj < par.num_fold; jj++)
			{
				out_tmp << result[i][mm][jj].mAP_train<< " ";
			}
			out_tmp << endl;
			for (int jj = 0; jj < par.num_fold; jj++)
			{
				out_tmp << result[i][mm][jj].AP_test<< " ";
			}
			out_tmp << endl;
			for (int jj = 0; jj < par.num_fold; jj++)
			{
				out_tmp << result[i][mm][jj].mAP_test<< " ";
			}
			out_tmp << endl;
			for (int jj = 0; jj < par.num_fold; jj++)
			{
				out_tmp << result[i][mm][jj].time<< " ";
			}
			out_tmp << endl;
		}
		
	}//que

	out_tmp.close();

	// write result into the files
	std::vector<vector<CResult> > res_que_alg;
	res_que_alg.resize(par.num_que);
	for (int i = 0; i < par.num_que; ++i){
		res_que_alg[i].resize(par.num_alg);
		for (int j = 0; j < par.num_alg; ++j){
			res_que_alg[i][j].Initialize(par.num_ticks);
		}
	}

	//sum of fold
	for (int i = 0; i < par.num_que; ++i)
		for (int j = 0; j < par.num_alg; ++j)
			for (int k = 0; k < par.num_fold; ++k){
				res_que_alg[i][j] = res_que_alg[i][j] + result[i][j][k];
			}

	//average
	for (int i = 0; i < par.num_que; ++i)
		for (int j = 0; j < par.num_alg; ++j)
			res_que_alg[i][j] = res_que_alg[i][j] / (double)par.num_fold;

	// compute the standard deviation \sqrt(sum(x-bar x)^2/num)
	for (int i = 0; i < par.num_que; ++i){
		for (int j = 0; j < par.num_alg; ++j){
		  double tmp_que = 0.0; 
		  double tmp_time = 0.0;
		  double tmp_AP_test = 0.0, tmp_mAP_test = 0.0, tmp_AP_train = 0.0, tmp_mAP_train = 0.0;
		  for (int k = 0; k < par.num_fold; ++k){
			// res_que_alg[i][j] = res_que_alg[i][j] + result[i][j][k];
			tmp_que += pow(result[i][j][k].que - res_que_alg[i][j].que,2); 
			tmp_time += pow(result[i][j][k].time - res_que_alg[i][j].time,2); 
			tmp_AP_test += pow(result[i][j][k].AP_test - res_que_alg[i][j].AP_test,2);
			tmp_mAP_test += pow(result[i][j][k].mAP_test - res_que_alg[i][j].mAP_test,2); 
			tmp_AP_train += pow(result[i][j][k].AP_train - res_que_alg[i][j].AP_train,2);
			tmp_mAP_train += pow(result[i][j][k].mAP_train - res_que_alg[i][j].mAP_train,2); 
		  }

		  res_que_alg[i][j].std_que = sqrt(tmp_que / par.num_fold);
		  res_que_alg[i][j].std_time = sqrt(tmp_time / par.num_fold);
		  res_que_alg[i][j].std_AP_test = sqrt(tmp_AP_test / par.num_fold);
		  res_que_alg[i][j].std_mAP_test = sqrt(tmp_mAP_test / par.num_fold); 
		  res_que_alg[i][j].std_AP_train = sqrt(tmp_AP_train / par.num_fold);
		  res_que_alg[i][j].std_mAP_train = sqrt(tmp_mAP_train / par.num_fold); 
		}
	}

	//output fixed query results
	string out_fixed_name = output_dir + data_file_name;// +"_M_Ini_" + NumberToString(par.M_Ini) + "_Norm2One_" + NumberToString(par.Norm2One) + "_AvgM_" + NumberToString(par.AvgM);
	out_fixed_name += "_fixed.txt";
	ofstream out_fixed;
	out_fixed.open(out_fixed_name.c_str());

	out_fixed << setw(15) << left << "Data" << setw(10) << left << "# Classes" << setw(10) << left << "# Features" 
			<< setw(10) << left << "# Instances" << setw(10) << left << "# Triplet_each_fold" << endl;

	out_fixed << setw(15) << left << this->data_file_name << setw(10) << left << data.num_classes << setw(10) << left << data.d 
			<< setw(10) << left << data.n << setw(10) << left << data.num_triplets_each_fold << endl << endl;

	out_fixed << setw(10) << left << "M_Ini" << setw(10) << left << "Norm2One" << setw(10) << left << "AvgM" 
			<< setw(10) << left << "Full_Matrix" << endl;

	out_fixed << setw(10) << left << par.M_Ini << setw(10) << left << par.Norm2One << setw(10) << left << par.AvgM 
			<< setw(10) << left << par.Full_Matrix << endl << endl;	

	out_fixed << setw(10) << left << "OASIS_C" << setw(10) << left << "CWS_r" << setw(10) << left << "CWS_eta" 
			<< setw(10) << left << "AROMA_r" << endl;

	out_fixed << setw(10) << left << par.OASIS_C << setw(10) << left << par.CWS_r << setw(10) << left << par.CWS_eta
			<< setw(10) << left << par.AROMA_r << endl << endl;

	//out_fixed << "OASIS_C=" << par.OASIS_C << "\t CWS_r=" << par.CWS_r << "\t CWS_eta=" << par.CWS_eta << "\t AROMA_r=" << par.AROMA_r << endl;
	int order[8];
	order[0] = 0; order[1] = 3; order[2] = 7; order[3] = 2; order[4] = 6; order[5] = 1; order[6] = 4; order[7] = 5;
	for (int i = 0; i < par.num_que; ++i){		
		out_fixed <<endl;
		out_fixed << setw(10) << left << "Algorithm" << " & " << setw(21) << right << "Query~(%)" << " & " << setw(21) << right << "AP~(Train)"
				<<" & " << setw(21) << right  << "mAP~(Train)" << " & " << setw(21) << right  << "AP~(Test)" <<" & " << setw(21) << right  << "mAP~(Test)" 
				<<" & " << setw(21) << right << "Time (s)" << endl;

		for (int k = 0; k < par.num_alg; ++k){
			int j = order[k];
			out_fixed << std::fixed;
			out_fixed << setw(10) << alg_names[j] << " & " << setprecision(3) 
			<< setw(8) << std::right << res_que_alg[i][j].que * 100	<< "$\\pm$" << setw(8) << std::left << res_que_alg[i][j].std_que * 100 << " & "
			<< setw(8) << std::right << res_que_alg[i][j].AP_train	<< "$\\pm$" << setw(8) << std::left << res_que_alg[i][j].std_AP_train << " & "
			<< setw(8) << std::right << res_que_alg[i][j].mAP_train << "$\\pm$" << setw(8) << std::left << res_que_alg[i][j].std_mAP_train << " & "
			<< setw(8) << std::right << res_que_alg[i][j].AP_test	<< "$\\pm$" << setw(8) << std::left << res_que_alg[i][j].std_AP_test << " & "
			<< setw(8) << std::right << res_que_alg[i][j].mAP_test	<< "$\\pm$" << setw(8) << std::left << res_que_alg[i][j].std_mAP_test << " & "
			<< setw(8) << std::right << res_que_alg[i][j].time		<< "$\\pm$" << setw(8) << std::left << res_que_alg[i][j].std_time
			<< endl;
		}
	}
	out_fixed.close();

  	//output varied query result  
	string output_file_name = output_dir + data_file_name+".txt";
	std::cout << "output_file_name = " << output_file_name << endl;
	ofstream output_file_varied(output_file_name.c_str());
	for (int j = 0; j<par.num_alg; j++){
		
		output_file_varied << std::fixed << std::setprecision(3);
		output_file_varied << alg_names[j] << " " << "que" << endl;
		for (int i = 0; i<par.num_que; i++)			
			output_file_varied << setw(9) << left  << res_que_alg[i][j].que;
		output_file_varied << endl;
		
		output_file_varied << alg_names[j] << " " << "std_que" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].std_que;		
		output_file_varied << endl;

		output_file_varied << alg_names[j] << " " << "AP_train" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].AP_train;		
		output_file_varied << endl;

		output_file_varied << alg_names[j] << " " << "std_AP_train" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].std_AP_train;
		output_file_varied << endl;

		output_file_varied << alg_names[j] << " " << "mAP_train" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].mAP_train;
		output_file_varied << endl;
			
		output_file_varied << alg_names[j] << " " << "std_mAP_train" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].std_mAP_train;
		output_file_varied << endl;

		output_file_varied << alg_names[j] << " " << "AP_test" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].AP_test;
		output_file_varied << endl;

		output_file_varied << alg_names[j] << " " << "std_AP_test" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].std_AP_test;
		output_file_varied << endl;
		
		output_file_varied << alg_names[j] << " " << "mAP_test" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].mAP_test;		
		output_file_varied << endl;

		output_file_varied << alg_names[j] << " " << "std_mAP_test" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].std_mAP_test;
		output_file_varied << endl;

		output_file_varied << alg_names[j] << " " << "time" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].time;
		output_file_varied << endl;	

		output_file_varied << alg_names[j] << " " << "std_time" << endl;
		for (int i = 0; i<par.num_que; i++)
			output_file_varied << setw(9) << left  << res_que_alg[i][j].std_time;
		output_file_varied << endl;	
	}
	output_file_varied.close();
	

	for (int i = 0; i < 2; i++)
	{
		delete models[i];
	}
	delete [] models;
}

void CExperiment::FindBestParameters()
{
	std::vector<string> alg_names;
	CModel *oasis, *arow, *arowc;
	oasis = new CModelOASIS;
	alg_names.push_back("OASIS");
	
	if (par.Full_Matrix == 1)
	{
		arow = new CModelCWS;
		arowc = new CModelAROMA;
		alg_names.push_back("CWS");
		alg_names.push_back("AROMA");
	}
	else
	{
		arow = new CModelCWSD;
		arowc = new CModelAROMAD;
		alg_names.push_back("CWS-d");
		alg_names.push_back("AROMA-d");
	}
	std::cout << "Starting find best parameters" << endl;
	string filename = output_dir + data_file_name + "_par.txt"; // Model[j]->Get_alg_name();
	//std::cout << "output_file_name = " << filename << endl;
	ofstream output_file_varied(filename.c_str());
	string filename_figure  = output_dir + data_file_name + "_par_figure.txt";
	ofstream output_file_varied_figure(filename_figure.c_str());

	if (!output_file_varied.is_open()){
		std::cout << filename << "\t could not be open\n";
		exit(1);
	}

	CResult result_tmp(par.num_ticks);

	double max_OASIS_Acc = 0.0, best_OASIS_C = 0.0;
	double max_CWS_Acc = 0.0, best_CWS_r = 0.0, best_CWS_eta = 0.0;
	double max_AROMA_Acc = 0.0, best_AROMA_r = 0.0;

	if (par.find_OASIS_C)
	{
		std::cout << "Start to find best OASIS_C\n";
		//std::cout << "/******************************************/" << endl;
		std::cout << setw(10) << "OASIS_C"
			<< setw(10) << "AP_test"
			<< setw(10) << "max_AP_test"
			<< endl;
		output_file_varied << setw(10) << "OASIS_C"
			<< setw(10) << "AP_test "
			<< setw(10) << "best_AP_test"
			<< endl;
		for (int i = 0; i < 11; i++)
		{
			par.OASIS_C = pow(10, -5 + i);
			//std::cout << par.OASIS_C << endl;
			double tmp_AP = 0.0;
			for (int k = 0; k < par.num_fold; k++)
			{
				par.ind_fold = k;
				oasis->SetAlgName(alg_names[0]);
				par.Reset(data.d, data.n);
				result_tmp.Reset();
				oasis->Learning(&result_tmp, &data, &par);
				oasis->TestOnTestID(&result_tmp, &data, &par);
				tmp_AP += result_tmp.AP_test;
				//std::cout <<"\t" << k << "  -th fold\t" << result_tmp.AP_test << endl;
			}
			tmp_AP = (double)tmp_AP / par.num_fold;
			if (tmp_AP > max_OASIS_Acc){
				max_OASIS_Acc = tmp_AP;
				best_OASIS_C = par.OASIS_C;
				//std::cout << "\t " << best_OASIS_C << "\t" << max_OASIS_Acc << endl;
			}
			
			output_file_varied << setw(10) << setprecision(4) << par.OASIS_C 
				<< setw(10) << setprecision(4) << tmp_AP 
				<< setw(10) << setprecision(4) << max_OASIS_Acc 
				<< endl;
		
			output_file_varied_figure << tmp_AP << " ";
			
			std::cout << setw(10) << setprecision(4) << par.OASIS_C
				<< setw(10) << setprecision(4) << tmp_AP
				<< setw(10) << setprecision(4) << max_OASIS_Acc
				<< endl;
		}
		output_file_varied_figure << endl;

		par.OASIS_C = best_OASIS_C;
		output_file_varied << "******************************************" << endl;
		output_file_varied << "Best OASIS_C: \t" << best_OASIS_C << "\t AP_test=" << max_OASIS_Acc << endl;
		output_file_varied << "******************************************" << endl;

		std::cout << "******************************************" << endl;
		std::cout << "Best OASIS_C: \t" << best_OASIS_C << "\t AP_test=" << max_OASIS_Acc << endl;
		std::cout << "******************************************" << endl;
	}
	else{
		std::cout << "Unkown how to optimize OASIS parameters" << std::endl;	
	}
	
	if (par.find_CWS_eta && par.find_CWS_r)
	{
		std::cout << "Finding best CWS_eta" << endl;
		output_file_varied << setw(10) << "CWS_eta "
			<< setw(10) << "CWS_r "
			<< setw(10) << "AP_test "
			<< setw(10) << "best_AP_test"
			<< endl;		
		std::cout << setw(10) << "CWS_eta"
			<< setw(10) << "CWS_r"
			<< setw(10) << "AP_test"
			<< setw(10) << "max_AP_test"
			<< endl;
		for (int i = 0; i < 11; i++)
		{
			par.CWS_eta = pow(10, -5 + i);
			for (int j = 0; j < 11; j++)
			{
				par.CWS_r = pow(10, -5 + j);
				double tmp_AP = 0.0;
				for (int k = 0; k < par.num_fold; k++)
				{
					par.ind_fold = k;
					arow->SetAlgName(alg_names[1]);
					par.Reset(data.d, data.n);
					result_tmp.Reset();
					arow->Learning(&result_tmp, &data, &par);
					arow->TestOnTestID(&result_tmp, &data, &par);
					tmp_AP += result_tmp.AP_test;
				}
				tmp_AP = (double)tmp_AP / par.num_fold;
				if (tmp_AP > max_CWS_Acc){
					max_CWS_Acc = tmp_AP;
					best_CWS_eta = par.CWS_eta;
					best_CWS_r = par.CWS_r;
				}
//				output_file_varied << par.CWS_eta << "\t" << par.CWS_r << "\t" << tmp_AP << "\t" << max_CWS_Acc << endl;
				output_file_varied << setw(10) << setprecision(4) << par.CWS_eta
					<< setw(10) << setprecision(4) << par.CWS_r
					<< setw(10) << setprecision(4) << tmp_AP
					<< setw(10) << setprecision(4) << max_CWS_Acc
					<< endl;

				output_file_varied_figure << tmp_AP << " ";

				//std::cout << par.CWS_eta << "\t" << par.CWS_r << "\t" << tmp_AP << "\t" << max_CWS_Acc << endl;
				std::cout << setw(10) << setprecision(4) << par.CWS_eta
					<< setw(10) << setprecision(4) << par.CWS_r
					<< setw(10) << setprecision(4) << tmp_AP
					<< setw(10) << setprecision(4) << max_CWS_Acc
					<< endl;
			}
		}
		par.CWS_r = best_CWS_r;
		par.CWS_eta = best_CWS_eta;
		output_file_varied << "******************************************" << endl;
		output_file_varied << "Best CWS_eta=" << best_CWS_eta << "\t CWS_r=" << best_CWS_r << "\t AP_test=" << max_CWS_Acc << endl;
		output_file_varied << "******************************************" << endl;
		
		output_file_varied_figure << endl;		

		std::cout << "******************************************" << endl;
		std::cout << "Best CWS_eta=" << best_CWS_eta << "\t Best CWS_r=" << best_CWS_r << "\tAP_test=" << max_CWS_Acc << endl;
		std::cout << "******************************************" << endl;
	}
	else if (par.find_CWS_eta && !par.find_CWS_r) // only to find arow_r
	{
		//output_file_varied << "CWS_eta \t CWS_r \t AP_test" << endl;
		output_file_varied << setw(10) << "CWS_eta "
			<< setw(10) << "CWS_r "
			<< setw(10) << "AP_test "
			<< setw(10) << "best_AP_test "
			<< endl;

		//std::cout << "/******************************************/" << endl;
		//std::cout << "CWS_eta \t CWS_r \t AP_test \t max_AP_test" << endl;
		std::cout << setw(10) << "CWS_eta"
			<< setw(10) << "CWS_r"
			<< setw(10) << "AP_test"
			<< setw(10) << "max_AP_test"
			<< endl;
		for (int i = 0; i < 11; i++)
		{
			par.CWS_eta = pow(2, -5 + i);
			double tmp_AP = 0.0;
			for (int k = 0; k < par.num_fold; k++)
			{
				par.ind_fold = k;
				arow->SetAlgName(alg_names[1]);
				par.Reset(data.d, data.n);
				result_tmp.Reset();
				arow->Learning(&result_tmp, &data, &par);
				arow->TestOnTestID(&result_tmp, &data, &par);
				tmp_AP += result_tmp.AP_test;
			}
			tmp_AP = (double)tmp_AP / par.num_fold;
			if (tmp_AP > max_CWS_Acc){
				max_CWS_Acc = tmp_AP;
				best_CWS_eta = par.CWS_eta;
				best_CWS_r = par.CWS_r;
			}
			//output_file_varied << par.CWS_eta << "\t" << par.CWS_r << "\t" << tmp_AP << "\t" << max_CWS_Acc << endl;
			//std::cout << par.CWS_eta << "\t" << par.CWS_r << "\t" << tmp_AP << "\t" << max_CWS_Acc << endl;
			
			output_file_varied << setw(10) << setprecision(4) << par.CWS_eta
				<< setw(10) << setprecision(4) << par.CWS_r
				<< setw(10) << setprecision(4) << tmp_AP
				<< setw(10) << setprecision(4) << max_CWS_Acc
				<< endl;
			
			output_file_varied_figure << tmp_AP << " ";

			std::cout << setw(10) << setprecision(4) << par.CWS_eta
				<< setw(10) << setprecision(4) << par.CWS_r
				<< setw(10) << setprecision(4) << tmp_AP
				<< setw(10) << setprecision(4) << max_CWS_Acc
				<< endl;
		}
		par.CWS_r = best_CWS_r;
		par.CWS_eta = best_CWS_eta;
		output_file_varied << "******************************************" << endl;
		output_file_varied << "Best CWS_eta=" << best_CWS_eta << "\t CWS_r=" << best_CWS_r << "\t AP_test=" << max_CWS_Acc << endl;
		output_file_varied << "******************************************" << endl;

		output_file_varied_figure << endl;

		std::cout << "******************************************" << endl;
		std::cout << "Best CWS_eta=" << best_CWS_eta << "\t Best CWS_r=" << best_CWS_r << "\tAP_test=" << max_CWS_Acc << endl;
		std::cout << "******************************************" << endl;
	}
	else if (!par.find_CWS_eta && par.find_CWS_r) // only to find arow_r
	{
		//output_file_varied << "CWS_eta \t CWS_r \t AP_test" << endl;
		output_file_varied << setw(10) << "CWS_eta "
			<< setw(10) << "CWS_r "
			<< setw(10) << "AP_test "
			<< setw(10) << "best_AP_test "
			<< endl;
		//std::cout << "/******************************************/" << endl;
		//std::cout << "CWS_eta \t CWS_r \t AP_test \t max_AP_test" << endl;
		std::cout << setw(10) << "CWS_eta"
			<< setw(10) << "CWS_r"
			<< setw(10) << "AP_test"
			<< setw(10) << "max_AP_test"
			<< endl;
		for (int i = 0; i < 11; i++)
		{
			par.CWS_r = pow(2, -5 + i);
			double tmp_AP = 0.0;
			for (int k = 0; k < par.num_fold; k++)
			{
				par.ind_fold = k;
				arow->SetAlgName(alg_names[1]);
				par.Reset(data.d, data.n);
				result_tmp.Reset();
				arow->Learning(&result_tmp, &data, &par);
				arow->TestOnTestID(&result_tmp, &data, &par);
				tmp_AP += result_tmp.AP_test;
			}
			tmp_AP = (double)tmp_AP / par.num_fold;
			if (tmp_AP > max_CWS_Acc){
				max_CWS_Acc = tmp_AP;
				best_CWS_eta = par.CWS_eta;
				best_CWS_r = par.CWS_r;
			}
		//	output_file_varied << par.CWS_eta << "\t" << par.CWS_r << "\t" << tmp_AP << "\t" << max_CWS_Acc << endl;
			//std::cout << par.CWS_eta << "\t" << par.CWS_r << "\t" << tmp_AP << "\t" << max_CWS_Acc << endl;

			output_file_varied << setw(10) << setprecision(4) << par.CWS_eta
				<< setw(10) << setprecision(4) << par.CWS_r
				<< setw(10) << setprecision(4) << tmp_AP
				<< setw(10) << setprecision(4) << max_CWS_Acc
				<< endl;
			
			output_file_varied_figure << tmp_AP << " ";

			std::cout << setw(10) << setprecision(4) << par.CWS_eta
				<< setw(10) << setprecision(4) << par.CWS_r
				<< setw(10) << setprecision(4) << tmp_AP
				<< setw(10) << setprecision(4) << max_CWS_Acc
				<< endl;
		}
		par.CWS_r = best_CWS_r;
		par.CWS_eta = best_CWS_eta;
		output_file_varied << "******************************************" << endl;
		output_file_varied << "Best CWS_eta=" << best_CWS_eta << "\t CWS_r=" << best_CWS_r << "\t AP_test=" << max_CWS_Acc << endl;
		output_file_varied << "******************************************" << endl;

		output_file_varied_figure << endl;

		std::cout << "******************************************" << endl;
		std::cout << "Best CWS_eta=" << best_CWS_eta << "\t Best CWS_r=" << best_CWS_r << "\tAP_test=" << max_CWS_Acc << endl;
		std::cout << "******************************************" << endl;
	}
	else{
		std::cout << "Unkown how to optimize CWS parameters" << std::endl;
	}


	if (par.find_AROMA_r){
		std::cout << "Finding best AROMA_r" << endl;
		//output_file_varied << " AROMA_r" << endl;
		output_file_varied << setw(10) << "AROMA_r "			
			<< setw(10) << "AP_test "
			<< setw(10) << "best_AP_test "
			<< endl;
		//std::cout << "/******************************************/" << endl;
		std::cout << setw(10) << "AROMA_r"
			<< setw(10) << "AP_test"
			<< setw(10) << "max_AP_test"
			<< endl;
		for (int i = 0; i < 11; i++)
		{
			double tmp_r = pow(10, -5 + i);
			par.AROMA_r = tmp_r;
			double tmp_AP = 0.0;
			for (int j = 0; j < par.num_fold; j++)
			{
				par.ind_fold = j;
				arowc->SetAlgName(alg_names[2]);
				par.Reset(data.d, data.n);
				result_tmp.Reset();
				arowc->Learning(&result_tmp, &data, &par);
				arowc->TestOnTestID(&result_tmp, &data, &par);
				tmp_AP += result_tmp.AP_test;
			}
			tmp_AP = tmp_AP / par.num_fold;
			if (tmp_AP > max_AROMA_Acc){
				max_AROMA_Acc = tmp_AP;
				best_AROMA_r = par.AROMA_r;
			}
			output_file_varied << setw(10) << setprecision(4) << par.AROMA_r
				<< setw(10) << setprecision(4) << tmp_AP
				<< setw(10) << setprecision(4) << max_AROMA_Acc
				<< endl;
			
			output_file_varied_figure << tmp_AP << " ";

			std::cout << setw(10) << setprecision(4) << par.AROMA_r
				<< setw(10) << setprecision(4) << tmp_AP
				<< setw(10) << setprecision(4) << max_AROMA_Acc
				<< endl;
		}
		par.AROMA_r = best_AROMA_r;
		output_file_varied << "******************************************" << endl;
		output_file_varied << "Best AROMA_r=" << best_AROMA_r << "\t AP_test=" << max_AROMA_Acc << endl;
		output_file_varied << "******************************************" << endl;
		
		output_file_varied_figure << endl;

		std::cout << "******************************************" << endl;
		std::cout << "Best AROMA_r=" << best_AROMA_r << "\t AP_test=" << max_AROMA_Acc << endl;
		std::cout << "******************************************" << endl;
	}
	else
	{
		std::cout << "Unkown how to optimize AROMA parameters" << std::endl;		
	}

	output_file_varied.close();
	output_file_varied_figure.close();

	delete oasis;
	delete arow;
	delete arowc;
	std::cout << "Finished!" << endl;
}