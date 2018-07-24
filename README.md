# mem_monitor
### 创建一个后台进程，在系统的内存(对于应用程序来说)到达某个上限时重启系统
### 计算公式 内存使用率（百分比）=100*(MemTotal-MemFree-Buffers-Cached)/MemTotal
### git clone https://github.com/LegendaryZealot/mem_monitor.git
### cd mem_monitor/src
### make
### ./app targetPer(int/(0-100)) dalay(int/ms)
