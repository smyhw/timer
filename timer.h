#pragma once
#include <list>
#include <thread>

//程序状态
//2 = 正常运行
//4 - 测试命令测试
int proc_status;
time_t proc_time;//处理时间

class timer_task{
    private:
    public:
        timer_task(std::string command_a,std::string timer_string_a);
        int do_it();
        std::string command;
        std::string timer_string;
        std::thread last_run;
};
std::list<timer_task> task_list;//任务列表