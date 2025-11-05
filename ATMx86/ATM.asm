section .data
    ;Menu Optiions here. We can actually just start like this. wow assmebly is something.
    menu_prompt     db 10, "!!!!! ATM Menu !!!!!", 10
                    db "1. Depositing", 10
                    db "2. Withdrawing", 10
                    db "3. Checking Balance", 10
                    db "4. Exiting", 10
                    db "Enter your choice (1-4): ", 0
    menu_len        equ $ - menu_prompt
    
    ;Prompts for inputting 
    deposit_prompt  db 10, "Enter deposit amount: ", 0
    deposit_len     equ $ - deposit_prompt
    withdraw_prompt db 10, "Enter withdrawal amount: ", 0
    withdraw_len    equ $ - withdraw_prompt
    
    ;Output for when operations are done
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
    
    ;Initial original balance
    initial_balance dd 1000

section .bss
    balance     resd 1      ;Current balance (4 bytes)
    user_input  resb 10     
    digit_buffer resb 12    

section .text
    global _start

;Main program. I see where C gets its int main() thing
_start:
    ;Starting balance
    mov eax, [initial_balance]
    mov [balance], eax

main_loop:
    ;Display menu
    call display_menu
    
    ;user choice
    call get_input
    call parse_number
    mov ebx, eax        
    
    cmp ebx, 1
    je deposit_choice
    cmp ebx, 2
    je withdraw_choice
    cmp ebx, 3
    je balance_choice
    cmp ebx, 4
    je exit_choice
    
    mov ecx, invalid_choice
    mov edx, invalid_choice_len
    call print_string
    jmp main_loop

deposit_choice:
    call deposit_proc
    jmp main_loop

withdraw_choice:
    call withdraw_proc
    jmp main_loop

balance_choice:
    call check_balance_proc
    jmp main_loop

exit_choice:
    call exit_proc

display_menu:
    mov ecx, menu_prompt
    mov edx, menu_len
    call print_string
    ret

deposit_proc:
    ;Getting deposit amount
    mov ecx, deposit_prompt
    mov edx, deposit_len
    call print_string
    
    call get_input
    call parse_number
    
    ;Validating amount
    cmp eax, 0
    jle invalid_deposit
    
    ;Updating balance
    add [balance], eax
    
    ;Success message
    mov ecx, deposit_success
    mov edx, deposit_success_len
    call print_string
    ret

invalid_deposit:
    mov ecx, invalid_amount
    mov edx, invalid_amount_len
    call print_string
    ret

withdraw_proc:
    ;Getting withdrawal amount
    mov ecx, withdraw_prompt
    mov edx, withdraw_len
    call print_string
    
    call get_input
    call parse_number
    
    ;Validating amount
    cmp eax, 0
    jle invalid_withdraw
    
    cmp eax, [balance]
    jg insufficient_funds
    
    ;Updating balance
    sub [balance], eax
    
    mov ecx, withdraw_success
    mov edx, withdraw_success_len
    call print_string
    ret

insufficient_funds:
    mov ecx, insufficient_msg
    mov edx, insufficient_len
    call print_string
    ret

invalid_withdraw:
    mov ecx, invalid_amount
    mov edx, invalid_amount_len
    call print_string
    ret

check_balance_proc:
    mov ecx, balance_msg
    mov edx, balance_len
    call print_string
    
    mov eax, [balance]
    call print_number
    call print_newline
    ret

exit_proc:
    mov ecx, goodbye_msg
    mov edx, goodbye_len
    call print_string
    
    mov eax, 1          
    xor ebx, ebx        
    int 0x80

print_string:
    mov eax, 4          
    mov ebx, 1          
    int 0x80
    ret

get_input:
    mov eax, 3          
    mov ebx, 0          
    mov ecx, user_input
    mov edx, 10
    int 0x80
    ret

parse_number:
    xor eax, eax        
    xor ebx, ebx        
    mov ecx, user_input
    
parse_loop:
    mov bl, [ecx]
    cmp bl, 10          
    je parse_done
    cmp bl, '0'
    jb parse_error
    cmp bl, '9'
    ja parse_error
    
    ;Converting digit and adding to balance
    sub bl, '0'
    imul eax, 10
    add eax, ebx
    
    inc ecx
    jmp parse_loop

parse_error:
    ;Return -1 on error
    mov eax, -1         

parse_done:
    ret

print_number:
    push eax
    push ebx
    push ecx
    push edx
    push esi
    
    mov esi, digit_buffer + 11  
    mov byte [esi], 0           
    mov ebx, 10                 
    
    cmp eax, 0
    jne convert_loop
    
    dec esi
    mov byte [esi], '0'
    jmp print_digits

convert_loop:
    xor edx, edx       
    div ebx             
    add dl, '0'         
    dec esi             
    mov [esi], dl       
    
    test eax, eax       
    jnz convert_loop    

print_digits:
    mov ecx, digit_buffer + 12
    sub ecx, esi        
    
    mov eax, 4          ;sys_write
    mov ebx, 1          ;stdout
    mov edx, ecx        ;length
    mov ecx, esi        ;pointer to string
    int 0x80
    
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

;Printing newline at the end
print_newline:
    push ecx
    push edx
    mov ecx, newline
    mov edx, 1
    call print_string
    pop edx
    pop ecx
    ret