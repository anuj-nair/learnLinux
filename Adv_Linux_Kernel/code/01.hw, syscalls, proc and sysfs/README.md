# Linux Kernel Fundamentals: Chapter 1, Surveying the Linux Kernel 
 
1. What kernel version is your Linux system running? 
**Ans.**
	 ```
	 uname -r
	 ```		
2. What is the size of the kernel file that corresponds to the kernel your system is running? 
**Ans.**
	 ```
	 ls -l /boot | grep "vmlinuz"	
	 ```	
3. How much RAM is available to your running kernel? Note: It may or may not be the amount of physical RAM on your system. 
**Ans.**
	 ```
	 free -h | awk '/Mem/ {print $2}'
	 ```	
4. The command strace will display the system calls that a process makes as it runs. Using the man command, determine what option for strace will show a summary, with a count, of the number of times a process called each system call. Using that option, what system call is called the most by the command date? 
**Ans.**
	 ```
		strace -c date
	 ```	
5. Can you determine, using strace, what system call is used to change the directory?  
**Ans.**
	 ```
	 strace bash -c "cd /home"
	 ```
		
6. By looking at include/uapi/asm-generic/unistd.h determine about how many system calls are defined in your kernel source. 
**Ans.**
   ```
   grep "define __NR"|wc -l
   ```	
7. Run a sleep 100 with & (to put it in the background). What files does its process have open? 
**Ans.**
   ```
   sleep 100 &
   cd /proc/<PID>/fd
   ls -l
   tty
   ```	
8. Does your system have a PCI Ethernet device?  
**Ans.**
   ```
   lspci | grep -i ethernet
   ```		
9. Is the kernel variable ip_forward (under /proc/sys/…) set to 1 or 0 on your system? 
**Ans.**
   ```
   cat $(find /proc/sys/net | grep -i "ip_forward$")
   sudo sysctl net.ipv4.ip_forward=0 or 1
   ```
	
10. According to /sys/block, do you have a block device (disk) sda? If so, do you have device files for partitions of sda? How many? Using strace, does the command fdisk -l (run it as root), open any files under /sys/dev/block? 
**Ans.**
    ```
    sudo strace fdisk -l|& grep sys/dev/block| grep open |wc -l
    ```

11. Using dmesg and grep, do you see the kernel reporting the kernel command line? If not, can you determine if the boot messages from the kernel were lost? Does your system have a log file that recorded the boot messages? You can grep for BOOT_IMAGE under /var/log to look. 
**Ans.**
		```
		sudo dmesg|grep -i "command"
		cd /var/log
		sudo grep -rl "BOOT_IMAGE"
		```		
12. What other device files are character devices and share the same major number with /dev/null?
**Ans.**
    ```
    cd /dev
    ls -l| grep "^c"| grep " 1,"
    ```	
