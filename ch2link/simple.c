#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct foostruct {
    int data;
    struct list_head list;
};

static LIST_HEAD(mylist);

/* This function is called when the module is loaded. */
int simple_init(void)
{
    struct foostruct *first;
    struct foostruct *second;

    struct list_head *pos;
    struct foostruct *fooptr;

    first = kmalloc(sizeof(*first), GFP_KERNEL);
    first->data = 1;
    INIT_LIST_HEAD(&first->list);
    // first->list = LIST_HEAD_INIT(first->list);
    second = kmalloc(sizeof(*second), GFP_KERNEL);
    second->data = 2;
    INIT_LIST_HEAD(&second->list);
    // second->list = LIST_HEAD_INIT(second->list);
    
    list_add(&second->list, &mylist);
    list_add(&first->list, &mylist);

    list_for_each(pos, &mylist) {
        fooptr = list_entry(pos, struct foostruct, list);
        printk(KERN_INFO "Data: %d \n", fooptr->data);
    }

    // list_for_each_entry(fooptr, &mylist, list) {

    printk(KERN_INFO "Loading Module\n");

    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) 
{
	
    struct foostruct *fooptr;
    struct foostruct *next;

    list_for_each_entry_safe(fooptr, next, &mylist, list) {
        list_del(&fooptr->list);
        kfree(fooptr);
    }

    printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
