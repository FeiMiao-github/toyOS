# A := @
# x86_64

BIT := i386 

AS_FLAGS = -g -O -n -Iinclude
LDFLAGS =  -m elf_$(BIT) -nostdlib -N -e _start -Ttext 0x7c00

CPP := g++
CPPFLAGS := -g -fno-builtin -fno-stack-protector -fomit-frame-pointer -fstrength-reduce -nostdinc -Wall
CPPFLAGS += -std=c++2a
LDCPPFLAGS := -m elf_$(BIT) -nostdlib -N -e 0 -Ttext 0x10000

CC := gcc
CCFLAGS := -g -fno-builtin -fno-stack-protector -fomit-frame-pointer -fstrength-reduce -nostdinc -Wall

AS_FLAGS += --32
CPPFLAGS += -m32
CCFLAGS += -m32

QEMU := qemu-system-$(BIT)
TERMINAL := gnome-terminal
TOYOS_IMG := toyOS.img

TOOL_DIR := $(ROOT_PATH)/tools
BUILD_DIR := $(ROOT_PATH)/build
LOG_DIR := $(ROOT_PATH)/log
BIN_DIR := $(ROOT_PATH)/bin
BOOT_DIR := $(ROOT_PATH)/boot
INIT_DIR := $(ROOT_PATH)/init
KERNEL_DIR := $(ROOT_PATH)/kernel
INCLUDE_DIR := $(ROOT_PATH)/include

