#include <stdio.h>

int main() {
    char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
    char destination[101];
    
	// TODO: 在这里添加你的代码
    char *src = source;       // 定义指针 src，指向源字符串的开头
    char *dest = destination; // 定义指针 dest，指向目标字符串的开头
    
    // 只要 src 当前指向的字符不是结束符 '\0'，就一直循环
    while (*src != '\0') {
        *dest = *src; // 1. 将 src 指向的字符，赋值到 dest 指向的内存里
        src++;        // 2. src 指针向后移动一个字符的位置
        dest++;       // 3. dest 指针也向后移动一个字符的位置
    }
    
    // 极其重要的一步：拷贝结束后，手动在目标字符串末尾加上结束符 '\0'
    *dest = '\0';
    
    printf("拷贝后的字符串: %s", destination);
    
    return 0;
}