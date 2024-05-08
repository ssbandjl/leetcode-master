#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>  // for kmalloc
#include <linux/list.h>  // for kernel link
#include <linux/kernel.h>
 
#include <linux/netlink.h> //for netlink communication not used here
 
typedef struct car{
    int     door_number;
    char    *color;
    char    *model;
    struct list_head list;
} CAR ;
 
 
//声明并初始化链表头节点
LIST_HEAD(carlist);
 
//模块入口函数，必须有返回值，否则编译报警告或错误
static int test_netlink_init(void){
    //用于迭代的游标，作用类似于for循环中的 i
    struct car *acar; 
    
    //声明节点并申请内存,GFP_KERNEL是内存类型
    struct car *redcar   = kmalloc(sizeof(CAR), GFP_KERNEL);
    struct car *blackcar = kmalloc(sizeof(CAR), GFP_KERNEL);
    
    //初始化链表的list
    INIT_LIST_HEAD(&redcar->list);
    INIT_LIST_HEAD(&blackcar->list);   
    
    //填充节点的内容
    blackcar->door_number = 4;
    blackcar->color       = "black";
    blackcar->model       = "Y";
 
    redcar->door_number   = 2;
    redcar->color         = "red";
    redcar->model         = "E";
 
    //添加链表节点到内核链表中
    list_add(&redcar->list,   &carlist);
    list_add(&blackcar->list, &carlist);
    
    //链表遍历
    list_for_each_entry(acar, &carlist, list) {
        printk("车门数[%d]\n",   acar->door_number);
        printk("车的颜色[%s]\n", acar->color      );
        printk("车的模具[%s]\n", acar->model      );
 
        if(!memcmp(acar->color, "black", sizeof("black"))){
            printk("存在黑色车\n");
        }
 
    }
 
    //入口函数需有返回值
    return 0;
}
 
static void test_netlink_exit(void){
    struct car *acar = NULL;
    printk("内核模块退出清理\n");
    list_for_each_entry(acar, &carlist, list){
        list_del(&acar->list);
        //使用kfree释放内存
        kfree(acar);
    }
    return;
}
 
 
module_init(test_netlink_init);
module_exit(test_netlink_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("everyone");
MODULE_DESCRIPTION("内核链表demo");