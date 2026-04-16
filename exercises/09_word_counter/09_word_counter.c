#include <stdio.h>
#include <ctype.h>
int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    // TODO: 在这里添加你的代码
    int i = 0;
    while (str[i] != '\0') {
        // 1. 判断当前字符是不是单词的一部分（字母或者是单引号）
        int isCurrentWordChar = isalpha(str[i]) || str[i] == '\'';
        
        // 2. 判断前一个字符是不是分隔符（如果 i==0，说明是第一个字符，默认前面是分隔符）
        int isPrevSeparator = (i == 0) || !(isalpha(str[i-1]) || str[i-1] == '\'');
        
        // 3. 如果当前是单词字符，且前一个是分隔符，说明我们遇到了一个新单词！
        if (isCurrentWordChar && isPrevSeparator) {
            wordCount++;
        }
        
        i++; // 检查下一个字符
    }
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}