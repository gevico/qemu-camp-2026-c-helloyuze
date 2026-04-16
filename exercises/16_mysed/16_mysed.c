#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

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

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}