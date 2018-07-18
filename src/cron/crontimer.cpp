

#include "crontimer.h"
#include "cronman.h"

#include "exptimer.h"

namespace cron {

mstring getRunningStatusDescription( ExpTimerRunningStatus eStatus ) {
    switch(eStatus) {
    case AUTOTASK_RUNNING_STATUS_BADTIMER://-2://	// ���ܽ���timer���ʽ
        return "��Ч�Ķ�ʱ��";
    case AUTOTASK_RUNNING_STATUS_APPERROR://-1://	// Ӧ�ó�������˴���
        return "Ӧ�ó������";
    case AUTOTASK_RUNNING_STATUS_OK://0://	// ������������
        return "һ������";
    case AUTOTASK_RUNNING_STATUS_NOTSTARTUP://1://	// ����δ����
        return "����δ����";
    case AUTOTASK_RUNNING_STATUS_PAUSED: // ��ͣ��
        return "������ͣ";
    case AUTOTASK_RUNNING_STATUS_OVERDUE://	// ���������
        return "�������";
    case AUTOTASK_RUNNING_STATUS_UNTILNEXTSYSREBOOT://	// ��Ҫ�´λ��������������ִ��
        return "�´ο���ִ��";
    case AUTOTASK_RUNNING_STATUS_UNTILNEXTMINDERREBOOT:	// ��Ҫ���������������ִ��
        return "�´�������������";
    case AUTOTASK_RUNNING_STATUS_BASEDONEXETERNALPROG://	// �������ⲿ����û������
        return "�������ⲿ����û������";
    //////////////////////////////////////////////////////////////////////////
    // ����ʱ��
    case AUTOTASK_RUNNING_STATUS_TIMENOTMATCH://	// �޿�ִ�е�ʱ��ƥ��
        return "�޿�ִ�е�ʱ��ƥ��";
    case AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC://	// ��Ȼ����δ���ڣ��������µ�ʱ�������û�л�����ִ����
        return "����û�л�����ִ��";
    default:
        return "δ֪��־";
    }
}

mstring getExecFlagText( ExpTimerExecFlag eFlag ) {
    switch (eFlag) {
    case AUTOTASK_EXEC_NOTSET: // 0,	// δ����
        return "δ����";
    // ���ʱ��
    case AUTOTASK_EXEC_AFTERSYSBOOT: // 0x00000001,	// ϵͳ����
        return "ϵͳ����";
    case AUTOTASK_EXEC_AFTERTASKSTART : // 0x00000002,	// ��������
        return "����������";
    case AUTOTASK_EXEC_AFTERMINDERSTART : // 0x00000004,// ����������
        return "����������";
    case AUTOTASK_EXEC_AFTERPROGSTART : // 0x00000008,// �ⲿ��������
        return "������ⲿ��������";
    case AUTOTASK_EXEC_AFTERPROGEXIT : // 0x00000010,// �ⲿ�����˳�
        return "������ⲿ�����˳�";
    // ����ʱ����
    case AUTOTASK_EXEC_ATDATE : // 0x00010000,	// �������� 2011/11/11
        return "����";
    case AUTOTASK_EXEC_ATYEARDAY : // 0x00010000,	// ������� 2011/11/11
        return "ÿ���ĳ��";
    case AUTOTASK_EXEC_ATDAILY : // 0x00020000,	// ÿ��x��
        return "ÿ��x��";
    case AUTOTASK_EXEC_ATMONTHDAY : // 0x00040000,	// ÿ�µ�x��
        return "ÿ�µ�x��";
    case AUTOTASK_EXEC_ATWEEKDAY : // 0x00080000,	// ÿ�µ�x�� x[������|��һ��|����|��4��]
        return "ÿ�µ�x��";

    //////////////////////////////////////////////////////////////////////////
    case AUTOTASK_EXEC_RELATE_EXECANDTHEN : // 0x01000000,	// ���ʱ��֮���ٴ�ִ�ж�μ��
        return "���ʱ��֮���ٴ�ִ�ж�μ��";
    }
    ASSERT(false);
    return "GetExecFlagText->δ֪����";
}

CronTimer::CronTimer() {
}

CronTimer::CronTimer( int nID,dt::time tmBegin,dt::time tmEnd,
                      const mstring& szWhen,const mstring& szReminder,const mstring& szXFiled ) {
    VERIFY(update(tmBegin, tmEnd, szWhen, szReminder, szXFiled));
}

CronTimer::~CronTimer(void) {
}

ExpTimerRunningStatus CronTimer::start(int nTaskID) {
    auto ret = ExpTimerRunningStatus::AUTOTASK_RUNNING_STATUS_BADTIMER;
    if (timer_ && !timer_->started()) {
        dt::time next_exec = dt::now();
        ret = timer_->startFrom(next_exec, [](void* d) {
            // ִ�лص�����
            std::cout << " task timer --";

        }, &nTaskID);
        if (AUTOTASK_RUNNING_STATUS_OK == ret) {
            // ��ǰ����
            if (reminderEnabled()) {
                reminder_->setRemindTimer(next_exec, [](void* d) {
                    // ....
                    std::cout << " reminder --";
                }, &nTaskID);
            }
        }
    }
    return ret;
}

bool CronTimer::stop() {
    if (started()) {
        bool bHasReminder = false;
// 		if (timer_task_->stop()) {
// 			if (bHasReminder && (INVALID_ID != nTaskID)) {
// 				setRemindTimer(nTaskID, nextExecTime());
// 			}
// 			return false;
//         }
    }
    return true;
}

bool CronTimer::update( dt::time tmB, dt::time tmEnd, const mstring& szExpWhen,
                        const mstring& szExpRemind, const mstring& szExpXFiled) {
    ASSERT(!started());
    xfield_ = szExpXFiled;

    if (timer_ = ExpTimer::create(szExpWhen)) {
        timer_->setLifeTime(tmB, tmEnd);
//
// 		if (!reminder_->setExp(szExpRemind))
// 			return false;
    }


    return true;
}

bool CronTimer::enableReminder( int nTaskID,bool bEnable/*=true */ ) {
//     bool bRet;
//     if (bEnable) {
//         if (started()) {
//             bRet = setRemindTimer(nTaskID,m_tmNextExec);
//         }
//         m_dwTimerFlag &= ~TIMER_FLAG_REMINDERDISABLED;
//     } else {
// 		timer_reminder_->stop();
//         m_dwTimerFlag |= TIMER_FLAG_REMINDERDISABLED;
//         bRet = true;
//     }
//     return bRet;
    return false;
}
}
