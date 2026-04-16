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

    // TODO: 在这里添加你的代码
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

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

static int build_dict_path(const char *filename, char *dict_path, size_t dict_path_size) {
  const char *last_slash;
  size_t dir_len;

  if (!filename || !dict_path || dict_path_size == 0) {
    return -1;
  }

  last_slash = strrchr(filename, '/');
  if (!last_slash) {
    return snprintf(dict_path, dict_path_size, "dict.txt") >= 0 ? 0 : -1;
  }

  dir_len = (size_t)(last_slash - filename);
  if (dir_len + strlen("/dict.txt") + 1 > dict_path_size) {
    return -1;
  }

  memcpy(dict_path, filename, dir_len);
  dict_path[dir_len] = '\0';
  strcat(dict_path, "/dict.txt");
  return 0;
}

int __cmd_mytrans(const char* filename) {
  HashTable *table = create_hash_table();
  char dict_path[512] = {0};
  const char *dict_candidates[3] = {NULL, "src/mytrans/dict.txt", "dict.txt"};
  int dict_loaded = 0;
  int i;
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (build_dict_path(filename, dict_path, sizeof(dict_path)) == 0) {
    dict_candidates[0] = dict_path;
  }

  for (i = 0; i < 3; i++) {
    if (!dict_candidates[i]) {
      continue;
    }
    if (load_dictionary(dict_candidates[i], table, &dict_count) == 0) {
      dict_loaded = 1;
      break;
    }
  }

  if (!dict_loaded) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件: %s\n", filename);
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    // 使用 strtok 按空格分割单词
    char *word = strtok(line, " ");
    while (word != NULL) {
      const char *translation = hash_table_lookup(table, word);
      printf("原文: %s\t", word);
      if (translation) {
          printf("翻译: %s\n", translation);
      } else {
          printf("未找到该单词的翻译。\n");
      }

      word = strtok(NULL, " ");
    }
  }

  free_hash_table(table);
  return 0;
}
