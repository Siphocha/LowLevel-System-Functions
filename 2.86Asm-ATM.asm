section .data
    ; Menu strings
    menu_prompt     db 10, "=== ATM Menu ===", 10
                    db "1. Deposit", 10
                    db "2. Withdraw", 10
                    db "3. Check Balance", 10
                    db "4. Exit", 10
                    db "Enter your choice (1-4): ", 0
    menu_len        equ $ - menu_prompt
    
    ; Input prompts
    deposit_prompt  db 10, "Enter deposit amount: ", 0
    deposit_len     equ $ - deposit_prompt
    withdraw_prompt db 10, "Enter withdrawal amount: ", 0
    withdraw_len    equ $ - withdraw_prompt
    
    ; Output messages
    balance_msg     db 10, "Current balance: ", 0
    balance_len     equ $ - balance_msg
    deposit_success db 10, "Deposit successful!", 10, 0
    deposit_success_len equ $ - deposit_success
    withdraw_success db 10, "Withdrawal successful!", 10, 0
    withdraw_success_len equ $ - withdraw_success
    insufficient_msg db 10, "Error: Insufficient funds!", 10, 0
    insufficient_len equ $ - insufficient_msg
    invalid_choice  db 10, "Error: Invalid choice!", 10, 0
    invalid_choice_len equ $ - invalid_choice
    invalid_amount  db 10, "Error: Invalid amount!", 10, 0
    invalid_amount_len equ $ - invalid_amount
    goodbye_msg     db 10, "Thank you for using our ATM!", 10, 0
    goodbye_len     equ $ - goodbye_msg
    
    newline         db 10, 0
    
    ; Initial balance
    initial_balance dd 1000

section .bss
    balance     resd 1      ; Current balance (4 bytes)
    user_input  resb 10     ; Buffer for user input
    amount      resd 1      ; Temporary storage for amounts

section .text
    global _start

; Main program
_start:
    ; Initialize balance
    mov eax, [initial_balance]
    mov [balance], eax

main_loop:
    ; Display menu
    call display_menu
    
    ; Get user choice
    call get_input
    call parse_number
    mov ebx, eax        ; Store choice in EBX
    
