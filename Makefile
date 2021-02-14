all: arm_emulator
	
arm_emulator: main.c *.c 
	gcc $^ -g -o $@