//********************************************************************
//* 文件名称： ViteRegister.hpp
//* 文件功能： vite注册用的功能类
//* 创 建 人： 罗志彬
//* 创建时间： 2019年1月28日
//********************************************************************
#ifndef CTIME_OUT_HPP
#define CTIME_OUT_HPP
#include <QDateTime>
#include <functional>
#include <QThread>
class CTimeout
{
public:
    CTimeout( long long llSecond =0, int llMSecond =0 )
    {
        m_llBegin = QDateTime::currentDateTime().toMSecsSinceEpoch();
        m_llTimeout = llSecond * 1000 + llMSecond;
    }

    bool isTimeout()
    {
        if( !m_llTimeout )
        {// 如果没设置超时时间，就一直卡住
            return false;
        }
        long long llNow = QDateTime::currentDateTime().toMSecsSinceEpoch();
        return llNow > ( m_llTimeout + m_llBegin );
    }

    long long secondSinceBegin()
    {

        return milisecondSinceBegin()/1000 ;
    }
    long long milisecondSinceBegin()
    {
        long long llNow = QDateTime::currentDateTime().toMSecsSinceEpoch();
        return llNow - m_llBegin;
    }

public:
    long long m_llTimeout;
    long long m_llBegin;
};

template< int timeout = 0 >
class CWaitCondition
{
public:
    bool operator()(std::function<bool ()> funcCondition)
    {

        CTimeout tmot (timeout );
        while( !funcCondition()  && !tmot.isTimeout() )
        {// 条件谓词不满足，并且线程没结束，
            QThread::msleep(1);
            qApp->processEvents();
        }
        // 返回值是是否超时，方便区分是条件满足退出的，还是超时退出的
        return !tmot.isTimeout();
    }
};

template <int timeout = 0>
class COnlyWait : public CWaitCondition<timeout>
{
public:
    bool operator()()
    {
        return CWaitCondition<timeout>::operator()(
                    []()->bool{return false;});
    }
};

using CWait30S = CWaitCondition<30>;
using CWait10S = CWaitCondition<10>;
using CWaitForever = CWaitCondition<0>;
using CWaitNone = CWaitCondition<-1>;


#endif
