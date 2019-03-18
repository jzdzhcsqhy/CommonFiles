////////////////////////////////////////////////////////////////////////////////
//
// File Name:	CommonAlgorithm.hpp
// Class Name:	CCommonAlgorithm
// Description:	ͨ�û����㷨�������������ң�������
// Author:		��־��
// Date:		2018��11��9��16:11:36
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __COMMON_ALGORITHM_HPP__
#define __COMMON_ALGORITHM_HPP__

#pragma once

#include <afxstr.h>
#include <vector>
#include <string>
#include <algorithm>
#include <comutil.h>
#include <list>
#include <functional>
using namespace std;


class CCommonAlgorithm
{
public:

	// ���ģ�庯��  
	// ʹ��funcCall�Ա�β��ֽ�����һ����
	// icnt ��θ��� funcCall �ص����� ����Ϊ������ֵ
	template<typename T>
	static std::vector<int> deal_each( int iCnt, std::function<bool(int iIndex, T& item)> funcCall, ... )
	{
		// ��ʼ�����ؽṹ��
		std::vector<int> vctRs;

		// ��ʼ������б�
		va_list ap;

		// ��λ����
		va_start(ap, funcCall);

		// ����ͬ��ģ����ʹ���
		vctRs = deal_each<T>(iCnt, funcCall, ap);
		// ��������б�
		va_end(ap);
		// ���ؽ��
		return vctRs;
	}

	// ���ģ�庯��  
	// ʹ��funcCall�Ա�β��ֽ�����һ����
	// icnt ��θ��� funcCall �ص����� ����Ϊ������ֵ
	template<typename T>
	static std::vector<int> deal_each( int iCnt, std::function<bool(int iIndex, T& item)> funcCall, va_list ap )
	{

		// ��ʼ�����ؽṹ��
		std::vector<int> vctRs;
		// ��������б�
		for( int i=0; i<iCnt; i++ )
		{
			// ��ò���
			T& item = va_arg(ap, T);
			// ���ûص��������д���
			if( funcCall(i,item) )
			{// ����ɹ����ͼ���һ��0
				vctRs.push_back(0);
			}
			else
			{//  ���ʧ�ܣ�����-1��Ϊ����
				vctRs.push_back(-1);
			}
		}
		// ���ؽ��
		return vctRs;
	}

	//������dwMask �ӵ͵��� ��iIndex ��������λ ���ó�Ϊ bTrue
	static bool SetBit(DWORD* pdwMask,  int iIndex, bool bTrue)
	{
		if( bTrue )
		{// �����Ϊһ�� ��λ֮��λ��
			*pdwMask |= ( (0x00000001) << iIndex);
		}
		else
		{// �����Ϊ0�� ��λȡ��֮��λ��
			*pdwMask &= (~(0x00000001 << iIndex) );
		}
		return true;
	}



	// ����һ�����룬���ΪÿһλҪ���ó�ʲô, ���ҵ���
	_forceinline static DWORD MakeMask( int iCnt, ... )
	{
		va_list ap;
		va_start(ap,iCnt);
		DWORD dwMask = 0;

		// ��SetBit������һ�������󶨵�dwMask
		auto funcSetBit = std::bind(&CCommonAlgorithm::SetBit,&dwMask, std::tr1::placeholders::_1,std::tr1::placeholders::_2);

		// ����ͨ���㷨���д���
		CCommonAlgorithm::deal_each<bool>(iCnt, funcSetBit , ap);
		va_end(ap);
		return dwMask;
	}

	// ��������ĳһλ�Ƿ�Ϊ1
	_forceinline static bool CheckBit( DWORD dwMask, int iBit )
	{
		return !!(dwMask & (0x00000001<<iBit) );
	}
private:

};


#endif //__COMMON_STRING_HPP__
