	.data
L_4:
	.asciiz "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
L_2:
	.asciiz "False"
L_1:
	.asciiz "True"
L_3:
	.asciiz "None"
L_0:
	.asciiz "\n"
	.text
	.globl main
main:
	sw $ra,-100($sp)
	sw $fp,-104($sp)
	move $fp, $sp
	addi $sp,$sp,-120
	j L_8
L_8:
	j L_6
L_5:
	la $t0,L_1
	sw $t0,-4($fp)
	j L_7
L_6:
	la $t0,L_2
	sw $t0,-4($fp)
L_7:
	li $v0,4
	lw $a0,-4($fp)
	syscall
	la $t0,L_0
	sw $t0,-8($fp)
	li $v0,4
	lw $a0,-8($fp)
	syscall
	j L_12
L_12:
	j L_9
L_9:
	la $t0,L_1
	sw $t0,-12($fp)
	j L_11
L_10:
	la $t0,L_2
	sw $t0,-12($fp)
L_11:
	li $v0,4
	lw $a0,-12($fp)
	syscall
	la $t0,L_0
	sw $t0,-16($fp)
	li $v0,4
	lw $a0,-16($fp)
	syscall
	li $t0,1
	sw $t0,-24($fp)
	li $t0,1
	sw $t0,-28($fp)
	lw $t1,-24($fp)
	lw $t2,-28($fp)
	beq $t1,$t2,L_14
	j L_13
L_13:
	la $t0,L_1
	sw $t0,-20($fp)
	j L_15
L_14:
	la $t0,L_2
	sw $t0,-20($fp)
L_15:
	li $v0,4
	lw $a0,-20($fp)
	syscall
	la $t0,L_0
	sw $t0,-32($fp)
	li $v0,4
	lw $a0,-32($fp)
	syscall
	li $t0,1
	sw $t0,-40($fp)
	li $t0,0
	sw $t0,-44($fp)
	lw $t1,-40($fp)
	lw $t2,-44($fp)
	beq $t1,$t2,L_17
	j L_16
L_16:
	la $t0,L_1
	sw $t0,-36($fp)
	j L_18
L_17:
	la $t0,L_2
	sw $t0,-36($fp)
L_18:
	li $v0,4
	lw $a0,-36($fp)
	syscall
	la $t0,L_0
	sw $t0,-48($fp)
	li $v0,4
	lw $a0,-48($fp)
	syscall
	j L_22
L_22:
	j L_20
L_19:
	la $t0,L_1
	sw $t0,-52($fp)
	j L_21
L_20:
	la $t0,L_2
	sw $t0,-52($fp)
L_21:
	li $v0,4
	lw $a0,-52($fp)
	syscall
	la $t0,L_0
	sw $t0,-56($fp)
	li $v0,4
	lw $a0,-56($fp)
	syscall
	j L_26
L_26:
	j L_23
L_23:
	la $t0,L_1
	sw $t0,-60($fp)
	j L_25
L_24:
	la $t0,L_2
	sw $t0,-60($fp)
L_25:
	li $v0,4
	lw $a0,-60($fp)
	syscall
	la $t0,L_0
	sw $t0,-64($fp)
	li $v0,4
	lw $a0,-64($fp)
	syscall
	j L_27
L_30:
	j L_27
L_27:
	la $t0,L_1
	sw $t0,-68($fp)
	j L_29
L_28:
	la $t0,L_2
	sw $t0,-68($fp)
L_29:
	li $v0,4
	lw $a0,-68($fp)
	syscall
	la $t0,L_0
	sw $t0,-72($fp)
	li $v0,4
	lw $a0,-72($fp)
	syscall
	li $t0,5
	sw $t0,-80($fp)
	li $t0,5
	sw $t0,-84($fp)
	lw $t1,-80($fp)
	lw $t2,-84($fp)
	beq $t1,$t2,L_31
	j L_34
L_34:
	li $t0,5
	sw $t0,-88($fp)
	li $t0,4
	sw $t0,-92($fp)
	lw $t1,-88($fp)
	lw $t2,-92($fp)
	beq $t1,$t2,L_31
	j L_32
L_31:
	la $t0,L_1
	sw $t0,-76($fp)
	j L_33
L_32:
	la $t0,L_2
	sw $t0,-76($fp)
L_33:
	li $v0,4
	lw $a0,-76($fp)
	syscall
	la $t0,L_0
	sw $t0,-96($fp)
	li $v0,4
	lw $a0,-96($fp)
	syscall
main_done:
	lw $ra,-100($fp)
	lw $fp,-104($fp)
	addi $sp,$sp,120
	jr $ra
