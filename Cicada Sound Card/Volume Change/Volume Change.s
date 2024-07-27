;			CICADA SOUND CARD VOLUME
;			VERNON BILLINGSLEY C2024
; 
;	A SAMPLE PROGRAM FOR THE CICADA SOUND CARD DESIGNED OF THE RC2014 Z80 SINGLE
; 	BOARD COMPUTER. THE SOUND CARD SHOULD BE IN THE DEFAULT STARTUP MODE, IE MIDI MODE,
;	AR ENVELOPE MODE, DEFAULT GAIN..
; 
            .ORG    0X8000 
; 
PORT        EQU     0X30 	;CHANGE TO MATCH YOUR CARD
NOTE_ON     EQU     0X01 
NOTE_OFF    EQU     0X00 
VOLUME:     DB      0X40 
; 
START:      LD      A,0X9B 	;LOAD THE MIDDLE C NOTE TO THE A REGISTER
            OUT     (PORT),A 	;SEND TO THE BUS
; 
            LD      BC,0X01 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY 
; 
            LD      A,NOTE_ON 
            OUT     (PORT),A 
; 
            LD      BC,0X01 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY 
; 
V_UP:       LD      HL,VOLUME 	;LOAD THE ADDRESS OF VOLUME
            LD      A,(HL) 	;LOAD THE CONTENTS AT ADDRESS OF HL					
            OUT     (PORT),A 
; 
            LD      BC,0X10 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY 
;			
            LD      B,0X5F 
            INC     (HL) 
            LD      A,(HL) 
            CP      B 
            JP      NZ,V_UP 
; 
V_DOWN:     LD      HL,VOLUME 	;LOAD THE ADDRESS OF VOLUME
            LD      A,(HL) 	;LOAD THE CONTENTS AT ADDRESS OF HL					
            OUT     (PORT),A 
; 
            LD      BC,0X10 	;LOAD A SHORT DELAY TO ALLOW THE ARDUINO TO READ THE BUS
            CALL    DELAY 	
;			
            LD      B,0X40 
            DEC     (HL) 
            LD      A,(HL) 
            CP      B 
            JP      NZ,V_DOWN 
; 
            LD      A,NOTE_OFF 
            OUT     (PORT),A 
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
			
