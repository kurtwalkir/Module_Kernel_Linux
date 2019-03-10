#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

static struct kobject *my_kobj;

static ssize_t my_sys_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
      struct module * mod;
      struct list_head * pos;
      int len = 0;
      list_for_each(pos, THIS_MODULE->list.prev) {

      mod = container_of(pos, struct module, list);
      len += sprintf(buf+len, "%s\n", mod->name);
      }
      return len;
}

static ssize_t my_sys_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
      return 0;
}

static struct kobj_attribute my_attribute =__ATTR(my_sys, 0755, my_sys_show, my_sys_store);

int __init mod_init(void)
{
        int retval;
        my_kobj = kobject_create_and_add("my_kobject", kernel_kobj);
      	if (!my_kobj)
      		return -ENOMEM;

      	retval = sysfs_create_file(my_kobj, &my_attribute.attr);
      	if (retval)
      		kobject_put(my_kobj);

      	return retval;
}

void __exit mod_exit(void)
{
        kobject_put(my_kobj);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_DESCRIPTION("Module list");
MODULE_AUTHOR("Alexander Mordyk <kurtwalkir@gmail.com>");
MODULE_LICENSE("GPL");
