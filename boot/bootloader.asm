	.equ    CR0_PE_OPEN,		1
	.set 	PROT_MODE_CSEG,        0x8                     # kernel code segment selector
	.set 	PROT_MODE_DSEG,        0x10                    # kernel data segment selector
	.set 	SMAP,                  0x534d4150              # 'SMAP' ASCII code

	.code16
	.global _start, HLT_LOOP
_start:
	cli
	cld	
	xorw %ax, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %fs

	movw %ax, %ss
	movw $0xfe00, %sp

# 设置图形模式
#	movw $0x0013, %ax
#	int $0x10

probe_memory:
	movl $0, 0x8000;
	xorl %ebx, %ebx;
	movw $0x8004, %di
start_probe:
	movl $0xE820, %eax
	movl $20, %ecx
	movl $SMAP, %edx
	int $0x15
	jnc cont
	movw $12345, 0x8000
	jmp finish_probe
cont:
	addw $20, %di
	incl 0x8000
	cmpl $0, %ebx
	jnz start_probe

finish_probe:
	# Switch from real to protected mode
	lgdt gdtdesc
	movl %cr0, %eax
	orl  CR0_PE_OPEN, %eax
	movl %eax, %cr0

	ljmp $PROT_MODE_CSEG, $protect_seg

	.code32
protect_seg:
	movw $PROT_MODE_DSEG, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	
	movl $_start, %esp
	movl $0, %ebp
	call bootmain
	
HLT_LOOP:
	jmp HLT_LOOP

# Bootstrap GDT
	.p2align 2                                      # force 4 byte alignment
gdt:
	.quad 0
	.word 0xffff, 0x0000, 0x9a00, 0x00cf
	.word 0xffff, 0x0000, 0x9200, 0x00cf
endgdt:

gdtdesc:
	.word endgdt-gdt-1                              # sizeof(gdt) - 1
	.long gdt                                       # address gdt

