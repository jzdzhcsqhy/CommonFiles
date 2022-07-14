#ifndef OWN_BIT_SET_HPP__
#define OWN_BIT_SET_HPP__
#include <math.h>
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <vector>
using namespace  std;
// 自己写了一个bitset的类，就简单实现功能就行了，
class COwnBitset
{
private:
	// 数据类型，使用uint
	typedef unsigned int DataType;
public:

	// 构造函数，ivol 容量，代表存多少位
	COwnBitset(int iVol = 512):m_iBits(iVol),m_bIsInitialized(false)
	{
		// 计算每一个数据段有多少二进制位
		m_iEachInt = sizeof(unsigned int)*8;
		// 计算一共需要多少段
		int iBlock = ceil(1.0 * m_iBits/m_iEachInt );

		// 向数据区域添加数据，初始化为0
        for( int i=0; i< iBlock; i++ )
		{
			m_vctData.push_back(0);
		}
	}

	// 设置二进制位，将第iPos位的值设置为 bCheck
	virtual void SetBit(int iPos, bool bCheck )
	{
		if( iPos < 0 || iPos >= m_iBits)
		{
			return ;
		}
		// 计算所在的段
		int iBlock = iPos/m_iEachInt;
		// 计算所在的位
		int iBit = iPos % m_iEachInt;
		// 获得该段的值
		DataType uiValue = m_vctData[iBlock];

		// 构造掩码
		DataType bMask = ( 0x01 << iBit );

		// 将对应二进制位设置为指定的值
		if( bCheck )
		{
			uiValue |= bMask;
		}
		else
		{
			uiValue &=~(bMask);
		}
		// 更新数据存储
		m_vctData[iBlock] = uiValue;
	}

	// 检查 ipos对应的二进制位的值
	virtual bool CheckBit( int iPos )
	{
		if( iPos < 0 || iPos >= m_iBits)
		{
			return false;
		}
		// 计算所在的段
		int iBlock = iPos/m_iEachInt;
		// 计算所在的位
		int iBit = iPos % m_iEachInt;
		// 获得该段的值
		DataType uiValue = m_vctData[iBlock];

		// 构造掩码
		DataType bMask = ( 0x01 << iBit );

		return !! (bMask & uiValue );
	}


	// 计算二进制位数量，
	// b 为真时，计算1的数量，b为假是，计算0的数量
	virtual int Count( bool b ) 
	{
		int iTrueCount = 0;
		// 计算所有数据中二进制1的位置
		for( size_t i=0; i< m_vctData.size(); i ++ )
		{
			iTrueCount += GetTrueCount(m_vctData[i]);
		}

		if( b )
		{//如果为真，返回结果
			return iTrueCount;
		}
		else
		{// 如果为假，返回0的数量
			return m_iBits - iTrueCount;
		}
	}

public:
	// 重新设置大小
	void Resize( int iSize )
	{
		m_vctData.clear();
        m_iBits = iSize;

        // 计算每一个数据段有多少二进制位
		m_iEachInt = sizeof(unsigned int)*8;
		// 计算一共需要多少段
		int iBlock = ceil(1.0 * m_iBits/m_iEachInt );

		// 向数据区域添加数据，初始化为0
		for( int i=0; i< iBlock; i++ )
		{
			m_vctData.push_back(0);
		}
	}

	// 重置所有二进制位的值
	void Reset( bool bCheck )
	{
		// 初始化为0
		DataType dataValue = 0;
		// 
		if( bCheck )
		{// -1 == 0xFFFFFFFF
			dataValue = -1;
		}
		for( size_t i=0; i< m_vctData.size(); i++ )
		{
			m_vctData[i] = dataValue;		
		}
	}

	// 序列化
	std::string to_string()
	{
		std::string strRs = "";
		// 遍历所有的段
		for( size_t i=0; i< m_vctData.size(); i++ )
		{
			// 获取数据段引用
			DataType& dt = m_vctData[i];
			
			// 将数据段格式化为16进制字符标识
			std::string strFmt = string_format("%%0%dx", sizeof(DataType)*2);
			// 将该数据段追加到结果
			strRs += string_format(strFmt.c_str(), dt);
		}
		// 返回结果
		return strRs;
	}

	// 从字符串反序列化
	bool Initialize( std::string strCode )
	{
		// 重置所有数据位为0
		Reset(false);
		
		// 计算每段中的字符数。
		int iCnt = sizeof(DataType)*2;

		std::string strTmp = "";
		size_t szIndex = 0;
		// 遍历开始
		for( size_t i=0; i<strCode.length(); i++ )
		{
			// 获取字符
			strTmp += strCode[i];
            if( strTmp.length() == static_cast<size_t>(iCnt) )
			{// 当临时字符串长度是每段字符数时，开始读取数据

				// 使用字符串初始化数据段
				DataType dt = FromString(strTmp);
				strTmp = "";
				// 将数据段存储到对应索引位置
				m_vctData[szIndex++] = dt;
			}
		}

		// 设置初始化标志位
		m_bIsInitialized = true;
		return true;
	}
	
	// 判断一个bitset是否已经初始化过
	bool IsInitialized()
	{
		return m_bIsInitialized;
	}

private:

	// 从16进制字符串中解析数据
	DataType FromString(std::string strIn )
	{
		DataType dt = 0;

		for( size_t i=0; i< strIn.length() && i<2*sizeof(DataType); i++ )
		{
			char ch = strIn[i];

			if( ch >= '0' && ch <='9')
			{
				ch = ch - '0';	
			}
			else if( ch >='A' && ch <='F' )
			{
				ch = ch - 'A' +10;
			}
			else if( ch >='a' && ch <='f' )
			{
				ch = ch - 'a' +10;
			}
			else
			{
				return 0;
			}

			// 计算移位位置
			int iShift = ( sizeof(DataType) *2 - i -1 ) *4 ;
			
			// 将一个字符的数据移位到对应位置
			DataType tmp = ch;
			tmp = tmp << iShift;
			dt |= tmp;
		}
		return dt;
	}

	// 格式化字符串
	string string_format(const char * fmt, ...)
	{  
		try
		{
            std::string sr;
            va_list args0,args1;
            va_start(args0,fmt);
            va_copy(args1,args0);
            size_t num_of_chars = static_cast<size_t>(std::vsnprintf(nullptr,0,fmt,args0));
            va_end(args0);

            sr.resize(num_of_chars+1,'\0');
            std::vsnprintf(const_cast<char*>(sr.data()),sr.size(),fmt,args1);
            sr.resize(num_of_chars);

            va_end(args1);
            return sr;
		}
		catch (...)
		{
			return "";	
		}
	}

	// 获取一个数据段中二进制位为1的个数
	int GetTrueCount( DataType d )
	{
		
		int iCnt = 0;
		while( d )
		{
			// 这个操作能够将最后一个为1的二进制位变为0
			// 10001100 -1 =10001011
			// 10001100 & 10001011 = 10001000, 最后一个二进制位为1的，变成0了，
			// 如此循环，直到d为0，就能直到里面有多少个1
			d &=(d-1);
			iCnt ++;
		}
		return iCnt;
	}

	// 每一个数据段有多少位
	int m_iEachInt;
	// 容量一共有多少位
	int m_iBits;
	// 存储真实数据的
	std::vector<unsigned int> m_vctData;
	// 标识是否初始化过
	bool m_bIsInitialized;
};

#endif
