#pragma once

namespace cron {

	inline mstring getRunningStatusDescription(TimerRunningStatus eStatus) {
		switch (eStatus) {
		case TimerRunningStatus::kBadTimer://-2://	// ���ܽ���timer���ʽ
			return "��Ч�Ķ�ʱ��";
		case TimerRunningStatus::kAppError://-1://	// Ӧ�ó�������˴���
			return "Ӧ�ó������";
		case TimerRunningStatus::kOk://0://	// ������������
			return "һ������";
		case TimerRunningStatus::kNotStartup://1://	// ����δ����
			return "����δ����";
		case TimerRunningStatus::kPaused: // ��ͣ��
			return "������ͣ";
		case TimerRunningStatus::kOverdue://	// ���������
			return "�������";
		case TimerRunningStatus::kUntilNextSysReboot://	// ��Ҫ�´λ��������������ִ��
			return "�´ο���ִ��";
		case TimerRunningStatus::kUntilNextAppReboot:	// ��Ҫ���������������ִ��
			return "�´�������������";
		case TimerRunningStatus::kBasedOnExternalApp://	// �������ⲿ����û������
			return "�������ⲿ����û������";
			//////////////////////////////////////////////////////////////////////////
			// ����ʱ��
		case TimerRunningStatus::kTimeNotMatch://	// �޿�ִ�е�ʱ��ƥ��
			return "�޿�ִ�е�ʱ��ƥ��";
		case TimerRunningStatus::kNoChanceExec://	// ��Ȼ����δ���ڣ��������µ�ʱ�������û�л�����ִ����
			return "����û�л�����ִ��";
		default:
			return "δ֪��־";
		}
	}

	inline mstring getExecFlagText(ExpTimerExecFlag eFlag) {
		switch (eFlag) {
		case AUTOTASK_EXEC_NOTSET: // 0,	// δ����
			return "δ����";
			// ���ʱ��
		case AUTOTASK_EXEC_AFTERSYSBOOT: // 0x00000001,	// ϵͳ����
			return "ϵͳ����";
		case AUTOTASK_EXEC_AFTERTASKSTART: // 0x00000002,	// ��������
			return "����������";
		case AUTOTASK_EXEC_AFTERMINDERSTART: // 0x00000004,// ����������
			return "����������";
		case AUTOTASK_EXEC_AFTERPROGSTART: // 0x00000008,// �ⲿ��������
			return "������ⲿ��������";
		case AUTOTASK_EXEC_AFTERPROGEXIT: // 0x00000010,// �ⲿ�����˳�
			return "������ⲿ�����˳�";
			// ����ʱ����
		case AUTOTASK_EXEC_ATDATE: // 0x00010000,	// �������� 2011/11/11
			return "����";
		case AUTOTASK_EXEC_ATYEARDAY: // 0x00010000,	// ������� 2011/11/11
			return "ÿ���ĳ��";
		case AUTOTASK_EXEC_ATDAILY: // 0x00020000,	// ÿ��x��
			return "ÿ��x��";
		case AUTOTASK_EXEC_ATMONTHDAY: // 0x00040000,	// ÿ�µ�x��
			return "ÿ�µ�x��";
		case AUTOTASK_EXEC_ATWEEKDAY: // 0x00080000,	// ÿ�µ�x�� x[������|��һ��|����|��4��]
			return "ÿ�µ�x��";

			//////////////////////////////////////////////////////////////////////////
		case AUTOTASK_EXEC_RELATE_EXECANDTHEN: // 0x01000000,	// ���ʱ��֮���ٴ�ִ�ж�μ��
			return "���ʱ��֮���ٴ�ִ�ж�μ��";
		}
		ASSERT(false);
		return "GetExecFlagText->δ֪����";
	}

}

