#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#define BUF_SIZE 128
#define NAME_SIZE 128
#define VALUE_SIZE 128
#define MEMTOTAL "MemTotal:"
#define MEMFREE "MemFree:"
#define MEMBUFFERS "Buffers:"
#define MEMCACHED "Cached:"


void Sleep(int ms);


int targetPersent;
int delay;
char buf[BUF_SIZE];

int main(int argc, char const *argv[])
{
    if(3!=argc)
    {
        perror("arg error:int:targetPersent(0-100) int:delay(ms)");
        exit(-1);
    }
    memset(buf,'0',sizeof(buf));
    sprintf(buf,"%s %s",argv[1],argv[2]);
    if(2!=sscanf(buf,"%d %d",&targetPersent,&delay))
    {
        perror("arg error:int:targetPersent(0-100) int:delay(ms)");
        exit(-1);
    }
    int forkCode=fork();
    if(-1==forkCode)
    {
        printf("fork error!\n");
    }
    if(0==forkCode)
    {
        TryToReboot();
    }
    if(0<forkCode)
    {
        printf("created pid:%d\n",forkCode);
        exit(0);
    }
    return 0;
}

void TryToReboot()
{
    int curr_persent;

    long mem_total;
    long mem_free;
    long mem_buffers;

    long mem_cached;
    char name[NAME_SIZE];
    char value[VALUE_SIZE];

    while(1)
    {    
        FILE *fp=fopen("/proc/meminfo","r");
        if(fp)
        {
            while(memset(buf,'0',BUF_SIZE),fgets(buf,BUF_SIZE,fp))
            {
                memset(name,'0',NAME_SIZE);
                memset(value,'0',VALUE_SIZE);
                sscanf(buf,"%s %s",name,value);
                if(0==strcmp(MEMTOTAL,name))
                {
                    mem_total=atol(value);
                }
                if(0==strcmp(MEMFREE,name))
                {
                    mem_free=atol(value);
                }
                if(0==strcmp(MEMBUFFERS,name))
                {
                    mem_buffers=atol(value);
                }
                if(0==strcmp(MEMCACHED,name))
                {
                    mem_cached=atol(value);
                }
            }
        }
        fclose(fp);
        curr_persent=(int)(100.0*(mem_total-mem_buffers-mem_cached-mem_free)/mem_total);
        if(curr_persent>targetPersent)
        {
            system("reboot");
        }
        Sleep(delay);
    }
}

void Sleep(int ms)  
{  
    struct timeval tmp;  
    tmp.tv_sec = 0;  
    tmp.tv_usec = ms * 1000; 
    select(0, NULL, NULL, NULL, &tmp);  
}