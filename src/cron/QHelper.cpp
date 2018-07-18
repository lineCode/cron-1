
#include "qhelper.h"

namespace cron {

uint32_t QHelper::HowManySeconds( int nX,WCHAR cUnit ) {
    switch (cUnit) {
    case L's':
    case L'S':
        return nX;
    case L'm':
    case L'M':
        return nX * 60;
    case L'h':
    case L'H':
        return nX * 3600;
    default:
        ASSERT(false);
        return 0;
    }
}

mstring QHelper::GetTimeUnitString(WCHAR cUnit) {
    switch (cUnit) {
    case L's':
    case L'S':
        return "��";
    case L'm':
    case L'M':
        return "��";
    case L'h':
    case L'H':
        return "ʱ";
    default:
        ASSERT(false);
        return "��Ч��λ";
    }
}

mstring QHelper::MakeReleateExp( ExpTimerExecFlag exec,
                                 int span, wchar_t unit, /* �����eExec?��ʱ�� */
                                 int span1 /*= 0*/, wchar_t unit1 /*= L's'*/, /* ?������ 0 Ϊ�޼�� */
                                 int exec_count /*= 0*/ ) {
    mstring exp;
    // R=1;P=20m;Q=22m;C=100;
    // R
    exp.Format("R=%d;", exec);

    //P - span1
    exp.AppendFormat("P=%d%c;", span, unit);

    // Q - span2
    if (span1 > 0) {
        // span2
        exp.AppendFormat("Q=%d%c;",span1, unit1);
        // C - exec count
        if (exec_count > 0) {
            exp.AppendFormat("C=%d;",exec_count);
        }
    }
    return exp;
}

bool QHelper::ParseInt(__in const CStdStringW& src, __out int& nT) {
    nT = std::stoi(src);
    return true;
}

bool QHelper::MakeAbsExp(ExpTimerExecFlag flag,dt::time& tmB, dt::time& tmE,
                         StringArray& arDatePots, StringArray& arTimePots,
                         __out mstring& sResultExp, __out mstring& sError) {
    if (arTimePots.empty() || arDatePots.empty()) {
        ASSERT(false);
        sError = "��������ָ��һ�����ں�һ��ʱ��";
        return false;
    }

    // �Ȼ�ȡʱ���
    mstring sTimePots;
    std::vector<uint32_t> vExist;
    for (unsigned int i = 0; i < arTimePots.size(); ++i) {
        dt::time tTime;
        if (!dt::parse(tTime, arTimePots[i], dt::ParseFlag::kFlagTime)) {
            sError = "ִ��ʱ������";
            return false;
        }
        uint32_t dwT = dt::make_uint_time(tTime);
        if (std::find(vExist.begin(), vExist.end(), dwT) == vExist.end()) {
            vExist.push_back(dwT);
            sTimePots.AppendFormat("%u,", dwT);
        }
    }
    // ȥ�����һ���ַ�','
    sTimePots.TrimRight(L',');

    // ���ڲ���
    if (AUTOTASK_EXEC_ATDATE == flag) {
        // �������ں�ʱ���ִ��
        dt::time tmNow = dt::now();
        dt::time tDate;
        dt::parse(tDate, arDatePots[0]);
        if (dt::compare_date(tDate, tmNow) < 0) {
            sError = "ִ�������Ѿ���ȥ��";
            return false;
        }
        // �Զ����������������Ϊ���ʵ�ʱ��
        if ((tmB >= tmE) || (tmE <= tmNow) || (tmB <= tmNow)
                || (tmB > tDate) || (tmE < tDate)) {
            tmB = tmNow;
            tmE = tDate + date::days(1);
        }
        sResultExp.Format("A=%d;X=%u;T=%s;",AUTOTASK_EXEC_ATDATE, dt::make_uint_date(tDate), sTimePots);
        return true;
    } else if (AUTOTASK_EXEC_ATYEARDAY == flag) {
        // ������һ��������ǰ��
        if (arDatePots.size() != 2) {
            sError = "����ִ��     ����ֻ����2����[0]:ִ�����ڣ�[1]:��ǰ����";
            return false;
        }

        sResultExp.Format("A=%d;Q=%d;X=%s;T=%s;",AUTOTASK_EXEC_ATYEARDAY,
                          std::stol(arDatePots[1]), arDatePots[0], sTimePots);
        return true;
    } else if (AUTOTASK_EXEC_ATDAILY == flag) {
        // ÿ��x��ִ��һ��
        if (arDatePots.size() > 1) {
            sError = "���ռ��ִ��   ����̫��";
            return false;
        }
        int nXDay;
        if (!QHelper::ParseInt(arDatePots[0], nXDay)) {
            sError = "���ռ��ִ��   ������������      Ӧ��ָ��һ���������";
        }
        if ((nXDay < 1) || (nXDay > 30)) {
            sError = "���ռ��ִ��   �����ΧӦ����[1,30]֮��";
            return false;
        }
        sResultExp.Format("A=%d;P=%u;T=%s;", AUTOTASK_EXEC_ATDAILY, nXDay, sTimePots);
        return true;
    } else if (AUTOTASK_EXEC_ATWEEKDAY == flag) {
        // �ܼ�
        int v, weeks = 0;
        for (unsigned int i = 0; i < arDatePots.size(); ++i) {
            if (!QHelper::ParseInt(arDatePots[i], v) || (v > 6)) {
                sError = "����ִ��   ������������    Ӧ��ָ��һ��[0, 6]������";
                return false;
            }
            weeks |= (0x1<<v);
        }
        if (0 == weeks) {
            sError = "����ִ��   ������������    Ӧ������ָ��һ��[0, 6]������";
            return false;
        }
        sResultExp.Format("A=%d;X=%u;T=%s;",AUTOTASK_EXEC_ATWEEKDAY, weeks, sTimePots);
        return true;
    } else if (AUTOTASK_EXEC_ATMONTHDAY == flag) {
        int d, days = 0;
        for (unsigned int i = 0; i < arDatePots.size(); ++i) {
            if (!QHelper::ParseInt(arDatePots[i], d) || (d > 31)) {
                sError = "����ִ��   ������������    ������Χ[0, 31], 0 - ÿ�ն�ִ��";
                return false;
            } else if (0 == d) {
                days = 0x0fffffff;  // ȫ��ִ��
                break;
            }
            days |= (0x1 << d);
        }
        if (0 == days) {
            sError = "����ִ��   ������������    Ӧ������ָ��һ��[0, 31]������,  0 - ÿ�ն�ִ��";
            return false;
        }
        sResultExp.Format("A=%d;X=%u;T=%s;",AUTOTASK_EXEC_ATMONTHDAY,  days, sTimePots);
        return true;
    }
    sError = "���Ա��ʽ    ִ�в�������";
    return false;
}

mstring QHelper::MakeRemindExp( int nA, WCHAR cAUnit,
                                const mstring&sSound,const mstring&sMsg ) {
    cAUnit = tolower(cAUnit);
    if ((nA <= 0) || ((cAUnit != L's') && (cAUnit != L'm') && (cAUnit != L'h')))
        return "";
    mstring sRet;
    sRet.Format("A=%d%c;\nS=%s;\nM=%s;\n",nA,cAUnit,sSound,sMsg);
    return sRet;
}

bool QHelper::ParseUnitTime(__in const CStdStringW& src, __out int &nT, __out wchar_t& cUnit) {
    CStdStringW tmp = src;
    if (tmp.Trim().IsEmpty())
        return false;

    cUnit = tolower(tmp.back());
    if ( (L's' != cUnit) && (L'm' != cUnit) && (L'h' != cUnit))
        return false;
    tmp.pop_back();

    return QHelper::ParseInt(tmp, nT);
}
}
