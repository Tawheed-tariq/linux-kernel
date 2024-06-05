#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/mm.h>

SYSCALL_DEFINE0(get_free_memory) {
    struct sysinfo i;
    si_meminfo(&i);
    return i.freeram;
}
