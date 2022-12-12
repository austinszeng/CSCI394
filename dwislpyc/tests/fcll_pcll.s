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
	sw $ra,-64($sp)
	sw $fp,-68($sp)
	move $fp, $sp
	addi $sp,$sp,-88
	li $t0,2
	sw $t0,-16($fp)
	lw $a0,-16($fp)
	jal f
	move $t0,$v0
	sw $t0,-12($fp)
	lw $a0,-12($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-20($fp)
	li $v0,4
	lw $a0,-20($fp)
	syscall
	li $t0,3
	sw $t0,-4($fp)
	lw $t1,-4($fp)
	move $t0,$t1
	sw $t0,-36($fp)
	lw $a0,-36($fp)
	jal sqr
	move $t0,$v0
	sw $t0,-28($fp)
	lw $t1,-4($fp)
	move $t0,$t1
	sw $t0,-44($fp)
	li $t0,1
	sw $t0,-48($fp)
	lw $t1,-44($fp)
	lw $t2,-48($fp)
	add $t0,$t1,$t2
	sw $t0,-40($fp)
	lw $a0,-40($fp)
	jal sqr
	move $t0,$v0
	sw $t0,-32($fp)
	lw $t1,-28($fp)
	lw $t2,-32($fp)
	add $t0,$t1,$t2
	sw $t0,-24($fp)
	lw $a0,-24($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-52($fp)
	li $v0,4
	lw $a0,-52($fp)
	syscall
	li $t0,12
	sw $t0,-8($fp)
	lw $t1,-8($fp)
	move $t0,$t1
	sw $t0,-56($fp)
	lw $a0,-56($fp)
	li $t0,3
	sw $t0,-60($fp)
	lw $a1,-60($fp)
	jal g
main_done:
	lw $ra,-64($fp)
	lw $fp,-68($fp)
	addi $sp,$sp,88
	jr $ra
g:
	sw $ra,-20($sp)
	sw $fp,-24($sp)
	move $fp, $sp
	addi $sp,$sp,-40
	sw $a0,0($fp)
	sw $a1,4($fp)
	li $t0,5
	sw $t0,-4($fp)
	li $t0,2
	sw $t0,-8($fp)
	li $t0,1
	sw $t0,-12($fp)
	li $t0,0
	sw $t0,-16($fp)
	lw $v0,-16($fp)
	j g_done
g_done:
	lw $ra,-20($fp)
	lw $fp,-24($fp)
	addi $sp,$sp,40
	jr $ra
sqr:
	sw $ra,-16($sp)
	sw $fp,-20($sp)
	move $fp, $sp
	addi $sp,$sp,-40
	sw $a0,0($fp)
	lw $t1,0($fp)
	move $t0,$t1
	sw $t0,-8($fp)
	lw $t1,0($fp)
	move $t0,$t1
	sw $t0,-12($fp)
	lw $t1,-8($fp)
	lw $t2,-12($fp)
	mult $t0,$t1,$t2
	sw $t0,-4($fp)
	lw $v0,-4($fp)
	j sqr_done
sqr_done:
	lw $ra,-16($fp)
	lw $fp,-20($fp)
	addi $sp,$sp,40
	jr $ra
f:
	sw $ra,-32($sp)
	sw $fp,-36($sp)
	move $fp, $sp
	addi $sp,$sp,-56
	sw $a0,0($fp)
	lw $t1,0($fp)
	move $t0,$t1
	sw $t0,-4($fp)
	li $t0,1
	sw $t0,-8($fp)
	lw $t1,-4($fp)
	lw $t2,-8($fp)
	add $t0,$t1,$t2
	sw $t0,0($fp)
	lw $t1,0($fp)
	move $t0,$t1
	sw $t0,-12($fp)
	lw $a0,-12($fp)
	li $v0,1
	syscall
	la $t0,L_0
	sw $t0,-16($fp)
	li $v0,4
	lw $a0,-16($fp)
	syscall
	nop
	lw $t1,0($fp)
	move $t0,$t1
	sw $t0,-24($fp)
	li $t0,42
	sw $t0,-28($fp)
	lw $t1,-24($fp)
	lw $t2,-28($fp)
	mult $t0,$t1,$t2
	sw $t0,-20($fp)
	lw $v0,-20($fp)
	j f_done
f_done:
	lw $ra,-32($fp)
	lw $fp,-36($fp)
	addi $sp,$sp,56
	jr $ra
