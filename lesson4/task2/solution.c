#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "solution_node"
#define MY_MAJOR 240
#define MY_MINOR 0

static dev_t dev;
static struct cdev hcdev;
static int cnt = 1;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define IOC_MAGIC 'k'

#define SUM_LENGTH _IOWR(IOC_MAGIC, 1, char*)
#define SUM_CONTENT _IOWR(IOC_MAGIC, 2, char*)

static long unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
  static int len = 0;
  static long num = 0;
  static long sum = 0;

  switch (cmd)
  {
     case SUM_LENGTH:
          len += strlen((char*)arg);
          return len;
        break;

     case SUM_CONTENT:
          kstrtol((char*)arg, 10, &num);
          sum += num;
          return sum;
        break;
     default:
        return -1;
  }
  return 0;
}

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release,
  .unlocked_ioctl = unlocked_ioctl
};

static int device_open(struct inode *inode, struct file *filp)
{
  return 0;
}

static int device_release(struct inode *inode, struct file *filp)
{
  return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
   return -EINVAL;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
  return -EINVAL;
}

int __init mod_init(void)
{
	int ret;
	dev = MKDEV(MY_MAJOR, MY_MINOR);
	ret = register_chrdev_region(dev, cnt , DEVICE_NAME);
	if (ret<0) printk(KERN_ERR"Can not reg dev.");
	cdev_init(&hcdev,&fops);
	hcdev.owner = THIS_MODULE;
	ret = cdev_add(&hcdev,dev,cnt);
	if (ret<0) printk(KERN_ERR"Can not add dev.");

	printk(KERN_INFO"Module load suc.");
	return 0;
}

void __exit mod_exit(void)
{
  cdev_del (&hcdev);
  unregister_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR), cnt);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_DESCRIPTION("IOCTR");
MODULE_AUTHOR("Alexander Mordyk <kurtwalkir@gmail.com>");
MODULE_LICENSE("GPL");
