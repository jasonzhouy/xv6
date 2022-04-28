// Mutual exclusion lock.
struct spinlock {
    // 锁的持有状态（这把自旋锁当前是否被某个进程持有）
    uint locked;       // Is the lock held?

    // For debugging:
    char *name;        // Name of lock.
    struct cpu *cpu;   // The cpu holding the lock.
};

