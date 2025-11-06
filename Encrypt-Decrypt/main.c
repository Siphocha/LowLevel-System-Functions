#include "diary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_menu();
void add_entry();
void view_entries();
void clear_input_buffer();

int main() {
    printf("=== Personal Diary Manager ===\n");
    printf("Secure encryption with symmetric key (key = 4)\n\n");
    
    int choice;
    
    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch(choice) {
            case 1:
                add_entry();
                break;
            case 2:
                view_entries();
                break;
            case 3:
                printf("Goodbye! Your diary entries are securely encrypted.\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 3);
    
    return 0;
}

void display_menu() {
    printf("\n--- Diary Menu ---\n");
    printf("1. Add new diary entry\n");
    printf("2. View all diary entries\n");
    printf("3. Exit\n");
}

void add_entry() {
    char entry[MAX_ENTRY_LENGTH];
    
    printf("Enter your diary entry (max %d characters):\n", MAX_ENTRY_LENGTH - 1);
    if (fgets(entry, sizeof(entry), stdin) != NULL) {
        // Remove newline character
        entry[strcspn(entry, "\n")] = 0;
        
        if (strlen(entry) > 0) {
            store_encrypted_to_memory(entry);
        } else {
            printf("Entry cannot be empty!\n");
        }
    }
}

void view_entries() {
    if (entry_count == 0) {
        printf("No diary entries found.\n");
        return;
    }
    
    printf("\n=== Accessing Encrypted Diary ===\n");
    if (!verify_user()) {
        printf("Authentication failed! Access denied.\n");
        return;
    }
    
    printf("\n=== Your Diary Entries (Decrypted) ===\n");
    for (int i = 0; i < entry_count; i++) {
        printf("\nEntry %d:\n", i + 1);
        printf("Encrypted (stored): %s\n", entries[i].encrypted_data);
        printf("Decrypted (view): ");
        render_plaintext(entries[i].encrypted_data);
        printf("---\n");
    }
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}