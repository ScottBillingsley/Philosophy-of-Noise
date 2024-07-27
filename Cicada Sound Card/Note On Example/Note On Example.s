;			CICADA SOUND CARD NOTE ON EXAMPLE
;			VERNON BILLINGSLEY C2024
; 
;	A SAMPLE PROGRAM FOR THE CICADA SOUND CARD DESIGNED OF THE RC2014 Z80 SINGLE
; 	BOARD COMPUTER. THE SOUND CARD SHOULD BE IN THE DEFAULT STARTUP MODE, IE MIDI MODE,
;	AR ENVELOPE MODE, DEFAULT GAIN..
; 
            .ORG    0X8000 
; 
PORT        EQU     0X30 	;Change to match your card
NOTE_ON     EQU     0X01 
NOTE_OFF    EQU     0X00 
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
            LD      BC,0X200 	;TURN NOTE ON FOR APROX. 1/2 SECOND
            CALL    DELAY 
; 
            LD      A,NOTE_OFF 
            OUT     (PORT),A 
            RET      
; 
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

