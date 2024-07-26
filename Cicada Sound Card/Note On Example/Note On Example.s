;			Cicada Sound Card Note On Example
;			Vernon Billingsley c2024
; 
;	A sample program for the Cicada Sound Card designed of the RC2014 Z80 single
; 	board computer. The sound card should be in the default startup mode, ie MIDI mode,
;	AR envelope mode, default GAIN..
; 
            .ORG    0x8000 
; 
PORT        EQU     0x30 
NOTE_ON     EQU     0x01 
NOTE_OFF    EQU     0x00 
; 
START:      LD      a,0x9b ;Load the Middle C note to the A register
            OUT     (0x30),a ;Send to the bus
; 
            LD      bc,0x01 ;Load a short delay to allow the arduino to read the bus
            CALL    delay 
; 
            LD      a,NOTE_ON 
            OUT     (0x30),a 
; 
            LD      bc,0x200 ;Turn note on for aprox. 1/2 second
            CALL    delay 
; 
            LD      a,NOTE_OFF 
            OUT     (0x30),a 
            RET      
; 
; 
; 
;*** Delay, aprox 250 mS per 0x100
;    Counts loaded into register BC before call 
DELAY:               

OUTER:               
            LD      de,0x200 
INNER:               
            DEC     de 
            LD      a,d 
            OR      e 
            JP      nz,inner 
            DEC     bc 
            LD      a,b 
            OR      c 
            JP      nz,outer 
            RET      
; 
