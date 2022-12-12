	.data
L_14:
	.asciiz "I hit 1"
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
	sw $ra,-84($sp)
	sw $fp,-88($sp)
	move $fp, $sp
	addi $sp,$sp,-104
	li $t0,0
	sw $t0,-8($fp)
L_5:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-12($fp)
	li $t0,2
	sw $t0,-16($fp)
	lw $t1,-12($fp)
	lw $t2,-16($fp)
	blt $t1,$t2,L_6
	j L_7
L_6:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-20($fp)
	li $t0,1
	sw $t0,-24($fp)
	lw $t1,-20($fp)
	lw $t2,-24($fp)
	add $t0,$t1,$t2
	sw $t0,-8($fp)
	j L_5
L_7:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-28($fp)
	lw $a0,-28($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-32($fp)
	li $v0,4
	lw $a0,-32($fp)
	syscall
	li $t0,0
	sw $t0,-8($fp)
L_8:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-36($fp)
	li $t0,2
	sw $t0,-40($fp)
	lw $t1,-36($fp)
	lw $t2,-40($fp)
	blt $t1,$t2,L_9
	j L_10
L_9:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-44($fp)
	li $t0,1
	sw $t0,-48($fp)
	lw $t1,-44($fp)
	lw $t2,-48($fp)
	beq $t1,$t2,L_11
	j L_12
L_11:
	la $t0,L_14
	sw $t0,-52($fp)
	li $v0,4
	lw $a0,-52($fp)
	syscall
	la $t0,L_0
	sw $t0,-56($fp)
	li $v0,4
	lw $a0,-56($fp)
	syscall
	j L_13
L_12:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-60($fp)
	lw $a0,-60($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-64($fp)
	li $v0,4
	lw $a0,-64($fp)
	syscall
L_13:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-68($fp)
	li $t0,1
	sw $t0,-72($fp)
	lw $t1,-68($fp)
	lw $t2,-72($fp)
	add $t0,$t1,$t2
	sw $t0,-8($fp)
	j L_8
L_10:
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-76($fp)
	lw $a0,-76($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-80($fp)
	li $v0,4
	lw $a0,-80($fp)
	syscall
main_done:
	lw $ra,-84($fp)
	lw $fp,-88($fp)
	addi $sp,$sp,104
	jr $ra
