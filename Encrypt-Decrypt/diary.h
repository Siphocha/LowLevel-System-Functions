#ifndef DIARY_H
#define DIARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100
#define MAX_ENTRY_LENGTH 1000
#define ENCRYPTION_KEY 4

typedef struct {
    char encrypted_data[MAX_ENTRY_LENGTH];
    char plaintext_data[MAX_ENTRY_LENGTH];
    int is_encrypted;
} diary_entry;

void encrypt_entry(const char* plaintext, char* encrypted);
void decrypt_entry(const char* encrypted, char* plaintext);
int verify_user();
void store_encrypted_to_memory(const char* plaintext);
void render_plaintext(const char* encrypted);

extern diary_entry entries[MAX_ENTRIES];
extern int entry_count;

#endif