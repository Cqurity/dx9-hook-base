#pragma once
#include "pch.h"

#ifndef _UTIL_HPP_
#define _UTIL_HPP_

namespace Util
{
	inline bool Mask(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return false;
		return (*szMask) == NULL;
	}

	inline DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, const char* szMask)
	{
		for (DWORD i = 0; i < dwLen; i++)
			if (Mask((BYTE*)(dwAddress + i), bMask, szMask))
				return (DWORD)(dwAddress + i);
		return 0;
	}
}

#endif
