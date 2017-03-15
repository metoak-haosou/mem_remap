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

#define _MIN 0
#define _MAJ 116 
#define _NAME "chartest"

//物理地址的宏定义
#define GPIO_BASE 		0xE000A000
#define DATA_0_RO 		(GPIO_BASE+0x60)
#define DATA_0 			(GPIO_BASE+0x40)
#define MASK_DATA_0_LSW (GPIO_BASE+0x0 )
#define DIRM_0 			(GPIO_BASE+0x204)
#define OEN_0 			(GPIO_BASE+0x208)

static dev_t dev;
static struct cdev *cdev = NULL;
struct class *drv_class;

static int 	*pDATA_0_RO		=NULL;
static int 	*pDATA_0		=NULL;
static int 	*pMASK_DATA_0_LSW=NULL;
static int 	*pDIRM_0		=NULL;
static int 	*pOEN_0			=NULL;
//打开文件函数
static int 
open_chartest(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"open\n");
	
	return 0;
}
//关闭文件函数
static int 
release_chartest(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"release\n");
	return 0;
	
}
//读函数
static ssize_t 
read_chartest (struct file *fp, char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"read\n");
	return 0;
}
//写函数
static ssize_t 
write_chartest (struct file *fp, const char __user *ubuf, size_t count, loff_t *ppos)
{	
	printk(KERN_NOTICE"write\n");
	return 0;
}
//文件操作函数集
static struct file_operations fops_chartest={
	.owner = THIS_MODULE,
	.open = open_chartest,
	.release = release_chartest,
	.read = read_chartest,
	.write = write_chartest,
};

//初始化函数
static int __init 
chartest_init(void)
{	
	dev = MKDEV(_MAJ,_MIN);
	if(register_chrdev_region(dev,1,_NAME)){
		alloc_chrdev_region(&dev,_MIN,1,_NAME);
	}
	
	cdev = cdev_alloc();
	if (!cdev) {
		printk("at file %s  line %d function %s : cdev_alloc failed\n",__FILE__,__LINE__,__FUNCTION__);
		goto out1;
	}
	cdev_init(cdev,&fops_chartest);
	if(cdev_add(cdev,dev,1)){
		printk("at file %s  line %d function %s : cdev_add failed\n",__FILE__,__LINE__,__FUNCTION__);
		goto out2;
	}
	
	drv_class = class_create(THIS_MODULE,_NAME);
	if(!drv_class){
		printk("class_create error\n");
		goto out3;
	}
	
	if (IS_ERR(device_create(drv_class, NULL,dev,NULL, _NAME))){
		printk("Could not create files\n");
		goto out4;
	}else
		printk("device_create success\n");
	
	//内存映射
	request_mem_region(DATA_0_RO,4,"DATA_0_RO");
	pDATA_0_RO=ioremap(DATA_0_RO,4);
	
	request_mem_region(DATA_0,4,"DATA_0");
	pDATA_0=ioremap(DATA_0,4);
	
	request_mem_region(MASK_DATA_0_LSW,4,"MASK_DATA_0_LSW");
	pMASK_DATA_0_LSW=ioremap(MASK_DATA_0_LSW,4);
	
	request_mem_region(DIRM_0,4,"DIRM_0");
	pDIRM_0=ioremap(DIRM_0,4);
	
	request_mem_region(OEN_0,4,"OEN_0");
	pOEN_0=ioremap(OEN_0,4);
	
	printk(KERN_NOTICE"Hello %s.\n","world");
	printk(KERN_NOTICE"dev_t : major = %d,minor = %d\n",MAJOR(dev),MINOR(dev));
	return 0;
out4:
	device_destroy(drv_class, dev);
out3:
	class_destroy(drv_class);
out2:
	cdev_del(cdev);
	cdev = NULL;
out1:
	unregister_chrdev_region(dev,1);
	return 0;
}

//卸载函数
static void __exit 
chartest_exit(void)
{
	if(cdev){
		//cdev的删除
		cdev_del(cdev);
		//反注册设备号
		unregister_chrdev_region(dev,1);
		printk(KERN_NOTICE"Goodbye %s\n","world");
		//设备文件的删除
		device_destroy(drv_class, dev);
		class_destroy(drv_class);//注意：这里必须device_destroy在前面
		//反映射
		
		iounmap(pDATA_0_RO);
		release_mem_region(DATA_0_RO,4);
		
		iounmap(pDATA_0);
		release_mem_region(DATA_0,4);
		
		iounmap(pMASK_DATA_0_LSW);
		release_mem_region(MASK_DATA_0_LSW,4);
		
		iounmap(pDIRM_0);
		release_mem_region(DIRM_0,4);
		
		iounmap(pOEN_0);
		release_mem_region(OEN_0,4);
		
	}
}

//一些必要的信息的声明
MODULE_AUTHOR("haosou");
MODULE_VERSION("v1.0");
MODULE_DESCRIPTION("A test to char device driver");
MODULE_LICENSE("GPL");
//初始化函数和卸载函数的注册
module_init(chartest_init);
module_exit(chartest_exit);

