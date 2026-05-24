#include "rvemu.h"

void mmu_load_elf(mmu_t *mmu, int fd) {
    u8 buf[sizeof(elf64_ehdr_t)];
    FILE *file = fdopen(fd, "rb");
    if (fread(buf, 1, sizeof(elf64_ehdr_t), file) != sizeof(elf64_ehdr_t)) {
        fatal("file too small");
    }
    // Check that the file is a valid ELF file.
    elf64_ehdr_t *ehdr = (elf64_ehdr_t *)buf;   // cast buf to a pointer.
    if (*(u32 *)ehdr != *(u32 *)ELFMAG) {   // tricking the compiler to process all 4 bytes at once.
        fatal("bad ELF file");
    }
    // Check that the ELF file is for RISC-V.
    if (ehdr->e_machine != EM_RISCV || ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
        fatal("unsupported ELF file");
    }

    mmu->entry = ehdr->e_entry;
}