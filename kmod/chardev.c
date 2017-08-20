/* INCLUDES */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kfifo.h>
#include <asm/uaccess.h>
#include "chardev.h"

/* DEFINES */
#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define FIFO_SIZE   128

/* PROTOTYPES */
static int initialize_module(void);
static void deinitialize_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static long device_ioctl(struct file *file, unsigned int ioctl_num, 
                  unsigned long ioctl_param);

/* GLOBALS */
static int Major;
static int Device_Open = 0;
static int Modifier = 0;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl
};

static DECLARE_KFIFO(kqueue, unsigned char, FIFO_SIZE);

/* FUNCTIONS */
static int initialize_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
      printk(KERN_ALERT "Registering char device failed with %d\n", Major);
      return Major;
    }
    
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
        
    INIT_KFIFO(kqueue);

    return SUCCESS;
}

static void deinitialize_module(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
    if (Device_Open)
    {
        return -EBUSY;
    }

    Device_Open++;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;

    module_put(THIS_MODULE);

    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, 
                           loff_t * offset)
{
    int bytes_read = 0;
    char byte = 0;

    while (length)
    {

        if (kfifo_get(&kqueue, &byte) == 0)
        {
            break;
        }
        
        byte += Modifier;                
        put_user(byte, buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, 
                            size_t length, loff_t * off)
{
    int bytes_read = 0;
    char byte = 0;

    while (length)
    {
        get_user(byte, buffer++);
        kfifo_put(&kqueue, byte);
        length--;
        bytes_read++;
    }
    
    return bytes_read;
}

static long device_ioctl(struct file *file, unsigned int ioctl_num, 
                         unsigned long ioctl_param)
{
    /* Switch according to the ioctl called */
    switch (ioctl_num)
    {
        case IOCTL_SET_MODIFIER:
            Modifier = (int) ioctl_param;
            break;
      
        default:
            /* Should not happen */
            break;
    }

    return SUCCESS;
}

module_init(initialize_module);
module_exit(deinitialize_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Savic <stefan.savic@rt-rk.com>");