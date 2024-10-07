	.ORG 0X8000
;	
;	A porting of Willard Nico version of
;   People's Computer Company's mainframe game Shooting Stars
;	Byte Magazine May, 1976 p. 42
;	Restart calls are for Stephen Cousins Small Computer Monitor on
;	the RC2014 retro Z80 computer
;	Vernon Billingsley	c2024
;	https://github.com/ScottBillingsley/Philosophy-of-Noise
;
BEGIN:		CALL	LINEFEED		;Start off by doing a line feed
			LD		HL, MESS0		;Address of message to display
			CALL	TXT_DISPLAY		;Display the start screen
			CALL	IN_CHAR			;Get a charactor from the input device
			CP		'n'
			JP		Z, GAME_STR		;If no instructions are wanted, start game
			CP		'N'
			JP		Z, GAME_STR
			CP		0X1B			;Check for escape to exit the game
			JP		Z, GIVE_UP
			CALL	LINEFEED		;Otherwise, display the rules
			LD		HL, PAGE1
			CALL	TXT_DISPLAY
			CALL	IN_CHAR			;Wait for a key press
			CALL	LINEFEED
			LD		HL, PAGE2
			CALL	TXT_DISPLAY
			CALL	IN_CHAR
			CALL	LINEFEED
			LD		HL, PAGE3
			CALL	TXT_DISPLAY
			CALL	IN_CHAR
			CALL	LINEFEED
GAME_STR:	CALL	LINEFEED
			CALL	LINEFEED
			LD		HL, UNIVERSE	;Setup the start universe
			LD		(HL), 0X00
			INC		HL
			LD		(HL), 0X01
			CALL	RST_SHOT		;Clear the score on restart
RE_ENTER:	CALL	DISP_UNIV		;Display the universe
;	************	WIN OR LOSE		***********************			
			CALL	WIN_TEST		;Check for win or lose
			CP		0X00
			JP		Z, SKIP7		;If nither win or lose, skip to continue
			CP		0X0F			;Check for lose
			JP		Z, U_LOST
;			Returning here, we have a winner
			CALL	LINEFEED
			CALL	LINEFEED
			LD		HL, MESS4
			CALL	TXT_DISPLAY		;Display the first part of the win message
			CALL	DPLY_SCORE		;Display the number of shots
			LD		HL, MESS5
			CALL	TXT_DISPLAY		;Display the second part
			CALL	IN_CHAR			;Wait for a key input
			CP		'Y'
			JP		Z, BEGIN
			CP		'y'
			JP		Z, BEGIN
			JP		GIVE_UP
;	************	LOST			**********************
U_LOST:		CALL	LINEFEED
			CALL	LINEFEED
			LD		HL, MESS3		;Load the lost message
			CALL	TXT_DISPLAY
			CALL	IN_CHAR			;Wait for a key input
			CP		'Y'
			JP		Z, GAME_STR
			CP		'y'
			JP		Z, GAME_STR		;If yes, restart the game
			CALL	LINEFEED
			CALL	LINEFEED
			LD		HL, MESS9
			CALL	TXT_DISPLAY
			CALL	DPLY_SCORE
			LD		HL, MESS10
			CALL	TXT_DISPLAY
			JP		GIVE_UP			;Otherwise jump to exit
;	************	GAME PLAY		************************			
SKIP7:		CALL	LINEFEED
			CALL	LINEFEED
			LD		HL, MESS7		;Ask for a target
			CALL	TXT_DISPLAY
			CALL	IN_CHAR			;Wait for charactor input in A
			CALL	TEST_SHOT		;Test for a valid key
			CP		0X1B			;Check for escape key
			JP		NZ, SKIP6
			CALL	R_U_SURE
			CP		'N'				;Check for exit
			JP		Z, GIVE_UP
			CP		'n'
			JP		Z, GIVE_UP
			JP		BEGIN			;Otherwise, start over
SKIP6:		CP		0X00			;Check for no match
			JP		NZ, SKIP4
			LD		HL, MESS2
			CALL	TXT_DISPLAY		;Tell it was not a vlaid key
			CALL	LINEFEED
			JP		RE_ENTER		;Try again
			;Upon returning here, there is a valid number in A 
			;and the mask address in HL
SKIP4:		CALL	TEST_STAR		;Test for a valid star
			CP		0X00			;Check for black hole
			JP		NZ, SKIP5
			LD		HL, MESS1		;Not a star, complain
			CALL	TXT_DISPLAY
			CALL	LINEFEED
			JP		RE_ENTER		;Jump back to try again
			;Upon returning here, there is vald star and the mask address is in HL
SKIP5:		CALL	CHANGE_UNI		;Up data the universe
			CALL	ADD_SHOT		;Update the score
			JP		RE_ENTER
;
;
EXIT:		CALL	LINEFEED
			RET						;Exit
;
;			Give up the game
GIVE_UP:	CALL	LINEFEED
			LD		HL, MESS8
			CALL	TXT_DISPLAY
			JP		EXIT
;
;			Are you sure you want to give up?
R_U_SURE:	CALL	LINEFEED
			LD		HL, MESS6
			CALL	TXT_DISPLAY
			CALL	IN_CHAR
			RET		
;
;			Update the score
ADD_SHOT:	LD		HL, SCORE
			LD		A, (HL)
			INC		A
			LD		(HL), A
			RET
;
;			Reset the score to zero
RST_SHOT:	LD		HL, SCORE
			LD		A, 0X00
			LD		(HL), A
			RET
;			
;			Test the input key in A for a valid key, return match in A, and table address in HL
;			or Zero in A if no match
TEST_SHOT:	CP		0X1B			;Check for the escape key
			JP		Z, MATCH
			CALL	PRINT_CHAR		;Echo the input back to the screen
			LD		HL, MASKTAB		;Load the address for the mask table
			LD		B, 0X0A			;Loop 9 times
NXT_TEST:	DEC		B
			JP		Z, NO_KEY		;If the key is not found in the table complain
			LD		D, (HL)			;Get the key data from the first column of the table
			CP		D				;Compare key in A to D
			JP		Z, MATCH
			INC		HL
			INC		HL
			INC		HL
			INC		HL				;Incerment 4 to the next key column
			JP		NXT_TEST
NO_KEY:		LD		A, 0X00		
MATCH:		RET
;
;			Test for a valid star, number in A, mask address in HL
;			Return zero in A if not star, otherwise HL holds mask address
TEST_STAR:	LD		DE, UNIVERSE	;Load the address of the universe
			LD		A, (DE)			;Load universe to A
			LD		C, A			;Load A to C
			INC 	HL				;Points to positon mask
			LD		A, (HL)			;Load the mask in A
			CP		0X00
			JP		NZ, NOT_FIVE
			INC		DE
			LD		A, (DE)			;DE plus one holds number 5 data
			JP		IS_STAR			;Return data, either 0 or 1
NOT_FIVE:	AND		C				;And A with C, if not zero it's a star other wise a black hole
			CP		0X00
			JP		Z, NOT_STAR
			JP		IS_STAR			;A will be greater than 0 if a star
NOT_STAR:	LD		A, 0X00			;A will be zero if not a star
IS_STAR:	RET
;
;			Up data the universe by XORing with the mask
CHANGE_UNI:	INC		HL				;Point to the xor mask
			LD		A, (HL)			;Get the mask
			LD		C, A			;Store in C
			LD		DE, UNIVERSE	;Load the universe
			LD		A, (DE)			
			XOR		C				;XOR with C
			LD		(DE), A			;Store the universe
			INC		HL				;Point to bit 5 mask
			LD		A, (HL)
			LD		C, A			;Store it in C
			INC		DE				;Point to bit 5
			LD		A, (DE)
			XOR		C				;XOR the bit
			LD		(DE), A			;Store the bit
			RET
;
;			Display the universe to the screen
DISP_UNIV:	PUSH	DE
			PUSH	BC
			CALL	LINEFEED
			CALL	LINEFEED		;Skip a line to start
			CALL	PRINT_TAB
			OR		A				;Clear A register and flags
			LD		HL, UNIVERSE
			LD		D, (HL)			;Load the universe into D
			INC		HL
			LD		E, (HL)			;Load number 5 into E
			LD		B, 0X0A			;Loop count of 10
DISP_LOOP:	DEC		B
			JP		Z, RETURN1		;If we reach zero, return 
			LD		A, B			;Load the count into A
			CP		0X03			;If 3, add line feed and tab
			JP		NZ, SKIP0
			CALL	LINEFEED
			CALL	PRINT_TAB
SKIP0:		CP		0X06			;If 6 add line feed and tab
			JP		NZ, SKIP1
			CALL	LINEFEED
			CALL	PRINT_TAB
SKIP1:		CP		0X05			;5 is special
			JP		NZ, SKIP2
			OR		A				;Clear A and the flags
			LD		A, E			;Place number 5 into A
			RRC		A				;Place into Carry 
			JP		SKIP3
SKIP2:		OR		A
			LD		A, D			;Load universe into A
			RRC		A				;Place the bit into the carry flag
			LD		D, A			;Store universe back into d
SKIP3:		JP		C, STAR			;If the carry is set goto print a star
			LD		A, '-'			;Otherwise print a black hole
			CALL	PRINT_CHAR
			JP		SPACE
STAR:		LD		A, '*'			;Print a star
			CALL	PRINT_CHAR
SPACE:		LD		A, ' '			;Space it out a little to make it look nicer
			CALL	PRINT_CHAR
			JP		DISP_LOOP		;Go back for the next count
RETURN1:	POP		BC
			POP		DE
			RET
;
;			Test for a win or lose, return in A
;			0 for nither, 0x0f for lost, 0xff for win
WIN_TEST:	LD		HL, UNIVERSE
			INC		HL
			LD		A, (HL)			;Check position 5
			CP		0X00			;Check for zero
			JP		NZ, NITHER		;If position 5 is not zero it's nither a win or lose
			DEC		HL
			LD		A, (HL)			;Load byte 1 into A
			CP		0XFF			;All edge posions stars?
			JP		Z, WINNER
			CP		0X00			;All black holes
			JP		Z, LOOSER		;The game is lost
NITHER:		LD		A, 0X00
			JP		WIN_RTN
LOOSER:		LD		A, 0X0F
			JP		WIN_RTN
WINNER:		LD		A, 0XFF
WIN_RTN:	RET	
;
;			Display the score to the screen
DPLY_SCORE:	PUSH	DE
			PUSH	BC
			LD		B, 0X30			;Load the count registers, ones
			LD		C, 0X30			;Tens
			LD		D, 0X30			;Hundreds
			LD		E, 0X00			;Clear the count register		
			LD		HL, SCORE		;The address of the score 
TEST_IT:	LD		A, (HL)			;Load the score to A
			CP		E
			JP		Z, SHOW_IT		;If count equals score, display
			INC		E
			INC		B				;Add to the ones
			LD		A, B			;PLace B into A
			CP		0X3A			;Compare 9 + 1, overflow
			JP		NZ, TEST_IT
			INC		C				;Add one to the tens place
			LD		B, 0X30			;Reset the ones
			LD		A, C
			CP		0X3A			;Compare 9 + 1, overflow
			JP		NZ, TEST_IT
			INC		D				;Add one to the hundreds
			LD		C, 0X30			;Reset the tens
			JP		TEST_IT
SHOW_IT:	LD		A, D
			CP		0X30			;Check for zero
			JP		Z, NO_HUND		;If the hundreds is zero, skip display
			CALL	PRINT_CHAR
			LD		A, C			;If there a hundreds, there are tens
			JP		HUNDREDS		;Skip checking tens for zero
NO_HUND:	LD		A, C
			CP		0X30			;Check the tens for zero, if hundreds is zero
			JP		Z, NO_TENS
HUNDREDS:	CALL	PRINT_CHAR
NO_TENS:	LD		A, B
			CALL	PRINT_CHAR		;Display the ones
			POP		BC
			POP		DE
			RET	
;
;           Print a charactor in A to the screen
PRINT_CHAR: PUSH    DE
            PUSH    BC
            LD      C, 0X02
            RST     0X30
            POP     BC
            POP     DE
            RET					
;
;			Print a tab to the screen
PRINT_TAB:	PUSH	DE
			PUSH	BC
			PUSH	AF
			LD		A, ' '
			LD		C, 0X02
			RST		0X30
			RST		0X30
			RST		0X30
			RST		0X30
			POP		AF
			POP		BC
			POP		DE
			RET
;
;			Print a text message to the screen
TXT_DISPLAY:PUSH	DE
			PUSH	BC
			LD		DE, HL			;Put the address in the DE register
			LD		C, 0X06			;Function to display a line
			RST		0X30
			POP		BC
			POP		DE
			RET
;
;			Return an input charactor in A
IN_CHAR:	PUSH	BC
			OR		A
			LD		C, 0X01			;Input charactor function
			RST		0X30			;Call the input function and wait for charactor
			POP		BC
			RET
;
;			Print a line feed to the screen
LINEFEED:	PUSH	BC
			LD		C, 0X07
			RST		0X30
			POP		BC
			RET
;			
;			Hold the universe.
;			Byte one holds positions 1 to 4 and 6 to 9
;			Byte two holds position 5
UNIVERSE:	DB	0X00, 0X00
;			Keep score
SCORE:		DB	0X00
;			The mask table
;			Byte one is the key number
;			Byte two is the position in the universe byte
;			Byte three is the XOR byte for the universe byte
;			Byte four is the XOR byte for position 5
MASKTAB:	DB	0X31, 0X01, 0X0B, 0X01
			DB	0X32, 0X02, 0X07, 0X00
			DB	0X33, 0X04, 0X16, 0X01
			DB	0X34, 0X08, 0X29, 0X00
			DB  0X35, 0X00, 0X5A, 0X01
			DB  0X36, 0X10, 0X94, 0X00
			DB  0X37, 0X20, 0X68, 0X01
			DB  0X38, 0X40, 0XE0, 0X00
			DB  0X39, 0X80, 0XD0, 0X01
;
;			Start screen message
MESS0:		DB	"                                                  ", 5
			DB	"                                                  ", 5
			DB	"  S H O    SSS     TTTTT     AAA      RRR      SSS", 5
			DB	"           S         T       A A      R R      S  ", 5
			DB	"  O - T    SSS       T       AAA      RRR      SSS", 5
			DB	"             S       T       A A      RR         S", 5
			DB 	"  I N G    SSS       T       A A      R R      SSS", 5
			DB	"  ********** S H O O T I N G  S T A R S **********", 5
			DB	"              A  BRAIN  TEASER  GAME              ", 5
			DB	" ", 5
			DB	"  DO YOU WANT INSTRUCTIONS ---=>(Y/N/ESC)", 0
;			
MESS1:		DB	" ", 5
			DB	"  HEY! YOU CAN ONLY SHOOT STARS ", 5
			DB	"  NOT BLACK HOLES.              ", 5
			DB	" ", 5
			DB  "  TRY AGAIN!", 5, 0
;
MESS2:		DB	"  THAT WASN'T A VALID STAR NUMBER! ", 5
			DB	" ", 5
			DB  "  TRY AGAIN!", 5, 0
;
MESS3:		DB	"  YOU LOST THE GAME!",5
			DB	"  WANT TO SHOOT MORE STARS? ---=>(Y/N)", 0
;
MESS4:		DB  "  YOU WIN! GOOD SHOOTING!", 5
			DB	"  YOU FIRED :", 0
;
MESS5:		DB	" SHOTS.", 5
			DB	"  BEST POSSIBLE SCORE IS 11 SHOTS.", 5
			DB	"  WANT TO SHOOT AGAIN, DEADEYE? ---=>(Y/N)", 0
;
MESS6:		DB	"  YOU GIVE UP TOO EASILY!", 5
			DB	"  WHAT TO SHOOT MORE STARS? ---=>(Y/N)", 0
;
MESS7:		DB	"  YOUR SHOT?  ", 0
;
MESS8:		DB  " ", 5
			DB	"  THANKS FOR PLAYING. ", 5
			DB	"  SORRY TO SEE YOU LEAVE...", 5, 0
;
MESS9:		DB	"  YOU SHOT ", 0
;
MESS10:		DB	" STARS. ", 5, 0
;
PAGE1:		DB	"  THERE ARE STARS         *", 5
			DB  "  THERE ARE BLACK HOLES   -", 5
			DB	"  IN THE UNIVERSE:       X  X  X", 5
			DB  "                         X  X  X", 5
			DB  "                         X  X  X", 5
			DB  " ", 5
			DB  "  YOU SHOOT A STAR ", 5
			DB  "  (NOT A BLACK HOLE)", 5
			DB	"  BY TYPING IT'S NUMBER: 1  2  3", 5
			DB  "                         4  5  6", 5
			DB  "                         7  8  9", 5
			DB  " ", 5
			DB  "  THAT CHANGES THE STAR INTO A BLACK HOLE.", 5
			DB  " ", 5
			DB  " (TO SEE MORE RULES, TYPE ANY KEY)", 5
			DB  " ", 0
;			
PAGE2:		DB  "  EACH STAR IS IN A GALAXY. WHEN YOU ", 5
			DB  "  SHOOT A STAR, EVERYTHING IN IT'S GALAXY", 5
			DB  "  CHANGES. STARS BECOME BLACK HOLES", 5
			DB  "  AND BLACK HOLES BECOME STARS.", 5
			DB  " ", 5
			DB  "  GALAXIES:", 5
			DB  " ", 5
			DB  "  1  *  -    *  2  *    -  *  3    *  -  -    -  *  -", 5
			DB  "  *  *  -    -  -  -    -  *  *    4  -  -    *  5  *", 5
			DB  "  -  -  -    -  -  -    -  -  -    *  -  -    -  *  -", 5
			DB  " ", 5
			DB  "  -  -  *    -  -  -    -  -  -    -  -  -", 5
			DB  "  -  -  6    *  *  -    -  -  -    -  *  *", 5
			DB  "  -  -  *    7  *  -    *  8  *    -  *  9", 5
			DB  " ", 5
			DB  "  (TYPE ANY KEY FOR THE LAST PAGE OF RULES.)", 5
			DB  " ", 0
;
PAGE3:      DB  "  THE GAME STARTS", 5
			DB  "  WITH THE UNIVERSE     -  -  -", 5
			DB  "  LIKE THIS:            -  *  -", 5
			DB  "                        -  -  -", 5
			DB  " ", 5
			DB  "  YOU WIN WHEN YOU      *  *  *", 5
			DB  "  CHANGE IT TO THIS:    *  -  *", 5
			DB  "                        *  *  *", 5
			DB  " ", 5
			DB  "  READY TO PLAY. TYPE ANY KEY TO START", 5
			DB  "  THE GAME. GOOD LUCK...", 5
			DB  " ", 0
;
