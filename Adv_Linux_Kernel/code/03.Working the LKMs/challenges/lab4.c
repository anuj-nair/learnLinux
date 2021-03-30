#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
MODULE_AUTHOR("Anuj Nair");
MODULE_DESCRIPTION("A very simple loadble module that does almost nothing.");
static int number = 3999;
static char* word = "cherry";

module_param(number,int, S_IRUGO);
MODULE_PARM_DESC(number,"A sample integer modifiable parameter");
module_param(word, charp, S_IRUGO);
MODULE_PARM_DESC(word,"A sample string modifiable parameter");
int my_init_module(void)
{
	printk("number=%d, word=%s\n",number,word);
	return 0;
}
void my_cleanup_module(void)
{
	printk("The module is now unloaded\n");
	return;
}
module_exit(my_cleanup_module);
module_init(my_init_module);
MODULE_LICENSE("GPL");
