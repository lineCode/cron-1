#pragma once

#include <list>
#include <vector>

#include "stdstring.h"
typedef CStdStr<char> mstring;
typedef std::vector<mstring> StringArray;


#ifndef INVALID_ID
#define INVALID_ID (-1)
#endif

#ifndef _HasFlag
#	define _HasFlag(l,f) ((l) & (f))
#endif

#ifndef _AddFlag
#	define _AddFlag(l,f) ((l) |= (f))
#endif

#ifndef _RemoveFlag
#	define _RemoveFlag(l,f) ((l) &= ~(f))
#endif


namespace cron {
enum {
    // ˢ��autotask,
    // lParam
    //		- autoTaskID
    MWND_CMD_REFRESHAUTOTASK = 1,
    // �л�Ӧ�ó���ģʽ,
    // lParam
    //		- ENUM_APP_MODE
    MWND_CMD_ENTERAPPMODE,

    // ѡ��autotask��Ŀ
    // lParam
    //		- AutoTask ID
    EVENTWND_CMD_SELECTAUTOTASK,

    // �û�ѡ������һ������ִ��
    // lParam
    //		- AutoTask ID
    EVENTWND_NOTIFY_USERJUMPEVENTEXEC,

    // ��autotask����
    // LPQMSG_PARAM
    //		-wParam		ENUM_TASK_OPERATION
    //		-lParam		AutoTask ID
    //		-lData
    //				- ��wParamΪDELETE��ʱ�򣬴�ֵΪɾ��ԭ��ENUM_TASK_RUNNING_STATUS
    //				- ��wParamΪToggleRemind����ֵΪ���û��߽���ֵ��BOOL��
    MWND_CMD_AUTOTASKOPERATION,
    // lParam
    //		LPCWSTR -- messag
    MWND_CMD_SHOWMESSAGE,
    // �ػ�����
    // lParam
    //
    MWND_CMD_SHUTDOWN,
    // ��������
    // LParam
    // -
    MWND_CMD_REBOOT,
    // ��Ϣһ�������ʾ����
    // LParam
    // - ��ʾʱ��	��λ�루s��
    MWND_CMD_SHOWSCREENSAVER,

    // ��ʾ�Զ��������ʾ��
    // lParam
    //		- LPTASK_REMINDER_PARAM
    MWND_CMD_SHOWAUTOTASKREMIND,
    // ��ʾ�Զ��������ʾ��
    // lParam ��ʹ��
    MWND_CMD_SHOWOVERDUETASK,
    // ������
    // lParam
    //  - LPQMSG_PARAM
    //		wParam ENUM_TASK_DOWHAT
    //		lParam ����wParam ����
    MWND_CMD_DOWORK,
    // ��ʾ���
    // lParam
    //		-string: funcbar button ID�����磺"btn_funcbar_tasks"
    MWND_CMD_SHOWPANE,
    // ���¹��
    // lParam��wParam
    MWND_CMD_UPDATEAD,

    //////////////////////////////////////////////////////////////////////////
    // notifys
    // todo �����½���ɾ����״̬����
    // lParam
    //		-
    MWND_NOTIFY_TODOTASKCHANGED,
    // autotask �����½���ɾ��
    // lParam
    //		-
    MWND_NOTIFY_EVENTNUMCHANGED,

    // autoTask ����
    //		- lParam	  TaskID
    MWND_NOTIFY_AUTOTASKSTART,
    // autoTask ɾ��
    //		- lParam	  TaskID
    MWND_NOTIFY_AUTOTASKDELETE,
    // ���ڵ�����ɾ��
    //		- lParam	  TaskID
    MWND_NOTIFY_AUTOTASKOVERDUE,
    // autoTask �༭
    //		- lParam	  TaskID
    MWND_NOTIFY_AUTOTASKEDIT,
    // autoTask ��ͣ
    //		- lParam	  TaskID
    MWND_NOTIFY_AUTOTASKPAUSE,
    // autotask
    MWND_NOTIFY_AUTOTASKTOGGLEREMINDER,
    // auto Task ִ���ˣ���ʱ�����ɾ����������ʾ�ˣ�����еĻ���
    // lParam
    //		- TaskID
    MWND_NOTIFY_AUTOTASKFIRED,
    // ��������ɾ�������ã�����
    // lParam
    //		- QAutoTask* ����Ϊ NULL
    MWND_NOTIFY_OVERDUETASKCHANGED,
    // �½���һ���Զ�����
    // lParam
    //		- QAutoTask*
    MWND_NOTIFY_AUTOTASKADDED,
    // �½���һ��Ŀ¼
    // lParam
    //		- QCate*
    MWND_NOTIFY_CATEGORYADDED,
    // ������һ��Ŀ¼
    // lParam
    //		- QCate*
    MWND_NOTIFY_CATEGORYCHANGED,
    // ɾ����һ��Ŀ¼
    // lParam
    //		- QCate*
    MWND_NOTIFY_CATEGORYDELETED,

    //////////////////////////////////////////////////////////////////////////
    // �ƻ���Ŀ�仯
    // lParam
    //		- int   �µ�plan��Ŀ
    MWND_NOTIFY_PLANNUMCHANGED,
    //////////////////////////////////////////////////////////////////////////
    // QPictureLoader �Ĳ���ͼƬ��ʱ������
    // lParam
    //          int     ����ͼ������
    PICTRUELOADER_PLAYTIMER_FIRED,

    //////////////////////////////////////////////////////////////////////////
    /** planѡ��֪ͨ
    *	params:
    *		-[in]
    *          lParam  QPlan* ����Ϊnull
    **/
    VIEWPLAN_NOTIFY_PLANSELCHANGED,

    /** ���stage֪ͨ
    *	params:
    *		-[in]
    *          lParam  QStage*
    **/
    VIEWPLAN_NOTIFY_STAGEADDED,    //

    /** ɾ��stage֪ͨ
    *	params:
    *		-[in]
    *          lParam  nStageID
    **/
    VIEWPLAN_NOTIFY_STAGEDELETED,

    /** ���goal֪ͨ
    *	params:
    *		-[in]
    *          lParam  QStage*
    *              ������NULL
    **/
    VIEWPLAN_NOTIFY_STAGESELCHANGED,

    /** ���goal֪ͨ
    *	params:
    *		-[in]
    *          lParam  QGoal*
    **/
    VIEWPLAN_NOTIFY_GOALADDED,

    /** ���goal֪ͨ
    *	params:
    *		-[in]
    *          lParam  GOAL ID
    **/
    VIEWPLAN_NOTIFY_GOALDELETED,

    /** ���goal֪ͨ
    *	params:
    *		-[in]
    *          lParam  QGoal*
    *              ������NULL
    **/
    VIEWPLAN_NOTIFY_GOALSELCHANGED,

    /** ���goalitem֪ͨ
    *	params:
    *		-[in]
    *          lParam  QGoalitem*
    **/
    VIEWPLAN_NOTIFY_GOALSUBITEMADDED,

    /** ɾ��goalitme֪ͨ
    *	params:
    *		-[in]
    *          lParam  QGoal* (goalitem�ĸ���ָ��)
    **/
    VIEWPLAN_NOTIFY_GOALSUBITEMDELETED,

    /** goalitem״̬�仯֪ͨ
    *	params:
    *		-[in]
    *          lParam  QGoalitem*
    **/
    VIEWPLAN_NOTIFY_GOALSUBITEMSTATUSCHANGED,
};

enum ENUM_SSTYLE_TYPE {
    STYLE_TYPE_NOTSET = 0,
    STYLE_TYPE_COLOR = 1,
    STYLE_TYPE_GRADIENT = 2,
    STYLE_TYPE_IMAGE = 3,
    STYLE_TYPE_IMAGEANDCOLOR = 4,
};

//////////////////////////////////////////////////////////////////////////
// ��������
enum ENUM_TASK_DOWHAT {
    TASK_DO_NOTSET = 0,	// δ����
    TASK_DO_LONGPLAN = 1,	// ���ڵĵ���ƻ�
    TASK_DO_REMIND = 2, // ��ʾ��Ϣ
    TASK_DO_EXECPROG = 3,  // ִ�г���
    TASK_DO_SYSSHUTDOWN = 4,  // �ػ�
    TASK_DO_SYSREBOOT = 5,  // ����
    TASK_DO_BREAKAMOMENT = 6, // ��Ϣһ���
};

enum ENUM_TASK_OPERATION {
    TASK_OP_START,		// ����
    TASK_OP_PAUSE,		// ��ͣ
    TASK_OP_DELETE,		// ɾ��
    TASK_OP_EDIT,		// �༭
    TASK_OP_JUMPOVEREXEC, // ��������ִ��
    TASK_OP_TOGGLEREMIND, // ��ֹ��ʾ
};

enum ENUM_TASK_EXECFLAG {
    TASK_EXEC_NOTSET = 0,	// ϵͳ����
    // ���ʱ��
    TASK_EXEC_AFTERSYSBOOT = 0x00000001,	// ϵͳ����
    TASK_EXEC_AFTERTASKSTART = 0x00000002,	// ��������
    TASK_EXEC_AFTERMINDERSTART = 0x00000004,// ����������
    TASK_EXEC_AFTERPROGSTART = 0x00000008,// �ⲿ��������
    TASK_EXEC_AFTERPROGEXIT = 0x00000010,// �ⲿ�����˳�
    // ����ʱ����
    TASK_EXEC_ATDATE = 0x00010000,	// �������� 2011/11/11
    TASK_EXEC_ATDAILY = 0x00020000,	// ÿ��x��
    TASK_EXEC_ATMONTHDAY = 0x00040000,	// ÿ�µ�x��
    TASK_EXEC_ATWEEKDAY = 0x00080000,	// ÿ�µ�x�� x[������|��һ��|����|��4��]

    //////////////////////////////////////////////////////////////////////////
    TASK_EXEC_RELATE_EXECANDTHEN = 0x01000000,	// ���ʱ��֮���ٴ�ִ�ж�μ��

};

// ����״̬
enum ENUM_TASK_STATUS {
    TASK_STATUS_NOTBEGIN = 0,	// ��δ���е���������
    TASK_STATUS_PROCESSING = 1,	// ������
    TASK_STATUS_PAUSE = 2,		// ��ͣ
    TASK_STATUS_ABORT = 3,		// ���
    TASK_STATUS_FINISH = 4,		// ����
    TASK_STATUS_OVERDUE = 5,	// ������ڣ�CurrentTime > EndTime
};

// ��������ʱ״̬
enum ENUM_TASK_RUNNING_STATUS {
    TASK_RUNNING_STATUS_BADTIMER = -2,	// ���ܽ���timer���ʽ
    TASK_RUNNING_STATUS_APPERROR = -1,	// Ӧ�ó�������˴���
    TASK_RUNNING_STATUS_OK = 0,	// ������������
    TASK_RUNNING_STATUS_NOTSTARTUP = 1,	// ����δ����
    TASK_RUNNING_STATUS_OVERDUE,	// ���������
    TASK_RUNNING_STATUS_UNTILNEXTSYSREBOOT,	// ��Ҫ�´λ��������������ִ��
    TASK_RUNNING_STATUS_UNTILNEXTMINDERREBOOT,	// ��Ҫ���������������ִ��
    TASK_RUNNING_STATUS_BASEDONEXETERNALPROG,	// �������ⲿ����û������
    //////////////////////////////////////////////////////////////////////////
    // ����ʱ��
    TASK_RUNNING_STATUS_TIMENOTMATCH,	// �޿�ִ�е�ʱ��ƥ��
    TASK_RUNNING_STATUS_NOCHANCETOEXEC,	// ��Ȼ����δ���ڣ��������µ�ʱ�������û�л�����ִ����
};

// �������ȼ�
enum ENUM_TASK_PRIORITY {
    TASK_PRIORITY_LOWEST = 1,	// ���
    TASK_PRIORITY_LOW = 2,		// ��
    TASK_PRIORITY_NOMAL = 3,	// ����
    TASK_PRIORITY_HIGH = 4,		// ��
    TASK_PRIORITY_HIGHEST = 5,	// ������ȼ�
};

enum ENUM_WORK_TYPE {
    WORK_TYPE_AUTOTASKFIRED,
};

enum ENUM_SSTYLE_WHICH {
    STYLE_WHICH_NOTSET = 0,
    STYLE_WHICH_BACKGROUND = 1,
    STYLE_WHICH_FOREGROUND = 2,
};

enum ENUM_CONTACT_FLAG {
    CONTACT_FLAG_REMINDBIRTHDAY = 0x00000001,
};

enum ENUM_EVENT_TRACK {
    TRACK_EVENT_PROGSTARUP,
    TRACK_EVENT_PROGEXIT,
    TRACK_EVENT_SYSTEMSTARTUP,
    TRACK_EVENT_SYSTEMEXIT,
};


enum ENUM_AUTOTASK_FLAG {
    AUTOTASK_FLAG_PAUSED = 0x1,		// ��ִͣ��

    AUTOTASK_FLAG_CONTACTBIRTHDAY = 0x10000000,		// ��ϵ��������ʾ��
};

//////////////////////////////////////////////////////////////////////////
// ��������
enum ENUM_AUTOTASK_DOWHAT {
    AUTOTASK_DO_NOTSET = 0,	// δ����
    AUTOTASK_DO_REMIND = 2, // ��ʾ��Ϣ
    AUTOTASK_DO_EXECPROG = 3,  // ִ�г���
    AUTOTASK_DO_SYSSHUTDOWN = 4,  // �ػ�
    AUTOTASK_DO_SYSREBOOT = 5,  // ����
    AUTOTASK_DO_BREAKAMOMENT = 6, // ��Ϣһ���
};

enum ENUM_AUTOTASK_OPERATION {
    AUTOTASK_OP_START,		// ����
    AUTOTASK_OP_PAUSE,		// ��ͣ
    AUTOTASK_OP_DELETE,		// ɾ��
    AUTOTASK_OP_EDIT,		// �༭
    AUTOTASK_OP_JUMPOVEREXEC, // ��������ִ��
    AUTOTASK_OP_TOGGLEREMIND, // ��ֹ��ʾ
};


enum ExpTimerExecFlag {
    AUTOTASK_EXEC_NOTSET = 0,	// ϵͳ����
    // ���ʱ��
    AUTOTASK_EXEC_AFTERSYSBOOT = 0x00000001,	// ϵͳ����
    AUTOTASK_EXEC_AFTERTASKSTART = 0x00000002,	// ��������
    AUTOTASK_EXEC_AFTERMINDERSTART = 0x00000004,// ����������
    AUTOTASK_EXEC_AFTERPROGSTART = 0x00000008,// �ⲿ��������
    AUTOTASK_EXEC_AFTERPROGEXIT = 0x00000010,// �ⲿ�����˳�
    // ����ʱ����
    AUTOTASK_EXEC_ATDATE = 0x00010000,	// �������� 2011/11/11
    AUTOTASK_EXEC_ATDAILY = 0x00020000,	// ÿ��x��
    AUTOTASK_EXEC_ATMONTHDAY = 0x00040000,	// ÿ�µ�x��
    AUTOTASK_EXEC_ATWEEKDAY = 0x00080000,	// ÿ�µ�x�� x[������|��һ��|����|��4��]
    AUTOTASK_EXEC_ATYEARDAY = 0x00100000,	// ÿ���ĳ������ 2011~2020 �� [11/11]��������һ����ǰ������������������

    //////////////////////////////////////////////////////////////////////////
    AUTOTASK_EXEC_RELATE_EXECANDTHEN = 0x01000000,	// ���ʱ��֮���ٴ�ִ�ж�μ��
};

// ��������ʱ״̬
enum TimerRunningStatus {
    kBadTimer = -2,	// ���ܽ���timer���ʽ
    kAppError = -1,	// Ӧ�ó�������˴���
    kOk = 0,	// ������������
    kNotStartup = 1,	// ����δ����
    kPaused,	// ����δ����
    kOverdue,	// ���������
    kUntilNextSysReboot,	// ��Ҫ�´λ��������������ִ��
    kUntilNextAppReboot,	// ��Ҫ���������������ִ��
    kBasedOnExternalApp,	// �������ⲿ����û������
    // ����ʱ��
    kTimeNotMatch,	// �޿�ִ�е�ʱ��ƥ��
    kNoChanceExec,	// ��Ȼ����δ���ڣ��������µ�ʱ�������û�л�����ִ����
};

}

