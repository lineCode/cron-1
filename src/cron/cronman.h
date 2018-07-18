#pragma once

#include "crontimer.h"
#include "singleton.h"

namespace cron {

class CronMan : public Singleton<CronMan> {
public:
    ~CronMan() {
        deinit();
    }

    bool init();

    CronTimer* get(int nID);
    CronTimer* add(const dt::time &tmBegin,const dt::time& tmEnd,
                   const mstring& szWhen,const mstring& szReminder,const mstring& szXField);
    bool edit(CronTimer* pTimer,const dt::time &tmBegin,const dt::time& tmEnd,
              const mstring& szWhen,const mstring& szReminder,const mstring& szXField);
    bool setRemindExp(CronTimer* pTimer,const mstring& pszRmdExp);

    bool destroy( CronTimer *pTimer );
    bool remove( CronTimer* pTimer );

    bool stop(CronTimer* pTimer);
    ExpTimerRunningStatus start(CronTimer *pTimer,int nTaskID);
    bool enableReminder(CronTimer*pTimer,int nTaskID,bool bEnabled);

protected:
	std::list<CronTimer*>::iterator _find(int nID);
    void deinit();

private:

private:
	std::list<CronTimer*> timers_;
};

}
