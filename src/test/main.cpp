#include <iostream>
#include <chrono>
#include "cron/exptimer.h"
// #include <xlocale>

using namespace cron;

class MyTask {
public:
	MyTask() {
		std::cout << "MyTask Instance created:" << dt::format_time(dt::now(), true) << std::endl;
	}

	void doTask() {
		std::cout << "do task:" << dt::format_time(dt::now(), true) <<std::endl;
	}
};

int main() {
	// std::cout.imbue(std::locale("chs"));

	MyTask a_task;
	// ���������5sִ��һ�Σ�Ȼ��ÿ��2sִ��һ�Σ�����ִ��5�κ�ֹͣ
	if (auto t = ExpTimer::create("R=2;P=5s; Q = 2s; C = 5;")) {
		std::cout << "timer description:" << t->description() << std::endl;
		auto tm_now = dt::now();
		t->startFrom(tm_now, [](void *p) {
			reinterpret_cast<MyTask*>(p)->doTask();
		}, &a_task);
	}

	// ��Ҫ������߳������˳����Թ۲�Task��ִ��
	std::this_thread::sleep_for(std::chrono::minutes(1));
}

