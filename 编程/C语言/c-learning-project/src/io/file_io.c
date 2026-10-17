/**
 * @file file_io.c
 * @brief 文件IO模块实现
 */

#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* ========================================
 * 文件IO演示
 * ======================================== */
void demo_file_io(void)
{
    printf("========== 文件IO ==========\n");

    const char *test_file = "test_output.txt";

    /* 写入文本 */
    const char *text = "Hello, C语言文件操作！\n第二行内容\n第三行\n";
    if (write_text_file(test_file, text) == 0) {
        printf("  写入文本文件成功\n");
    }

    /* 读取文本 */
    size_t size = 0;
    char *content = read_text_file(test_file, &size);
    if (content != NULL) {
        printf("  读取文本 (%zu 字节):\n", size);
        printf("  ----\n  %s  ----\n", content);
        free(content);
    }

    /* 文件大小 */
    long fsize = file_size(test_file);
    printf("  文件大小: %ld 字节\n", fsize);

    /* 文件存在检查 */
    printf("  文件存在: %s\n", file_exists(test_file) ? "是" : "否");
    printf("  不存在的文件: %s\n", file_exists("no_such_file.txt") ? "存在" : "不存在");

    /* 二进制文件 */
    int data[] = {10, 20, 30, 40, 50};
    const char *bin_file = "test_binary.dat";
    write_binary_file(bin_file, data, sizeof(data));
    printf("  写入二进制文件: %zu 字节\n", sizeof(data));

    size_t bin_size = 0;
    int *bin_data = (int*)read_binary_file(bin_file, &bin_size);
    if (bin_data != NULL) {
        printf("  读取二进制: ");
        for (size_t i = 0; i < bin_size / sizeof(int); i++) {
            printf("%d ", bin_data[i]);
        }
        printf("\n");
        free(bin_data);
    }

    /* 文件复制 */
    const char *copy_file = "test_copy.txt";
    if (copy_file(test_file, copy_file) == 0) {
        printf("  文件复制成功: %s -> %s\n", test_file, copy_file);
    }

    /* 格式化写入 */
    const char *fmt_file = "test_formatted.txt";
    write_formatted(fmt_file, "整数: %d, 浮点数: %.2f, 字符串: %s\n",
                    42, 3.14, "Hello");
    printf("  格式化写入完成\n");

    /* 清理测试文件 */
    remove(test_file);
    remove(bin_file);
    remove(copy_file);
    remove(fmt_file);
    printf("  测试文件已清理\n");

    printf("\n");
}

/* ========================================
 * 文本文件写入
 * ======================================== */
int write_text_file(const char *filename, const char *content)
{
    if (filename == NULL || content == NULL) return -1;
    FILE *fp = fopen(filename, "w");  /* "w" 写入模式，会覆盖 */
    if (fp == NULL) return -1;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    fclose(fp);
    return (written == len) ? 0 : -1;
}

/* ========================================
 * 文本文件读取（全部读入内存）
 * ======================================== */
char* read_text_file(const char *filename, size_t *out_size)
{
    if (filename == NULL) return NULL;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    /* 获取文件大小 */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size < 0) { fclose(fp); return NULL; }

    /* 分配内存（+1 用于字符串结束符）*/
    char *buf = (char*)malloc(size + 1);
    if (buf == NULL) { fclose(fp); return NULL; }

    size_t read = fread(buf, 1, size, fp);
    buf[read] = '\0';
    fclose(fp);

    if (out_size != NULL) *out_size = read;
    return buf;
}

/* ========================================
 * 逐行读取
 * ======================================== */
int read_lines(const char *filename, char **lines, int max_lines)
{
    if (filename == NULL || lines == NULL || max_lines <= 0) return -1;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return -1;

    char buf[1024];
    int count = 0;
    while (count < max_lines && fgets(buf, sizeof(buf), fp) != NULL) {
        /* 去掉换行符 */
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (len > 1 && buf[len - 2] == '\r') buf[len - 2] = '\0';  /* Windows \r\n */

        lines[count] = strdup(buf);  /* 复制字符串 */
        count++;
    }
    fclose(fp);
    return count;
}

/* ========================================
 * 二进制文件写入
 * ======================================== */
int write_binary_file(const char *filename, const void *data, size_t size)
{
    if (filename == NULL || data == NULL) return -1;
    FILE *fp = fopen(filename, "wb");  /* "wb" 二进制写入模式 */
    if (fp == NULL) return -1;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return (written == size) ? 0 : -1;
}

/* ========================================
 * 二进制文件读取
 * ======================================== */
void* read_binary_file(const char *filename, size_t *out_size)
{
    if (filename == NULL) return NULL;

    FILE *fp = fopen(filename, "rb");  /* "rb" 二进制读取模式 */
    if (fp == NULL) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size < 0) { fclose(fp); return NULL; }

    void *buf = malloc(size);
    if (buf == NULL) { fclose(fp); return NULL; }

    size_t read = fread(buf, 1, size, fp);
    fclose(fp);

    if (out_size != NULL) *out_size = read;
    return buf;
}

/* ========================================
 * 文件复制
 * ======================================== */
int copy_file(const char *src, const char *dest)
{
    if (src == NULL || dest == NULL) return -1;

    FILE *fsrc = fopen(src, "rb");
    if (fsrc == NULL) return -1;

    FILE *fdest = fopen(dest, "wb");
    if (fdest == NULL) { fclose(fsrc); return -1; }

    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fsrc)) > 0) {
        if (fwrite(buf, 1, n, fdest) != n) {
            fclose(fsrc);
            fclose(fdest);
            return -1;
        }
    }

    fclose(fsrc);
    fclose(fdest);
    return 0;
}

/* ========================================
 * 文件大小
 * ======================================== */
long file_size(const char *filename)
{
    if (filename == NULL) return -1;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) return -1;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

/* ========================================
 * 文件存在检查
 * ======================================== */
int file_exists(const char *filename)
{
    if (filename == NULL) return 0;
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}

/* ========================================
 * 格式化输出到文件
 * ======================================== */
int write_formatted(const char *filename, const char *format, ...)
{
    if (filename == NULL || format == NULL) return -1;

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) return -1;

    va_list ap;
    va_start(ap, format);
    int result = vfprintf(fp, format, ap);
    va_end(ap);

    fclose(fp);
    return (result >= 0) ? 0 : -1;
}
