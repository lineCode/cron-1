#include "relatetimer.h"
#include "dict_zh.h"

namespace cron {

RelateTimer::RelateTimer(const mstring& exp)
    :ExpTimer(exp, TimerType::kTypeRelate) {
}

RelateTimer::~RelateTimer() {
}

mstring RelateTimer::description() {
    mstring when_des, time_part;
    when_des.Format("��[%s] [%d][%s]֮��",
                    dict::getExecFlagText(eflag_exec_), span_, dict::timeUnitStr(span_unit_));
    // then every
    if (isExecSpan2()) {
        time_part.Format("Ȼ��ÿ[%d][%s]ִ��", span2_, dict::timeUnitStr(span2_unit_));
        when_des += time_part;
        // after x times stop
        if (isExecCount()) {
            time_part.Format("��[%d] �κ�ֹͣ", exec_count_);
            when_des += time_part;
        }
    }
    return when_des;
}

bool RelateTimer::parse() {
    if (exp().IsEmpty())
        return false;

    mstring sExpTest = exp();
    char prop;
    mstring val;
    while (!sExpTest.IsEmpty()) {
        if (!_parse_prop_val(sExpTest, prop, val))
            return false;
        switch (prop) {
        case 'R': {
            eflag_exec_ = (TimerExecType)std::stol(val);
            break;
        }
        case 'P': {
            if (!_parse_span_time(val, span_unit_, span_))
                return false;
            break;
        }
        case 'Q': { // �ڶ���ʱ����
            if (!_parse_span_time(val, span2_unit_, span2_))
                return false;
            break;
        }
        case 'C': { // ִ�д���
            exec_count_ = std::stol(val);
            break;
        }
        default:
            return false;
        }
    }
    return true;
}

TimerRunningStatus RelateTimer::_CheckWith(
    const dt::time& tm_start, const dt::time& tm_test, __out dt::time& tm_exec, __out int32_t &period_s) {
    if (tm_test >= life_end_) {
        return TimerRunningStatus::kOverdue;
    }
    period_s = execSpanSeconds2();
    // ��һ��ִ��ʱ���ǣ�
    dt::time tm_first_exec = tm_start + dt::secs(execSpanSeconds());

    // ����˵�һ��ִ��ʱ��
    if (tm_first_exec <= tm_test) {
        if (!isExecSpan2()) {
            // �����һ��ִ��ʱ�䣬���ҷǶ��ִ��
            if (TimerExecType::kAfterSysBoot == eflag_exec_)
                return TimerRunningStatus::kUntilNextSysReboot; // �ȴ�ϵͳ����
            else if (TimerExecType::kAfterAppStart == eflag_exec_)
                return TimerRunningStatus::kUntilNextAppReboot; // �ȴ���������
            else // �Ƕ�ο�ִ�У�����
                return TimerRunningStatus::kNoChanceExec;
        }

        // ��μ��ִ��
        // �Դӵ�һ�ο�ִ��ʱ�䵽tm_test�Ѿ���ȥ�˶೤ʱ��
        int64_t dTotalSeconds = dt::total_seconds(tm_test - tm_first_exec);
        // �ڹ�ȥ����ô��ʱ�������ִ�ж��ٴΣ�
        exec_count_already_ = dTotalSeconds / execSpanSeconds2(); //ִ�д���
        if (isExecCount() && (exec_count_already_ >= exec_count_)) {
            // ��ִ�д����Ѿ��������ܹ���Ҫִ�еĴ���
            if (TimerExecType::kAfterSysBoot == eflag_exec_)
                return TimerRunningStatus::kUntilNextSysReboot; // �ȴ�ϵͳ����
            else if (TimerExecType::kAfterAppStart == eflag_exec_)
                return TimerRunningStatus::kUntilNextAppReboot; // �ȴ���������
            else // �Ƕ�ο�ִ�У�����
                return TimerRunningStatus::kNoChanceExec;
        } else {
            // ��ִ�д�����û�г������ܹ���Ҫִ�еĴ���
            tm_exec = tm_first_exec + dt::secs((exec_count_already_ + 1) * execSpanSeconds2());
            if (tm_exec >= life_end_) {
                return TimerRunningStatus::kOverdue;
            }
            return TimerRunningStatus::kOk;
        }
    } else {
        tm_exec = tm_first_exec;
        return TimerRunningStatus::kOk;
    }
}

RelateTimer::TimerBehavior RelateTimer::onFired(OnTimeoutCallback cb, void *d) {
    cb(d);

    exec_count_already_++;
    if (isExecCount() && exec_count_already_ >= exec_count_) {
        // ��Ҫֹͣ��ʱ����
        return TimerBehavior::kStop;
    }
    return TimerBehavior::kContinue;
}

// tm_test �������������뼶�𽫻������Ϊ0
TimerRunningStatus RelateTimer::getNextExecTimeFrom(
    __inout dt::time& tm_test,__out dt::time& tm_exec,__out int32_t &period_s) {
    if (tm_test >= life_end_) {
        return TimerRunningStatus::kOverdue;	// ����
    }

    switch (eflag_exec_) {
    case TimerExecType::kAfterSysBoot: {	//= 0x00000001,	// ϵͳ����
        // return _RelateTime_CheckWith(QProcessMan::GetSystemStartupTime(),tm_test,tm_exec, period_s);
        break;
    }
    case TimerExecType::kAfterAppStart: {	// = 0x00000004,// ����������
        //return _RelateTime_CheckWith(QProcessMan::GetCurrentProcessStartupTime(),tm_test,tm_exec, period_s);
        break;
    }
    /*	case TASK_EXEC_AFTERPROGSTART:	// = 0x00000008,// �ⲿ��������
    {
    QTime tmProgStart;
    if (QProcessMgr::IsExeRun(m_sXFiledExp,tmProgStart))
    {
    return _RelateTime_CheckWith(tmProgStart,tm_test,tm_exec);
    }
    return TASK_RUNNING_STATUS_BASEDONEXETERNALPROG;
    }
    case TASK_EXEC_AFTERPROGEXIT:	// = 0x00000010,// �ⲿ�����˳�
    {
    return TASK_RUNNING_STATUS_BASEDONEXETERNALPROG;
    }*/
    case TimerExecType::kAfterTimerStart: {	// = 0x00000002,	// ��������
        return _CheckWith(life_begin_, tm_test, tm_exec, period_s);
    }
    }
    ASSERT(false);
    return TimerRunningStatus::kBadTimer;
}

}
