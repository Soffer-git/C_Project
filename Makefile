all: arm_emulator *.c *.h
	
arm_emulator: main.c *.c
	gcc $^ -g -o $@