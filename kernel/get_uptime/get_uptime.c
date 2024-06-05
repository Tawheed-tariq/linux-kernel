#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>

SYSCALL_DEFINE0(get_uptime) {
    struct timespec64 uptime;
    ktime_get_boottime_ts64(&uptime);
    return uptime.tv_sec;
}
