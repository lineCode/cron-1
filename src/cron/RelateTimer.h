#pragma once

#include "exptimer.h"
#include "defs.h"


//////////////////////////////////////////////////////////////////////////
// ʱ�䣺[2018/4/11]
//
//  ���ʱ����ʽ��
//	R = 1; P = 20m; Q = 22m; C = 100;
//		R: �����1��TASK_EXEC_AFTERSYSBOOT��
//  	P : 20minutes��ִ������ P[1s~24h]
//  	Q : Ȼ��ÿ��22minutesִ��һ�� Q[1s~24h]
//  	C : ִ��100�κ�ֹͣ��C[1~86400]
//
//////////////////////////////////////////////////////////////////////////

namespace cron {

class RelateTimer : public ExpTimer {
public:
    RelateTimer(const mstring& exp);
    ~RelateTimer();

    virtual mstring whenDoString() override;

    virtual bool parse() override;

    virtual ExpTimerRunningStatus getNextExecTimeFrom(__inout dt::time& tmTest,
            __out dt::time& tmExec, __out int32_t &period_s) override;

    uint32_t execSpanSeconds() const {
        return helper::howManySecs(span_, span_unit_);
    }

    uint32_t execSpanSeconds2()const {
        return helper::howManySecs(span2_, span2_unit_);
    }

    // ִ�е�һ�κ��Ƿ��ټ��ִ��
    bool isExecSpan2()const {
        return (span2_ > 0);
    }

    bool isExecCount()const {
        // -1 �����޴Σ�0 �ǲ�ִ�У� >0 �Ǿ����
        return (exec_count_ == -1) || exec_count_ > 0;
    }

protected:
    ExpTimerRunningStatus _CheckWith(const dt::time& tm_start, const dt::time& tm_test,
                                     __out dt::time& tm_exec, __out int32_t &period_s);

    virtual EnumTimerFiredFlag onTimerFired(OnTimeoutCallback cb, void *d) override;

private:
    // �����ENUM_TASK_EXECFLAG��ָʾ�������
    // ���������ʱ��ִ�е�һ�����񣬵�λ����Ϊ��
    // ��ʱ������Ϊ24Сʱ֮��
    uint32_t			span_;		// ���ʱ���������
    wchar_t			span_unit_;	// ԭʼ��λs/m/h
    // ����ڵ�һ������ִ�к����ʱ��ִ��
    uint32_t			span2_;	// �ڶ���ʱ����
    wchar_t			span2_unit_;
    int				exec_count_;	// ��һ��ִ�к���ִ�еĴ���
    uint32_t exec_count_already_ = 0;	// �Ѿ�ִ�еĴ���
    ExpTimerExecFlag	eflag_exec_;
};

}
