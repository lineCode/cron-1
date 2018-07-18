#include "exptimer.h"
#include "abstimer.h"
#include "relatetimer.h"

namespace cron {

ExpTimer::ExpTimer(const mstring& exp, TimerType t)
    :exp_(exp), type_(t) {
}

ExpTimer::~ExpTimer() {
}

ExpTimer* ExpTimer::create(const mstring& exp) {
    if (exp.IsEmpty()) {
        return nullptr;
    }

    ExpTimer* t = nullptr;
    switch (exp.GetAt(0)) {
    case 'R':
        t = new RelateTimer(exp);
		t->parse();
		break;

    case 'A':
        t = new AbsTimer(exp);
		t->parse();
		break;
    }
    return t;
}

void ExpTimer::stop() {
    if (timer_id_ > 0) {
        timer::stop(timer_id_);
        timer_id_ = 0;
    }
}

ExpTimer::TimerBehavior ExpTimer::onFired(OnTimeoutCallback cb, void *d) {
    cb(d);

    // 刷新timer
    return TimerBehavior::kRefresh;
}

bool ExpTimer::setTimer(int64_t due, int64_t period, OnTimeoutCallback cb, void* d) {
    stop();

    timer_id_ = timer::set(due, period, [=](void*) {
        switch (this->onFired(cb, d)) {
        case TimerBehavior::kContinue:
            break;

        case TimerBehavior::kStop:
            stop();
            break;

        case TimerBehavior::kRefresh:
            startFrom(dt::now(), cb, d);
            break;
        };
    }, d);
    return timer_id_ > 0;
}

bool ExpTimer::_parse_to_array(__inout mstring& exp, __out IntArray & ar) {
    ar.clear();
    int idx;
    while (!exp.IsEmpty()) {
        idx = exp.Find(L',');
        if (idx != -1) {
            ar.push_back(std::stol(exp.Mid(0, idx)));
            exp = exp.Mid(idx + 1);
        } else {
            ar.push_back(std::stol(exp));
            break;
        }
    }
    return ar.size() > 0;
}

bool ExpTimer::_parse_span_time(__in const mstring &exp, __out char& unit, __out uint32_t& span) {
    int len = exp.GetLength();
    unit = tolower(exp.back());
    span = std::stol(exp.Mid(0, len - 1));
    switch (unit) {
    case 'm':
    case 'M':
    case 's':
    case 'S':
    case 'H':
    case 'h':
        return true;
    }
    ASSERT(false);
    return false;
}

ExpTimerRunningStatus ExpTimer::startFrom(dt::time &tmBegin, OnTimeoutCallback cb, void* d) {
    if (started()) {
        return AUTOTASK_RUNNING_STATUS_OK;
    }

    // 先停掉原来的
    stop();

    int64_t lHowLongToExec;
    ExpTimerRunningStatus eStatus;
    while (true) {
        int32_t period_s = 0;
        eStatus = getNextExecTimeFrom(tmBegin, tm_next_exec_, period_s);
        if (eStatus != AUTOTASK_RUNNING_STATUS_OK) {
            break;
        }
        lHowLongToExec = dt::total_seconds(tm_next_exec_ - dt::now());
        if (lHowLongToExec <= 0) {
            // 往后找
            tmBegin = tm_next_exec_ + dt::secs(1);
            continue;
        }

        if (cb) {
            if (setTimer(lHowLongToExec * 1000, period_s * 1000, cb, d)) {
                return AUTOTASK_RUNNING_STATUS_APPERROR;
            }
        }
        break;
    }
    return eStatus;
}
}
