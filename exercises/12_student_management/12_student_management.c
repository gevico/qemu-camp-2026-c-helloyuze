#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[20];
    char name[50];
    int age;
} Student;

int main() {
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return 1;
    }
    
    Student *students[3];
    
    for (int i = 0; i < 3; i++) 
    {
	    // TODO: 在这里添加你的代码
       
        students[i] = (Student*)malloc(sizeof(Student));
        
        // 养成好习惯：检查内存是否分配成功
        if (students[i] == NULL) {
            printf("内存分配失败！\n");
            fclose(file); // 记得关文件
            return 1;
        }

        // 2. 从文件中读取数据，存入刚分配的结构体中
        // 注意：读取字符串（id, name）不需要加 &，读取整数（age）需要加 & 取地址
        fscanf(file, "%19s %49s %d", students[i]->id, students[i]->name, &students[i]->age);
    }
    

    
    fclose(file);
    
    for (int i = 0; i < 3; i++) 
    {
        printf("学号：%s, 姓名：%s, 年龄：%d\n", 
               students[i]->id, 
               students[i]->name, 
               students[i]->age);
    }
    
    for (int i = 0; i < 3; i++) 
    {
        free(students[i]);
    }
    
    return 0;
}