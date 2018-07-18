#pragma once

#include "ExpTimer.h"


struct ReminderParam {
    int		nSeconds; // ��ǰ����������
    //int		nTaskID; // ����id
    void*	data;
    dt::time	tmExec; // �����ִ��ʱ��
    mstring sSound;	// �����ļ�·��
    mstring sMsg; // ��ʾ��Ϣ
};

class ReminderExp : public ExpTimer {
public:
    ReminderExp();
    ~ReminderExp();

    bool setExp(const mstring& exp);

    mstring remindExp()const {
        return exp_;
    }

    // ִ������ǰ����ʾ���ʽ
    // ��ʽ��
    // A=15[s|h|m];\nsound=path;\nmsg=message;\n
    // A=15[s|h|m] ִ������ǰ15[�룬�֣�ʱ]��ʾ
    // S=path; path Ϊ�����ļ�·�� ,��Ϊ����������
    // M=message; messsage Ϊ�Զ�����Ϣ����Ϊ���Զ�����Ϣ
    bool parse(const mstring&sExp, __out int &nA,
               __out wchar_t&cAUnit, __out mstring&sSound, __out mstring&sMsg);

    bool getRemindString(__out mstring& sReminderDes);

    bool setRemindTimer(const dt::time& t_exe, OnTimeoutCallback cb, void* d);

private:
    // ��ʾ���ʽ
    mstring exp_;

    // ������ʾ������
    ReminderParam   trp_;
};

