#pragma once

namespace cron {

	namespace dict {

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

		inline mstring getExecFlagText(TimerExecType eFlag) {
			switch (eFlag) {
			case TimerExecType::kNotSet: // 0,	// δ����
				return "δ����";
				// ���ʱ��
			case TimerExecType::kAfterSysBoot: // 0x00000001,	// ϵͳ����
				return "ϵͳ����";
			case TimerExecType::kAfterTimerStart: // 0x00000002,	// ��������
				return "����������";
			case TimerExecType::kAfterAppStart: // 0x00000004,// ����������
				return "����������";
			case TimerExecType::kAfterExternalAppStart: // 0x00000008,// �ⲿ��������
				return "������ⲿ��������";
			case TimerExecType::kAfterAppExit: // 0x00000010,// �ⲿ�����˳�
				return "������ⲿ�����˳�";
				// ����ʱ����
			case TimerExecType::kAtDate: // 0x00010000,	// �������� 2011/11/11
				return "����";
			case TimerExecType::kAtYearDay: // 0x00010000,	// ������� 2011/11/11
				return "ÿ���ĳ��";
			case TimerExecType::kAtDaily: // 0x00020000,	// ÿ��x��
				return "ÿ��x��";
			case TimerExecType::kAtMonthDay: // 0x00040000,	// ÿ�µ�x��
				return "ÿ�µ�x��";
			case TimerExecType::kAtWeekday: // 0x00080000,	// ÿ�µ�x�� x[������|��һ��|����|��4��]
				return "ÿ�µ�x��";

				//////////////////////////////////////////////////////////////////////////
			case TimerExecType::kExecAndThen: // 0x01000000,	// ���ʱ��֮���ٴ�ִ�ж�μ��
				return "���ʱ��֮���ٴ�ִ�ж�μ��";
			}
			ASSERT(false);
			return "GetExecFlagText->δ֪����";
		}

		inline mstring timeUnitStr(char unit) {
			switch (unit) {
			case 's':
			case 'S':
				return "��";
			case 'm':
			case 'M':
				return "��";
			case 'h':
			case 'H':
				return "ʱ";
			default:
				ASSERT(false);
				return "��Ч��λ";
			}
		}


	}

}

