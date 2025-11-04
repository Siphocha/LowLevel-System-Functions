#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <elf_executable_path>\n", argv[0]);
        return 1;
    }

    // Use fixed output filename "random.txt"
    const char *output_file = "random.txt";

    // Create the objdump command
    char command[1024];
    snprintf(command, sizeof(command),
             "objdump -d -j .text \"%s\" > \"%s\"",
             argv[1], output_file);

    printf("Disassembling .text section of '%s'...\n", argv[1]);
    printf("Output will be saved to '%s'\n", output_file);

    // Execute command
    int result = system(command);

    if (result == 0) {
        printf("Disassembly completed successfully!\n");
        printf("Output saved to: %s\n", output_file);
    } else {
        fprintf(stderr, "Error: Disassembly failed. Make sure objdump is available.\n");
        return 1;
    }

    return 0;
}