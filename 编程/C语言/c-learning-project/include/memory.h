/**
 * @file memory.h
 * @brief 内存管理模块头文件
 *
 * 包含：malloc/free/calloc/realloc、内存池、引用计数、内存调试
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>   /* size_t */

/* ===== 内存基础演示 ===== */
void demo_memory_basics(void);

/* ===== 内存池（固定大小块） ===== */
typedef struct MemoryPool MemoryPool;

MemoryPool* mempool_create(size_t block_size, size_t block_count);
void        mempool_destroy(MemoryPool *pool);
void*       mempool_alloc(MemoryPool *pool);
void        mempool_free(MemoryPool *pool, void *ptr);
size_t      mempool_used_count(const MemoryPool *pool);
size_t      mempool_free_count(const MemoryPool *pool);
void        mempool_print_status(const MemoryPool *pool);

/* ===== 引用计数智能指针 ===== */
typedef struct {
    void   *data;
    size_t  ref_count;
    void  (*destroy_fn)(void *);  /* 析构函数 */
} RefCounted;

RefCounted* refcount_create(size_t data_size, void (*destroy_fn)(void *));
RefCounted* refcount_retain(RefCounted *rc);
void        refcount_release(RefCounted *rc);
void*       refcount_data(const RefCounted *rc);
size_t      refcount_count(const RefCounted *rc);

/* ===== 带调试追踪的malloc/free ===== */
void* debug_malloc(size_t size, const char *file, int line);
void  debug_free(void *ptr, const char *file, int line);
void  debug_memory_leak_check(void);

#define MALLOC(size)  debug_malloc((size), __FILE__, __LINE__)
#define FREE(ptr)     debug_free((ptr), __FILE__, __LINE__)

#endif /* MEMORY_H */
