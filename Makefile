export ROOT_PATH := $(shell pwd)

include tools/config.mk

KERNEL_SRC_DIR := init kernel kernel/driver kernel/mem lib debug

KERNEL_SRC := $(wildcard .cpp, $(addsuffix /*.cpp, $(addprefix $(ROOT_PATH)/, $(KERNEL_SRC_DIR))))
KERNEL_TMP := $(KERNEL_SRC:cpp=o)
KERNEL_OBJ := $(subst $(ROOT_PATH), $(ROOT_PATH)/build, $(KERNEL_TMP))

.PHONY: creat_dir FORCE
init: creat_dir

creat_dir:
	$(A) mkdir -p $(BUILD_DIR);
	$(A) mkdir -p $(LOG_DIR);
	$(A) mkdir -p $(BIN_DIR)

################################################################################################
$(TOOL_DIR)/sign: $(TOOL_DIR)/sign.cpp
	$(A) make -C $(TOOL_DIR)
	
$(BIN_DIR)/bootloader: FORCE
	$(A) make -C $(BOOT_DIR)

$(BIN_DIR)/kernel: $(KERNEL_OBJ)
	$(A) $(LD) $(LDCPPFLAGS) -o $(BIN_DIR)/kernel.tmp $(KERNEL_OBJ);
	$(A) objdump -S $(BIN_DIR)/kernel.tmp > $(LOG_DIR)/kernel.log.asm;
	$(A) objdump -t $(BIN_DIR)/kernel.tmp > $(LOG_DIR)/kernel.log.sym;
	$(A) nm $(BIN_DIR)/kernel.tmp | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aU] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)'| sort\
		> $(LOG_DIR)/kernel.map;
	$(A) cp -f $(BIN_DIR)/kernel.tmp kernel.tmp;
	$(A) strip kernel.tmp;
	$(A) objcopy -O binary -R .note -R .comment kernel.tmp $@;
	$(A) rm kernel.tmp;

$(KERNEL_OBJ):$(ROOT_PATH)/build/%.o: $(ROOT_PATH)/%.cpp
	$(A) $(CPP) $(CPPFLAGS) -I$(INCLUDE_DIR) -Os -c $< -o $@
	
################################################################################################
$(TOYOS_IMG): $(BIN_DIR)/bootloader $(BIN_DIR)/kernel $(TOOL_DIR)/sign
	$(A) dd if=/dev/zero of=$@.tmp bs=512 count=2000 > /dev/null 2>&1;
	$(A) dd if=$(BIN_DIR)/bootloader of=$@.tmp conv=notrunc > /dev/null 2>&1;
	$(A) dd if=$(BIN_DIR)/kernel seek=1 bs=512 count=4 of=$@.tmp conv=notrunc > /dev/null 2>&1;
	$(A) $(TOOL_DIR)/sign $@.tmp;
	$(A) mv $@.tmp $@;

################################################################################################
qemu: $(TOYOS_IMG)
	$(A)$(QEMU) -no-reboot -parallel stdio -hda $< -serial null -k en-us -m 256M

debug: $(TOYOS_IMG)
	$(A)$(QEMU) -S -s -parallel stdio -hda $< -serial null &
	$(A)sleep 2
	$(A)$(TERMINAL) -e "gdb -q -x tools/gdbinit"

log: $(TOYOS_IMG)
	$(A) hexdump -C $^ > $(LOG_DIR)/$^.$@

################################################################################################
include $(KERNEL_OBJ:.o=.d)
$(ROOT_PATH)/build/%.d: $(ROOT_PATH)/%.cpp
	$(A) mkdir -p $(addprefix $(BUILD_DIR)/, $(KERNEL_SRC_DIR)); \
	$(A) set -e; \
	$(A) rm -rf $@; \
	$(A) $(CPP) -M $(CPPFLAGS) -I$(INCLUDE_DIR) $< > $@.$$$$; \
	$(A) sed 's,\($(notdir $*)\).o[ :]*, $(dir $@)\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(A) rm -rf $@.$$$$

################################################################################################
.PHONY: clean dist-clean
clean:
	$(A) rm -rf $(TOYOS_IMG) log/* build/* bin/*

dist-clean:
	$(A) rm -rf $(TOYOS_IMG) log build bin

