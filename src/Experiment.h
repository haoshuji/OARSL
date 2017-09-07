#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "Result.h"
#include "Model.h"
#include "Data.h"
#include "Result.h"
#include "Parameter.h"

#include <algorithm> //max min function
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <string.h>

#include "ModelCWS.h"
#include "ModelCWSD.h"
#include "ModelOASIS.h"
#include "ModelAROMA.h"
#include "ModelAROMAD.h"
#include <sstream>


using namespace std;

template <typename T>
string NumberToString(T Number)
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}

class CExperiment
{
    public:
        CParameter par;
        CData data;
		string data_file_name;
		string trainID_file_name;
		string testID_file_name;
		string triplets_file_name;
		string data_file_location;
		string data_file_full_name;
		string setting_file_full_name;
		string output_dir;
    public:
        /* ====================  LIFECYCLE     ======================================= */                             /* constructor */

        void ParseCommandLine(int argc, char **argv);

        void ExitWithHelp();        

        //void Run_binary_experiment(int argc, char **argv);   

		void AOS(int argc, char **argv);
        void PassiveLearn();
        void ActiveLearn();
		void FindBestParameters();
        
        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  METHODS       ======================================= */

        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  METHODS       ======================================= */

        /* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class CExperiment  ----- */

#endif
