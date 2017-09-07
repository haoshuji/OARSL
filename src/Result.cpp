#include "Result.h"
#include <stdio.h>

CResult::CResult()
{
    num_ticks = 0;
    err_n = NULL;
    err_p = NULL;
    t_n = NULL;
    t_p = NULL;
    que_num = NULL;
    time_ = NULL;
	AP_test = 0.0;
	mAP_test = 0.0;
	mAP_train = 0.0;
	AP_train = 0.0;
	std_AP_test = 0.0;
	std_mAP_test = 0.0;
	std_mAP_train = 0.0;
	std_AP_train = 0.0;
	que = 0.0;
	time = 0.0;
	std_time = 0.0;
	std_que = 0.0;
}

CResult::CResult(int m_num_ticks)
{
    num_ticks = m_num_ticks;
    err_n = new double[num_ticks];
    err_p = new double[num_ticks];
    t_n = new double[num_ticks];
    t_p = new double[num_ticks];
    que_num = new double[num_ticks];
    time_ = new double[num_ticks];
}

void CResult::Initialize(int m_num_ticks)
{

    if(!err_n)
    {
        num_ticks = m_num_ticks;
        err_n = new double[num_ticks];
        err_p = new double[num_ticks];
        t_n = new double[num_ticks];
        t_p = new double[num_ticks];
        que_num = new double[num_ticks];
        time_ = new double[num_ticks];
    }
     for (int i = 0; i < num_ticks; ++i)
    {
        err_n[i] = 0;
        err_p[i] = 0;
        t_n[i] = 0;
        t_p[i] = 0;
        que_num[i] = 0;
        time_[i] = 0;
    }

	 AP_test = 0.0;
	 mAP_test = 0.0;
	 mAP_train = 0.0;
	 AP_train = 0.0;
	 this->std_mAP_test = 0.0;
	 this->std_mAP_train = 0.0;
	 this->std_AP_test = 0.0;
	 this->std_AP_train = 0.0;
	 que = 0.0;
	 std_que = 0.0;
	 time = 0.0;
}

CResult& CResult::operator=(const CResult &result)
{
    if (num_ticks == 0)
    {
        Initialize(result.num_ticks);
    }

    for (int i = 0; i < num_ticks; ++i)
    {
        err_n[i] = result.err_n[i];
        err_p[i] = result.err_p[i];
        t_n[i] = result.t_n[i];
        t_p[i] = result.t_p[i];
        que_num[i] = result.que_num[i];
        time_[i] = result.time_[i];
    }
	this->AP_test = result.AP_test;
	this->mAP_test = result.mAP_test;
	this->AP_train = result.AP_train;
	this->mAP_train = result.mAP_train;
	this->que = result.que;
	this->time = result.time;

	this->std_AP_test = result.std_AP_test;
	this->std_mAP_test = result.std_mAP_test;
	this->std_AP_train = result.std_AP_train;
	this->std_mAP_train = result.std_mAP_train;
	this->std_que = result.std_que;
	this->std_time = result.std_time;

	return *this;
}

CResult& CResult::operator+(const CResult &result)
{
    if (num_ticks == 0)
    {
        Initialize(result.num_ticks);
    }

    for(int i = 0; i<num_ticks; ++i)
    {
        err_n[i] += result.err_n[i];
        err_p[i] += result.err_p[i];
        t_n[i] += result.t_n[i];
        t_p[i] += result.t_p[i];
        que_num[i] += result.que_num[i];
        time_[i] += result.time_[i];
    }
	this->AP_test += result.AP_test;
	this->mAP_test += result.mAP_test;
	this->AP_train += result.AP_train;
	this->mAP_train += result.mAP_train;
	this->que += result.que;
	this->time += result.time;

	this->std_AP_test += result.std_AP_test;
	this->std_mAP_test += result.std_mAP_test;
	this->std_AP_train += result.std_AP_train;
	this->std_mAP_train += result.std_mAP_train;
	this->std_que += result.std_que;
	this->std_time += result.std_time;
    return *this;    
}

CResult& CResult::operator/(const double num_fold)
{
    
    for(int i = 0; i<num_ticks; ++i)
    {
        err_n[i] /= num_fold;
        err_p[i] /= num_fold;
        t_n[i] /= num_fold;
        t_p[i] /= num_fold;
        que_num[i] /= num_fold;
        time_[i] /= num_fold;
    }
	this->AP_test /= num_fold;
	this->mAP_test /= num_fold;
	this->AP_train /= num_fold;
	this->mAP_train /= num_fold;
	this->que /= num_fold;
	this->time /= num_fold;

	this->std_AP_test /= num_fold;
	this->std_mAP_test /= num_fold;
	this->std_AP_train /= num_fold;
	this->std_mAP_train /= num_fold;
	this->std_que /= num_fold;
	this->std_time /= num_fold;
    return *this;    
}

void CResult::Reset()
{
    for(int t =0; t<num_ticks; t++)
    {
        err_n[t] = 0;
        err_p[t] = 0;
        t_n[t] = 0;
        t_p[t] = 0;
        que_num[t] = 0;
        time_[t] = 0.0;
    }
	this->AP_test = 0.0;
	this->mAP_test = 0.0;
	this->AP_train = 0.0;
	this->mAP_train = 0.0;
	this->std_mAP_test = 0.0; 
	this->std_mAP_train = 0.0;
	this->std_AP_test = 0.0;
	this->std_AP_train = 0.0;
	this->que = 0.0;
	this->time = 0.0;
	this->std_que = 0.0;
	this->std_time = 0.0;
}
CResult::~CResult()
{
    delete []err_n;
    delete []err_p;
    delete []t_n;
    delete []t_p;
    delete []que_num;
    delete []time_;
}
