
////////////////////////////////////////////////////////////////////////////////
// 文件名:CommonLog.hpp
// 类  名:CLogger
// 描  述:用来管理日志的类
// 创建者:罗志彬
// 日  期:2017-12-19
////////////////////////////////////////////////////////////////////////////////

#pragma once


class CLogger
{
public:
	//构造函数
	//
	CLogger( LPCTSTR lpszFile = nullptr )
	{
		m_pFile = nullptr;
		m_strFileName = CString(lpszFile);
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
	//函数名称:openFile
	//函数说明:打开日志文件
	//返 回 值:
	//参数说明:
	//    lpszFilename:日志文件名
	//创建人员：罗志彬
	//创建日期:2017-12-19
	//备    注：
	/******************************************************************************/
	bool openFile( LPCTSTR lpszFilename = nullptr )
	{
		//获得文件名
		CString strFile(lpszFilename);
		try
		{
			
			if(strFile.IsEmpty() )
			{//如果参数的文件名是空的
				//则使用默认文件名
				strFile = CCommonString::get_app_path() + _T("CommonLog.txt");
			}
			//将文件名记录在本地
			m_strFileName = strFile;

			//以追加方式打开文件
			_tfopen_s(&(m_pFile),strFile, _T("a+"));
			if( !m_pFile )
			{//如果打开失败
				//获取错误信息
				LPVOID lpMsgBuf = NULL;
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
				//记录错误信息
				m_strError.Empty();
				if (lpMsgBuf != NULL)
				{
					m_strError = CString((LPCTSTR)lpMsgBuf);
					LocalFree(lpMsgBuf);
				}
				return false;
			}


			return true;
		}
		catch (CException* e)
		{//异常处理
			TCHAR szError[1024];
			e->GetErrorMessage(szError, 1024);
			m_strError.Format(_T("%s\n"),szError);
			e->Delete();
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
	void Append( LPCTSTR lpszContent )
	{
		if( !m_pFile )
		{//如果日志文件没有打开
			//则打开它
			openFile(m_strFileName);

			// 文件打开失败，直接返回
			if (m_pFile == NULL)
			{
				return;
			}
		}

		//将字符串写入文件
		_ftprintf(m_pFile,_T("%s\n"),lpszContent);
		//刷新缓冲区
		fflush(m_pFile);
	}

	void Append( const string& strContent )
	{
		if( !m_pFile )
		{//如果日志文件没有打开
			//则打开它
			openFile(m_strFileName);

			// 文件打开失败，直接返回
			if (m_pFile == NULL)
			{
				return;
			}
		}

		//将字符串写入文件
		_ftprintf(m_pFile,_T("%s\n"),strContent.c_str());
		//刷新缓冲区
		fflush(m_pFile);
	}

	//获得错误信息
	CString getError()
	{
		return m_strError;
	}

	//重载流操作运算符
	const void operator<<( LPCTSTR lpszContent )
	{
		Append(lpszContent);
	}

private:
	FILE* m_pFile;			//文件指针
	CString m_strError;		//错误信息
	CString m_strFileName;	//文件名
};
