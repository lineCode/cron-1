#pragma once

#include <iostream>
#include <vector>

#include "console.h"
#include "../cron/qhelper.h"

using namespace std;

namespace con = JadedHoboConsole;

class QArgvParser {
    typedef vector<mstring> StrArray;
    typedef StrArray::iterator StrArrayItr;
    struct {

    };

public:
    // ��һ����Set
    BOOL SetArgv(_TCHAR* argv[], int nArgc) {
        if (nArgc < 3) {
            m_arErrors.push_back("����̫�٣�");
            return FALSE;
        }

        m_args.clear();

        // �ӵڶ����������м�¼
        mstring sArgv;
        for (int i = 1; i < nArgc; i++) {
            sArgv = argv[i];
            sArgv.Trim();
            if (!sArgv.IsEmpty()) {
                m_args.push_back(mstring(argv[i]).Trim());
            }
        }

        if (m_args.size() < 2) {
            m_arErrors.push_back("����̫�٣�");
            return FALSE;
        }
        m_arErrors.clear();
        return TRUE;
    }

    // �ڶ���������
    BOOL ParseArgs(__out ENUM_AUTOTASK_DOWHAT& eDoWhat, __out mstring& sDoWhatParam,
                   __out mstring& sWhenDo, __out mstring& sRemindExp,
                   __out dt::time& tmBegin, __out dt::time& tmEnd) {
        BOOL bOK = FALSE;
        do {
            // ��ʲô
            eDoWhat = GetDoWhat(sDoWhatParam);
            if (AUTOTASK_DO_NOTSET == eDoWhat)
                break;

            // ��ʱ��
            if (!GetExecTime(sWhenDo, tmBegin, tmEnd))
                break;

            // ����������ʾ
            if (!GetTaskRemind(sRemindExp))
                break;

            bOK = TRUE;
        } while (false);

        return bOK;
    }

    /** �ڿ���̨�������
     *	params: none
    **/
    void EchoErrors() {
        std::wcout<<con::bg_black<<con::fg_red;
        for (auto i = m_arErrors.begin(); i != m_arErrors.end(); ++i) {
            std::wcout<<*i<<std::endl;
        }
        std::wcout<<con::fg_white;
    }

#ifdef _DEBUG
    void TestString(const CStdString& src) {
        StrArray ar;
        SplitString(src, ar);

        std::wcout<<src<<" :  size: "<<ar.size()<<"\n------------"<<endl;
        for (auto i = ar.begin(); i != ar.end(); ++i) {
            std::wcout<<*i<<std::endl;
        }
        std::wcout<<"-----------------"<<endl;
    }

    // ���Դ���
    void TestSplitString() {
        TestString("a;b;c");

        TestString("a;;c");

        TestString("a;b;");

        TestString(";b;c");

        TestString(";b;");

        TestString(";;");
    }
#endif

protected:
    /** ��ȡִ������
     *	return:
     *      AUTOTASK_DO_NOTE    ��Ч�����help���
    **/
    ENUM_AUTOTASK_DOWHAT GetDoWhat(__out mstring& sDoWhat) {
        ASSERT(m_args.size() >= 2);

        ENUM_AUTOTASK_DOWHAT eRet = AUTOTASK_DO_NOTSET;

        StrArray::iterator iB = m_args.begin();
        mstring s1 = *iB;
        m_args.erase(iB);

        BOOL bNeedParam = FALSE;
        if (s1.CompareNoCase("-m") == 0) {
            bNeedParam = TRUE;
            eRet = AUTOTASK_DO_REMIND; // remind message
        } else if (s1.CompareNoCase("-e") == 0) {
            bNeedParam = TRUE;
            eRet = AUTOTASK_DO_EXECPROG;
        } else if (s1.CompareNoCase("-s") == 0) {
            eRet = AUTOTASK_DO_SYSSHUTDOWN;
        } else if (s1.CompareNoCase("-b") == 0) {
            eRet = AUTOTASK_DO_BREAKAMOMENT;
        }

        if (AUTOTASK_DO_NOTSET  == eRet) {
            m_arErrors.push_back("��Ч������ִ�����ͣ�");
        } else if (bNeedParam) {
            iB = m_args.begin();
            mstring s2 = *iB;
            m_args.erase(iB);

            if (!IsCommand(s2)) {
                sDoWhat = s2;
            } else {
                mstring sTmp;
                sTmp.Format("���� %s ȱ�ٲ���", s1);
                m_arErrors.push_back(sTmp);
                eRet = AUTOTASK_DO_NOTSET;
            }
        }

        return eRet;
    }

    /** ������ʾ
     *	return:
     *      TRUE    �㶨
     *	params:
     *		-[out]
     *      sRemind
    **/
    BOOL GetTaskRemind(__out mstring& sRemind) {
        // -w
        StrArrayItr i = FindX("-w");
        if (_End() == i) {
            return TRUE;
        }

        // ��һ�����ǲ���
        ++i;
        if (_End() == i) {
            m_arErrors.push_back("-w   ������ʾ    δָ������");
            return FALSE;
        }

        StrArray arP;
        if (SplitString(*i, arP) != 3) {
            m_arErrors.push_back("-w   ������ʾ    ������������" + *i);
            return FALSE;
        }

        // ʱ��
        int nT;
        char unit;
        if (!helper::parseUnitTime(arP[0], nT, unit)) {
            m_arErrors.push_back("-w   ������ʾ    ��ʾʱ�����" + *i);
            return FALSE;
        }
        // ��ʾʱ�䲻�ܹ���
        DWORD nSecs = helper::howManySecs(nT, unit);
        if ((nSecs > 86400) || (nSecs < 5)) {
            m_arErrors.push_back("-w   ������ʾ    ��ʾʱ�䷶Χ[10s, 24h)��" + *i);
            return FALSE;
        }

        sRemind = helper::makeRemindExp(nT, unit, arP[1], arP[2]);
        return !sRemind.IsEmpty();
    }

    /** �����ִ��ʱ��
     *	return:
     *      TRUE    �����ɹ�
     *	params:
     *		-[out]
     *      sTime   ��������ı��ʽ���ܹ�ֱ�ӱ�QTimerʹ��
    **/
    BOOL GetExecTime(__out mstring& sTime, __out dt::time& tmBegin, __out dt::time& tmEnd) {
        if ( !GetLifePeriod(tmBegin, tmEnd) ) {
            return FALSE;
        }

        // ������ʱ��
        StrArrayItr iR = FindX("-r");
        StrArrayItr iA = FindX("-a");
        if ((_End() != iR) && (_End() != iA)) {
            m_arErrors.push_back("-r/-a    ִ��ʱ��    ֻ��ָ��һ��ִ��ʱ��");
            return FALSE;
        }

        if ( (_End() == iA) && (_End() == iR)) {
            m_arErrors.push_back("-r/-a    δ�趨ִ��ʱ��");
            return FALSE;
        }

        if (_End() != iA) {
            // ����ʱ��
            // ��һ�����ǲ���
            ++iA;
            if (_End() == iA) {
                m_arErrors.push_back("-a   ִ��ʱ�䣨����ʱ�䣩    δָ������");
                return FALSE;
            }
            return ParseAbsTime(*iA, tmBegin, tmEnd,sTime);
        }

        if (_End() != iR) {
            // ��һ�����ǲ���
            ++iR;
            if (_End() == iR) {
                m_arErrors.push_back("-r   ִ��ʱ�䣨���ʱ�䣩    δָ������");
                return FALSE;
            }
            return ParseRelateTime(*iR, sTime);
        }

        return FALSE;
    }

    /** �ж��Ƿ��������Ӣ��'-'��ͷ����Command
     *	return:
     *      TRUE    ��
     *	params:
     *		-[in]
     *          sText
    **/
    inline BOOL IsCommand(__in const CStdString& sText) {
        ASSERT(sText.size() > 1);
        return (sText[0] == '-');
    }

    /** ������Ч��
     *	return:
     *      TRUE    �ɹ�
    **/
    BOOL GetLifePeriod(__out dt::time& tmBegin, __out dt::time& tmEnd) {
        // -l
        StrArrayItr i = FindX("-");
        if (_End() == i) {
            tmBegin = dt::now();
            // ִ����Ч�ڣ�10�꣡�����˰ɡ�
            tmEnd = tmBegin + date::years(10);
            return TRUE;
        }

        // ��һ�����ǲ���
        ++i;
        if (_End() == i) {
            m_arErrors.push_back("-l   ������Ч��    δָ������");
            return FALSE;
        }
        StrArray arP;
        if (SplitString(*i, arP) != 2) {
            m_arErrors.push_back("-l   ������Ч��   ��������" + *i);
            return FALSE;
        }

        if (arP[0].IsEmpty()) {
            tmBegin = dt::now();
        } else if (!dt::parse(tmBegin, arP[0])) {
            m_arErrors.push_back("-l   ������Ч��   ��ʼʱ�����" + *i);
            return FALSE;
        }

#ifdef _DEBUG
        std::cout<<"��ʼʱ��:"<<dt::format_dt(tmBegin);
#endif

        if (arP[1].IsEmpty()) {
            tmBegin += date::years(10);
        } else if (!dt::parse(tmEnd, arP[1])) {
            m_arErrors.push_back("-l   ������Ч��   ����ʱ�����" + *i);
            return FALSE;
        }

#ifdef _DEBUG
        std::cout<<"��ʼʱ��:"<< dt::format_dt(tmEnd);
#endif

        if (tmBegin >= tmEnd) {
            m_arErrors.push_back("-l   ������Ч��   ��ʼʱ����ڽ���ʱ�䣺" + *i);
            return FALSE;
        }
        return TRUE;
    }

    BOOL ParseAbsTime(__in const mstring& src, __inout dt::time& tmBegin,
                      __inout dt::time& tmEnd, __out mstring& sTime) {
        // abs time
        ASSERT(tmEnd > tmBegin);

        StrArray arP;
        if (SplitString(src, arP) != 2) {
            m_arErrors.push_back("-a   ִ��ʱ�䣨����ʱ�䣩    ��������" + src);
            return FALSE;
        }

        // ���ڵ㣻ʱ���
        StrArray aS;
        if (SplitString(arP[0], aS, ':') != 2) {
            m_arErrors.push_back("-a   ִ��ʱ�䣨����ʱ�䣩    ��������: " + arP[0]);
            return FALSE;
        }

        ExpTimerExecFlag eFlag;
        // aS[0] ֻ����3�ֿ��ܣ�p,s,t
        if (aS[0].CompareNoCase("d") == 0) {
            // littlet ������
            eFlag = AUTOTASK_EXEC_ATDATE;
        } else if (aS[0].CompareNoCase("s") == 0) {
            eFlag = AUTOTASK_EXEC_ATDAILY;
        } else if (aS[0].CompareNoCase("w") == 0) {
            eFlag = AUTOTASK_EXEC_ATWEEKDAY;
        } else if (aS[0].CompareNoCase("m") == 0) {
            eFlag = AUTOTASK_EXEC_ATMONTHDAY;
        } else {
            m_arErrors.push_back("-a   ִ��ʱ�䣨����ʱ�䣩    ��Ч��������" + src);
            return FALSE;
        }
        StrArray arDatePots;
        SplitString(aS[1], arDatePots, ',');

        StrArray arTimePots;
        if (SplitString(arP[1], arTimePots, ',') <= 0) {
            m_arErrors.push_back("-a   ִ��ʱ�䣨����ʱ�䣩    ִ��ʱ�����Ч��" + src);
            return FALSE;
        }
        mstring sError;
        if ( !helper::makeAbsTimerExp(eFlag, tmBegin, tmEnd, arDatePots, arTimePots, sTime, sError )) {
            m_arErrors.push_back(sError);
            return FALSE;
        }
        return TRUE;
    }

    BOOL ParseRelateTime(__in const mstring& src, __out mstring& sTime) {
        StrArray arP;
        if (SplitString(src, arP) != 3) {
            m_arErrors.push_back("-r   ִ��ʱ�䣨���ʱ�䣩    ��������" + src);
            return FALSE;
        }

        // ����ڣ������ִ�д���
        StrArray aS;
        if (SplitString(arP[0], aS, ':') != 2) {
            m_arErrors.push_back("-r   ִ��ʱ�䣨���ʱ�䣩    ��������: " + src);
            return FALSE;
        }

        ExpTimerExecFlag eFlag;
        // aS[0] ֻ����3�ֿ��ܣ�p,s,t
        if (aS[0].CompareNoCase("p") == 0) {
            // littlet ������
            eFlag = AUTOTASK_EXEC_AFTERMINDERSTART;
        } else if (aS[0].CompareNoCase("s") == 0) {
            eFlag = AUTOTASK_EXEC_AFTERSYSBOOT;
        } else if (aS[0].CompareNoCase("t") == 0) {
            eFlag = AUTOTASK_EXEC_AFTERTASKSTART;
        } else {
            m_arErrors.push_back("-r   ִ��ʱ�䣨���ʱ�䣩    ��������" + src);
            return FALSE;
        }
        int nSpan = 0;
        char unit = 's';
        if (!helper::parseUnitTime(aS[1], nSpan, unit)) {
            m_arErrors.push_back("-r   ִ��ʱ�䣨���ʱ�䣩    ��������" + src);
            return FALSE;
        }

        // �����ִ�д���
        int nSpan1 = 0, nExecCount = 0;
        char cUnit1 = 's';
        if (!arP[1].IsEmpty()) {
            if (!helper::parseUnitTime(arP[1], nSpan1, cUnit1)) {
                m_arErrors.push_back("-r   ִ��ʱ�䣨���ʱ�䣩    ��������" + src);
                return FALSE;
            }
            if (!arP[2].IsEmpty()) {
                if (!helper::parseInt(arP[2], nExecCount)) {
                    m_arErrors.push_back("-r   ִ��ʱ�䣨���ʱ�䣩    ��������" + src);
                    return FALSE;
                }
            }
        }
        sTime = helper::makeRelateTimerExp(eFlag, nSpan, unit, nSpan1, cUnit1, nExecCount);
        return TRUE;
    }

    inline StrArrayItr _End() {
        return m_args.end();
    }

    StrArrayItr FindX(const mstring& sP ) {
        StrArrayItr iEnd = _End();
        for (StrArrayItr i = m_args.begin(); i != iEnd; ++i) {
            if (sP == *i) {
                return i;
            }
        }
        return iEnd;
    }

    /** �ָ��ַ�����������
     *	return:
     *      �ָ���ַ����������ָ�ĸ�����chSep�ĸ�������
     *      �ָ�ĸ���Ϊ COUNT(chSep) + 1
     *	params:
     *		-[in]
     *          src     Դ�ַ���
     *          chSep   �ָ����
     *		-[out]
     *          result  ���
    **/
    int SplitString(__in const mstring& src, __out StrArray& result,
                    __in const char chSep = ';') {
        result.clear();

        mstring tmp = src;
        if (tmp.Trim().IsEmpty()) {
            return 0;
        }
        // ���һ���� �ָ����Ļ�������������յ��ַ���
        BOOL bPushEmpty = (tmp.back() == chSep);
        do {
            int i = tmp.Find(chSep);
            if (-1 == i) {
                result.push_back(tmp);
                break;
            }
            result.push_back(tmp.Left(i));
            tmp = tmp.Mid(i + 1);
        } while ( !tmp.IsEmpty() );

        if (bPushEmpty) {
            result.push_back(mstring());
        }
        return result.size();
    }

private:
    StrArray    m_args;
    StrArray    m_arErrors;
};

