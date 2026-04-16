// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
  if (!str || *str == '\0') {
      return;  // 空指针或空字符串
  }

  // 1. 找到第一个非空白字符
  char *start = str;
  while (*start && isspace((unsigned char)*start)) {
      start++;
  }

  // 2. 如果全是空白，设为空字符串
  if (*start == '\0') {
      str[0] = '\0';
      return;
  }

  // 3. 找到最后一个非空白字符
  char *end = start + strlen(start) - 1;
  while (end > start && isspace((unsigned char)*end)) {
      end--;
  }

  // 4. 截断尾部空白
  *(end + 1) = '\0';

  // 5. 将有效内容移到字符串开头
  if (start != str) {
      memmove(str, start, end - start + 2);  // +2 包含 '\0'
  }
}


int load_dictionary(const char *filename, HashTable *table,
  uint64_t *dict_count) {
FILE *file = fopen(filename, "r");
if (!file) {
perror("无法打开词典文件");
return -1;
}

char line[1024];
char current_word[100] = {0};
char current_translation[1024] = {0};
int in_entry = 0;

// 逐行读取字典文件
while (fgets(line, sizeof(line), file) != NULL) {
// 去除行尾的换行符
line[strcspn(line, "\n")] = '\0';

// 判断是否为单词行（以 '#' 开头）
if (line[0] == '#') {
// 提取单词，跳过 '#'
strcpy(current_word, line + 1);
trim(current_word);
in_entry = 1;
}
// 判断是否为翻译行（以 "Trans:" 开头）
else if (strncmp(line, "Trans:", 6) == 0 && in_entry) {
// 提取翻译，跳过 "Trans:"
strcpy(current_translation, line + 6);
trim(current_translation);

// 插入哈希表
hash_table_insert(table, current_word, current_translation);
(*dict_count)++;

in_entry = 0;
}
}

fclose(file);
return 0;
}
