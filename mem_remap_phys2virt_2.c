/***
	这个驱动使用phys_to_virt做虚拟地址映射，然后用read间接想用户传递数据
***/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/bitrev.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/ioport.h>

#define _MAJ 240
#define _MIN 0
#define _NAME "mem_remap_phys2virt_2"

static dev_t dev;
static struct cdev *cdev_p;
static struct class *drv_class;

//打开文件函数
/**
	FUNCtION:mem_remap_phys2virt_2_open
	这个函数用来映射物理地址到虚拟地址上
**/
static int 
mem_remap_phys2virt_2_open(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"open\n");
	
	return 0;
}
//关闭文件函数
/**
	FUNCtION:mem_remap_phys2virt_2_release
	这个函数用来将open函数申请的虚拟地址返还给内核
**/
static int 
mem_remap_phys2virt_2_release(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"release\n");
	return 0;
	
}
//读函数
/**
	FUNCtION:mem_remap_phys2virt_2_read
	这个函数向用户返回一帧数据
**/
static ssize_t 
mem_remap_phys2virt_2_read (struct file *fp, char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"read\n");
	return 0;
}
//写函数
/**
	FUNCtION:mem_remap_phys2virt_2_write
	这个函数暂时不用
**/
static ssize_t 
mem_remap_phys2virt_2_write (struct file *fp, const char __user *ubuf, size_t count, loff_t *ppos)
{	
	printk(KERN_NOTICE"write\n");
	return 0;
}



static struct file_operations mem_remap_phys2virt_2_fops={
	.owner = THIS_MODULE,
	.open = mem_remap_phys2virt_2_open,
	.release = mem_remap_phys2virt_2_release,
	.read = mem_remap_phys2virt_2_read,
	.write = mem_remap_phys2virt_2_write,
};


/*
	init函数
*/
static int __init 
mem_remap_phys2virt_2_init(void)
{
	dev = MKDEV(_MAJ,_MIN);
	if(register_chrdev_region(dev,1,_NAME)){
		alloc_chrdev_region(&dev,_MIN,1,_NAME);
	}
	
	cdev_p = cdev_alloc();
	cdev_init(cdev_p,&mem_remap_phys2virt_2_fops);
	cdev_add(cdev_p,dev,1);
	drv_class = class_create(THIS_MODULE,_NAME);
	device_create(drv_class, NULL,dev,NULL, _NAME);
	return 0;
}



/*
	exit函数
*/
static void __exit
mem_remap_phys2virt_2_exit(void)
{
	cdev_del(cdev_p);
	//反注册设备号
	unregister_chrdev_region(dev,1);
	//设备文件的删除
	device_destroy(drv_class, dev);
	class_destroy(drv_class);//注意：这里必须device_destroy在前面
	
}

//一些必要的信息的声明
MODULE_AUTHOR("jie.wang@metoak.com");
MODULE_VERSION("v1.0");
MODULE_DESCRIPTION("利用ioremap将物理地址映射成虚拟地址并提供给用户");
MODULE_LICENSE("GPL");
//初始化函数和卸载函数的注册
module_init(mem_remap_phys2virt_2_init);
module_exit(mem_remap_phys2virt_2_exit);