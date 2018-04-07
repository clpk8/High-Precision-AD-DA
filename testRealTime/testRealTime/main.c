//
//  main.c
//  testRealTime
//
//  Created by linChunbin on 4/6/18.
//  Copyright © 2018 linChunbin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>        // standard symbolic constants and types (e.g. NULL); usleep
#include <wiringPi.h>    // needed for the wiringPi functions
#include <unistd.h>
#include <sched.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <stdint.h>


#define MY_PRIORITY 51 //above kernel
#define LED1  8        // wiringPi number corresponding to GPIO2.
#define LED2  9 //yellow
#define LED3  7 //green
#define LED4  21 //green
#define P1    27 //pushbutton 1
int count = 0;

int main(int argc, const char * argv[]) {
   struct sched_param param;
    param.sched_priority = 51;
    int check = sched_setscheduler(0, SCHED_FIFO, &param); //using FIFO
    //check error
    if(check < 0){
        printf("Scheduler error\n");
        exit(-1);
    }
    
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if(timer_fd < 0){
        printf("Create timer error\n");
        exit(-1);
    }
    
    //set timmer
    struct itimerspec itval;
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec = 1300000;//period of 1.3 ms
    
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_nsec = 100;//start a little bit late first time
    
    timerfd_settime(timer_fd, 0, &itval, NULL);
    
    //read to get it sync
    uint64_t num_periods = 0;
    long check1 = read(timer_fd, &num_periods, sizeof(num_periods));
    if(check1 < 0){
        printf("Readfile\n");
    }
    
    if(num_periods > 1){
        puts("MISSED WINDOW1\n");
    }
    
    while (1) {
        long check1 = read(timer_fd, &num_periods, sizeof(num_periods));
        if(check1 < 0){
            printf("Readfile\n");
        }
        
        if(num_periods > 1){
            puts("MISSED WINDOW2\n");
        }
        count++;
        printf("count:%d",count);
        
    }
    
    return 0;
}
