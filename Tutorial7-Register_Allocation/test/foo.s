	.text
	.abicalls
	.option	pic0
	.section	.mdebug.abi32,"",@progbits
	.nan	legacy
	.text
	.file	"foo.ll"
	.globl	foo                             # -- Begin function foo
	.p2align	2
	.type	foo,@function
	.set	nomicromips
	.set	nomips16
	.ent	foo
foo:                                    # @foo
	.cfi_startproc
	.frame	$sp,32,$ra
	.mask 	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	.set	noat
# %bb.0:
	addiu	$sp, $sp, -32
	.cfi_def_cfa_offset 32
	sw	$ra, 28($sp)                    # 4-byte Folded Spill
	sw	$17, 24($sp)                    # 4-byte Folded Spill
	sw	$16, 20($sp)                    # 4-byte Folded Spill
	.cfi_offset 31, -4
	.cfi_offset 17, -8
	.cfi_offset 16, -12
	jal	def
	nop
	beqz	$2, $BB0_2
	move	$16, $2
# %bb.1:
	jal	def
	nop
	move	$17, $2
	jal	use
	move	$4, $16
	j	$BB0_3
	addiu	$16, $17, 1
$BB0_2:
	jal	def
	nop
	move	$17, $2
	jal	use
	move	$4, $16
	addiu	$16, $17, 1
$BB0_3:
	jal	def
	nop
	jal	use
	move	$4, $2
	move	$2, $16
	lw	$16, 20($sp)                    # 4-byte Folded Reload
	lw	$17, 24($sp)                    # 4-byte Folded Reload
	lw	$ra, 28($sp)                    # 4-byte Folded Reload
	jr	$ra
	addiu	$sp, $sp, 32
	.set	at
	.set	macro
	.set	reorder
	.end	foo
$func_end0:
	.size	foo, ($func_end0)-foo
	.cfi_endproc
                                        # -- End function
	.globl	def                             # -- Begin function def
	.p2align	2
	.type	def,@function
	.set	nomicromips
	.set	nomips16
	.ent	def
def:                                    # @def
	.cfi_startproc
	.frame	$sp,0,$ra
	.mask 	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	.set	noat
# %bb.0:
	lui	$1, %hi(g)
	lw	$2, %lo(g)($1)
	addiu	$3, $2, 1
	jr	$ra
	sw	$3, %lo(g)($1)
	.set	at
	.set	macro
	.set	reorder
	.end	def
$func_end1:
	.size	def, ($func_end1)-def
	.cfi_endproc
                                        # -- End function
	.globl	use                             # -- Begin function use
	.p2align	2
	.type	use,@function
	.set	nomicromips
	.set	nomips16
	.ent	use
use:                                    # @use
	.cfi_startproc
	.frame	$sp,0,$ra
	.mask 	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	.set	noat
# %bb.0:
	lui	$2, %hi(g)
	jr	$ra
	sw	$4, %lo(g)($2)
	.set	at
	.set	macro
	.set	reorder
	.end	use
$func_end2:
	.size	use, ($func_end2)-use
	.cfi_endproc
                                        # -- End function
	.type	g,@object                       # @g
	.bss
	.globl	g
	.p2align	2
g:
	.4byte	0                               # 0x0
	.size	g, 4

	.section	".note.GNU-stack","",@progbits
	.text
