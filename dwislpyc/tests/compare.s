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
	sw $ra,-164($sp)
	sw $fp,-168($sp)
	move $fp, $sp
	addi $sp,$sp,-184
	li $t0,1
	sw $t0,-8($fp)
	li $t0,2
	sw $t0,-12($fp)
	lw $t1,-8($fp)
	lw $t2,-12($fp)
	blt $t1,$t2,L_5
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
	sw $t0,-16($fp)
	li $v0,4
	lw $a0,-16($fp)
	syscall
	li $t0,2
	sw $t0,-24($fp)
	li $t0,1
	sw $t0,-28($fp)
	lw $t1,-24($fp)
	lw $t2,-28($fp)
	blt $t1,$t2,L_8
	j L_9
L_8:
	la $t0,L_1
	sw $t0,-20($fp)
	j L_10
L_9:
	la $t0,L_2
	sw $t0,-20($fp)
L_10:
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
	li $t0,1
	sw $t0,-44($fp)
	lw $t1,-40($fp)
	lw $t2,-44($fp)
	ble $t1,$t2,L_11
	j L_12
L_11:
	la $t0,L_1
	sw $t0,-36($fp)
	j L_13
L_12:
	la $t0,L_2
	sw $t0,-36($fp)
L_13:
	li $v0,4
	lw $a0,-36($fp)
	syscall
	la $t0,L_0
	sw $t0,-48($fp)
	li $v0,4
	lw $a0,-48($fp)
	syscall
	li $t0,1
	sw $t0,-56($fp)
	li $t0,2
	sw $t0,-60($fp)
	lw $t1,-56($fp)
	lw $t2,-60($fp)
	ble $t1,$t2,L_14
	j L_15
L_14:
	la $t0,L_1
	sw $t0,-52($fp)
	j L_16
L_15:
	la $t0,L_2
	sw $t0,-52($fp)
L_16:
	li $v0,4
	lw $a0,-52($fp)
	syscall
	la $t0,L_0
	sw $t0,-64($fp)
	li $v0,4
	lw $a0,-64($fp)
	syscall
	li $t0,1
	sw $t0,-72($fp)
	li $t0,0
	sw $t0,-76($fp)
	lw $t1,-72($fp)
	lw $t2,-76($fp)
	ble $t1,$t2,L_17
	j L_18
L_17:
	la $t0,L_1
	sw $t0,-68($fp)
	j L_19
L_18:
	la $t0,L_2
	sw $t0,-68($fp)
L_19:
	li $v0,4
	lw $a0,-68($fp)
	syscall
	la $t0,L_0
	sw $t0,-80($fp)
	li $v0,4
	lw $a0,-80($fp)
	syscall
	li $t0,1
	sw $t0,-88($fp)
	li $t0,1
	sw $t0,-92($fp)
	lw $t1,-88($fp)
	lw $t2,-92($fp)
	beq $t1,$t2,L_20
	j L_21
L_20:
	la $t0,L_1
	sw $t0,-84($fp)
	j L_22
L_21:
	la $t0,L_2
	sw $t0,-84($fp)
L_22:
	li $v0,4
	lw $a0,-84($fp)
	syscall
	la $t0,L_0
	sw $t0,-96($fp)
	li $v0,4
	lw $a0,-96($fp)
	syscall
	li $t0,1
	sw $t0,-104($fp)
	li $t0,0
	sw $t0,-108($fp)
	lw $t1,-104($fp)
	lw $t2,-108($fp)
	beq $t1,$t2,L_23
	j L_24
L_23:
	la $t0,L_1
	sw $t0,-100($fp)
	j L_25
L_24:
	la $t0,L_2
	sw $t0,-100($fp)
L_25:
	li $v0,4
	lw $a0,-100($fp)
	syscall
	la $t0,L_0
	sw $t0,-112($fp)
	li $v0,4
	lw $a0,-112($fp)
	syscall
	li $t0,4
	sw $t0,-128($fp)
	li $t0,5
	sw $t0,-132($fp)
	lw $t1,-128($fp)
	lw $t2,-132($fp)
	mult $t0,$t1,$t2
	sw $t0,-120($fp)
	li $t0,20
	sw $t0,-124($fp)
	lw $t1,-120($fp)
	lw $t2,-124($fp)
	beq $t1,$t2,L_26
	j L_27
L_26:
	la $t0,L_1
	sw $t0,-116($fp)
	j L_28
L_27:
	la $t0,L_2
	sw $t0,-116($fp)
L_28:
	li $v0,4
	lw $a0,-116($fp)
	syscall
	la $t0,L_0
	sw $t0,-136($fp)
	li $v0,4
	lw $a0,-136($fp)
	syscall
	li $t0,4
	sw $t0,-152($fp)
	li $t0,5
	sw $t0,-156($fp)
	lw $t1,-152($fp)
	lw $t2,-156($fp)
	mult $t0,$t1,$t2
	sw $t0,-144($fp)
	li $t0,21
	sw $t0,-148($fp)
	lw $t1,-144($fp)
	lw $t2,-148($fp)
	beq $t1,$t2,L_29
	j L_30
L_29:
	la $t0,L_1
	sw $t0,-140($fp)
	j L_31
L_30:
	la $t0,L_2
	sw $t0,-140($fp)
L_31:
	li $v0,4
	lw $a0,-140($fp)
	syscall
	la $t0,L_0
	sw $t0,-160($fp)
	li $v0,4
	lw $a0,-160($fp)
	syscall
main_done:
	lw $ra,-164($fp)
	lw $fp,-168($fp)
	addi $sp,$sp,184
	jr $ra
