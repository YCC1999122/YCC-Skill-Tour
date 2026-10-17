/**
 * @file file_io.h
 * @brief 文件IO模块头文件
 *
 * 包含：文件读写、格式化IO、二进制文件、目录操作
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#include <stddef.h>

/* ===== 文件IO演示 ===== */
void demo_file_io(void);

/* ===== 文本文件操作 ===== */
int  write_text_file(const char *filename, const char *content);
char* read_text_file(const char *filename, size_t *out_size);

/* ===== 逐行读取 ===== */
int read_lines(const char *filename, char **lines, int max_lines);

/* ===== 二进制文件操作 ===== */
int write_binary_file(const char *filename, const void *data, size_t size);
void* read_binary_file(const char *filename, size_t *out_size);

/* ===== 文件复制 ===== */
int copy_file(const char *src, const char *dest);

/* ===== 文件信息 ===== */
long file_size(const char *filename);
int  file_exists(const char *filename);

/* ===== 格式化输出到文件 ===== */
int write_formatted(const char *filename, const char *format, ...);

#endif /* FILE_IO_H */
