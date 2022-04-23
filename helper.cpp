#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "timer.h"
#include "./lib/croncpp.h"
#include "./lib/cmdline.h"
#include "common.cpp"


namespace ark_timer_helper{
    
    void thread_cmd(std::string cmd){
        ark_lib::log::info(u8"执行指令 -> "+cmd);
        system(cmd.c_str());
    }

    //创建一个任务
    //会检测cron字符串是否合法
    bool create_task(std::string cron,std::string cmd){
        try{cron::make_cron(cron);}
        catch (cron::bad_cronexpr const & ex){ark_lib::log::warning(u8"参数cron错误，不能解析为cron字符串 -> "+(*ex.what()));}
        task_list.push_back(timer_task(cmd,cron));
        ark_lib::log::info(u8"创建任务 -> "+cron+" / "+cmd);
        return true;
    }

    std::string get_show_time(time_t time){
        char buffer[1024];
        struct tm* ttt = gmtime(&time);
        int fk = strftime(buffer,1024,u8"公元纪年 UTC %Y %m %d %X",ttt);
        std::string re = buffer;
        return re;
    }

    //检测给定时间是否符合这个cron字符串
    bool handle_cron(std::string cron_string,time_t do_time){
        bool is_att = 0;
        time_t next;
        try{
            //预测下一次时间
            auto cron = cron::make_cron(cron_string.c_str());
            next = cron::cron_next(cron, do_time-1);
        }catch (cron::bad_cronexpr const & ex){
            std::string err = ex.what();
            ark_lib::log::warning(u8"cron错误 -> "+err);
            return false;
        }

        if( next == do_time ){
            ark_lib::log::info(u8"======时间匹配成功======");
            ark_lib::log::info(u8"cron字符串 \t-> "+cron_string);
            ark_lib::log::info(u8"匹配时间 \t-> "+ark_timer_helper::get_show_time(do_time));
            return true;
        }
        return false;
    }

    //解析命令行参数
    void parse_args(int argc, char *argv[]){
        ark_lib::log::info(u8"开始处理配置...");
        cmdline::parser pr;
        pr.add<std::string>("cron", 'r', u8"cron字符串", false,"none");
        pr.add<std::string>("cmd", 'c', u8"要执行的shell命令", false,"none");
        pr.add<std::string>("log", 'l', u8"日志文件位置(如果需要写入日志到文件)", false,"none");
//        pr.add<std::string>("mode", 'm', u8"运行模式\v单文件模式(s)\t使用命令行参数r和c运行单个任务\n\t\t\t部署模式(d)\t将创建并读取配置文件\n\t\t", true, "s", cmdline::oneof<std::string>("d", "s"));
        pr.add<std::string>("file", 'f', u8"从指定文件加载任务(格式:\"* 0/10 * * * * > start cmd.exe -k whoami\";即以\'>\'分隔cron字符串和需要执行指令，一行一个)", false,"none");
        pr.parse_check(argc, argv);

        if(pr.get<std::string>("cron") != "none"){
            ark_timer_helper::create_task(pr.get<std::string>("cron"),pr.get<std::string>("cmd"));
        }

        if(pr.get<std::string>("file") != "none"){
            //TODO 读取文件并创建任务
            std::ifstream ifs;
            ifs.open(pr.get<std::string>("file"),std::ios::in);
            char line[1024];
            while(ifs.getline(line,1024)){
                std::string cron = "";
                std::string cmd = "";
                int zt = 0;//正在读取cron还是cmd?
                for(int num=0;line[num]!='\n';num++){
                    if(line[num]=='>'){num++;zt=1;}
                    if(zt==1){
                        cmd+=line[num];
                    }else{
                        cron+=line[num];
                    }
                }
                //得到cron和cmd
                ark_timer_helper::create_task(cron,cmd);
            }
            ifs.close();
        }

        if(pr.get<std::string>("log") != "none"){
            ark_lib::log::set_logfile(pr.get<std::string>("log"));
        }

        ark_lib::log::info(u8"配置处理完毕...");
    }
}