5   LET C = 0 : LET R = 0
10   FOR I = 1 TO 5
15   PRINT I
20   IF I<>3 THEN GOTO 30
21   IF C<>0 THEN GOTO 30
22   LET C = 1
23   PRINT "calling gosub"
24   GOSUB 10
30   IF I<>5 THEN GOTO 40
31   IF C<>1 THEN GOTO 40
32   IF R<>0 THEN GOTO 40
33   LET I = 3
34   LET R = 1
35   PRINT "call return"
36   RETURN 
40   NEXT I
50   END 
