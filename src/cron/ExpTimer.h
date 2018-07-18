#pragma once

#include "timerinterface.h"
#include "utils.h"
#include "exarray.h"

namespace cron {

// ��ʱ��������
enum class TimerType {
    kTypeInvalid = 0,
    kTypeAbs,
    kTypeRelate,
};

// ÿ���µĵڼ�����
enum ENUM_NTH_WEEK {
    FIRST_WEEK = 0x1,
    SECOND_WEEK = 0x2,
    THIRD_WEEK = 0x4,
    FORTH_WEEK = 0x8,
    FIVETH_WEEK = 0x10,
};

enum ENUM_NTH_WEEKDAY {
    WEEKDAY_SUNDAY = 0x1,
    WEEKDAY_MONDAY = 0x2,
    WEEKDAY_TUESDAY = 0x4,
    WEEKDAY_WEDNESDAY = 0x8,
    WEEKDAY_THURSDAY = 0x10,
    WEEKDAY_FRIDAY = 0x20,
    WEEKDAY_SATURDAY = 0x40,
};

//////////////////////////////////////////////////////////////////////////
// ʱ�䣺[2018/4/11]
// ʹ�÷�ʽ��
//	1,create
//	2,parse
//	3,startFrom
//////////////////////////////////////////////////////////////////////////
class TimerInterface;
class ExpTimer {
protected:
    enum class TimerBehavior {
        kStop = 1,
        kContinue,
        kRefresh,
    };

public:
    ExpTimer(const mstring& exp = "", TimerType t = TimerType::kTypeInvalid);
    ~ExpTimer();

    // ����һ��ʵ��
    static ExpTimer* create(const mstring& exp);

    void stop();

    // ��ĳ��ʱ�������
    ExpTimerRunningStatus startFrom(dt::time &begin, OnTimeoutCallback cb, void* d);

    // ��ʱ��
    virtual mstring description() = 0;

    // ִ��ʱ���
    virtual void execTimeSpot(__out std::vector<dt::time>& time_points) {
    }

    inline bool setLife(dt::time begin, dt::time end) {
        if (end <= begin) {
            return false;
        }
        life_begin_ = begin;
        life_end_ = end;
        return true;
    }

    inline dt::time lifeBegin()const {
        return life_begin_;
    }

    inline dt::time lifeEnd() const {
        return life_end_;
    }

    inline bool started()const {
        return timer_id_ > 0;
    }

    inline mstring exp()const {
        return exp_;
    }

protected:
    // ����
    virtual bool parse() = 0;

    virtual TimerBehavior onFired(OnTimeoutCallback cb, void *d);

    // tmTest֮����´�ִ��ʱ��
    // tmTest �������������뼶�𽫻������Ϊ0
    // period_s ���ִ�е�����
    virtual  ExpTimerRunningStatus getNextExecTimeFrom(__inout dt::time& tmTest, __out dt::time& tmExec, __out int32_t &period_s) = 0;

    bool setTimer(int64_t due, int64_t period, OnTimeoutCallback cb, void* d);

protected:
    // exp: r=1;b=2;c=3
    // result:
    //	exp = b=2;c=3;
    //	sProp = r;
    //	val = 1;
    bool _parse_prop_val(__inout mstring&exp, __out char& prop, __out mstring& val) {
        int idx = exp.Find(L'=');
        if ((-1 == idx) || (idx != 1) || ((idx = exp.Find(L';')) == -1)) {
            return false;
        }
        prop = exp[0];
        val = exp.Mid(2, idx - 2);
        exp = exp.Mid(idx + 1);
        return true;
    }

    // 2,4,8,9
    // �� Ӣ�ġ�,�� �ָ��������ַ���ת��������ֵ
    bool _parse_to_array(__inout mstring& exp, __out IntArray & ar);

    // 200[s|m|h]
    // 200m = 200 * 60s;  unit = m;
    // 200h = 200 * 3600s	unit = h;
    // 200s = 200s;		unit = s;
    bool _parse_span_time(__in const mstring &exp, __out char& unit, __out uint32_t& span);

protected:
    ExpTimerExecFlag eflag_exec_;
    dt::time			life_begin_;	// �������Чʱ��
    dt::time			life_end_;	// �������ʱ��

private:
    int timer_id_ = 0;
    TimerType type_ = TimerType::kTypeInvalid;
    mstring exp_;
    dt::time		tm_next_exec_;//	�´�ִ��ʱ��
};

}
