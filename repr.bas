5   DIM A1(6)
10   A$="123"
15   PRINT  : PRINT " ---- ROUND 1 ---- " : PRINT
20   PRINT "a$ is ";A$ : FOR Z=1 TO 3 : A1(Z)=ASC(MID$(A$, Z, 1)) : PRINT " a1(";Z;") is";A1(Z) : NEXT Z
30   PRINT "a1(1): ";A1(1)," a1(2): ";A1(2)," a1(3): ";A1(3)
35   PRINT  : PRINT " ---- ROUND 2 ---- " : PRINT
40   FOR Z=1 TO 3
50   A1(Z)=ASC(MID$(A$, Z, 1))
60   PRINT " a1(";Z;") is";A1(Z)
70   NEXT Z
80   PRINT "a1(1): ";A1(1)," a1(2): ";A1(2)," a1(3): ";A1(3)
