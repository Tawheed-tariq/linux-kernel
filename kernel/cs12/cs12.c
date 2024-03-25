#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(cs12)
{
    printk("I am exceptme creating my own system call named cs12\n");
    return 0;
}