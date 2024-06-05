#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched/signal.h>

SYSCALL_DEFINE0(get_process_count) {
    int count = 0;
    struct task_struct *task;
    for_each_process(task) {
        count++;
    }
    return count;
}
