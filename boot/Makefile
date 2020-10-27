include $(ROOT_PATH)/tools/config.mk

CUR_OBJ_DIR := $(BUILD_DIR)/boot
CUR_SRC_CPP_FILE := $(notdir $(wildcard .c, ./*.c))
CUR_OBJ_FILE := $(addprefix $(CUR_OBJ_DIR)/, $(CUR_SRC_CPP_FILE:c=o))
##################################################################################################################
all: creat_dir $(BIN_DIR)/bootloader

.PHONY: creat_dir
creat_dir:	
	$(A) mkdir -p $(CUR_OBJ_DIR)
	
################################################################################################
$(BIN_DIR)/bootloader: $(CUR_OBJ_DIR)/bootloader.out
	$(A)nm $< | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aU] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)'| sort \
		> $(LOG_DIR)/bootloader.map;
	$(A) objcopy  -S -O binary $< $@

$(CUR_OBJ_DIR)/bootloader.out: $(CUR_OBJ_DIR)/bootloader.obj $(CUR_OBJ_FILE)
	$(A) ld $(LDFLAGS) $^ -o $@;
	$(A) rm -rf bootloader.tmp;
	$(A) objdump -S $@ > $(LOG_DIR)/bootloader.log.asm

$(CUR_OBJ_DIR)/bootloader.obj: bootloader.asm
	$(A) as $(AS_FLAGS) $< -o $@

$(CUR_OBJ_FILE): $(CUR_OBJ_DIR)/%.o: %.c
	$(A) $(CC) -c $(CCFLAGS) -I$(INCLUDE_DIR) -O3 $< -o $@



