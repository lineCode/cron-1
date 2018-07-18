
#include "reminderexp.h"


namespace cron {

ReminderExp::ReminderExp() {
}

ReminderExp::~ReminderExp() {
}

bool ReminderExp::setExp(const mstring& exp) {
    exp_ = exp;
    return true;
}

bool ReminderExp::parse(const mstring&sExp, __out int &nA,
                        __out wchar_t&cAUnit, __out mstring&sSound, __out mstring&sMsg) {
    wchar_t cProp;
    mstring sPart, sValue, sTemp = sExp;
    int idx;
    while (true) {
        idx = sTemp.Find(";\n");
        if (-1 == idx)
            return false;
        sPart = sTemp.Left(idx + 1);
        sTemp = sTemp.Mid(idx + 2);
        if (!_parse_prop_val(sPart, cProp, sValue))
            return false;
        switch (cProp) {
        case L'A': {
            if (!helper::parseUnitTime(sValue, nA, cAUnit) || (nA <= 0))
                return false;
            break;
        }
        case L'S': {	// sound
            sSound = sValue;
            break;
        }
        case L'M': {
            sMsg = sValue;
            break;
        }
        default:
            return false;
        }
        if (sTemp.IsEmpty())
            break;
    }
    return true;
}

bool ReminderExp::getRemindString(__out mstring& sReminderDes) {
    int nA;
    wchar_t cUnit;
    mstring sSound, sMsg;
    if (parse(exp_, nA, cUnit, sSound, sMsg)) {
        sReminderDes.Format(
            "��ִ��ǰ:%d %s. ��������:%s. ��ʾ��Ϣ:%s",
            nA, helper::timeUnitStr(cUnit),
            sSound.IsEmpty() ? "��" : sSound,
            sMsg.IsEmpty() ? "��" : sMsg);
        return true;
    }
    return false;
}

// �˺���ִ���������������Ժ���
// ������ֽ�ϻ�һ�������꣬�����3��ʱ���������˼·����ʾʱ�䣬����ִ��ʱ�䣬����ʱ��
// �ƶ�����ʱ���������
bool ReminderExp::setRemindTimer(const dt::time& t_exe, OnTimeoutCallback cb, void* d) {
    dt::time t_now = dt::now();
    if (dt::total_seconds(t_exe - t_now) < 5) {
        // ��������ִ��ʱ��̫���ˣ��Ͳ���ʾ�ˡ�
        return false;
    }

    wchar_t unit = '\0';
    int nA = 0;
    mstring sound, msg;
    if (!parse(exp_, nA, unit, sound, msg)) {
        return false;
    }

    trp_.nSeconds = helper::howManySecs(nA, unit);
    ASSERT(trp_.nSeconds > 0);
    trp_.data = d;
    trp_.sSound = sound;
    trp_.sMsg = msg;
    trp_.tmExec = t_exe;

    // ��ʱӦ����ʾʱ��
    dt::time tmRemind = t_exe - dt::secs(trp_.nSeconds);
    if (t_now > tmRemind) {
        // �Ѿ�������ʾʱ����
        // Ӧ��������ʾ
        trp_.nSeconds = dt::total_seconds(t_exe - t_now);   // ��������ִ��ʱ���ж�����

        if (cb) {
            cb(&trp_);
        }
        return true;
    }

    // ��û�е���ʾʱ��
    // ���廹�ж�����ȥ��ʾ�أ�
    uint32_t dwSecToRemind = dt::total_seconds(tmRemind - t_now);
    if (dwSecToRemind < 5) {
        // ������ʾʱ�仹��5�룬̫������ֱ����ʾ��ʾ���
        // ���ǵ���ʱӦ�ü������ʱ��
        trp_.nSeconds += dwSecToRemind;
        if (cb) {
            cb(&trp_);
        }
        return true;
    }

    trp_.nSeconds = helper::howManySecs(nA, unit);
    // ����������ʾ��ʱ�仹�㹻������ô��Ҫ����һ����ʱ���ص�����
    // ���ص�����ʱ���򴰿ڷ���Ϣ��֪ͨ��ʾ��ʾ����
    // �������ζ�ʱ����ִ����ʾ�ص�

    // ���ζ�ʱ��
    return setTimer(dwSecToRemind * 1000, 0, cb, &trp_);
}
}
