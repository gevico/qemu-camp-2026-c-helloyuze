#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
      case ET_REL:  
          type_str = "Relocatable"; 
          break;
      case ET_EXEC: 
          type_str = "Executable"; 
          break;
      case ET_DYN:  
          type_str = "Shared Object/PIE"; 
          break;
      default:      
          // 把 ET_NONE 和其他所有不认识的类型都归为 Unknown
          type_str = "Unknown"; 
          break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  char filepath[2][256] = {
    "./17_myfile.o",
    "./17_myfile",
  };

  int fd;
  Elf64_Ehdr ehdr;

  // TODO: 在这里添加你的代码
  for (int i = 0; i < 2; i++) {
    printf("=== 解析文件: %s ===\n", filepath[i]);

    // 1. 打开文件 (只读模式)
    fd = open(filepath[i], O_RDONLY);
    if (fd < 0) {
        perror("  [错误] 无法打开文件");
        printf("\n");
        continue;
    }

    // 2. 读取 ELF 头部 (只读取 sizeof(Elf64_Ehdr) 这么大的数据，也就是文件最开头的 64 字节左右)
    ssize_t bytes_read = read(fd, &ehdr, sizeof(Elf64_Ehdr));
    if (bytes_read != sizeof(Elf64_Ehdr)) {
        printf("  [错误] 无法读取完整的 ELF 头部\n\n");
        close(fd);
        continue;
    }

    // 3. 安全检查：验证 Magic Number (魔数)
    // 所有的 ELF 文件前四个字节必须是: 0x7F, 'E', 'L', 'F'
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        printf("  [错误] 这不是一个合法的 ELF 文件！\n\n");
        close(fd);
        continue;
    }

    // 4. 调用我们补全的函数，传入结构体里的 e_type 字段
    print_elf_type(ehdr.e_type);

    // 5. 关闭文件
    close(fd);
    printf("\n");
}
  return 0;
}