#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    // TODO: 在这里添加你的代码
    // 1. 定位第一个 '/' 之后的起始点 (old_str 的起点)
    const char* start_old = cmd + 2;

    // 2. 寻找第二个 '/'
    const char* end_old = strchr(start_old, '/');
    if (end_old == NULL) return -1; // 格式错误，没找到第二个 '/'

    // 3. 定位第二个 '/' 之后的起始点 (new_str 的起点)
    const char* start_new = end_old + 1;

    // 4. 寻找第三个 '/'
    const char* end_new = strchr(start_new, '/');
    if (end_new == NULL) return -1; // 格式错误，没找到第三个 '/'

    // 5. 计算 old_str 的长度并分配内存 (+1 是为了存放 '\0')
    size_t len_old = end_old - start_old;
    *old_str = (char*)malloc(len_old + 1);
    if (*old_str == NULL) return -1; // 内存分配失败
    
    strncpy(*old_str, start_old, len_old);
    (*old_str)[len_old] = '\0'; // 必须手动加上字符串结束符

    // 6. 计算 new_str 的长度并分配内存
    size_t len_new = end_new - start_new;
    *new_str = (char*)malloc(len_new + 1);
    if (*new_str == NULL) {
        free(*old_str); // 如果这里分配失败，要记得把上面成功的释放掉，防止内存泄漏
        return -1;
    }
    
    strncpy(*new_str, start_new, len_new);
    (*new_str)[len_new] = '\0';


    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // TODO: 在这里添加你的代码
    if (str == NULL || old == NULL || new == NULL) return;

    // 1. 找到 old 在 str 中第一次出现的物理地址
    char* pos = strstr(str, old);
    if (pos == NULL) return; // 原字符串中没有要替换的内容，直接结束

    size_t len_old = strlen(old);
    size_t len_new = strlen(new);
    
    // 计算尾部需要搬运的字符长度 (即 old 字符串之后剩下的所有内容)
    size_t len_tail = strlen(pos + len_old); 

    // 2. 搬运尾部数据，为 new 字符串腾出或压缩空间
    // [极其重要]: 这里必须用 memmove，绝对不能用 strcpy 或 memcpy
    // +1 是为了把原字符串末尾自带的 '\0' 也一并搬运过去
    memmove(pos + len_new, pos + len_old, len_tail + 1);

    // 3. 将 new 字符串“镶嵌”到腾出的空位中
    // 这里不需要拷贝 '\0'，所以长度就是 len_new
    memcpy(pos, new, len_new);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}