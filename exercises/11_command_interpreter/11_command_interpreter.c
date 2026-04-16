#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 10
#define MAX_INPUT_LENGTH 256

int shell_parse(char *buf, char *argv[]);
void execute_command(int argc, char *argv[]);

// in:  format -> hello world "hello world" 1234567
// Multiple command parameters entered from the command line (number of parameters does not exceed 10)
// out:  format -> Parameter X: Content, Length: X
// Parse the command parameters through a character pointer array, and sequentially display the content and length of each parameter on the screen
//
int main(void)
{
    FILE *file;
    char input[MAX_INPUT_LENGTH];
    char *argv[MAX_ARGS + 1] = {NULL};
    int argc = 0;

    file = fopen("command_file.txt", "r");
    if (!file) {
        fprintf(stderr, "❌ Error: Cannot open input file '%s'\n", "command_file.txt");
        return 1;
    }

    printf("✅ Reading commands from '%s':\n\n", "command_file.txt");

    while (fgets(input, MAX_INPUT_LENGTH, file) != NULL) {
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0 || strspn(input, " \t") == strlen(input)) {
            continue;
        }

        printf("➡️  Input: %s\n", input);

        argc = shell_parse(input, argv);

        if (argc == 0) {
            printf("⚠️  No valid command parsed.\n\n");
            continue;
        }

        execute_command(argc, argv);
        printf("\n");
    }

    fclose(file);
    return 0;
}

// shell_parse 和 execute_command 保持不变
int shell_parse(char *buf, char *argv[])
{
    int argc = 0;
    int state = 0;
    // TODO: 在这里添加你的代码，完成命令行解析
    // 功能：将输入字符串buf按空格分割成多个参数，存入argv数组
    // 返回：参数个数argc
    // 提示：使用状态机的方式处理，注意处理字符串结束符
  
    // 遍历整个输入字符串，直到遇到真正的字符串结束符 '\0'
    for (int i = 0; buf[i] != '\0'; i++) {
        
        if (state == 0) {
            // 当前在单词外部
            if (buf[i] != ' ') {
                // 遇到非空格字符，说明找到了一个新单词的开头！
                argv[argc] = &buf[i]; // 把这个字符的地址存入 argv 数组
                argc++;               // 参数数量加 1
                state = 1;            // 切换状态：进入单词内部
            }
        } 
        else if (state == 1) {
            // 当前在单词内部
            if (buf[i] == ' ') {
                // 遇到空格，说明这个单词结束了！
                buf[i] = '\0';        // 关键操作：把空格替换为结束符，就地截断字符串
                state = 0;            // 切换状态：离开单词，回到外部寻找下一个单词
            }
        }
    }
    
    // 按照标准规范，argv 数组的最后一个有效参数后面，通常需要补一个 NULL 指针。
    // 这对于后续调用 execvp 等系统函数执行命令是必须的。
    argv[argc] = NULL; 
    
    return argc;
}

void execute_command(int argc, char *argv[])
{
    printf("Parsing result: Total %d parameters\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("Parameter %d: Content: %s, Length: %zu\n",
               i + 1, argv[i], strlen(argv[i]));
    }

    if (strcmp(argv[0], "help") == 0) {
        printf("This is help command\n");
    } else if (strcmp(argv[0], "echo") == 0) {
        printf("Echo: ");
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    } else if (strcmp(argv[0], "add") == 0 && argc == 3) {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        printf("%d + %d = %d\n", a, b, a + b);
    } else {
        printf("Unknown command: %s\n", argv[0]);
    }
}