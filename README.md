<h1 align="center"> Tavaheed tariq </h1>
<h3 align="center"> 2022BITE008</h3>
<h4 align="center">Adding a system call to linux kernel and testing it using a C program</h4>

# system call

A ***system call*** is a programmatic way in which a computer program requests a service from the kernel of the operating system it is executed on. A system call is a way for programs to ***interact with the operating system***.


# Adding a system call to Linux kernel

## Installation

1. First of all before installing the new linux kernel we check the version of current running linux kernel.
```bash
$ uname -a
Linux tawheed 6.6.9-amd64 #1 SMP PREEMPT_DYNAMIC Kali 6.6.9-1kali1 (2024-01-08) x86_64 GNU/Linux
```

my current version is `6.6.9-amd64`.

Now we update the system and install some useful packages
```bash
sudo apt update && sudo apt upgrade -y
```

```bash
sudo apt-get install build-essential libncurses5-de libssl-dev libelf-dev bison flex dwarves zstd screen -y
```

2. Download the source code of latest linux kernel from [here](https://kernel.org/), if you already have then no need to download once more.
   OR 
   you can clone git repository of linux kernel
```bash
git clone https://github.com/torvalds/linux.git
```

if you installed from the `kernel.org` then you need to extract the package first, to extract the package use:

```bash
tar xf <package_name>
```

change directory to linux kernel

```bash
cd <package_name>
```

in my case it was: `cd linux`

## 1. Creation

Now after you have installed necessary packages and linux kernel, we will start adding a basic system call to linux kernel.
you are now currently in linux directory which you just downloaded, you can check its content by listing `ls` the files inside it:

![image](https://github.com/Tawheed-tariq/linux-kernel/assets/143424182/80f67a74-52a8-4316-b00e-4c95d362f68b)


1. Change your directory to kernel
   
```bash
cd kernel
```

inside kernel directory you can see lot of system calls like fork , exec . Here we will add our system call.

2. Inside kernel directory, create a folder with name same as your system call name, in my case I choose system call name to be `cs12` you can choose your own name.
   
```bash
mkdir cs12
cd cs12
```

3. Inside the `cs12` directory we will create a file name `cs12.c` 
   
```c
code cs12.c
```

I am using vs-code as a text editor you can use nano, vim or any other, based on your choice.

now write the following code to the above created file:
```c
#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(cs12)
{
    printk("I am exceptme creating my own system call named cs12\n");
    return 0;
}
```

You can also change  the code as  you like.

`SYSCALL_DEFINE0` is used for those system calls which have 0 arguments, similarly for `n` arguments we will use `SYSCALL_DEFINEn` where `n` is integer number.

also create a Makefile inside this directory
```c
code Makefile
```

write the following code inside Makefile:
```c
obj-y := cs12.o
```

3. Now we will make changes in Makefile of kernel directory, for that go to kernel directory
```bash
cd ..
code Makefile
```

inside Makefile of kernel directory , and find `obj-y`, you will find something like this:
![image](https://github.com/Tawheed-tariq/linux-kernel/assets/143424182/6464c598-9434-41ec-a955-5682cdedb863)


write this line of code anywhere you want:
```c
obj-y += cs12/
```
in my case it is written after `time/` directory (last line).

4. Now go to the linux directory using :
```bash
cd ../
```

and open this filepath in your text editor `include/linux/syscalls.h` :
```c
code include/linux/syscalls.h
```

Navigate to the bottom of it and write the following code just above `#endif`.
```c
asmlinkage long sys_cs12(void);
```

Some architectures (e.g. x86) have their own architecture-specific syscall tables, but several other architectures share a generic syscall table. Add your new system call to the generic list by adding an entry to the list in `include/uapi/asm-generic/unistd.h`:

```c
code include/uapi/asm-generic/unistd.h
```

```c
#define __NR_cs12 462
__SYSCALL(__NR_cs12, sys_cs12)
```

Also update the `__NR_syscalls` count to reflect the additional system call, and note that if multiple new system calls are added in the same merge window, your new syscall number may get adjusted to resolve conflicts.

![image](https://github.com/Tawheed-tariq/linux-kernel/assets/143424182/d4de83b7-4ade-44f3-a025-9c139c5d5628)


The file `kernel/sys_ni.c` provides a fallback stub implementation of each system call, returning `-ENOSYS`. Add your new system call here too:

```c
code kernel/sys_ni.c
```

```c
COND_SYSCALL(cs12);
```

![image](https://github.com/Tawheed-tariq/linux-kernel/assets/143424182/cfcc009e-3ba9-42bc-a4dd-28fb56dfd728)


To wire up your new system call for x86 platforms, you need to update the master syscall tables. Assuming your new system call isn't special in some way (see below), this involves a "common" entry (for x86_64 and x32) in `arch/x86/entry/syscalls/syscall_64.tbl`:

```c
code arch/x86/entry/syscalls/syscall_64.tbl
```

```c
462   common   cs12     sys_cs12
```

![image](https://github.com/Tawheed-tariq/linux-kernel/assets/143424182/cd79bf85-2714-4aac-95d2-691d0443c5b7)


and an "i386" entry in `arch/x86/entry/syscalls/syscall_32.tbl`:

```c
code arch/x86/entry/syscalls/syscall_32.tbl
```

```c
462   i386   cs12     sys_cs12
```

![image](https://github.com/Tawheed-tariq/linux-kernel/assets/143424182/375ecceb-7de2-4455-b3d4-b7493a73662d)


Again, these numbers are liable to be changed if there are conflicts in the relevant merge window.

## 2. Building Up

1. Copy the config file of your current kernel version to linux directory
```bash
cp -v /boot/config-$(uname -r) .config
```

2. Configure the kernel.
Make sure the window of your terminal is maximized.
Open the configuration window with the following command.

```bash
sudo make menuconfig
```

save and exit.

3. Edit the `.config` file
   
```c
code .config
```

- Set CONFIG_SYSTEM_TRUSTED_KEYS to ""
- Set CONFIG_SYSTEM_REVOCATION_KEYS to ""

4. Compile the linux kernel  
   
```bash
sudo make -j$(nproc)
```

this will take time , in my case it took about 1 hour and 20 mins to compile.

5. Install the linux kernel modules
   
```bash
sudo make modules_install -j$(nproc)
```

```bash
sudo make install -j$(nproc)
```

6. Update grub config
   
```bash
sudo update-initramfs -c -k 6.9.0-rc1
sudo update-grub
```

i used here `6.9.0-rc1` because I downloaded this linux kernel version , you will write your own linux kernel version there

7. Reboot the device
   
   you can reboot manually or used this command to reboot:
```bash
sudo reboot
```

## Result

Now after rebooting the system you can use this command to check your kernel version:
```bash
$ uname -a  
Linux tawheed 6.9.0-rc1 #1 SMP PREEMPT_DYNAMIC Mon Mar 25 12:17:01 IST 2024 x86_64 GNU/Linux
```

in my case kernel version is changed to `6.9.0-rc1`.

#### Now we will check that the system call we added is working or not

create a c file and store it where ever you want to store it, I name the file as `report.c`.
Write the following code inside it:

```c
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define __NR_cs12 462

long cs12_syscall(void){
	return syscall(__NR_cs12);
}

int main(){
	long activity;
	activity = cs12_syscall();
	if(activity != 0){
		perror("system call cs12 appears to have failed.");
	}
	else{
		system("dmesg");
	}
	return 0;
}
```

run the file using gcc.

```c
gcc -o report report.c
```

```c
./report
```

check the last line of output.
At the bottom, you should now see the following.

![image](https://github.com/Tawheed-tariq/linux-kernel/assets/143424182/cc85253a-8742-4896-9f2b-9694c2cafc85)


