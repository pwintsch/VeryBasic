10      REM
20      REM Tiny BASIC Interpreter and Compiler Project
30      REM Tic-tac-toe Sample Game
40      REM
50      REM Released as public domain by Damian Gareth Walker, 2019
60      REM Created: 21-Sep-2019
70      REM Adapted to NanoBasic by Peter Wintsch, November 2022
80      REM
90      REM --- Variables
100      REM     A   - first square in line examined
105      A=0
110      REM     B   - second square in line examined
115      B=0
120      REM     C   - third square in line examined
125      C=0
130      REM     D   - player whose pieces to count
135      D=0
140      REM     E   - number of D's pieces on a line
145      E=0
150      REM     F   - first square of line to examine
155      F=0
160      REM     G   - game winner
165      G=0
170      REM     H   - which side the human takes
175      H=0
180      REM     I   - increment for line to examine
185      I=0
190      REM     L   - line to examine
195      L=0
200      REM     M   - where to move (various uses)
205      M=0
210      REM     N   - piece found in a square
215      N=0
220      REM     P   - player currently playing
225      P=0
230      REM     Q   - square to examine
235      Q=0
240      REM     R-Z - contents of the board
241      R=0
242      S=0
243      T=0
244      U=0
245      V=0
246      W=0
247      X=0
248      Y=0
249      Z=0
260      REM --- Main Program
270      GOSUB 350
280      GOSUB 500
290      GOSUB 690
300      STOP
320      REM --- Subroutine to initialise the game
330      REM     Outputs: H - Human play order
340      REM              P - Whose turn it is
350      PRINT "Tic tac toe. Board positions are:"
360      PRINT " 1  2  3"
370      PRINT " 4  5  6"
380      PRINT " 7  8  9"
390      PRINT "Play first or second (1/2)?"
400      INPUT H
410      IF H<1 THEN GOTO 350
420      IF H>2 THEN GOTO 350
430      LET P=1
440      RETURN
460      REM --- Subroutine to take turns
470      REM     Inputs:  H - who is the human
480      REM              P - whose turn it is
490      REM     Outputs: G - who won the game
500      IF P=H THEN GOSUB 770
510      IF P<>H THEN GOSUB 900
520      GOSUB 1640
530      IF G>0 THEN RETURN
540      LET P=3-P
550      IF R=0 THEN GOTO 500
560      IF S=0 THEN GOTO 500
570      IF T=0 THEN GOTO 500
580      IF U=0 THEN GOTO 500
590      IF V=0 THEN GOTO 500
600      IF W=0 THEN GOTO 500
610      IF X=0 THEN GOTO 500
620      IF Y=0 THEN GOTO 500
630      IF Z=0 THEN GOTO 500
640      RETURN
660      REM --- Victory
670      REM Inputs: H   - which side was the human
680      REM         P   - player who won
690      IF G=H THEN PRINT "You win!"
700      IF G<>0 THEN IF G<>H THEN PRINT "Computer wins"
710      IF G=0 THEN PRINT "A draw"
720      RETURN
740      REM --- Subroutine to allow the player to move
750      REM     Inputs:  P   - player number
760      REM     Outputs: M   - where the player wishes to move
770      PRINT "Move? "
780      INPUT Q
790      IF Q<1 THEN GOTO 770
800      IF Q>9 THEN GOTO 770
810      GOSUB 1790
820      IF N<>0 THEN GOTO 770
830      LET M=Q
840      GOSUB 1950
850      RETURN
870      REM --- Subroutine to make the computer's move
880      REM     Inputs:  P   - player number
890      REM     Outputs: M   - the move chosen
900      LET M=0
910      LET D=3-H
920      GOSUB 1150
930      IF M>0 THEN GOTO 1050
940      LET D=H
950      GOSUB 1150
960      IF M=0 THEN IF V=0 THEN LET M=5
970      IF M=0 THEN IF R=0 THEN LET M=1    
980      IF M=0 THEN IF T=0 THEN LET M=3    
990      IF M=0 THEN IF X=0 THEN LET M=7    
1000      IF M=0 THEN IF Z=0 THEN LET M=9
1010      IF M=0 THEN IF S=0 THEN LET M=2    
1020      IF M=0 THEN IF U=0 THEN LET M=4    
1030      IF M=0 THEN IF Y=0 THEN LET M=8    
1040      IF M=0 THEN IF W=0 THEN LET M=6
1045      REM LABEL 135    
1050      GOSUB 1950
1060      PRINT "Computer move ",M
1070      RETURN
1090      REM --- Identify moves to win or avoid a loss
1100      REM     Inputs:  D   - player whose pieces we're counting
1110      REM     Changes: E   - number of pieces on line being scanned
1120      REM              F   - first square in winning line
1130      REM              I   - increment of winning line
1140      REM              L   - line being scanned (counter)
1145      REM LABEL 145
1150      LET L=1
1155      REM Label 146
1160      GOSUB 1360
1170      IF E<2 THEN GOTO 1220
1180      IF A=0 THEN LET M=F
1190      IF B=0 THEN LET M=F+I
1200      IF C=0 THEN LET M=F+I+I
1210      IF M>0 THEN RETURN
1215      REM LABEL 152
1220      LET L=L+1
1230      IF L<9 THEN GOTO 1360
1240      RETURN
1260      REM --- Count a player's pieces on a line
1270      REM     Inputs:  D   - player whose pieces we're counting
1280      REM              L   - line number
1290      REM     Changes: F   - first square on the line
1300      REM              I   - increment of the line
1310      REM              Q   - individual squares to examine
1320      REM     Outputs: A   - contents of first square
1330      REM              B   - contents of second square
1340      REM              C   - contents of third square
1350      REM              E   - number of the player's pieces
1355      REM LABEL 170
1360      IF L>3 THEN GOTO 1400
1370      LET F=3*L-2
1380      LET I=1
1390      GOTO 1460
1395      REM LABEL 174
1400      IF L>6 THEN GOTO 1440
1410      LET F=L-3
1420      LET I=3
1430      GOTO 1460
1435      REM LABEL 178
1440      LET F=1+2*(L-7)
1450      LET I=4-2*(L-7)
1455      REM LABEL 180
1460      LET E=0
1470      LET Q=F
1480      GOSUB 1790
1490      LET A=N
1500      IF N=D THEN LET E=E+1
1510      LET Q=Q+I
1520      GOSUB 1790
1530      LET B=N
1540      IF N=D THEN LET E=E+1
1550      LET Q=Q+I
1560      GOSUB 1790
1570      LET C=N
1580      IF N=D THEN LET E=E+1
1590      RETURN
1610      REM --- Subroutine to check for a win
1620      REM     Inputs:  R-Z - board squares
1630      REM     Outputs: G   - the winning player (0 for neither)
1640      LET G=0
1650      IF R>0 THEN IF R=S THEN IF S=T THEN LET G=R
1660      IF U>0 THEN IF U=V THEN IF V=W THEN LET G=U
1670      IF X>0 THEN IF X=Y THEN IF Y=Z THEN LET G=X
1680      IF R>0 THEN IF R=U THEN IF U=X THEN LET G=R
1690      IF S>0 THEN IF S=V THEN IF V=Y THEN LET G=S
1700      IF T>0 THEN IF T=W THEN IF W=Z THEN LET G=T
1710      IF R>0 THEN IF R=V THEN IF V=Z THEN LET G=R
1720      IF T>0 THEN IF T=V THEN IF V=X THEN LET G=T
1730      RETURN
1750      REM --- Subroutine to see what piece is in a square
1760      REM     Inputs:  Q   - the square to check
1770      REM              R-Z - the contents of the squares
1780      REM     Outputs: N   - the piece in that square
1790      LET N=0
1800      IF Q=1 THEN LET N=R
1810      IF Q=2 THEN LET N=S
1820      IF Q=3 THEN LET N=T
1830      IF Q=4 THEN LET N=U
1840      IF Q=5 THEN LET N=V
1850      IF Q=6 THEN LET N=W
1860      IF Q=7 THEN LET N=X
1870      IF Q=8 THEN LET N=Y
1880      IF Q=9 THEN LET N=Z
1890      RETURN
1910      REM --- Subroutine to put a piece in a square
1920      REM     Inputs:  P   - the player whose piece should be placed
1930      REM              M   - the square to put the piece in
1940      REM     Changes: R-Z - the contents of the squares
1950      IF M=1 THEN LET R=P
1960      IF M=2 THEN LET S=P
1970      IF M=3 THEN LET T=P
1980      IF M=4 THEN LET U=P
1990      IF M=5 THEN LET V=P
2000      IF M=6 THEN LET W=P
2010      IF M=7 THEN LET X=P
2020      IF M=8 THEN LET Y=P
2030      IF M=9 THEN LET Z=P
2040      RETURN
