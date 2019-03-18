
////////////////////////////////////////////////////////////////////////////////
// �ļ���:CommonLog.hpp
// ��  ��:CLogger
// ��  ��:����������־����
// ������:��־��
// ��  ��:2017-12-19
////////////////////////////////////////////////////////////////////////////////

#pragma once


class CLogger
{
public:
	//���캯��
	//
	CLogger( LPCTSTR lpszFile = nullptr )
	{
		m_pFile = nullptr;
		m_strFileName = CString(lpszFile);
	}
	//��������
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
	//��������:openFile
	//����˵��:����־�ļ�
	//�� �� ֵ:
	//����˵��:
	//    lpszFilename:��־�ļ���
	//������Ա����־��
	//��������:2017-12-19
	//��    ע��
	/******************************************************************************/
	bool openFile( LPCTSTR lpszFilename = nullptr )
	{
		//����ļ���
		CString strFile(lpszFilename);
		try
		{
			
			if(strFile.IsEmpty() )
			{//����������ļ����ǿյ�
				//��ʹ��Ĭ���ļ���
				strFile = CCommonString::get_app_path() + _T("CommonLog.txt");
			}
			//���ļ�����¼�ڱ���
			m_strFileName = strFile;

			//��׷�ӷ�ʽ���ļ�
			_tfopen_s(&(m_pFile),strFile, _T("a+"));
			if( !m_pFile )
			{//�����ʧ��
				//��ȡ������Ϣ
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
				//��¼������Ϣ
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
		{//�쳣����
			TCHAR szError[1024];
			e->GetErrorMessage(szError, 1024);
			m_strError.Format(_T("%s\n"),szError);
			e->Delete();
			return false;
		}
	}

	/******************************************************************************/
	//��������:Append
	//����˵��:����־�ļ���׷������
	//�� �� ֵ:
	//����˵��:
	//    lpszContent:��Ҫ׷�ӵ�����
	//������Ա����־��
	//��������:2017-12-19
	//��    ע��
	/******************************************************************************/
	void Append( LPCTSTR lpszContent )
	{
		if( !m_pFile )
		{//�����־�ļ�û�д�
			//�����
			openFile(m_strFileName);

			// �ļ���ʧ�ܣ�ֱ�ӷ���
			if (m_pFile == NULL)
			{
				return;
			}
		}

		//���ַ���д���ļ�
		_ftprintf(m_pFile,_T("%s\n"),lpszContent);
		//ˢ�»�����
		fflush(m_pFile);
	}

	void Append( const string& strContent )
	{
		if( !m_pFile )
		{//�����־�ļ�û�д�
			//�����
			openFile(m_strFileName);

			// �ļ���ʧ�ܣ�ֱ�ӷ���
			if (m_pFile == NULL)
			{
				return;
			}
		}

		//���ַ���д���ļ�
		_ftprintf(m_pFile,_T("%s\n"),strContent.c_str());
		//ˢ�»�����
		fflush(m_pFile);
	}

	//��ô�����Ϣ
	CString getError()
	{
		return m_strError;
	}

	//���������������
	const void operator<<( LPCTSTR lpszContent )
	{
		Append(lpszContent);
	}

private:
	FILE* m_pFile;			//�ļ�ָ��
	CString m_strError;		//������Ϣ
	CString m_strFileName;	//�ļ���
};
