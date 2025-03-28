	.ORG 0XB000
;
;	Demo reprogramming the charset
;
M_ADDRESS 	EQU	0X3C00			;Base address of the video memory
C_ADDRESS	EQU 0X3F2			;The byte addres to reprogram the charset
								;The next 8 bytes are the bytes to reprogram 
BEGIN:			LD	HL, M_ADDRESS	;Data address of new charset
			LD	DE, C_ADDRESS
			ADD HL, DE
			INC HL				;Start at base address plus 1
				
			LD	DE, TEXT		;Bytes to write
			LD	BC, 0X08		;Number of bytes to write
NEXT:			LD	A, (DE)		
			LD	(HL), A
			INC HL
			INC DE
			DEC BC
			LD A, B
			OR C
			JP NZ, NEXT
			
			LD	HL, M_ADDRESS	;Video ram start address
			LD	DE, C_ADDRESS	;Reprogram byte
			ADD HL, DE			;Pixel to write in memory
			LD	A, 0X0E			;Charactor to reprogram
			LD	(HL), A	

			RET	
;


TEXT:		DB	0X3C,0X7F,0XDB,0XFF,0XFF,0XDB,0X66,0X3C
