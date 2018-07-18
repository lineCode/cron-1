#include "abstimer.h"

namespace cron {

AbsTimer::AbsTimer(const mstring& exp)
    :ExpTimer(exp, TimerType::kTypeAbs) {
}

AbsTimer::~AbsTimer() {
}

bool AbsTimer::parse() {
    if (exp().IsEmpty())
        return false;

    mstring tmp_exp = exp(), val;
    char prop;
    while (!tmp_exp.IsEmpty()) {
        if (!_parse_prop_val(tmp_exp, prop, val))
            return false;
        switch (prop) {
        case L'A': {
            eflag_exec_ = (ExpTimerExecFlag)std::stol(val);
            break;
        }
        case L'X': {
            if (!_parse_to_array(val, arx_))
                return false;
            std::stable_sort(arx_.begin(), arx_.end());
            break;
        }
        case L'S': { // ʱ��
            t_begin_ = std::stol(val);
            break;
        }
        case L'E': { // ʱ��
            t_end_ = std::stol(val);
            break;
        }
        case L'Q': { // ʱ��
            span_ = std::stol(val);
            break;
        }
        case L'P': { // ���ʱ��ִ��
            if (!_parse_to_array(val, arx_))
                return false;
            if (arx_[0] < 1)
                return false;
            break;
        }
        case L'T': { // ʱ���ִ��
            if (!_parse_to_array(val, ar_time_))
                return false;
            // #ifdef _DEBUG
            // 				QTime tmT;
            // 				for (int i = 0; i < m_arTime.size(); i++)
            // 				{
            // 					tmT = QTime::ParseTime(m_arTime[i]);
            // 					TRACE(tmT.Format("%H:%M:%S\n"));
            // 				}
            // #endif
            // ��С��������
            std::stable_sort(ar_time_.begin(), ar_time_.end());
            break;
        }
        default:
            return false;
        }
    }
    return true;
}

// AtDate
ExpTimerRunningStatus AbsTimer::nextExecDate(__inout uint32_t& the_date) {
    if (the_date > dt::make_uint_date(life_end_)) {
        return AUTOTASK_RUNNING_STATUS_OVERDUE;	// ����
    }

    if (arx_.size() < 1) {
        return AUTOTASK_RUNNING_STATUS_BADTIMER;
    }

    switch (eflag_exec_) {
    case AUTOTASK_EXEC_ATYEARDAY: {
        // ��Ҫ���Ե�ִ������
        dt::time t_test = dt::parse_uint_date(the_date);

        std::tm tmp_test = dt::to_calendar_time(t_test);
        for (int year = tmp_test.tm_year - 1; year < tmp_test.tm_year + 2; ++year) {
            // �趨��ִ�е�
            dt::time t_exec = dt::parse_uint_date(arx_[0]);
            std::tm tmp_exec = dt::to_calendar_time(t_exec);
            dt::set_date(t_exec, year, tmp_exec.tm_mon, tmp_exec.tm_mday);
            // ִ�е��ȥ��ǰ����ִ��ʱ��
            dt::time t_adv = t_exec;
            t_adv -= date::days(span_);

            // ���ִ��ʱ��������û�е���ǰ����ִ��ʱ�䣬��ô������ǰ������ִ��ʱ��
            if ((dt::compare_date(t_adv, t_test) >= 0) && (dt::compare_date(t_adv, lifeEnd()) <= 0)) {
                the_date = dt::make_uint_date(t_adv);
                return AUTOTASK_RUNNING_STATUS_OK;
            }

            // �������ʱ�������[��ǰ�㣬ִ�е�]֮�䣬��ô�����������ʱ���
            if ((dt::compare_date(t_test, t_adv) >= 0) && (dt::compare_date(t_test, t_exec) <= 0)) {
                the_date = dt::make_uint_date(t_test);
                return AUTOTASK_RUNNING_STATUS_OK;
            }
        }
        return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;
    }

    case AUTOTASK_EXEC_ATDATE: {
        int idx = arx_.find_first_lgoreq(the_date);
        if (-1 != idx) {
            the_date = arx_[idx];
            return AUTOTASK_RUNNING_STATUS_OK;
        }
        return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC; // �޻�����ִ��
    }

    case AUTOTASK_EXEC_ATDAILY: {
        ASSERT(arx_.size() == 1);
        auto tsDate = dt::parse_uint_date(the_date) - dt::parse_uint_date(dt::make_uint_date(life_begin_));
        uint32_t xDay = dt::total_days(tsDate) % arx_[0]; // ���м����´�ִ��
        if (xDay > 0) {
            dt::time tmNextExecDate = dt::parse_uint_date(the_date) + date::days(xDay);
            if (dt::compare_date(tmNextExecDate, life_end_) > 0) // ���������ʱ�����������֮��
                return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;	// �޻���ִ���ˡ�
            the_date = dt::make_uint_date(tmNextExecDate);
            return AUTOTASK_RUNNING_STATUS_OK;
        } else if (0 == xDay) { //dwDate ����Ϊִ������
            return AUTOTASK_RUNNING_STATUS_OK;
        }
        // ��Ӧ��ִ�е����
        return AUTOTASK_RUNNING_STATUS_APPERROR;
    }
    case AUTOTASK_EXEC_ATWEEKDAY: {
        // ����xִ��
        ASSERT(arx_.size() == 1);
        if (0 == arx_[0]) // ����������һ���������ǿ���ִ�е�
            return AUTOTASK_RUNNING_STATUS_BADTIMER;
        dt::time tmTest = dt::parse_uint_date(the_date);
        for (int iTestCount = 0; iTestCount < 7; iTestCount++) {
            // ����7��֮�ڵ�ִ�����,
            std::tm tmp_test = dt::to_calendar_time(tmTest);
            if (arx_[0] & (0x01 << (tmp_test.tm_wday - 1))) { // 0-sunday,1-monday...6-saturday
                the_date = dt::make_uint_date(tmTest);
                return AUTOTASK_RUNNING_STATUS_OK;
            }
            tmTest += date::days(1);
            if (dt::compare_date(tmTest, life_end_) > 0) {
                return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;
            }
        }
        // ��Ӧ��ִ�е����,��ǰ�������һ��֮�ڱ���һ���ǿ���ִ�е�
        return AUTOTASK_RUNNING_STATUS_APPERROR;
    }
    case AUTOTASK_EXEC_ATMONTHDAY: {
        ASSERT(arx_.size() == 1);
        if (0 == arx_[0]) // ����������һ���������ǿ���ִ�е�
            return AUTOTASK_RUNNING_STATUS_BADTIMER;
        dt::time tmTest = dt::parse_uint_date(the_date);
        for (int iTestCount = 0; iTestCount < 31; iTestCount++) {
            std::tm tmp_test = dt::to_calendar_time(tmTest);
            if (arx_[0] & (0x1 << (tmp_test.tm_mday))) {
                the_date = dt::make_uint_date(tmTest);
                return AUTOTASK_RUNNING_STATUS_OK;
            }
            tmTest += date::days(1); // ��һ��
            if (dt::compare_date(tmTest, life_end_) > 0) {
                return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;
            }
        }
        // ��Ӧ��ִ�е��������Ϊ��31��֮�ڣ�����һ���ǿ���ִ�е�
        return AUTOTASK_RUNNING_STATUS_APPERROR;
    }
    default: {
        ASSERT(false);
        return AUTOTASK_RUNNING_STATUS_APPERROR;
    }
    }
    return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;
}

mstring AbsTimer::description() {
    mstring when_des;
    mstring time_part;

    dt::time tm_test;
    for (int i = 0; i < ar_time_.size(); ++i) {
        tm_test = dt::parse_uint_time(ar_time_[i]);
        time_part += (dt::format_time(tm_test));
    }
    time_part.TrimRight(L',');

    switch (eflag_exec_) {
    case AUTOTASK_EXEC_ATYEARDAY: {
        tm_test = dt::parse_uint_date(arx_[0]);
        mstring tmp;
        if (0 != span_) {
            tmp.Format(" - ��ǰ%d��", span_);
        }
        mstring date_part;
        std::tm t = dt::to_calendar_time(tm_test);
        date_part.Format("%d/%d%s", t.tm_mon, t.tm_mday, tmp);
        when_des.Format("�� <b .yellow>[%s] [%s]</b>", date_part, time_part);
        break;
    }
    case AUTOTASK_EXEC_ATDATE: {
        tm_test = dt::parse_uint_date(arx_[0]);
        when_des.Format("�� <b .yellow>[%s] [%s]</b>",
                        dt::format_date(tm_test), time_part);
        break;
    }
    case AUTOTASK_EXEC_ATDAILY: {
        ASSERT(arx_.size() == 1);
        when_des.Format("ÿ <b .yellow>[%d]</b> ��� <b .yellow>[%s]</b>",
                        arx_[0], time_part);
        break;
    }
    case AUTOTASK_EXEC_ATWEEKDAY: {
        // ����xִ��
        ASSERT(arx_.size() == 1);
        mstring sWeekdays, sTemp;
        for (int iWeekday = 0; iWeekday < 7; iWeekday++) {
            // ����7��֮�ڵ�ִ�����,
            if (arx_[0] & (0x01 << iWeekday)) { // 0-sunday,1-monday...6-saturday
                sTemp.Format("%d,", iWeekday);
                sWeekdays += sTemp;
            }
        }
        sWeekdays = sWeekdays.Left(sWeekdays.GetLength() - 1);
        when_des.Format("<b .yellow>����[%s]</b>(0-����,...,6-����)�� [%s]<br />", sWeekdays, time_part);
        break;
    }
    case AUTOTASK_EXEC_ATMONTHDAY: {
        ASSERT(arx_.size() == 1);
        mstring days, tmp;
        for (int iDay = 0; iDay < 31; iDay++) {
            if (arx_[0] & (0x1 << iDay)) {
                tmp.Format("%d,", iDay);
                days += tmp;
            }
        }
        days = days.Left(days.GetLength() - 1);
        when_des.Format("ÿ <b .yellow>��[%s]</b> �� [%s]", days, time_part);
        break;
    }
    }
    return when_des;
}


void AbsTimer::execTimeSpot(std::vector<dt::time>& vTimes) {
    vTimes.clear();
    for (int i = 0; i < ar_time_.size(); ++i) {
        vTimes.push_back(dt::parse_uint_time(ar_time_[i]));
    }
}

// ���tmExec�����ڴ���tmTest�����ڣ�tmNext�趨Ϊm_arTime[0],����TASK_RUNNING_STATUS_OK
// ���tmExec�����ڵ���tmTest�����ڣ�tmNext�趨Ϊ���ڵ���tmTest.MakeTime()��ֵ��
//		���m_arTime�а���������ʱ�䣬����TASK_RUNNING_STATUS_OK
//		���������������ʱ�䣬����TASK_RUNNING_STATUS_TIMENOTMATCH
// ���tmExec������С��tmTest�����ڣ������߼�������������TASK_RUNNING_STATUS_OVERDUE
ExpTimerRunningStatus AbsTimer::nextRightTimeFrom(__in const dt::time&tmTest,
        __in const dt::time& tm_exec, __inout uint32_t &dwNextExecTime) {
    ASSERT(ar_time_.size());
    if (ar_time_.size()) {
        // ����ʱ��ֵ
        int iCmp = dt::compare_date(tm_exec, tmTest);
        if (iCmp > 0) {
            dwNextExecTime = ar_time_[0];
            return AUTOTASK_RUNNING_STATUS_OK;
        } else if (0 == iCmp) {
            int idx = ar_time_.find_first_lgoreq(dt::make_uint_time(tmTest));
            if (-1 != idx) {
                dwNextExecTime = ar_time_[idx];
                return AUTOTASK_RUNNING_STATUS_OK;
            }
            return AUTOTASK_RUNNING_STATUS_TIMENOTMATCH;
        } else {
            return AUTOTASK_RUNNING_STATUS_OVERDUE;
        }
    }
    // �����Ӧ�ñ�ִ�е�
    ASSERT(false);
    return AUTOTASK_RUNNING_STATUS_BADTIMER;
}

// tmTest �������������뼶�𽫻������Ϊ0
ExpTimerRunningStatus AbsTimer::getNextExecTimeFrom( __inout dt::time& tm_test,
        __out dt::time& tm_exec, __out int32_t &period_s) {
    period_s = 0;

    if (tm_test >= life_end_) {
        return AUTOTASK_RUNNING_STATUS_OVERDUE;	// ����
    }

    uint32_t next_exec_date, next_exec_time;
    ExpTimerRunningStatus status;
    dt::time tmTempTest = tm_exec = tm_test;
    while (true) {
        next_exec_date = dt::make_uint_date(tm_exec);
        status = nextExecDate(next_exec_date); // ִ������
        if (AUTOTASK_RUNNING_STATUS_OK == status) {
            tm_exec = dt::parse_uint_date(next_exec_date);
            status = nextRightTimeFrom(tmTempTest, tm_exec, next_exec_time);
            if (AUTOTASK_RUNNING_STATUS_OK == status) {
                // ִ��ʱ��
                tm_exec = dt::combine_date_time(next_exec_date, next_exec_time);
                if (tm_exec > life_end_) // ������ϳɵ�ʱ���Ƿ񳬹�����������
                    return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;
                else
                    return AUTOTASK_RUNNING_STATUS_OK;
            } else if (AUTOTASK_RUNNING_STATUS_TIMENOTMATCH == status) {
                // ˵����ǰ���Ե������ǲ�����ִ�еģ�ֻ�ܱȽ����������ִ��
                // ��ʱʱ���������С��
                tm_exec += date::days(1);
                dt::set_time(tm_exec, 0, 0, 0);
                dt::set_time(tmTempTest, 0, 0, 0);
                continue;
            } else {
                return status;
            }
        }
        return status;
    }
    return AUTOTASK_RUNNING_STATUS_BADTIMER;
}

}
