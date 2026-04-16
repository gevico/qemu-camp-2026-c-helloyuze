#include "myfile.h"

void print_elf_type(uint16_t e_type) {
    const char *type_str;
    switch (e_type) {
    case ET_NONE:
      type_str = "Unknown (ET_NONE)";
      break;
    case ET_REL:
      type_str = "Relocatable (ET_REL)";
      break;
    case ET_EXEC:
      type_str = "Executable (ET_EXEC)";
      break;
    case ET_DYN:
      type_str = "Shared Object/PIE (ET_DYN)";
      break;
    case ET_CORE:
      type_str = "Core Dump (ET_CORE)";
      break;
    default:
      if (e_type >= ET_LOOS && e_type <= ET_HIOS)
        type_str = "OS-Specific";
      else if (e_type >= ET_LOPROC && e_type <= ET_HIPROC)
        type_str = "Processor-Specific";
      else
        type_str = "Invalid";
    }
    printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int __cmd_myfile(const char* filename) {
    char filepath[256];
    int fd;
    Elf64_Ehdr ehdr;

    strcpy(filepath, filename);
    fflush(stdout);
    printf("filepath: %s\n", filepath);

    // TODO: 在这里添加你的代码
    fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("  [错误] 无法打开文件");
        printf("\n");
        return -1;
    }

    // 2. 读取 ELF 头部 (只读取 sizeof(Elf64_Ehdr) 这么大的数据，也就是文件最开头的 64 字节左右)
    ssize_t bytes_read = read(fd, &ehdr, sizeof(Elf64_Ehdr));
    if (bytes_read != sizeof(Elf64_Ehdr)) {
        printf("  [错误] 无法读取完整的 ELF 头部\n\n");
        close(fd);
        return -1;
    }

    // 3. 安全检查：验证 Magic Number (魔数)
    // 所有的 ELF 文件前四个字节必须是: 0x7F, 'E', 'L', 'F'
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        printf("  [错误] 这不是一个合法的 ELF 文件！\n\n");
        close(fd);
        return -1;
    }


    print_elf_type(ehdr.e_type);
    close(fd);
    return 0;
}