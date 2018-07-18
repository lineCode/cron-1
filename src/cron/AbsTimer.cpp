#include "abstimer.h"

namespace cron {

AbsTimer::AbsTimer(const mstring& exp)
    :ExpTimer(exp, TimerType::kTypeAbs) {
}

AbsTimer::~AbsTimer() {
}

bool AbsTimer::parse() {
	if (exp().IsEmpty()) {
		return false;
	}

	mstring tmp = exp();
	mstring val;
    char prop;
    while (!tmp.IsEmpty()) {
		if (!_parse_prop_val(tmp, prop, val)) {
			return false;
		}

        switch (prop) {
        case 'A': {
            eflag_exec_ = (ExpTimerExecFlag)std::stol(val);
            break;
        }
        case 'X': {
			if (!_parse_to_array(val, arx_)) {
				return false;
			}
            std::stable_sort(arx_.begin(), arx_.end());
            break;
        }
        case 'S': { // ʱ��
            t_begin_ = std::stol(val);
            break;
        }
        case 'E': { // ʱ��
            t_end_ = std::stol(val);
            break;
        }
        case 'Q': { // ʱ��
            span_ = std::stol(val);
            break;
        }
        case 'P': { // ���ʱ��ִ��
			if (!_parse_to_array(val, arx_)) {
				return false;
			}
			if (arx_[0] < 1) {
				return false;
			}
            break;
        }
        case 'T': { // ʱ���ִ��
			if (!_parse_to_array(val, ar_time_)) {
				return false;
			}
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
TimerRunningStatus AbsTimer::nextExecDate(__inout uint32_t& the_date) {
    if (the_date > dt::make_uint_date(life_end_)) {
        return TimerRunningStatus::kOverdue;	// ����
    }

    if (arx_.size() < 1) {
        return TimerRunningStatus::kBadTimer;
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
                return TimerRunningStatus::kOk;
            }

            // �������ʱ�������[��ǰ�㣬ִ�е�]֮�䣬��ô�����������ʱ���
            if ((dt::compare_date(t_test, t_adv) >= 0) && (dt::compare_date(t_test, t_exec) <= 0)) {
                the_date = dt::make_uint_date(t_test);
                return TimerRunningStatus::kOk;
            }
        }
        return TimerRunningStatus::kNoChanceExec;
    }

    case AUTOTASK_EXEC_ATDATE: {
        int idx = arx_.find_first_lgoreq(the_date);
        if (-1 != idx) {
            the_date = arx_[idx];
            return TimerRunningStatus::kOk;
        }
        return TimerRunningStatus::kNoChanceExec; // �޻�����ִ��
    }

    case AUTOTASK_EXEC_ATDAILY: {
        ASSERT(arx_.size() == 1);
        auto tsdate = dt::parse_uint_date(the_date) - dt::parse_uint_date(dt::make_uint_date(life_begin_));
        uint32_t xday = dt::total_days(tsdate) % arx_[0]; // ���м����´�ִ��
        if (xday > 0) {
            dt::time tmNextExecDate = dt::parse_uint_date(the_date) + date::days(xday);
            if (dt::compare_date(tmNextExecDate, life_end_) > 0) // ���������ʱ�����������֮��
                return TimerRunningStatus::kNoChanceExec;	// �޻���ִ���ˡ�
            the_date = dt::make_uint_date(tmNextExecDate);
            return TimerRunningStatus::kOk;
        } else if (0 == xday) { //ddate ����Ϊִ������
            return TimerRunningStatus::kOk;
        }
        // ��Ӧ��ִ�е����
        return TimerRunningStatus::kAppError;
    }
    case AUTOTASK_EXEC_ATWEEKDAY: {
        // ����xִ��
        ASSERT(arx_.size() == 1);
        if (0 == arx_[0]) // ����������һ���������ǿ���ִ�е�
            return TimerRunningStatus::kBadTimer;
        dt::time tm_test = dt::parse_uint_date(the_date);
        for (int iTestCount = 0; iTestCount < 7; iTestCount++) {
            // ����7��֮�ڵ�ִ�����,
            std::tm tmp_test = dt::to_calendar_time(tm_test);
            if (arx_[0] & (0x01 << (tmp_test.tm_wday - 1))) { // 0-sunday,1-monday...6-saturday
                the_date = dt::make_uint_date(tm_test);
                return TimerRunningStatus::kOk;
            }
            tm_test += date::days(1);
            if (dt::compare_date(tm_test, life_end_) > 0) {
                return TimerRunningStatus::kNoChanceExec;
            }
        }
        // ��Ӧ��ִ�е����,��ǰ�������һ��֮�ڱ���һ���ǿ���ִ�е�
        return TimerRunningStatus::kAppError;
    }
    case AUTOTASK_EXEC_ATMONTHDAY: {
        ASSERT(arx_.size() == 1);
		if (0 == arx_[0]) {// ����������һ���������ǿ���ִ�е�
			return TimerRunningStatus::kBadTimer;
		}
        dt::time tm_test = dt::parse_uint_date(the_date);
        for (int iTestCount = 0; iTestCount < 31; iTestCount++) {
            std::tm tmp_test = dt::to_calendar_time(tm_test);
            if (arx_[0] & (0x1 << (tmp_test.tm_mday))) {
                the_date = dt::make_uint_date(tm_test);
                return TimerRunningStatus::kOk;
            }
            tm_test += date::days(1); // ��һ��
            if (dt::compare_date(tm_test, life_end_) > 0) {
                return TimerRunningStatus::kNoChanceExec;
            }
        }
        // ��Ӧ��ִ�е��������Ϊ��31��֮�ڣ�����һ���ǿ���ִ�е�
        return TimerRunningStatus::kAppError;
    }
    default: {
        ASSERT(false);
        return TimerRunningStatus::kAppError;
    }
    }
    return TimerRunningStatus::kNoChanceExec;
}

mstring AbsTimer::description() {
    mstring when_des;
    mstring time_part;

    dt::time tm_test;
    for (int i = 0; i < ar_time_.size(); ++i) {
        tm_test = dt::parse_uint_time(ar_time_[i]);
        time_part += (dt::format_time(tm_test));
    }
    time_part.TrimRight(',');

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
        when_des.Format("��[%s] [%s]", date_part, time_part);
        break;
    }
    case AUTOTASK_EXEC_ATDATE: {
        tm_test = dt::parse_uint_date(arx_[0]);
        when_des.Format("��[%s] [%s]", dt::format_date(tm_test), time_part);
        break;
    }
    case AUTOTASK_EXEC_ATDAILY: {
        ASSERT(arx_.size() == 1);
        when_des.Format("ÿ[%d]���[%s]", arx_[0], time_part);
        break;
    }
    case AUTOTASK_EXEC_ATWEEKDAY: {
        // ����xִ��
        ASSERT(arx_.size() == 1);
        mstring weekdays, tmp;
        for (int weekday = 0; weekday < 7; weekday++) {
            // ����7��֮�ڵ�ִ�����,
            if (arx_[0] & (0x01 << weekday)) { // 0-sunday,1-monday...6-saturday
                tmp.Format("%d,", weekday);
                weekdays += tmp;
            }
        }
        weekdays = weekdays.Left(weekdays.GetLength() - 1);
        when_des.Format("����[%s](0-����,...,6-����)�� [%s]", weekdays, time_part);
        break;
    }
    case AUTOTASK_EXEC_ATMONTHDAY: {
        ASSERT(arx_.size() == 1);
        mstring days, tmp;
        for (int day = 0; day < 31; day++) {
            if (arx_[0] & (0x1 << day)) {
                tmp.Format("%d,", day);
                days += tmp;
            }
        }
        days = days.Left(days.GetLength() - 1);
        when_des.Format("ÿ��[%s] �� [%s]", days, time_part);
        break;
    }
    }
    return when_des;
}


void AbsTimer::execTimeSpot(std::vector<dt::time>& time_points) {
    time_points.clear();
    for (int i = 0; i < ar_time_.size(); ++i) {
        time_points.push_back(dt::parse_uint_time(ar_time_[i]));
    }
}

// ���tm_exec�����ڴ���tm_test�����ڣ�tmNext�趨Ϊm_arTime[0],����TASK_RUNNING_STATUS_OK
// ���tm_exec�����ڵ���tm_test�����ڣ�tmNext�趨Ϊ���ڵ���tm_test.MakeTime()��ֵ��
//		���m_arTime�а���������ʱ�䣬����TASK_RUNNING_STATUS_OK
//		���������������ʱ�䣬����TASK_RUNNING_STATUS_TIMENOTMATCH
// ���tm_exec������С��tm_test�����ڣ������߼�������������TASK_RUNNING_STATUS_OVERDUE
TimerRunningStatus AbsTimer::nextRightTimeFrom(__in const dt::time&tm_test,
        __in const dt::time& tm_exec, __inout uint32_t &next_exec) {
    ASSERT(ar_time_.size());
    if (ar_time_.size()) {
        // ����ʱ��ֵ
        int iCmp = dt::compare_date(tm_exec, tm_test);
        if (iCmp > 0) {
            next_exec = ar_time_[0];
            return TimerRunningStatus::kOk;
        } else if (0 == iCmp) {
            int idx = ar_time_.find_first_lgoreq(dt::make_uint_time(tm_test));
            if (-1 != idx) {
                next_exec = ar_time_[idx];
                return TimerRunningStatus::kOk;
            }
            return TimerRunningStatus::kTimeNotMatch;
        } else {
            return TimerRunningStatus::kOverdue;
        }
    }
    // �����Ӧ�ñ�ִ�е�
    ASSERT(false);
    return TimerRunningStatus::kBadTimer;
}

// tm_test �������������뼶�𽫻������Ϊ0
TimerRunningStatus AbsTimer::getNextExecTimeFrom( __inout dt::time& tm_test,
        __out dt::time& tm_exec, __out int32_t &period_s) {
    period_s = 0;
    if (tm_test >= life_end_) {
        return TimerRunningStatus::kOverdue;	// ����
    }

    uint32_t next_exec_date, next_exec_time;
    TimerRunningStatus status;
    dt::time tm_temp = tm_exec = tm_test;
    while (true) {
        next_exec_date = dt::make_uint_date(tm_exec);
        status = nextExecDate(next_exec_date); // ִ������
        if (TimerRunningStatus::kOk == status) {
            tm_exec = dt::parse_uint_date(next_exec_date);
            status = nextRightTimeFrom(tm_temp, tm_exec, next_exec_time);
            if (TimerRunningStatus::kOk == status) {
                // ִ��ʱ��
                tm_exec = dt::combine_date_time(next_exec_date, next_exec_time);
				if (tm_exec > life_end_) { // ������ϳɵ�ʱ���Ƿ񳬹�����������
					return TimerRunningStatus::kNoChanceExec;
				}
				else {
					return TimerRunningStatus::kOk;
				}
            } else if (TimerRunningStatus::kTimeNotMatch == status) {
                // ˵����ǰ���Ե������ǲ�����ִ�еģ�ֻ�ܱȽ����������ִ��
                // ��ʱʱ���������С��
                tm_exec += date::days(1);
                dt::set_time(tm_exec, 0, 0, 0);
                dt::set_time(tm_temp, 0, 0, 0);
                continue;
            } else {
                return status;
            }
        }
        return status;
    }
    return TimerRunningStatus::kBadTimer;
}

}
