#pragma once

// ���³����������֮���Эͬ�¼���
#define LITTLET_UPDATER_NEWNAME L"LittleTUpdater.exe.new"

// ���θ��������ı��ļ���
//#define LITTLET_UPDATE_CONTENT_FILENEWNAME L"history.txt"
#define LITTLET_UPDATE_HISTORY_FILENEWNAME L"history.txt"

// ���³����������֮���Эͬ�¼���
#define LITTLET_UPDATER_EVENTNAME L"QSOFT_LITTLET_UPDATER"

// LittleT�����̺�Shell���̼�ͨ�ŵĹܵ�����
#define LITTLET_SHELL_PIPLENAME L"\\\\.\\pipe\\LittleT_Shell_PipelName"

// LittleT�����̺�Shell���̼�ͨ�ŵĹܵ� buffer��С
const int   G_LITTLET_SHELL_PIPEL_BUFFER_SIZE = 4096;

//Ӧ�ó������ƣ�ϵͳΨһ��ÿ��ֻ������һ��ʵ��
#define LITTLET_APP_NAME L"QSoft_LittleT"

//LIttleT�����������󣬽��ᴴ��һ���¼���
// ��������ͨ�������¼��Ƿ�������ж������̵��������Ƿ��Ѿ��������
#define LITTLET_MAINWND_STARTUP_EVENTNAME L"QSoft_LittleT_MainWndEvent"

// LITTLET_SHELL_PIPLENAME
typedef struct _LittleTShell_CopyData_Param {
    ENUM_AUTOTASK_DOWHAT eDoWhat;
    char     szDoWhatParam[256];
    char     szWhenDo[256];
    char     szRemindExp[256];
    char     szResult[256];
    double      tmBegin;
    double      tmEnd;
}*LP_LittleTShell_CopyData_Param;

// littleTShell �� LittleT֮���ͨ����Ϣ
//  ��Ϣ WM_COPYDATA ��wParam Ϊ��ö���е�һ��ֵ
enum ENUM_LITTLET_SHELL_COMMANDCODE {
    LITTLET_SHELL_COMMANDCODE_ADDAUTOTASK = 1,
};

// littleTShell �� LittleT֮���ͨ����Ϣ
//  ��Ϣ WM_COPYDATA �ķ���ֵ����Ҫͨ��SendMessage������Ϣ
enum ENUM_LITTLET_SHELL_COMMANDRESULT {
    LITTLET_SHELL_COMMANDRESULT_ERROR = 0,  // ����
    LITTLET_SHELL_COMMANDRESULT_OK = 1, // �ɹ�����
};
