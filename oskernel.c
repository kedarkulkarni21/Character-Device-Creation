#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/proc_fs.h>
#include<asm/uaccess.h>
#include<linux/slab.h>
#include<linux/semaphore.h>
#include<linux/init.h>
#include<linux/miscdevice.h>
#include<linux/platform_device.h>
#include<linux/rtc.h>
#include<linux/sched.h>

#define buff_size 7000000

//Function definitions
static int device_open(struct inode *inode, struct file *filep);
static int __init my_char_device_init(void);
static ssize_t device_read(struct file *filep, char __user *out, size_t size, loff_t *offset);
static int device_close(struct inode *inode, struct file *filep);
static void __exit my_char_device_exit(void);

//Declaring a file-operations struct
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.open = device_open,
	.release = device_close,
};

//Declaring a device struct
static struct miscdevice my_char_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "my_char_device",
	.fops = &fops
};

//Registering the device with kernel
static int __init my_char_device_init(void)
{
	int major_number = misc_register(&my_char_device);
	if (major_number < 0) 
	{
		printk(KERN_ALERT "Registering char device failed with %d\n", major_number);
		return major_number;
	}
	printk(KERN_INFO "MY_CHAR_DEVICE: registered correctly with major number %d\n", major_number);
	return 0;
}

//Implementing the fops functions
static int device_open(struct inode *inode, struct file *filep)
{
	printk(KERN_INFO "my_char_device: opened device\n");
	return 0;
}

//Implementing the read function
static ssize_t device_read(struct file *filep, char __user *out, size_t size, loff_t *offset)
{
	long state; 
	char buffer[buff_size];
	char state_type[1000]; 
	int i;
	int temp = 0;
	int kd = 0; 
	int value = 0; 
	int state_count[25] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
	char *states[25] = { 
	"TASK_RUNNING",
	"TASK_INTERRUPTIBLE",
	"TASK_UNINTERRUPTIBLE",
	"__TASK_STOPPED",
	"__TASK_TRACED",
	"EXIT_DEAD",
	"EXIT_ZOMBIE",
	"EXIT_TRACE",
	"TASK_DEAD",
	"TASK_WAKEKILL",
	"TASK_WAKING",
	"TASK_PARKED",
	"TASK_NOLOAD",
	"TASK_NEW",
	"TASK_STATE_MAX",
	"TASK_STATE_TO_CHAR_STR",
	"TASK_KILLABLE",
	"TASK_STOPPED",
	"TASK_TRACED",
	"TASK_IDLE",
	"TASK_NORMAL",
	"TASK_ALL"
	};
	
	struct task_struct *p;
	
	printk(KERN_INFO "my_char_device: reading from device\n");
	rcu_read_lock();
	for_each_process(p)
	{
		if(p)
		{
			int var = 0; 
			state = p->state;
			for(i = 0; i < 25; i++)
			{
				if(state & state_count[i])
				{
					temp = sprintf(state_type + var,"%s", states[i]);
					var = var + temp;
				}
			}
			kd = sprintf(buffer+value, "PID=%d  PPID=%d  CPU=%d  STATE=TASK_RUNNING, %s\n",p->pid,p->parent->pid,task_cpu(p),state_type );
			value = value + kd;
			printk(KERN_ALERT "Process id:%d  value:%d", p->pid, value);	 	

		}
	}
	rcu_read_unlock();
	copy_to_user(out, buffer, strlen(buffer));
	return value;
}

//Closing the device driver
static int device_close(struct inode *inode, struct file *filep)
{
	printk(KERN_INFO "my_char_device: Device successfully closed\n");
    return 0;
}

//Unregistering the character device
static void __exit my_char_device_exit(void)
{
	misc_deregister(&my_char_device);
}

module_init(my_char_device_init);
module_exit(my_char_device_exit);
