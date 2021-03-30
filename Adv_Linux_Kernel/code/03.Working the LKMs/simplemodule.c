#include <linux/init.h>
#include <linux/module.h>
int init_simple(void)
{
	printk("in init module simple\n");
	return 0;
}
void cleanup_simple(void)
{
	printk("in cleanup module simple\n");
	return;
}
module_init(init_simple);
module_exit(cleanup_simple);
MODULE_LICENSE("GPL");

