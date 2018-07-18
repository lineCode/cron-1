#pragma once

#include <list>
#include "exarray.h"
#include "constvalues.h"
#include "timerinterface.h"
#include "reminderexp.h"

// enum ENUM_AUTOTASK_EXECFLAG
// {
//     AUTOTASK_EXEC_NOTSET	= 0,	// ϵͳ����
//     // ���ʱ��
//     AUTOTASK_EXEC_AFTERSYSBOOT	= 0x00000001,	// ϵͳ����
//     AUTOTASK_EXEC_AFTERTASKSTART = 0x00000002,	// ��������
//     AUTOTASK_EXEC_AFTERMINDERSTART = 0x00000004,// ����������
//     AUTOTASK_EXEC_AFTERPROGSTART = 0x00000008,// �ⲿ��������
//     AUTOTASK_EXEC_AFTERPROGEXIT = 0x00000010,// �ⲿ�����˳�
//     // ����ʱ����
//     AUTOTASK_EXEC_ATDATE = 0x00010000,	// �������� 2011/11/11
//     AUTOTASK_EXEC_ATDAILY = 0x00020000,	// ÿ��x��
//     AUTOTASK_EXEC_ATMONTHDAY = 0x00040000,	// ÿ�µ�x��
//     AUTOTASK_EXEC_ATWEEKDAY = 0x00080000,	// ÿ�µ�x�� x[������|��һ��|����|��4��]
//
//     //////////////////////////////////////////////////////////////////////////
//     AUTOTASK_EXEC_RELATE_EXECANDTHEN = 0x01000000,	// ���ʱ��֮���ٴ�ִ�ж�μ��
//
// };
//
// // ��������ʱ״̬
// enum ENUM_AUTOTASK_RUNNING_STATUS
// {
//     AUTOTASK_RUNNING_STATUS_BADTIMER = -2,	// ���ܽ���timer���ʽ
//     AUTOTASK_RUNNING_STATUS_APPERROR = -1,	// Ӧ�ó�������˴���
//     AUTOTASK_RUNNING_STATUS_OK = 0,	// ������������
//     AUTOTASK_RUNNING_STATUS_NOTSTARTUP = 1,	// ����δ����
//     AUTOTASK_RUNNING_STATUS_PAUSED,	// ����δ����
//     AUTOTASK_RUNNING_STATUS_OVERDUE,	// ���������
//     AUTOTASK_RUNNING_STATUS_UNTILNEXTSYSREBOOT,	// ��Ҫ�´λ��������������ִ��
//     AUTOTASK_RUNNING_STATUS_UNTILNEXTMINDERREBOOT,	// ��Ҫ���������������ִ��
//     AUTOTASK_RUNNING_STATUS_BASEDONEXETERNALPROG,	// �������ⲿ����û������
//     //////////////////////////////////////////////////////////////////////////
//     // ����ʱ��
//     AUTOTASK_RUNNING_STATUS_TIMENOTMATCH,	// �޿�ִ�е�ʱ��ƥ��
//     AUTOTASK_RUNNING_STATUS_NOCHANCETOEXEC,	// ��Ȼ����δ���ڣ��������µ�ʱ�������û�л�����ִ����
// };

//
// // �������ȼ�
// enum ENUM_TASK_PRIORITY
// {
// 	TASK_PRIORITY_LOWEST = 1,	// ���
// 	TASK_PRIORITY_LOW = 2,		// ��
// 	TASK_PRIORITY_NOMAL = 3,	// ����
// 	TASK_PRIORITY_HIGH = 4,		// ��
// 	TASK_PRIORITY_HIGHEST = 5,	// ������ȼ�
// };
//

class ExpTimer;

enum ENUM_TIMER_FLAG {
    TIMER_FLAG_REMINDERDISABLED = 0x00000001,	// ������ʾ��ʱ��
};

mstring getRunningStatusDescription(ExpTimerRunningStatus eStatus);
mstring getExecFlagText(ExpTimerExecFlag eFlag);

class CronMan;
class CronTimer {
    friend class CronMan;
    enum ENUM_TIMER_TYPE {
        TIMER_TYPE_NOTSET = 0,
        TIMER_TYPE_RELATE,
        TIMER_TYPE_ABSOLUTE,
    };

public:
    CronTimer(void);

    CronTimer(int nID,dt::time tmBegin,dt::time tmEnd,
              const mstring& szWhen,const mstring& szReminder,const mstring& szXFiled);

    ~CronTimer(void);

    // ��������
    bool update(dt::time tmB, dt::time tmEnd, const mstring& szExpWhen, const mstring& szExpRemind, const mstring& szExpXFiled);

    // ��ʱ���Ƿ��Ѿ�����
    bool started()const {
        return false;
    }

    bool stop();

    ExpTimerRunningStatus start(int nTaskID);

    bool reminderEnabled()const {
        return !(timer_flag_ & TIMER_FLAG_REMINDERDISABLED);
    }

    bool enableReminder( int nTaskID,bool bEnable=true );

    int32_t ID()const {
        return 1;
    }
protected:
    mstring xFiled()const {
        return xfield_;
    }

private:
    mstring			xfield_;	//
    uint32_t			timer_flag_;

    ReminderExp* reminder_;
    ExpTimer* timer_ = nullptr;
};
