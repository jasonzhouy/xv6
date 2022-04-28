#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main() {
//    启动第一个CPU
    if (cpuid() == 0) {
        // 初始化UART，初始化终端IO，使用UART来和终端传输数据
        consoleinit();
        printfinit();
        printf("\n");
        printf("xv6 kernel is booting\n");
        printf("\n");
        kinit();         // physical page allocator
        kvminit();       // create kernel page table
        kvminithart();   // turn on paging
        // 初始化进程
        procinit();      // process table
        // 中断初始化，开启时钟中断 TODO
        trapinit();      // trap vectors
        // 设置内核中断向量表
        trapinithart();  // install kernel trap vector
        plicinit();      // set up interrupt controller
        // 响应外部中断，PLIC 全局中断
        plicinithart();  // ask PLIC for device interrupts
        binit();         // buffer cache
        iinit();         // inode table
        fileinit();      // file table
        virtio_disk_init(); // emulated hard disk
        // 创建出第一个用户进程
        userinit();      // first user process
        __sync_synchronize();
        started = 1;
    } else {
        while (started == 0);
        __sync_synchronize();
        printf("hart %d starting\n", cpuid());
        kvminithart();    // turn on paging
        trapinithart();   // install kernel trap vector
        plicinithart();   // ask PLIC for device interrupts
    }

    scheduler();
}
