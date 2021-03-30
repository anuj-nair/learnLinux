#include <linux/module.h>
#include <linux/sched.h>
MODULE_AUTHOR("Anuj Nair");
MODULE_DESCRIPTION("A very simple loadble module that does almost nothing.");
int my_init_module(void)
{
	printk("hello from your kernel module\n");
	return 0;
}
void my_cleanup_module(void)
{
	printk("goodbye from your kernel module\n");
	return;
}
module_exit(my_cleanup_module);
module_init(my_init_module);
MODULE_LICENSE("GPL");
