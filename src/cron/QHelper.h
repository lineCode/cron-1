#pragma once

#include <list>
#include <algorithm>
#include <iterator>
#include "defs.h"
#include "crontimer.h"

namespace cron {

class QHelper {
public:
    // s/m/h
    static mstring GetTimeUnitString(WCHAR cUnit);
    static uint32_t HowManySeconds( int nX,WCHAR cUnit );

    static bool ParseInt(__in const CStdStringW& src, __out int& nT);

    static mstring MakeReleateExp( ExpTimerExecFlag eExec,
                                   int nSpan, wchar_t cUnit,   // �����eExecһ��ʱ��
                                   int nSpan1 = 0, wchar_t cUnit1 = L's', // Ȼ������ 0 Ϊ�޼��
                                   int nExecCount = 0); // �ܹ���ִ�еĴ���, 0Ϊ������

    static bool MakeAbsExp(ExpTimerExecFlag eFlag,dt::time& tmB, dt::time& tmE,
                           StringArray& arDatePots, /* ִ�����ڵ� */ StringArray& arTimePots,
                           __out mstring& sResultExp, __out mstring& sError);

    static mstring MakeRemindExp( int nA, WCHAR cAUnit, const mstring&sSound,const mstring&sMsg );

    /** 10h��10s��10m ������ʱ���ʽ����
     *	return:
     *      TRUE    ���ݸ�ʽ��ȷ
     *	params:
     *		-[in]
     *          src     ���������ַ���
     *		-[out]
     *          nT      ���ֲ���
     *          cUnit   ��λ����
    **/
    static bool ParseUnitTime(__in const CStdStringW& src, __out int &nT, __out wchar_t& cUnit);
};
}
