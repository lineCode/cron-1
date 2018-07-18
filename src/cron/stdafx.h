// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include <stdio.h>
#include <tchar.h>


#define NOMINMAX


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "exarray.h"
typedef ExArray<int> IntArray;

#include "singleton.h"

#include "stdstring.h"
typedef CStdStringA mstring;
typedef std::vector<mstring> StringArray;

#ifndef _HasFlag
#	define _HasFlag(l,f) ((l) & (f))
#endif

#ifndef _AddFlag
#	define _AddFlag(l,f) ((l) |= (f))
#endif

#ifndef _RemoveFlag
#	define _RemoveFlag(l,f) ((l) &= ~(f))
#endif

#include "time.h"

