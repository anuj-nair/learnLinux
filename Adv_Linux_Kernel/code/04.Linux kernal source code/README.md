# Linux Kernel Fundamentals: Chapter 4, Examining Linux Kernel Source Code

## Basics

### `make help` Command

#### Cleaning targets:	
* `clean` - Remove most generated files but keep the config ...
* `mrproper` - Remove all generated files + config + various backup files
* `distclean` - mrproper +  remove editor backup and patch  files

#### Configuring targets
* `config` - Update current config utilizing a line-oriented program
* `nconfig` - Update current config utilizing a ncurses menu based program
* `menuconfig` - Update current config utilizing a menu based program
* `xconfig` - Update current config utilizing a QT based front-end
* `gconfig` - Update current config utilizing a GTK based front-end

#### Other generic targets:
* `all` - Build all targets marked with [`*`]
* `* vmlinux` - Build the bare kernel 
* `* modules` - Build all modules
* `modules_install` - Install all modules to INSTALL_MOD_PATH (default: "/")

#### Architecture specific target (x86):
* `* bzImage` -  Compared kernel image ("arch/x86/boot/bzImage")
* `install` - Install kernel using (your) ~/bin/installkernel or (distribution) /sbin/installkernel or install to $(INSTALL_PATH) and run lilo

## Challenge
Fetch the Ubuntu kernel source. 

1. Fetch the kernel source code. 
 
2. For your kernel, do make cscope and make tags. 
 
3. Test the ctags facility of the vi text editor. 
	 1. Look up the function in the Linux kernel called sys_open(). If you find macro definitions at first, do :tn to find the next definition, and until you find the function definition.
	 2. The sys_open() function makes use of other functions. Move your cursor to one of these functions and hit Ctrl-]. What happened? 
	 3. Hit the key sequence Ctrl-T. What happened? 
	 4. Use the :tag command to look up the tag atomic_inc. Use the :tn command. What did this do? What does the :ts 
