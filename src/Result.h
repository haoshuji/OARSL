#ifndef RESULT_H
#define RESULT_H

/*
 * =====================================================================================
 *        Class:  CResult
 *  Description:  store the temporal result of algorithm
 * =====================================================================================
 */
class CResult
{
    public:
        double *err_n, *err_p, *t_n, *t_p, *que_num, *time_;
		double AP_test, mAP_test;
		double AP_train, mAP_train;
		double que;
		double time;
		double std_que_num, std_que;
        double std_time;
        double std_AP_test, std_mAP_test, std_AP_train, std_mAP_train;		
        int num_ticks;
        /* ====================  LIFECYCLE     ======================================= */
        CResult();
        CResult(int num_ticks);
        void Initialize(int m_num_ticks); 
        void Reset();                           /* constructor */
        ~CResult();
        
        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */
        CResult& operator=(const CResult &result);
        CResult& operator+(const CResult &result);
        CResult& operator/(const double num_fold);
    protected:
        /* ====================  METHODS       ======================================= */

        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  METHODS       ======================================= */

        /* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class CResult  ----- */

#endif
