////////////////////////////////////////////////////////////////////////////////
//
// File Name:	CommonAlgorithm.hpp
// Class Name:	CCommonAlgorithm
// Description:	通用基础算法操作，包括查找，遍历等
// Author:		罗志彬
// Date:		2018年11月9日16:11:36
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

	// 变参模板函数  
	// 使用funcCall对变参部分进行逐一处理
	// icnt 变参个数 funcCall 回调函数 后续为参数真值
	template<typename T>
	static std::vector<int> deal_each( int iCnt, std::function<bool(int iIndex, T& item)> funcCall, ... )
	{
		// 初始化返回结构体
		std::vector<int> vctRs;

		// 初始化变参列表
		va_list ap;

		// 定位参数
		va_start(ap, funcCall);

		// 调用同名模板机型处理
		vctRs = deal_each<T>(iCnt, funcCall, ap);
		// 结束变参列表
		va_end(ap);
		// 返回结果
		return vctRs;
	}

	// 变参模板函数  
	// 使用funcCall对变参部分进行逐一处理
	// icnt 变参个数 funcCall 回调函数 后续为参数真值
	template<typename T>
	static std::vector<int> deal_each( int iCnt, std::function<bool(int iIndex, T& item)> funcCall, va_list ap )
	{

		// 初始化返回结构体
		std::vector<int> vctRs;
		// 遍历变参列表
		for( int i=0; i<iCnt; i++ )
		{
			// 获得参数
			T& item = va_arg(ap, T);
			// 调用回调函数进行处理
			if( funcCall(i,item) )
			{// 如果成功，就计入一个0
				vctRs.push_back(0);
			}
			else
			{//  如果失败，计入-1作为错误
				vctRs.push_back(-1);
			}
		}
		// 返回结果
		return vctRs;
	}

	//用来将dwMask 从低到高 第iIndex 个二进制位 设置成为 bTrue
	static bool SetBit(DWORD* pdwMask,  int iIndex, bool bTrue)
	{
		if( bTrue )
		{// 如果置为一， 移位之后按位或
			*pdwMask |= ( (0x00000001) << iIndex);
		}
		else
		{// 如果置为0， 移位取反之后按位与
			*pdwMask &= (~(0x00000001 << iIndex) );
		}
		return true;
	}



	// 构造一个掩码，变参为每一位要设置成什么, 从右到左
	_forceinline static DWORD MakeMask( int iCnt, ... )
	{
		va_list ap;
		va_start(ap,iCnt);
		DWORD dwMask = 0;

		// 将SetBit函数第一个参数绑定到dwMask
		auto funcSetBit = std::bind(&CCommonAlgorithm::SetBit,&dwMask, std::tr1::placeholders::_1,std::tr1::placeholders::_2);

		// 调用通用算法进行处理
		CCommonAlgorithm::deal_each<bool>(iCnt, funcSetBit , ap);
		va_end(ap);
		return dwMask;
	}

	// 检测掩码的某一位是否为1
	_forceinline static bool CheckBit( DWORD dwMask, int iBit )
	{
		return !!(dwMask & (0x00000001<<iBit) );
	}
private:

};


#endif //__COMMON_STRING_HPP__
