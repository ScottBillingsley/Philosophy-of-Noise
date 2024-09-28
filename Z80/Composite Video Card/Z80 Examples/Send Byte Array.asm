	.ORG 0X8000
;
;Send out a text array to the screen 
;
M_ADDRESS 	EQU	0X3C00
WORD_COUNT	EQU 0X03E8


BEGIN:		;HOUSEKEEPING
			LD HL, M_ADDRESS		;BASE ADDRESS OF VIDEO RAM
			LD DE, TEXT				;ADDRESS OF TEXT ARRAY
			LD BC, WORD_COUNT
NEXT:			LD A, (DE)
			LD (HL), A
			INC HL
			INC DE
			DEC BC
			LD A, B
			OR C
			JP NZ, NEXT
			RET
			
			
			

;			
;WORD_COUNT:	DW	0X03E8
;			
TEXT:			DB	'                                        '
			DB	' This is a test array sent from the Z80 '
			DB	'to the display to show the speed of     '
			DB  'direct memory writes.                   '
			DB  '                                        '
			DB	'                                        '
			DB	' The IDT7130 is running  parallel to    '
			DB  'the main memory at address 0x3C00.      '
			DB	'This is the top 1k of an unused part    '
			DB	'of rom.                                 '
			DB	'                                        '
			DB	'                                        '
			DB	' The AVR128DB48 reads the IDT7130 during'
			DB	'blanking period.                        '
			DB	'                                        '
			DB	'                                        '
			DB	'ABCDEFGHIJKLMNOPQRSTUVWXYZ              '
			DB	'abcdefghijklmnopqrstuvwxyz              '
			DB	'1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  '
			DB	'                                        '
			DB	' The quick brown fox jumped over the    '
			DB	'lazy dog.                               '
			DB	'                                        '
			DB	'0123456789012345678901234567890123456789'
			DB	'9876543210987654321098765432109876543210'			
