	.file	"AX12.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__RAMPZ__ = 0x3b
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.Ltext0:
	.cfi_sections	.debug_frame
.global	AX12_UpdateAll
	.type	AX12_UpdateAll, @function
AX12_UpdateAll:
.LFB7:
	.file 1 "src/AX12.c"
	.loc 1 18 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 19 0
	sts i+1,__zero_reg__
	sts i,__zero_reg__
	ret
	.cfi_endproc
.LFE7:
	.size	AX12_UpdateAll, .-AX12_UpdateAll
.global	AX12_InitAll
	.type	AX12_InitAll, @function
AX12_InitAll:
.LFB8:
	.loc 1 46 0
	.cfi_startproc
.LVL0:
	push r28
.LCFI0:
	.cfi_def_cfa_offset 3
	.cfi_offset 28, -2
	push r29
.LCFI1:
	.cfi_def_cfa_offset 4
	.cfi_offset 29, -3
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	movw r28,r24
	.loc 1 47 0
	call dynamixel_ax_init
.LVL1:
	.loc 1 48 0
	sts CAN_ID+1,r29
	sts CAN_ID,r28
/* epilogue start */
	.loc 1 49 0
	pop r29
	pop r28
.LVL2:
	ret
	.cfi_endproc
.LFE8:
	.size	AX12_InitAll, .-AX12_InitAll
.global	AX12_OnMessage
	.type	AX12_OnMessage, @function
AX12_OnMessage:
.LFB9:
	.loc 1 51 0
	.cfi_startproc
.LVL3:
	push r16
.LCFI2:
	.cfi_def_cfa_offset 3
	.cfi_offset 16, -2
	push r17
.LCFI3:
	.cfi_def_cfa_offset 4
	.cfi_offset 17, -3
	push r28
.LCFI4:
	.cfi_def_cfa_offset 5
	.cfi_offset 28, -4
	push r29
.LCFI5:
	.cfi_def_cfa_offset 6
	.cfi_offset 29, -5
	in r28,__SP_L__
	in r29,__SP_H__
.LCFI6:
	.cfi_def_cfa_register 28
	sbiw r28,36
.LCFI7:
	.cfi_def_cfa_offset 42
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 36 */
/* stack size = 40 */
.L__stack_usage = 40
	.loc 1 52 0
	lds r20,CAN_ID
	lds r21,CAN_ID+1
	ldi r22,0
	ldi r23,0
	movw r30,r24
	ld r16,Z
	ldd r17,Z+1
	ldd r18,Z+2
	ldd r19,Z+3
	cp r16,r20
	cpc r17,r21
	cpc r18,r22
	cpc r19,r23
	brne .L3
.LBB2:
	.loc 1 58 0
	ldd r20,Z+5
	ldi r21,0
	movw r22,r24
	subi r22,-6
	sbci r23,-1
	movw r24,r28
.LVL4:
	adiw r24,28
	call memcpy
.LVL5:
	.loc 1 59 0
	movw r22,r28
	subi r22,-15
	sbci r23,-1
	movw r24,r28
	adiw r24,26
	call dynamixel_ax_txrx
.LVL6:
	.loc 1 63 0
	lds r20,CAN_ID
	lds r21,CAN_ID+1
	ldi r22,0
	ldi r23,0
	std Y+1,r20
	std Y+2,r21
	std Y+3,r22
	std Y+4,r23
	.loc 1 64 0
	ldd r25,Y+5
	.loc 1 65 0
	andi r25,lo8(-2)
	ori r25,lo8(1<<1)
	std Y+5,r25
	.loc 1 67 0
	cpi r24,lo8(1)
	brne .L5
	.loc 1 68 0
	ldd r17,Y+18
	subi r17,lo8(-(1))
.LVL7:
	.loc 1 69 0
	mov r20,r17
	ldi r21,0
	movw r22,r28
	subi r22,-17
	sbci r23,-1
	movw r24,r28
.LVL8:
	adiw r24,7
	call memcpy
.LVL9:
.L6:
	.loc 1 74 0
	std Y+6,r17
	.loc 1 75 0
	movw r24,r28
	adiw r24,1
	call can_send_message
.LVL10:
.L3:
/* epilogue start */
.LBE2:
	.loc 1 77 0
	adiw r28,36
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	pop r17
	pop r16
	ret
.LVL11:
.L5:
.LBB3:
	.loc 1 72 0
	std Y+7,r24
	.loc 1 71 0
	ldi r17,lo8(1)
	rjmp .L6
.LBE3:
	.cfi_endproc
.LFE9:
	.size	AX12_OnMessage, .-AX12_OnMessage
	.local	i
	.comm	i,2,1
	.local	CAN_ID
	.comm	CAN_ID,2,1
.Letext0:
	.file 2 "/usr/avr/include/stdint.h"
	.file 3 "/usr/lib/gcc/avr/6.3.0/include/stddef.h"
	.file 4 "./libs/can/can.h"
	.file 5 "src/AX12.h"
	.file 6 "/usr/avr/include/stdlib.h"
	.file 7 "./libs/dynamixel/dynamixel.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x3d2
	.word	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF46
	.byte	0xc
	.long	.LASF47
	.long	.LASF48
	.long	.Ltext0
	.long	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.uleb128 0x3
	.long	.LASF2
	.byte	0x2
	.byte	0x7e
	.long	0x37
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x4
	.byte	0x2
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.long	.LASF3
	.byte	0x2
	.byte	0x80
	.long	0x50
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF4
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.long	.LASF6
	.byte	0x2
	.byte	0x82
	.long	0x69
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF7
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF8
	.uleb128 0x3
	.long	.LASF9
	.byte	0x2
	.byte	0x85
	.long	0x82
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x3
	.long	.LASF11
	.byte	0x3
	.byte	0xd8
	.long	0x50
	.uleb128 0x5
	.byte	0x1
	.byte	0x4
	.byte	0xad
	.long	0xbf
	.uleb128 0x6
	.string	"rtr"
	.byte	0x4
	.byte	0xae
	.long	0xbf
	.byte	0x2
	.byte	0x1
	.byte	0xf
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x7
	.long	.LASF12
	.byte	0x4
	.byte	0xaf
	.long	0xbf
	.byte	0x2
	.byte	0x1
	.byte	0xe
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF13
	.uleb128 0x5
	.byte	0xe
	.byte	0x4
	.byte	0xa9
	.long	0x106
	.uleb128 0x8
	.string	"id"
	.byte	0x4
	.byte	0xac
	.long	0x5e
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF14
	.byte	0x4
	.byte	0xb0
	.long	0x94
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x9
	.long	.LASF15
	.byte	0x4
	.byte	0xb8
	.long	0x2c
	.byte	0x2
	.byte	0x23
	.uleb128 0x5
	.uleb128 0x9
	.long	.LASF16
	.byte	0x4
	.byte	0xb9
	.long	0x106
	.byte	0x2
	.byte	0x23
	.uleb128 0x6
	.byte	0
	.uleb128 0xa
	.long	0x2c
	.long	0x116
	.uleb128 0xb
	.long	0x116
	.byte	0x7
	.byte	0
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF17
	.uleb128 0x3
	.long	.LASF18
	.byte	0x4
	.byte	0xbe
	.long	0xc6
	.uleb128 0xc
	.long	.LASF49
	.byte	0x17
	.byte	0x5
	.byte	0xd
	.long	0x196
	.uleb128 0x9
	.long	.LASF19
	.byte	0x5
	.byte	0xe
	.long	0x77
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.long	.LASF20
	.byte	0x5
	.byte	0xf
	.long	0x77
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x8
	.string	"id"
	.byte	0x5
	.byte	0x10
	.long	0x2c
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x9
	.long	.LASF21
	.byte	0x5
	.byte	0x11
	.long	0x2c
	.byte	0x2
	.byte	0x23
	.uleb128 0x11
	.uleb128 0x9
	.long	.LASF22
	.byte	0x5
	.byte	0x12
	.long	0x2c
	.byte	0x2
	.byte	0x23
	.uleb128 0x12
	.uleb128 0x9
	.long	.LASF23
	.byte	0x5
	.byte	0x13
	.long	0x45
	.byte	0x2
	.byte	0x23
	.uleb128 0x13
	.uleb128 0x9
	.long	.LASF24
	.byte	0x5
	.byte	0x14
	.long	0x45
	.byte	0x2
	.byte	0x23
	.uleb128 0x15
	.byte	0
	.uleb128 0x3
	.long	.LASF25
	.byte	0x5
	.byte	0x15
	.long	0x128
	.uleb128 0xd
	.long	.LASF26
	.byte	0x6
	.word	0x138
	.long	0x89
	.byte	0x1
	.byte	0x1
	.uleb128 0xd
	.long	.LASF27
	.byte	0x6
	.word	0x13d
	.long	0x1bd
	.byte	0x1
	.byte	0x1
	.uleb128 0xe
	.byte	0x2
	.long	0x1c3
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF28
	.uleb128 0xd
	.long	.LASF29
	.byte	0x6
	.word	0x142
	.long	0x1bd
	.byte	0x1
	.byte	0x1
	.uleb128 0xf
	.long	.LASF30
	.byte	0x1
	.byte	0x9
	.long	0x45
	.byte	0x5
	.byte	0x3
	.long	CAN_ID
	.uleb128 0x10
	.long	.LASF20
	.byte	0x1
	.byte	0xa
	.long	0x77
	.byte	0
	.uleb128 0xa
	.long	0x196
	.long	0x205
	.uleb128 0xb
	.long	0x116
	.byte	0x9
	.byte	0
	.uleb128 0x11
	.long	.LASF31
	.byte	0x1
	.byte	0xd
	.long	0x1f5
	.uleb128 0x11
	.long	.LASF32
	.byte	0x1
	.byte	0xe
	.long	0x2c
	.uleb128 0x12
	.string	"i"
	.byte	0x1
	.byte	0x10
	.long	0x89
	.byte	0x5
	.byte	0x3
	.long	i
	.uleb128 0x13
	.byte	0x1
	.long	.LASF37
	.byte	0x1
	.byte	0x33
	.byte	0x1
	.long	.LFB9
	.long	.LFE9
	.long	.LLST2
	.byte	0x1
	.long	0x326
	.uleb128 0x14
	.long	.LASF39
	.byte	0x1
	.byte	0x33
	.long	0x326
	.long	.LLST3
	.uleb128 0x15
	.long	.Ldebug_ranges0+0
	.uleb128 0xf
	.long	.LASF33
	.byte	0x1
	.byte	0x35
	.long	0x32c
	.byte	0x2
	.byte	0x8c
	.sleb128 26
	.uleb128 0xf
	.long	.LASF34
	.byte	0x1
	.byte	0x36
	.long	0x32c
	.byte	0x2
	.byte	0x8c
	.sleb128 15
	.uleb128 0x16
	.long	.LASF35
	.byte	0x1
	.byte	0x37
	.long	0x2c
	.long	.LLST4
	.uleb128 0x12
	.string	"msg"
	.byte	0x1
	.byte	0x3e
	.long	0x11d
	.byte	0x2
	.byte	0x8c
	.sleb128 1
	.uleb128 0x16
	.long	.LASF36
	.byte	0x1
	.byte	0x42
	.long	0x2c
	.long	.LLST5
	.uleb128 0x17
	.long	.LVL5
	.long	0x3a2
	.long	0x2c7
	.uleb128 0x18
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -14
	.uleb128 0x18
	.byte	0x6
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0xf3
	.uleb128 0x1
	.byte	0x68
	.byte	0x23
	.uleb128 0x6
	.byte	0
	.uleb128 0x17
	.long	.LVL6
	.long	0x3ad
	.long	0x2eb
	.uleb128 0x18
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x18
	.byte	0x6
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -27
	.byte	0
	.uleb128 0x17
	.long	.LVL9
	.long	0x3a2
	.long	0x30f
	.uleb128 0x18
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -35
	.uleb128 0x18
	.byte	0x6
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -25
	.byte	0
	.uleb128 0x19
	.long	.LVL10
	.long	0x3ba
	.uleb128 0x18
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -41
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xe
	.byte	0x2
	.long	0x11d
	.uleb128 0xa
	.long	0x2c
	.long	0x33c
	.uleb128 0xb
	.long	0x116
	.byte	0xa
	.byte	0
	.uleb128 0x13
	.byte	0x1
	.long	.LASF38
	.byte	0x1
	.byte	0x2e
	.byte	0x1
	.long	.LFB8
	.long	.LFE8
	.long	.LLST0
	.byte	0x1
	.long	0x36f
	.uleb128 0x14
	.long	.LASF40
	.byte	0x1
	.byte	0x2e
	.long	0x45
	.long	.LLST1
	.uleb128 0x1a
	.long	.LVL1
	.long	0x3c8
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF41
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.long	.LFB7
	.long	.LFE7
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x3a2
	.uleb128 0x1c
	.uleb128 0x11
	.long	.LASF35
	.byte	0x1
	.byte	0x1a
	.long	0x2c
	.uleb128 0x11
	.long	.LASF42
	.byte	0x1
	.byte	0x1b
	.long	0x45
	.byte	0
	.byte	0
	.uleb128 0x1d
	.byte	0x1
	.byte	0x1
	.long	.LASF50
	.long	.LASF50
	.uleb128 0x1e
	.byte	0x1
	.byte	0x1
	.long	.LASF43
	.long	.LASF43
	.byte	0x7
	.byte	0x32
	.uleb128 0x1f
	.byte	0x1
	.byte	0x1
	.long	.LASF44
	.long	.LASF44
	.byte	0x4
	.word	0x1c5
	.uleb128 0x1e
	.byte	0x1
	.byte	0x1
	.long	.LASF45
	.long	.LASF45
	.byte	0x7
	.byte	0x2f
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0xc
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0xc
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0xa
	.uleb128 0x2111
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST2:
	.long	.LFB9-.Ltext0
	.long	.LCFI2-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI2-.Ltext0
	.long	.LCFI3-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI3-.Ltext0
	.long	.LCFI4-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI4-.Ltext0
	.long	.LCFI5-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI5-.Ltext0
	.long	.LCFI6-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	.LCFI6-.Ltext0
	.long	.LCFI7-.Ltext0
	.word	0x2
	.byte	0x8c
	.sleb128 6
	.long	.LCFI7-.Ltext0
	.long	.LFE9-.Ltext0
	.word	0x2
	.byte	0x8c
	.sleb128 42
	.long	0
	.long	0
.LLST3:
	.long	.LVL3-.Ltext0
	.long	.LVL4-.Ltext0
	.word	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL4-.Ltext0
	.long	.LVL5-1-.Ltext0
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL5-1-.Ltext0
	.long	.LFE9-.Ltext0
	.word	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x68
	.byte	0x9f
	.long	0
	.long	0
.LLST4:
	.long	.LVL6-.Ltext0
	.long	.LVL8-.Ltext0
	.word	0x1
	.byte	0x68
	.long	.LVL11-.Ltext0
	.long	.LFE9-.Ltext0
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LLST5:
	.long	.LVL7-.Ltext0
	.long	.LVL10-.Ltext0
	.word	0x1
	.byte	0x61
	.long	.LVL11-.Ltext0
	.long	.LFE9-.Ltext0
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	0
	.long	0
.LLST0:
	.long	.LFB8-.Ltext0
	.long	.LCFI0-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI0-.Ltext0
	.long	.LCFI1-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI1-.Ltext0
	.long	.LFE8-.Ltext0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	0
	.long	0
.LLST1:
	.long	.LVL0-.Ltext0
	.long	.LVL1-1-.Ltext0
	.word	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL1-1-.Ltext0
	.long	.LVL2-.Ltext0
	.word	0x6
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.long	.LVL2-.Ltext0
	.long	.LFE8-.Ltext0
	.word	0x5
	.byte	0x3
	.long	CAN_ID
	.long	0
	.long	0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.word	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.word	0
	.word	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0
	.long	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.LBB2-.Ltext0
	.long	.LBE2-.Ltext0
	.long	.LBB3-.Ltext0
	.long	.LBE3-.Ltext0
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF22:
	.string	"tolerance"
.LASF40:
	.string	"canId"
.LASF37:
	.string	"AX12_OnMessage"
.LASF9:
	.string	"uint64_t"
.LASF11:
	.string	"size_t"
.LASF36:
	.string	"packetKernelLength"
.LASF17:
	.string	"sizetype"
.LASF38:
	.string	"AX12_InitAll"
.LASF30:
	.string	"CAN_ID"
.LASF24:
	.string	"timeoutMs"
.LASF50:
	.string	"memcpy"
.LASF43:
	.string	"dynamixel_ax_txrx"
.LASF13:
	.string	"__unknown__"
.LASF2:
	.string	"uint8_t"
.LASF8:
	.string	"long long int"
.LASF41:
	.string	"AX12_UpdateAll"
.LASF5:
	.string	"long int"
.LASF31:
	.string	"positionUpdateQueue"
.LASF44:
	.string	"can_send_message"
.LASF15:
	.string	"length"
.LASF1:
	.string	"unsigned char"
.LASF0:
	.string	"signed char"
.LASF14:
	.string	"flags"
.LASF10:
	.string	"long long unsigned int"
.LASF6:
	.string	"uint32_t"
.LASF47:
	.string	"src/AX12.c"
.LASF4:
	.string	"unsigned int"
.LASF3:
	.string	"uint16_t"
.LASF27:
	.string	"__malloc_heap_start"
.LASF33:
	.string	"txpacket"
.LASF45:
	.string	"dynamixel_ax_init"
.LASF25:
	.string	"AX12"
.LASF35:
	.string	"status"
.LASF32:
	.string	"positionUpdateQueueCount"
.LASF28:
	.string	"char"
.LASF48:
	.string	"/home/lukicdarkoo/Memristor/avr-actuator-control-board"
.LASF46:
	.string	"GNU C99 6.3.0 -mn-flash=2 -mno-skip-bug -mmcu=avr51 -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums"
.LASF16:
	.string	"data"
.LASF21:
	.string	"updatePeriodMs"
.LASF7:
	.string	"long unsigned int"
.LASF19:
	.string	"epochStart"
.LASF20:
	.string	"lastUpdate"
.LASF49:
	.string	"_AX12"
.LASF29:
	.string	"__malloc_heap_end"
.LASF23:
	.string	"requiredPosition"
.LASF39:
	.string	"canMsg"
.LASF12:
	.string	"extended"
.LASF34:
	.string	"rxpacket"
.LASF26:
	.string	"__malloc_margin"
.LASF42:
	.string	"value"
.LASF18:
	.string	"can_t"
	.ident	"GCC: (GNU) 6.3.0"
.global __do_clear_bss
