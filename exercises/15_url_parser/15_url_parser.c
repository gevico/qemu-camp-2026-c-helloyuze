#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */
 int parse_url(const char* url) {
    int err = 0;
    
    // 1. 健壮性检查：防止传入空指针
    if (url == NULL) {
        err = -EINVAL;
       
    }

    // 2. 找到参数的起点 '?'
    // strchr 返回指向第一次出现 '?' 位置的指针
    const char* question_mark = strchr(url, '?');
    if (question_mark == NULL) {
        // 没找到 '?'，说明没有参数，直接退出
        
    }

    // 3. 动态分配内存，复制 '?' 后面的查询字符串
    // 为什么要复制？因为原字符串 url 是 const 的，不能被修改！
    // question_mark + 1 就是跳过 '?' 号，从 "name=..." 开始复制
    char* query_string = strdup(question_mark + 1);
    if (query_string == NULL) {
        err = -ENOMEM; // 内存分配失败
      
    }

    // 4. 开始像切香肠一样切割字符串
    // strtok 会把找到的 '&' 替换成 '\0' (字符串结束符)
    char* pair = strtok(query_string, "&");
    
    while (pair != NULL) {
        // 此时 pair 指向类似 "name=John" 的字符串
        // 我们需要在这个键值对里找 '='
        char* eq_sign = strchr(pair, '=');
        
        if (eq_sign != NULL) {
            // 找到了 '='，我们把它强行改成 '\0'
            // 这样就把 "name=John" 切成了 "name\0John"
            *eq_sign = '\0'; 
            
            char* key = pair;            // key 的起点
            char* value = eq_sign + 1;   // value 的起点 (跳过刚才修改的 '\0')
            
            printf("key = %s, value = %s\n", key, value);

        } else {
            // 处理异常情况：只有 key 没有 '=' 和 value
            printf("%s : (null)\n", pair);
        }
        
        // 告诉 strtok 继续在刚才的字符串里找下一个 '&'
        pair = strtok(NULL, "&");
    }

    // 5. 释放我们前面 strdup 申请的堆内存！(极其重要)
    free(query_string);


}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}