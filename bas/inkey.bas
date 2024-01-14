10   PRINT "reading .."
20   C$ = INKEY()
30   IF C$<>"" THEN GOTO 50
40   GOTO 10
50   PRINT "c$ is ";C$
