# timer
a simple-file timer program , use cron , support win , easy to use  
一个单文件的定时任务执行器，使用cron字符串定时，支持win，易于使用
***

# How to use / 如何使用
## run only one task / 运行单个定时任务 
example/例如:  
`./timer.exe -r "* 0/1 * * * *" -c "shutdown -s -t 60"`
***
## use config file (run more than one task) / 同时运行多个任务(使用配置文件)
Use the parameter ` - f `to specify the location of the configuration file, for example `/ timer. exe -f cfg. txt`  
使用参数`-f`指定配置文件，例如`./timer.exe -f cfg.txt`  

The format of the config file like this:    
配置文件的格式如下:
```
* 0/15 * * * * > shutdown -s -t 60
* * 0/1 * * * > start cmd.exe
* * * 0/1 * * > start backup.bat
```
Put simply, one task per line, use the symbol '>' to separate cron and command  
简单地说，一行一个任务，使用符号'>'分隔cron字符串和需要执行的命令

# How to bulid / 如何编译
veeeeeery simple , download all files , and use `gcc timer.cpp`,done!  
非常简单，下载文件，然后`gcc timer.cpp`，一切完成~
***
# Other / 其他
use [croncpp](github.com/mariusbancila/croncpp) and [cmdline](github.com/tanakh/cmdline) in `lib` dir  
项目依赖了[croncpp](github.com/mariusbancila/croncpp)和[cmdline](github.com/tanakh/cmdline)，在`lib`文件夹中
