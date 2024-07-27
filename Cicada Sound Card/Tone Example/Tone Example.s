;			CICADA SOUND CARD TONE EXAMPLE
;			VERNON BILLINGSLEY C2024
; 
;	A SAMPLE PROGRAM FOR THE CICADA SOUND CARD DESIGNED OF THE RC2014 Z80 SINGLE
; 	BOARD COMPUTER. THE SOUND CARD SHOULD BE IN THE DEFAULT STARTUP MODE, IE MIDI MODE,
;	AR ENVELOPE MODE, DEFAULT GAIN..
; 
            .ORG    0X8000 
; 
PORT        EQU     0X30 
NOTE_ON     EQU     0X01 
NOTE_OFF    EQU     0X00 
;
;		CHOOSE A NOTE FREQ TO START WITH..
; 		GO WITH A4, 0XA4
START:		LD		HL, WOBBLE	;ENSURE THE COUNTERS ARE RESET ON STARTUP
			LD		(HL), 0X3F
			LD		HL, COUNT
			LD		(HL), 0X00
;
			LD      A,0XA4		;LOAD THE A4 NOTE, 440 HZ, TO THE A REGISTER
            OUT     (0X30),A 	;SEND TO THE BUS
            LD      BC,0X01 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY 
            LD      A,NOTE_ON 
            OUT     (0X30),A 
            LD      BC,0X01 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY
			LD		A, 0X10		;LOAD THE TONE COMMAND
			OUT		(0X30), A
            LD      BC,0X01 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY 
;
FREQ_UP:	LD		HL, WOBBLE
			LD		A, (TONE_UP);ADD 10 TO THE ACCUMULATOR
			OUT		(0X30), A
            LD      BC,0X02 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY
			INC		(HL)
			LD		A, (HL)
			CP		0X5E		;COUNT TO 94
			JP		NZ, FREQ_UP
;			
FREQ_DN:	LD		A, (TONE_DN);SUBTRACT 10 TO THE ACCUMULATOR
			OUT		(0X30), A
            LD      BC,0X02 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY
			DEC		(HL)
			LD		A, (HL)
			CP		0X1F		;COUNT DOWN TO 31
			JP		NZ, FREQ_DN
;
			LD		HL, COUNT
			INC		(HL)
			LD		A, (HL)
			CP		0X0A
			JP		NZ,FREQ_UP
;			
            LD      A,NOTE_OFF 
            OUT     (0X30),A 
            LD      BC,0X01 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY 
			LD 		A, 0X0F		;RESET CARD TO MIDI
			OUT		(0X30), A
            RET 
; 
; 
;*** DELAY, APROX 250 MS PER 0X100
;    COUNTS LOADED INTO REGISTER BC BEFORE CALL
DELAY:               

OUTER:               
            LD      DE,0X200 
INNER:               
            DEC     DE 
            LD      A,D 
            OR      E 
            JP      NZ,INNER 
            DEC     BC 
            LD      A,B 
            OR      C 
            JP      NZ,OUTER 
            RET      
;	
TONE_UP:	DB		0XCA	;ADD 10
TONE_DN:	DB		0XEA	;SUBTRACT 10	
WOBBLE:		DB		0X3F	
COUNT:		DB		0X00		
			