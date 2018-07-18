
#include "relatetimer.h"


namespace cron {

RelateTimer::RelateTimer(const mstring& exp)
    :ExpTimer(exp, ExpTimerType::kTimerTypeRelate) {
}

RelateTimer::~RelateTimer() {
}

mstring RelateTimer::whenDoString() {
    mstring when_des, time_part;
    when_des.Format("��<b .yellow>[%s] [%d][%s]</b>֮��",
                    getExecFlagText(eflag_exec_), span_, QHelper::GetTimeUnitString(span_unit_));
    // then every
    if (isExecSpan2()) {
        time_part.Format("Ȼ��ÿ <b .yellow>[%d][%s]</b>ִ��", span2_, QHelper::GetTimeUnitString(span2_unit_));
        when_des += "<br/>" + time_part;
        // after x times stop
        if (isExecCount()) {
            time_part.Format("�� <b .yellow>[%d]</b> �κ�ֹͣ", exec_count_);
            when_des += "<br />" + time_part;
        }
    }
    return when_des;
}

bool RelateTimer::parse() {
    if (exp().IsEmpty())
        return false;

    mstring sExpTest = exp();
    wchar_t cProp;
    mstring sValue;
    while (!sExpTest.IsEmpty()) {
        if (!_parse_prop_val(sExpTest, cProp, sValue))
            return false;
        switch (cProp) {
        case L'R': {
            eflag_exec_ = (ExpTimerExecFlag)std::stol(sValue);
            break;
        }
        case L'P': {
            if (!_parse_span_time(sValue, span_unit_, span_))
                return false;
            break;
        }
        case L'Q': { // �ڶ���ʱ����
            if (!_parse_span_time(sValue, span2_unit_, span2_))
                return false;
            break;
        }
        case L'C': { // ִ�д���
            exec_count_ = std::stol(sValue);
            break;
        }
        default:
            return false;
        }
    }
    return true;
}

ExpTimerRunningStatus RelateTimer::_CheckWith(
    const dt::time& tm_start, const dt::time& tm_test, __out dt::time& tm_exec, __out int32_t &period_s) {
    if (tm_test >= life_end_) {
        return AUTOTASK_RUNNING_STATUS_OVERDUE;
    }
    period_s = execSpanSeconds2();
    // ��һ��ִ��ʱ���ǣ�
    dt::time tm_first_exec = tm_start + dt::secs(execSpanSeconds());

    // ����˵�һ��ִ��ʱ��
    if (tm_first_exec <= tm_test) {
        if (!isExecSpan2()) {
            // �����һ��ִ��ʱ�䣬���ҷǶ��ִ��
            if (AUTOTASK_EXEC_AFTERSYSBOOT == eflag_exec_)
                return AUTOTASK_RUNNING_STATUS_UNTILNEXTSYSREBOOT; // �ȴ�ϵͳ����
            else if (AUTOTASK_EXEC_AFTERMINDERSTART == eflag_exec_)
                return AUTOTASK_RUNNING_STATUS_UNTILNEXTMINDERREBOOT; // �ȴ���������
            else // �Ƕ�ο�ִ�У�����
                return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;
        }

        // ��μ��ִ��
        // �Դӵ�һ�ο�ִ��ʱ�䵽tmTest�Ѿ���ȥ�˶೤ʱ��
        int64_t dTotalSeconds = dt::total_seconds(tm_test - tm_first_exec);
        // �ڹ�ȥ����ô��ʱ�������ִ�ж��ٴΣ�
        exec_count_already_ = dTotalSeconds / execSpanSeconds2(); //ִ�д���
        if (isExecCount() && (exec_count_already_ >= exec_count_)) {
            // ��ִ�д����Ѿ��������ܹ���Ҫִ�еĴ���
            if (AUTOTASK_EXEC_AFTERSYSBOOT == eflag_exec_)
                return AUTOTASK_RUNNING_STATUS_UNTILNEXTSYSREBOOT; // �ȴ�ϵͳ����
            else if (AUTOTASK_EXEC_AFTERMINDERSTART == eflag_exec_)
                return AUTOTASK_RUNNING_STATUS_UNTILNEXTMINDERREBOOT; // �ȴ���������
            else // �Ƕ�ο�ִ�У�����
                return AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC;
        } else {
            // ��ִ�д�����û�г������ܹ���Ҫִ�еĴ���
            tm_exec = tm_first_exec + dt::secs((exec_count_already_ + 1) * execSpanSeconds2());
            if (tm_exec >= life_end_) {
                return AUTOTASK_RUNNING_STATUS_OVERDUE;
            }
            return AUTOTASK_RUNNING_STATUS_OK;
        }
    } else {
        tm_exec = tm_first_exec;
        return AUTOTASK_RUNNING_STATUS_OK;
    }
}

RelateTimer::EnumTimerFiredFlag RelateTimer::onTimerFired(OnTimeoutCallback cb, void *d) {
    cb(d);

    exec_count_already_++;
    if (isExecCount() && exec_count_already_ >= exec_count_) {
        // ��Ҫֹͣ��ʱ����
        return EnumTimerFiredFlag::kTimerStop;
    }
    return EnumTimerFiredFlag::kTimerContinue;
}

// tmTest �������������뼶�𽫻������Ϊ0
ExpTimerRunningStatus RelateTimer::getNextExecTimeFrom(
    __inout dt::time& tmTest,__out dt::time& tmExec,__out int32_t &period_s) {
    if (tmTest >= life_end_) {
        return AUTOTASK_RUNNING_STATUS_OVERDUE;	// ����
    }

    switch (eflag_exec_) {
    case AUTOTASK_EXEC_AFTERSYSBOOT: {	//= 0x00000001,	// ϵͳ����
        // return _RelateTime_CheckWith(QProcessMan::GetSystemStartupTime(),tmTest,tmExec, period_s);
        break;
    }
    case AUTOTASK_EXEC_AFTERMINDERSTART: {	// = 0x00000004,// ����������
        //return _RelateTime_CheckWith(QProcessMan::GetCurrentProcessStartupTime(),tmTest,tmExec, period_s);
        break;
    }
    /*	case TASK_EXEC_AFTERPROGSTART:	// = 0x00000008,// �ⲿ��������
    {
    QTime tmProgStart;
    if (QProcessMgr::IsExeRun(m_sXFiledExp,tmProgStart))
    {
    return _RelateTime_CheckWith(tmProgStart,tmTest,tmExec);
    }
    return TASK_RUNNING_STATUS_BASEDONEXETERNALPROG;
    }
    case TASK_EXEC_AFTERPROGEXIT:	// = 0x00000010,// �ⲿ�����˳�
    {
    return TASK_RUNNING_STATUS_BASEDONEXETERNALPROG;
    }*/
    case AUTOTASK_EXEC_AFTERTASKSTART: {	// = 0x00000002,	// ��������
        return _CheckWith(life_begin_, tmTest, tmExec, period_s);
    }
    }
    ASSERT(false);
    return AUTOTASK_RUNNING_STATUS_BADTIMER;
}

}
