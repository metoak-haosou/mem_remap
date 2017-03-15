/***
	�������ʹ��phys_to_virt�������ַӳ�䣬Ȼ����read������û���������
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

//���ļ�����
/**
	FUNCtION:mem_remap_phys2virt_2_open
	�����������ӳ�������ַ�������ַ��
**/
static int 
mem_remap_phys2virt_2_open(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"open\n");
	
	return 0;
}
//�ر��ļ�����
/**
	FUNCtION:mem_remap_phys2virt_2_release
	�������������open��������������ַ�������ں�
**/
static int 
mem_remap_phys2virt_2_release(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"release\n");
	return 0;
	
}
//������
/**
	FUNCtION:mem_remap_phys2virt_2_read
	����������û�����һ֡����
**/
static ssize_t 
mem_remap_phys2virt_2_read (struct file *fp, char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"read\n");
	return 0;
}
//д����
/**
	FUNCtION:mem_remap_phys2virt_2_write
	���������ʱ����
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
	init����
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
	exit����
*/
static void __exit
mem_remap_phys2virt_2_exit(void)
{
	cdev_del(cdev_p);
	//��ע���豸��
	unregister_chrdev_region(dev,1);
	//�豸�ļ���ɾ��
	device_destroy(drv_class, dev);
	class_destroy(drv_class);//ע�⣺�������device_destroy��ǰ��
	
}

//һЩ��Ҫ����Ϣ������
MODULE_AUTHOR("jie.wang@metoak.com");
MODULE_VERSION("v1.0");
MODULE_DESCRIPTION("����ioremap�������ַӳ��������ַ���ṩ���û�");
MODULE_LICENSE("GPL");
//��ʼ��������ж�غ�����ע��
module_init(mem_remap_phys2virt_2_init);
module_exit(mem_remap_phys2virt_2_exit);