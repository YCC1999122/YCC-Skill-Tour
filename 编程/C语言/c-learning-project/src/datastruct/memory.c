/**
 * @file memory.c
 * @brief 内存管理模块实现
 *
 * 包含：基础内存操作、内存池、引用计数、内存泄漏检测
 */

#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================
 * 1. 内存基础演示
 * ======================================== */
void demo_memory_basics(void)
{
    printf("========== 内存管理基础 ==========\n");

    /* malloc - 分配未初始化内存 */
    int *arr1 = (int*)malloc(5 * sizeof(int));
    if (arr1 != NULL) {
        printf("  malloc: 分配了 %zu 字节，内容未初始化\n", 5 * sizeof(int));
        /* 内存内容是未定义的（垃圾值）*/
    }

    /* calloc - 分配并初始化为0 */
    int *arr2 = (int*)calloc(5, sizeof(int));
    if (arr2 != NULL) {
        printf("  calloc: 分配了 %zu 字节，全部初始化为0\n", 5 * sizeof(int));
        printf("    arr2[0]=%d, arr2[4]=%d\n", arr2[0], arr2[4]);
    }

    /* realloc - 重新分配内存 */
    arr2 = (int*)realloc(arr2, 10 * sizeof(int));
    if (arr2 != NULL) {
        printf("  realloc: 扩展到 %zu 字节，原数据保留\n", 10 * sizeof(int));
        /* 前5个元素保留原值 */
    }

    free(arr1);
    free(arr2);
    printf("  free: 内存已释放\n");

    /* 常见错误提醒 */
    printf("\n  常见内存错误:\n");
    printf("    - 重复释放 (double free)\n");
    printf("    - 释放后使用 (use-after-free)\n");
    printf("    - 内存泄漏 (memory leak)\n");
    printf("    - 缓冲区溢出 (buffer overflow)\n");
    printf("    - 野指针 (wild pointer)\n");

    /* 内存操作函数 */
    int src[5] = {1, 2, 3, 4, 5};
    int dst[5];
    memcpy(dst, src, 5 * sizeof(int));
    printf("\n  memcpy: 复制后 dst[2] = %d\n", dst[2]);

    int arr3[10];
    memset(arr3, 0xFF, sizeof(arr3));  /* 按字节填充 */
    printf("  memset: 全部填充为0xFF, arr3[0]=0x%08X\n", arr3[0]);

    /* memmove - 支持重叠区域 */
    int arr4[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    memmove(arr4 + 2, arr4, 4 * sizeof(int));  /* 重叠区域移动 */
    printf("  memmove(重叠): ");
    for (int i = 0; i < 8; i++) printf("%d ", arr4[i]);
    printf("\n");

    /* memcmp */
    int a[3] = {1, 2, 3};
    int b[3] = {1, 2, 4};
    printf("  memcmp: %d\n", memcmp(a, b, 3 * sizeof(int)));

    printf("\n");
}

/* ========================================
 * 2. 内存池实现（固定大小块 + 空闲链表）
 * ======================================== */

struct MemoryPool {
    unsigned char *data;          /* 整块内存 */
    unsigned char *free_list;     /* 空闲块链表 */
    size_t         block_size;    /* 每块大小 */
    size_t         block_count;   /* 总块数 */
    size_t         used_count;    /* 已用块数 */
};

MemoryPool* mempool_create(size_t block_size, size_t block_count)
{
    if (block_size < sizeof(void*) || block_count == 0) return NULL;

    MemoryPool *pool = (MemoryPool*)malloc(sizeof(MemoryPool));
    if (pool == NULL) return NULL;

    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->used_count = 0;

    /* 分配整块内存 */
    pool->data = (unsigned char*)malloc(block_size * block_count);
    if (pool->data == NULL) {
        free(pool);
        return NULL;
    }

    /* 构建空闲链表：每个块的开头存放下一个空闲块的指针 */
    pool->free_list = pool->data;
    unsigned char *p = pool->data;
    for (size_t i = 0; i < block_count - 1; i++) {
        unsigned char *next = p + block_size;
        *(unsigned char**)p = next;  /* 存放下一块地址 */
        p = next;
    }
    *(unsigned char**)p = NULL;  /* 最后一块指向NULL */

    return pool;
}

void mempool_destroy(MemoryPool *pool)
{
    if (pool != NULL) {
        free(pool->data);
        free(pool);
    }
}

void* mempool_alloc(MemoryPool *pool)
{
    if (pool == NULL || pool->free_list == NULL) return NULL;

    /* 从空闲链表头部取出一块 */
    unsigned char *block = pool->free_list;
    pool->free_list = *(unsigned char**)block;  /* 下一个空闲块 */
    pool->used_count++;
    return block;
}

void mempool_free(MemoryPool *pool, void *ptr)
{
    if (pool == NULL || ptr == NULL) return;

    /* 检查指针是否在池范围内 */
    unsigned char *p = (unsigned char*)ptr;
    if (p < pool->data || p >= pool->data + pool->block_size * pool->block_count) {
        return;
    }

    /* 归还到空闲链表头部 */
    *(unsigned char**)p = pool->free_list;
    pool->free_list = p;
    pool->used_count--;
}

size_t mempool_used_count(const MemoryPool *pool)
{
    return pool ? pool->used_count : 0;
}

size_t mempool_free_count(const MemoryPool *pool)
{
    return pool ? pool->block_count - pool->used_count : 0;
}

void mempool_print_status(const MemoryPool *pool)
{
    if (pool == NULL) {
        printf("(空内存池)");
        return;
    }
    printf("内存池: 块大小=%zuB, 总数=%zu, 已用=%zu, 空闲=%zu",
           pool->block_size, pool->block_count,
           pool->used_count, pool->block_count - pool->used_count);
}

/* ========================================
 * 3. 引用计数实现
 * ======================================== */

RefCounted* refcount_create(size_t data_size, void (*destroy_fn)(void *))
{
    RefCounted *rc = (RefCounted*)malloc(sizeof(RefCounted));
    if (rc == NULL) return NULL;

    rc->data = malloc(data_size);
    if (rc->data == NULL) {
        free(rc);
        return NULL;
    }
    memset(rc->data, 0, data_size);
    rc->ref_count = 1;
    rc->destroy_fn = destroy_fn;
    return rc;
}

RefCounted* refcount_retain(RefCounted *rc)
{
    if (rc == NULL) return NULL;
    rc->ref_count++;
    return rc;
}

void refcount_release(RefCounted *rc)
{
    if (rc == NULL) return;
    rc->ref_count--;
    if (rc->ref_count == 0) {
        if (rc->destroy_fn != NULL) {
            rc->destroy_fn(rc->data);
        }
        free(rc->data);
        free(rc);
    }
}

void* refcount_data(const RefCounted *rc)
{
    return rc ? rc->data : NULL;
}

size_t refcount_count(const RefCounted *rc)
{
    return rc ? rc->ref_count : 0;
}

/* ========================================
 * 4. 调试内存分配（泄漏检测）
 * ======================================== */

/* 简单的内存分配追踪链表 */
typedef struct MemTrackNode {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    struct MemTrackNode *next;
} MemTrackNode;

static MemTrackNode *g_mem_list = NULL;
static size_t g_total_alloc = 0;

void* debug_malloc(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);
    if (ptr == NULL) return NULL;

    /* 记录分配信息 */
    MemTrackNode *node = (MemTrackNode*)malloc(sizeof(MemTrackNode));
    if (node != NULL) {
        node->ptr = ptr;
        node->size = size;
        node->file = file;
        node->line = line;
        node->next = g_mem_list;
        g_mem_list = node;
        g_total_alloc += size;
    }
    return ptr;
}

void debug_free(void *ptr, const char *file, int line)
{
    (void)file; (void)line;
    if (ptr == NULL) return;

    /* 从追踪链表中移除 */
    MemTrackNode **pp = &g_mem_list;
    while (*pp != NULL) {
        if ((*pp)->ptr == ptr) {
            MemTrackNode *node = *pp;
            g_total_alloc -= node->size;
            *pp = node->next;
            free(node);
            break;
        }
        pp = &(*pp)->next;
    }
    free(ptr);
}

void debug_memory_leak_check(void)
{
    printf("  === 内存泄漏检测 ===\n");
    if (g_mem_list == NULL) {
        printf("  无泄漏！所有内存已正确释放。\n");
    } else {
        size_t leak_count = 0;
        MemTrackNode *node = g_mem_list;
        while (node != NULL) {
            leak_count++;
            printf("  泄漏 #%zu: %zu 字节 @ %s:%d (ptr=%p)\n",
                   leak_count, node->size, node->file, node->line, node->ptr);
            node = node->next;
        }
        printf("  总计 %zu 处泄漏，%zu 字节未释放\n", leak_count, g_total_alloc);
    }
}
