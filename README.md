# LowLevel-System-Functions
This git repo is for testing low level system functions of different niche instances

There are a total of 4 programs, some in their own folders. They are 
1. *ELF Disassembler:* We use objdump for binary type file disassembling 
2. *x86 ATM System:* An ATM simulation program but using Assembly and modular procedures.
3. *IoT Monitoring:* C extension with real-time working python dashboard showcasing the data.
4. *Diary Encryption:* Shared library for encrypted diary management.

# Running Each
 *1. ELF Disassembler:* 
 Compile: 
 gcc 1.ELF-Tool.c -o 1.ELF-Tool.exe
 ./disassembler 1.ELF-Tool.exe

 *2. x86 ATM System:*
 Compile:
 cd into "ATmx86" folder
 make ATM (only do this if .o file present of ATM and if ATM is present arleady no need).
 ./ATM

*3. IoT Monitoring:*
Compile:
cd into "System Monitoring System" folder
Install the python packages of matplotlib and setuptools as necessary. (If not already present).
make sensor_simulator
python3 iot_dashboard.py

*4. Diary Encryption:*
Compile
cd into "Encrypt-Decrypt" folder
LD_LIBRARY_PATH=. ./diary_manager




