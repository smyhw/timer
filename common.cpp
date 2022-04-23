
#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>


namespace ark_lib{

    std::string int2string(int input){
        std::string out;
        char tmp1[1024];
        sprintf(tmp1,u8"%d",input);
        out = tmp1;

        return out;
    }

    class log{
        public:
            static bool akl_init(std::string url);
            static void info(std::string txt);
            static void warning(std::string txt);
            static void set_logfile(std::string url);
        private:
            static void to_disk(std::string txt);
            static std::string log_file;
    };

    std::string log::log_file = "none";

    void log::info(std::string txt){
        txt = u8"[*]" + txt;
        std::cout<< txt.c_str() << std::endl;
        log::to_disk(txt);
    }

    void log::warning(std::string txt){
        txt = u8"[w]" + txt;
        std::cout<< txt.c_str() << std::endl;
        log::to_disk(txt);
    }
    void log::to_disk(std::string txt){
        if(log_file=="none"){return;}
        std::ofstream ofs;
        ofs.open(log_file,std::ios::app);
        ofs << txt << u8"\n";
        ofs.close();
    }
    void log::set_logfile(std::string url){
        log_file = url;
    }
}