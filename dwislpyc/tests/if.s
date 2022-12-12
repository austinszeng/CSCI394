	.data
L_15:
	.asciiz "False"
L_14:
	.asciiz "0"
L_9:
	.asciiz "1"
L_8:
	.asciiz "True"
L_4:
	.asciiz "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
L_2:
	.asciiz "False"
L_1:
	.asciiz "True"
L_13:
	.asciiz "1"
L_3:
	.asciiz "None"
L_0:
	.asciiz "\n"
	.text
	.globl main
main:
	sw $ra,-88($sp)
	sw $fp,-92($sp)
	move $fp, $sp
	addi $sp,$sp,-112
	li $t0,1
	sw $t0,-4($fp)
	lw $t1,-4($fp)
	move $t0,$t1
	sw $t0,-8($fp)
	li $t0,1
	sw $t0,-12($fp)
	lw $t1,-8($fp)
	lw $t2,-12($fp)
	beq $t1,$t2,L_5
	j L_6
L_5:
	la $t0,L_8
	sw $t0,-16($fp)
	li $v0,4
	lw $a0,-16($fp)
	syscall
	la $t0,L_0
	sw $t0,-20($fp)
	li $v0,4
	lw $a0,-20($fp)
	syscall
	la $t0,L_9
	sw $t0,-24($fp)
	li $v0,4
	lw $a0,-24($fp)
	syscall
	la $t0,L_0
	sw $t0,-28($fp)
	li $v0,4
	lw $a0,-28($fp)
	syscall
	lw $t1,-4($fp)
	move $t0,$t1
	sw $t0,-32($fp)
	li $t0,1
	sw $t0,-36($fp)
	lw $t1,-32($fp)
	lw $t2,-36($fp)
	beq $t1,$t2,L_10
	j L_11
L_10:
	la $t0,L_13
	sw $t0,-40($fp)
	li $v0,4
	lw $a0,-40($fp)
	syscall
	la $t0,L_0
	sw $t0,-44($fp)
	li $v0,4
	lw $a0,-44($fp)
	syscall
	j L_12
L_11:
	la $t0,L_14
	sw $t0,-48($fp)
	li $v0,4
	lw $a0,-48($fp)
	syscall
	la $t0,L_0
	sw $t0,-52($fp)
	li $v0,4
	lw $a0,-52($fp)
	syscall
L_12:
	j L_7
L_6:
	la $t0,L_15
	sw $t0,-56($fp)
	li $v0,4
	lw $a0,-56($fp)
	syscall
	la $t0,L_0
	sw $t0,-60($fp)
	li $v0,4
	lw $a0,-60($fp)
	syscall
L_7:
	li $t0,5
	sw $t0,-64($fp)
	li $t0,6
	sw $t0,-68($fp)
	lw $t1,-64($fp)
	lw $t2,-68($fp)
	blt $t1,$t2,L_16
	j L_17
L_16:
	li $t0,5
	sw $t0,-72($fp)
	lw $a0,-72($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-76($fp)
	li $v0,4
	lw $a0,-76($fp)
	syscall
	j L_18
L_17:
	li $t0,4
	sw $t0,-80($fp)
	lw $a0,-80($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-84($fp)
	li $v0,4
	lw $a0,-84($fp)
	syscall
L_18:
main_done:
	lw $ra,-88($fp)
	lw $fp,-92($fp)
	addi $sp,$sp,112
	jr $ra
