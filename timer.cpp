
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <list>
#include <thread>
#include <chrono>

#include "timer.h"
#include "common.cpp"
#include "./lib/cmdline.h"
#include "helper.cpp"


timer_task::timer_task(std::string command_a,std::string timer_string_a){
    this->command = command_a;
    this->timer_string = timer_string_a;
}
int timer_task::do_it(){
    std::thread threadA(ark_timer_helper::thread_cmd,this->command);
    threadA.detach();
    return 0;
}

void prc_main_loop(){

    while(proc_status==2){
//        ark_lib::log::warning(u8"now time id"+ark_lib::int2string(proc_time));
        //获取当前时间
        time_t now_time;
        now_time = time(NULL);
        if(proc_time<now_time){
            ark_lib::log::warning(u8"性能延迟 -> "+ark_lib::int2string(now_time-proc_time));
        }
        while(proc_time==now_time){
            now_time = time(NULL);
        }
        proc_time++;
        //遍历任务列表
        std::list<timer_task>::iterator itr;
        for(itr = task_list.begin() ; itr!=task_list.end() ; itr++){
            bool re = ark_timer_helper::handle_cron(itr->timer_string,proc_time);
            if(re){
                itr->do_it();
            }
        }
        
    }
    return;
}

int main(int argc, char *argv[]){
    system("chcp 65001");
    ark_lib::log::info(u8"start timer...");
    proc_status = 2;
    ark_timer_helper::parse_args(argc,argv);
    proc_time = time(NULL);
    
    ark_lib::log::info(u8"现在时间 -> "+ark_timer_helper::get_show_time(proc_time));
    //
//    task_list.push_back(timer_task("start cmd /k \"title w1\"","0/13 * * * * *"));
//    task_list.push_back(timer_task("start cmd /k \"title w2\"","0/15 * * * * *"));
    //
    if(task_list.empty()){
        ark_lib::log::warning(u8"没有任何任务，程序退出...");
        exit(1);
    }
    if(proc_status==4){
        ark_lib::log::warning(u8"命令测试模式 -> 执行所有命令...");
        std::list<timer_task>::iterator itr;
        for(itr = task_list.begin() ; itr!=task_list.end() ; itr++){
            itr->do_it();
        }
        ark_lib::log::warning(u8"命令测试模式 -> 所有命令执行完成，闲置中，ctrl+c以退出...");
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(100));
        }
    }
    ark_lib::log::info(u8"into main loop...");
    prc_main_loop();
    ark_lib::log::warning(u8"final");
}