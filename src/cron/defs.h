#pragma once

#include <list>
#include <vector>

#include "stdstring.h"
typedef CStdStr<char> mstring;
typedef std::vector<mstring> StringArray;


#ifndef INVALID_ID
#define INVALID_ID (-1)
#endif

#ifndef _HasFlag
#	define _HasFlag(l,f) ((l) & (f))
#endif

#ifndef _AddFlag
#	define _AddFlag(l,f) ((l) |= (f))
#endif

#ifndef _RemoveFlag
#	define _RemoveFlag(l,f) ((l) &= ~(f))
#endif

#ifndef __in
#define __in
#define __out
#define __inout
#endif

namespace cron {

enum class TimerExecType {
    kNotSet = 0,	// ϵͳ����

    // ���ʱ��
    kAfterSysBoot = 0x00000001,	// ϵͳ����
    kAfterTimerStart = 0x00000002,	// ��������
    kAfterAppStart = 0x00000004,// ����������
    kAfterExternalAppStart = 0x00000008,// �ⲿ��������
    kAfterAppExit = 0x00000010,// �ⲿ�����˳�

    // ����ʱ����
    kAtDate = 0x00010000,	// �������� 2011/11/11
    kAtDaily = 0x00020000,	// ÿ��x��
    kAtMonthDay = 0x00040000,	// ÿ�µ�x��
    kAtWeekday = 0x00080000,	// ÿ�µ�x�� x[������|��һ��|����|��4��]
    kAtYearDay = 0x00100000,	// ÿ���ĳ������ 2011~2020 �� [11/11]��������һ����ǰ������������������

    //////////////////////////////////////////////////////////////////////////
    kExecAndThen = 0x01000000,	// ���ʱ��֮���ٴ�ִ�ж�μ��
};

// ��������ʱ״̬
enum class TimerRunningStatus {
    kBadTimer = -2,	// ���ܽ���timer���ʽ
    kAppError = -1,	// Ӧ�ó�������˴���
    kOk = 0,	// ������������
    kNotStartup = 1,	// ����δ����
    kPaused,	// ����δ����
    kOverdue,	// ���������
    kUntilNextSysReboot,	// ��Ҫ�´λ��������������ִ��
    kUntilNextAppReboot,	// ��Ҫ���������������ִ��
    kBasedOnExternalApp,	// �������ⲿ����û������
    // ����ʱ��
    kTimeNotMatch,	// �޿�ִ�е�ʱ��ƥ��
    kNoChanceExec,	// ��Ȼ����δ���ڣ��������µ�ʱ�������û�л�����ִ����
};

}

