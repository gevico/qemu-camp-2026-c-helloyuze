#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];
  int paritition(int left, int right) { // 划分函数，引入随机化策略
    int k = left + rand() % (right - left + 1); // 生成 [left, right] 区间的随机数

    Student temp= students[k];
    students[k]=students[left];
    students[left]=temp;

    int i = left, j = right;
    Student pivot =  students[left];
    while (i < j) {
        while ( students[j].score <pivot.score && i < j) j--; // 找右侧大于或等于 pivot 的数
        if (i < j)
        students[i++] =  students[j]; // 覆盖
        while ( students[i].score > pivot.score && i < j) i++; // 找左侧小于或等于 pivot 的数
        if (i < j)
        students[j--] =  students[i]; // 覆盖
    }
    students[i] = pivot; // 放到中间
    return i;
}

 
void quick_sort(int left, int right)  {
    if (left< right) {
      int pivot = paritition(left,right);
      quick_sort( left, pivot - 1);
      quick_sort( pivot + 1, right);
    }
  }

int main(void) {
    FILE *file = fopen("03_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 03_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d（应为 1-%d）\n", n, MAX_STUDENTS);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    quick_sort(0, n - 1);

    // 输出结果
    printf("\n快速排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}