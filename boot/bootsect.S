    .arm
    
    .equ SVC_Stack_Size,0x00000100
    .equ IRQ_Stack_Size,0x00000800
    .equ FIQ_Stack_Size,0x1000
    .equ USR_MOD,0x10
    .equ SYS_MOD,0x1F
    .equ SVC_MOD,0x13
    .equ IRQ_MOD,0x12
    .equ FIQ_MOD,0x11
    .equ MOD_MASK,0x1f
    .equ ttb_address,0x10000
    .equ stack_start,0xFFFFFF

    .extern main
    .global _start, begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text
_start:/*初始化中断向量表 */
    b Reset_Handler		/* 复位中断 					*/	
	b Undefined_Handler	/* 未定义中断 					*/
	b SVC_Handler		/* SVC(Supervisor)中断 		*/
	b Prefetch_Handler	/* 预取终止中断 					*/
	b Data_Handler	/* 数据终止中断 					*/
	b NotUsed_Handler	/* 未使用中断					*/
	b IRQ_Handler		/* IRQ中断 					*/
FIQ_Handler:		/* FIQ(快速中断)未定义中断 			*/
    ldr r0, =FIQ_Handler
	bx r0

.align 4
stack_begin:
.rept (SVC_Stack_Size + IRQ_Stack_Size)
.byte 0
.endr
stack_top:

Reset_Handler: 
    cpsid i					// 关闭全局中断
	// 关闭I,DCache和MMU
	mrc     p15, 0, r0, c1, c0, 0     // 读取CP15的C1寄存器到R0中       		        	
    bic     r0,  r0, #(0x1 << 12)     // 清除C1寄存器的bit12位(I位)，关闭I Cache            	
    bic     r0,  r0, #(0x1 << 2)      // 清除C1寄存器的bit2(C位)，关闭D Cache    				
    bic     r0,  r0, #(0x1 << 1)      // 清除C1寄存器的bit1(A位)，关闭对齐						
    bic     r0,  r0, #(0x1 << 11)     // 清除C1寄存器的bit11(Z位)，关闭分支预测				
    bic     r0,  r0, #(0x1 << 0)      // 清除C1寄存器的bit0(M位)，关闭MMU				       	
    mcr     p15, 0, r0, c1, c0, 0     // 将r0寄存器中的值写入到CP15的C1寄存器中	 	

    @ initial cache
    mrc p15, 1, r0, c0, c0, 0 @ Read Cache Size ID 
    ldr r3, =#0x1FF
    and r0, r3, r0, LSR #13 @ r0 = no. of sets - 1
    mov r1, #0 @ r1 = way counter way_loop
way_loop:
    mov r3, #0 @ r3 = set counter set_loop
set_loop:
    mov r2, r1, LSL #30 
    orr r2, r3, LSL #5 @ r2 = set/way cache operation format
    mcr p15, 0, r2, c7, c6, 2 @ Invalidate line described by r2
    add r3, r3, #1 @ Increment set counter
    cmp r0, r3 @ Last set reached yet?
    bgt set_loop @ if not, iterate set_loop
    add r1, r1, #1 @ else, next
    cmp r1, #4 @ Last way reached yet?
    bne way_loop @ if not, iterate way_loop

    @  Branch Prediction Enable

    mov r1, #0
    mrc p15, 0, r1, c1, c0, 0 @ Read Control Register configuration data
    orr r1, r1, #(0x1 << 11) @ Global BP Enable bit
    mcr p15, 0, r1, c1, c0, 0 @ Write Control Register configuration data

    // 开启I,DCache和MMU

    mrc     p15, 0, r0, c1, c0, 0     		        	
    orr     r0,  r0, #(0x1 << 12)     
    orr     r0,  r0, #(0x1 << 2)      				
    orr     r0,  r0, #(0x1 << 1)      	
    mcr     p15, 0, r0, c1, c0, 0 
    ldr r0, =0b00000000000000000000110111100010  
    ldr r1, =ttb_address
    ldr r3, = 4095 
write_pte:
    orr r2, r0, r3, LSL #20 
    str r2, [r1, r3, LSL #2] 
    sub r3, r3, #1
    bne write_pte

    bic r0, r0, #0b1100 
    orr r0, r0, #0b0100 
    bic r0, r0, #0b111000000000000
    orr r0, r0, #0b101000000000000 
    orr r0, r0, #0b10000000000000000 
    str r0, [r1]

    mov r1,#0x0
    mcr p15, 0, r1, c2, c0, 2 
    ldr r1, =ttb_address
    mcr p15, 0, r1, c2, c0, 0

    ldr r1, =0x55555555
    mcr p15, 0, r1, c3, c0, 0
/*
    mrc p15, 0, r1, c1, c0, 0 
    orr r1, r1, #0x1 
    mcr p15, 0, r1, c1, c0, 0
    cpsie i  
*/

    ldr r6, =stack_start
    sub r6,r6, #0x01000
    mov sp, r6
    cpsid i
	ldr r0, =stack_top
	msr cpsr_c, IRQ_MOD
	mov sp, r0

	msr cpsr_c, SVC_MOD
	sub r0, r0,#IRQ_Stack_Size
	mov sp, r0

	msr cpsr_c, SYS_MOD
	and r6, r8
	add r6, r6, #0X1000	
	mov sp, r6

    b main

Undefined_Handler:
    ldr r0, =Undefined_Handler
	bx r0

SVC_Handler:
    ldr r0, =SVC_Handler
	bx r0
/*
    stmdb sp!, {r0-r12,lr}
	msr cpsr_c, SYS_MOD
	push {r0 - r3, r12}
	and r0, r7, #0xff
	bl  system_call
	mov r6,r0
	pop {r0 - r3, r12}
	blx r6
	msr cpsr_c, SVC_MOD
	ldmia sp!, {r0-r12,pc}^
    */

Prefetch_Handler:
    ldr r0, =Prefetch_Handler	
	bx r0

Data_Handler:
    ldr r0, =Data_Handler
	bx r0

NotUsed_Handler:
    ldr r0, =NotUsed_Handler
	bx r0

IRQ_Handler:
/*
    ldr r0, =IRQ_Handler
	bx r0
*/
    push {lr}			
	push {r0-r3, r12}
	mrs r0, spsr
	push {r0}
	mrc p15, 4, r3, c15, c0, 0 							
	add r3, r3, #0X2000			
	ldr r0, [r3, #0XC]
	push {r0, r3}
	ldr r2, =system_irqhandler
	blx r2	
	pop {r0, r3}				
	str r0, [r3, #0X10]
	pop {r0}						
	msr spsr_cxsf, r0
	pop {r0-r3, r12}
	pop {lr}
	subs pc, lr, #4
.text
endtext:
.data
enddata:
.bss
endbss:
