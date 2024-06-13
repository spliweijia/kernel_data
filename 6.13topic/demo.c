#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/slab.h>
unsigned int major = 0;
#define CNAME "mydev"
struct class *cls;
struct device *device;
char kbuf[128]="";
char *hbuf;
int mydev_open(struct inode *inode, struct file *file)
{
    printk("mydev_open access\n");
    return 0;
}

ssize_t mydev_read(struct file *file, char __user *ubuf, size_t size, loff_t *loff)
{
    
      int ret;
    printk("mydev_read access\n");
    strcpy(kbuf,hbuf);//将申请空间中的内容取出
    //拷贝kbuf字节大小
    if(size > sizeof(kbuf))  size = sizeof(kbuf);
    ret = copy_to_user(ubuf,kbuf,size); //将内核空间中的数据，拷贝到用户空间
    if(ret){
        printk("copy to user is error\n");
        return -EIO;
    }


    return size; 
}

ssize_t mydev_write(struct file *file, const char __user *ubuf, size_t size, loff_t *loff)
{
        int ret;
    printk("mydev_write access\n");
    
    if(size > sizeof(kbuf))  size = sizeof(kbuf);
    ret = copy_from_user(kbuf,ubuf,size); //将用户空间的数据，拷贝到内核空间
    if(ret){
        printk("copy form user is error\n");
        return -EIO;
    }
    printk("kbuf = %s\n",kbuf); //打印内核空间内容

       // 申请内存存储 "hello world"
    hbuf = kmalloc(12, GFP_KERNEL);
    if(hbuf==NULL){
        printk("kmalloc error\n");
        return -ENOMEM;
    }
 
    strcpy(hbuf,kbuf);//将内核空间内容放到申请空间中


    return size;     
}

int mydev_close(struct inode *inode, struct file *file)
{
    printk("mydev_close access\n");
    kfree(hbuf);
    return 0;     

}

//操作方法结构体
const struct file_operations fops = {
    .open = mydev_open,
    .read = mydev_read,
    .write = mydev_write,
    .release = mydev_close,
 };

//入口函数
static int __init demo_init(void)
{
    //注册字符设备驱动
    major = register_chrdev(0,CNAME,&fops);
    if(major < 0){
        printk("register chrdev is error\n");
        return -EIO;
    }
    printk("major = %d\n",major);//打印主设备号的值

    //向上层提交目录信息
    cls = class_create(THIS_MODULE,CNAME);
    if(IS_ERR(cls)){
        return PTR_ERR(cls);
    }
    //向上层提交设备节点信息
    device = device_create(cls,NULL,MKDEV(major,0),NULL,CNAME);
    if(IS_ERR(device)){
        return PTR_ERR(device);
    }
    
    return 0; 
}

//出口函数
static void __exit demo_exit(void)
{
    device_destroy(cls,MKDEV(major,0)); //取消向上层提交设备节点信息
    class_destroy(cls); //取消向生成提交目录信息  
    //注销字符设备驱动
    unregister_chrdev(major,CNAME);
    

}
module_init(demo_init);//指定入口地址
module_exit(demo_exit);//指定出口地址
MODULE_LICENSE("GPL");//许可证
