#include "diary.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

diary_entry entries[MAX_ENTRIES];
int entry_count = 0;

void encrypt_entry(const char* plaintext, char* encrypted) {
    int i = 0;
    while (plaintext[i] != '\0') {
        encrypted[i] = plaintext[i] + ENCRYPTION_KEY;
        i++;
    }
    encrypted[i] = '\0';
}

void decrypt_entry(const char* encrypted, char* plaintext) {
    int i = 0;
    while (encrypted[i] != '\0') {
        plaintext[i] = encrypted[i] - ENCRYPTION_KEY;
        i++;
    }
    plaintext[i] = '\0';
}

int verify_user() {
    char password[50]; //password is below
    char correct_password[] = "secret123"; 
    
    printf("Enter password: ");
    
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
    
    scanf("%49s", password);
    SetConsoleMode(hStdin, mode);
#else
    // Linux/Unix password input without echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    scanf("%49s", password);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    
    printf("\n");
    return strcmp(password, correct_password) == 0;
}

void store_encrypted_to_memory(const char* plaintext) {
    if (entry_count < MAX_ENTRIES) {
        encrypt_entry(plaintext, entries[entry_count].encrypted_data);
        strncpy(entries[entry_count].plaintext_data, plaintext, MAX_ENTRY_LENGTH - 1);
        entries[entry_count].is_encrypted = 1;
        entry_count++;
        printf("EStored encrypted successfully!\n");
    } else {
        printf("Diary is full! Cannot add more entries.\n");
    }
}

void render_plaintext(const char* encrypted) {
    char plaintext[MAX_ENTRY_LENGTH];
    decrypt_entry(encrypted, plaintext);
    printf("Decrypted Entry: %s\n", plaintext);
}