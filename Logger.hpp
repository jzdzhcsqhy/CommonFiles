
////////////////////////////////////////////////////////////////////////////////
// 文件名:Logger.hpp
// 类  名:CLogger
// 描  述:用来管理日志的类
// 创建者:罗志彬
// 日  期:2017-12-19
////////////////////////////////////////////////////////////////////////////////
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "CommonString.hpp"
#include <QDateTime>
#include <QThread>
#include <QMutex>
class CLogger
{
public:
	//构造函数
	//
    CLogger( const char* pName = nullptr )
	{
		m_pFile = nullptr;
        if( pName )
        {
            m_strFileName = std::string(pName);
        }

	}
	//析构函数
	//
	~CLogger()
	{
		if(m_pFile )
		{
			fclose(m_pFile);
			m_pFile = nullptr;
		}
	}

    /******************************************************************************/
    //函数名称:SetLastError
    //函数说明:打开日志文件
    //返 回 值:
    //参数说明:
    //    eno:错误码
    //创建人员：罗志彬
    //创建日期:2017-12-19
    //备    注：
    /******************************************************************************/
    std::string GetErrorInfo(  errno_t eno )
    {
        const char* pErrorInfo = strerror(eno);
        if( pErrorInfo )
        {
            std::string strRs  = pErrorInfo;
            return strRs;
        }

        return "";
    }

	/******************************************************************************/
	//函数名称:openFile
	//函数说明:打开日志文件
	//返 回 值:
	//参数说明:
	//    lpszFilename:日志文件名
	//创建人员：罗志彬
	//创建日期:2017-12-19
	//备    注：
	/******************************************************************************/
    bool openFile( std::string strFileName )
    {
        return openFile(strFileName.c_str());
    }

    bool openFile( const char* lpszFilename = nullptr )
	{
		//获得文件名
        std::string strFile(lpszFilename);
		try
		{

            if(m_pFile )
            {// 如果已经有打开的了，就关闭一下
                fclose(m_pFile);
                m_pFile = nullptr;
            }

            if(strFile.empty() )
			{//如果参数的文件名是空的
				//则使用默认文件名
                strFile = ("autolog.txt");
			}
			//将文件名记录在本地
			m_strFileName = strFile;

			//以追加方式打开文件
            //_tfopen_s(&(m_pFile),strFile,("a+");
            auto pError = fopen_s(&m_pFile,m_strFileName.c_str(),"a+" );

			if( !m_pFile )
			{//如果打开失败

                m_strError = GetErrorInfo(pError);
				return false;
			}


			return true;
		}
        catch (std::exception& e)
		{//异常处理
            const char* pException = e.what();
            if( pException )
            {
                m_strError = pException;
            }
			return false;
		}
	}

	/******************************************************************************/
	//函数名称:Append
	//函数说明:向日志文件中追加内容
	//返 回 值:
	//参数说明:
	//    lpszContent:需要追加的内容
	//创建人员：罗志彬
	//创建日期:2017-12-19
	//备    注：
	/******************************************************************************/
    void Append( const char* lpszContent )
	{
		if( !m_pFile )
		{//如果日志文件没有打开
			//则打开它
            openFile(m_strFileName);

			// 文件打开失败，直接返回
            if (m_pFile == nullptr)
			{
                return ;
			}
		}


		//将字符串写入文件
        fprintf_s(m_pFile,"%s\n",lpszContent);
        //_ftprintf(m_pFile,_T("%s\n"),lpszContent);
		//刷新缓冲区
		fflush(m_pFile);
	}


    void Append( const std::string& strContent )
	{
		if( !m_pFile )
		{//如果日志文件没有打开
			//则打开它
			openFile(m_strFileName);

			// 文件打开失败，直接返回
            if (m_pFile == nullptr)
			{
                return ;
			}
		}

        //将字符串写入文件
        fprintf_s(m_pFile,"%s\n",strContent.c_str());
		//刷新缓冲区
		fflush(m_pFile);
	}

    static std::string GetCurrentTime()
    {
//        time_t now = time(0);
//        tm *ltm = localtime(&now);


////        return CCommonString::Format("%04d-%02d-%02d %02d:%02d:%02d",
////                              ltm->tm_year,
////                              ltm->tm_mon+1,
////                              ltm->tm_mday,
////                              ltm->tm_hour,
////                              ltm->tm_min,
////                              ltm->tm_sec);
        return QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh.mm.ss.zzz").toStdString();
    }

	//获得错误信息
    std::string getError()
	{
		return m_strError;
	}

	//重载流操作运算符
    void operator<<( const char* lpszContent )
	{
		Append(lpszContent);
	}

    void operator<<( const std::string strContent )
    {
        Append(strContent);
    }

private:
	FILE* m_pFile;			//文件指针
    std::string m_strError;
    std::string m_strFileName;
};

#define LOG_TIME_T( content )
/*#define LOG_TIME_T( content ) \
{\
    std::string strContent = CCommonString::Format("Thread:%p##Pid:%d##%s %d\n%s",QThread::currentThread(),getpid(),__FILE__, __LINE__,std::string(content).c_str());\
    LOG_TIME( strContent )\
}*/

#define LOG_ENGINE_T( content ) \
{\
    std::string strContent = CCommonString::Format("Thread:%p##Pid:%d##%s %d\n%s",QThread::currentThread(),getpid(),__FILE__, __LINE__,std::string(content).c_str());\
    LOG_TIME( strContent )\
}

#define LOG_TIME( content ) \
{\
    CLogger logger; \
    logger.Append( CLogger::GetCurrentTime() + ":"+ content);\
}

#define  LOG(format, ...) \
{\
    CLogger logger; \
    logger.Append( CLogger::GetCurrentTime() + ":"+ CCommonString::Format(format,__VA_ARGS__));\
}


#endif
