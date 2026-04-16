#include <stdio.h>

// TODO：定义四则运算规则
// hint：似乎除数有一些特殊情况，需要处理


int add(int a, int b) { 
    return a+b ;
 }
int subtract(int a, int b) {  return a-b ;}
int multiply(int a, int b) {return a*b ;  }
int divide(int a, int b) {
    if (b == 0) {
       printf("错误：除数不能为0\n");
        return 0;
    }
    return a / b;
}



/**
 * @brief 计算器程序的主函数
 *
 * 该函数实现了一个简单的命令行计算器，支持加、减、乘、除四种基本运算。
 * 用户输入两个整数和一个运算符，程序通过函数指针数组调用相应的运算函数，
 * 并输出计算结果。
 *
 * @return int 程序退出状态码，0表示正常退出，1表示遇到错误（如无效运算符）
 */
int main() {
    int a, b;
    char op;

    // TODO
    // hint:这是一个函数指针数组，在这之前应该先定义指针
    int (*operations[])(int, int)= {add, subtract, multiply, divide};

    char operators[] = "+-*/";

    printf("输入两个整数和一个运算符 (+, -, *, /): ");
    scanf("%d %d %c", &a, &b, &op);

    // 查找对应的运算符索引
    int index = -1;
    for (int i = 0; i < 4; i++) {
        if (op == operators[i]) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("无效的运算符\n");
        return 1;
    }

    int result = operations[index](a, b);
    printf("%d %c %d = %d\n", a, op, b, result);

    return 0;
}