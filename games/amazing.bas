10 PRINT TAB(28);"AMAZING PROGRAM"
20 PRINT TAB(15);"CREATIVE COMPUTING  MORRISTOWN, NEW JERSEY"
30 PRINT:PRINT:PRINT:PRINT
100 INPUT "WHAT ARE YOUR WIDTH AND LENGTH";H,V
102 IF H<>1 AND V<>1 THEN GOTO 110
104 PRINT "MEANINGLESS DIMENSIONS.  TRY AGAIN.":GOTO 100
110 DIM W(H,V)
115 DIM V(H,V)
120 PRINT
130 PRINT
140 PRINT
150 PRINT
160 Q=0:Z=0:X=INT(RND()*H+1)
165 FOR I=1 TO H
170 IF I=X THEN GOTO 173
171 PRINT ".--";:GOTO 180
173 PRINT ".  ";
180 NEXT I
190 PRINT "."
195 C=1:W(X,1)=C:C=C+1
200 R=X:S=1:GOTO 260
210 IF R<>H THEN GOTO 240
215 IF S<>V THEN GOTO 230
220 R=1:S=1:GOTO 250
230 R=1:S=S+1:GOTO 250
240 R=R+1
250 IF W(R,S)=0 THEN GOTO 210
260 IF R-1=0 THEN GOTO 530
265 IF W(R-1,S)<>0 THEN GOTO 530
270 IF S-1=0 THEN GOTO 390
280 IF W(R,S-1)<>0 THEN GOTO 390
290 IF R=H THEN GOTO 330
300 IF W(R+1,S)<>0 THEN GOTO 330
310 X=INT(RND()*3+1)
320 ON X GOTO 790,820,860
330 IF S<>V THEN GOTO 340
334 IF Z=1 THEN GOTO 370
338 Q=1:GOTO 350
340 IF W(R,S+1)<>0 THEN GOTO 370
350 X=INT(RND()*3+1)
360 ON X GOTO 790,820,910
370 X=INT(RND()*2+1)
380 ON X GOTO 790,820
390 IF R=H THEN GOTO 470
400 IF W(R+1,S)<>0 THEN GOTO 470
405 IF S<>V THEN GOTO 420
410 IF Z=1 THEN GOTO 450
415 Q=1:GOTO 430
420 IF W(R,S+1)<>0 THEN GOTO 450
430 X=INT(RND()*3+1)
440 ON X GOTO 790,860,910
450 X=INT(RND()*2+1)
460 ON X GOTO 790,860
470 IF S<>V THEN GOTO 490
480 IF Z=1 THEN GOTO 520
485 Q=1:GOTO 500
490 IF W(R,S+1)<>0 THEN GOTO 520
500 X=INT(RND()*2+1)
510 ON X GOTO 790,910
520 GOTO 790
530 IF S-1=0 THEN GOTO 670
540 IF W(R,S-1)<>0 THEN GOTO 670
545 IF R=H THEN GOTO 610
547 IF W(R+1,S)<>0 THEN GOTO 610
550 IF S<>V THEN GOTO 560
552 IF Z=1 THEN GOTO 590
554 Q=1:GOTO 570
560 IF W(R,S+1)<>0 THEN GOTO 590
570 X=INT(RND()*3+1)
580 ON X GOTO 820,860,910
590 X=INT(RND()*2+1)
600 ON X GOTO 820,860
610 IF S<>V THEN GOTO 630
620 IF Z=1 THEN GOTO 660
625 Q=1:GOTO 640
630 IF W(R,S+1)<>0 THEN GOTO 660
640 X=INT(RND()*2+1)
650 ON X GOTO 820,910
660 GOTO 820
670 IF R=H THEN GOTO 740
680 IF W(R+1,S)<>0 THEN GOTO 740
685 IF S<>V THEN GOTO 700
690 IF Z=1 THEN GOTO 730
695 Q=1:GOTO 830
700 IF W(R,S+1)<>0 THEN GOTO 730
710 X=INT(RND()*2+1)
720 ON X GOTO 860,910
730 GOTO 860
740 IF S<>V THEN GOTO 760
750 IF Z=1 THEN GOTO 780
755 Q=1:GOTO 770
760 IF W(R,S+1)<>0 THEN GOTO 780
770 GOTO 910
780 GOTO 1000
790 W(R-1,S)=C
800 C=C+1:V(R-1,S)=2:R=R-1
810 IF C=H*V+1 THEN GOTO 1010
815 Q=0:GOTO 260
820 W(R,S-1)=C
830 C=C+1
840 V(R,S-1)=1:S=S-1:IF C=H*V+1 THEN GOTO 1010
850 Q=0:GOTO 260
860 W(R+1,S)=C
870 C=C+1:IF V(R,S)=0 THEN GOTO 880
875 V(R,S)=3:GOTO 890
880 V(R,S)=2
890 R=R+1
900 IF C=H*V+1 THEN GOTO 1010
905 GOTO 530
910 IF Q=1 THEN GOTO 960
920 W(R,S+1)=C:C=C+1:IF V(R,S)=0 THEN GOTO 940
930 V(R,S)=3:GOTO 950
940 V(R,S)=1
950 S=S+1:IF C=H*V+1 THEN GOTO 1010
955 GOTO 260
960 Z=1
970 IF V(R,S)=0 THEN GOTO 980
975 V(R,S)=3:Q=0:GOTO 1000
980 V(R,S)=1:Q=0:R=1:S=1:GOTO 250
1000 GOTO 210
1010 FOR J=1 TO V
1011 PRINT "I";
1012 FOR I=1 TO H
1013 IF V(I,J)<2 THEN GOTO 1030
1020 PRINT "   ";
1021 GOTO 1040
1030 PRINT "  I";
1040 NEXT I
1041 PRINT
1043 FOR I=1 TO H
1045 IF V(I,J)=0 THEN GOTO 1060
1050 IF V(I,J)=2 THEN GOTO 1060
1051 PRINT ":  ";
1052 GOTO 1070
1060 PRINT ":--";
1070 NEXT I
1071 PRINT "."
1072 NEXT J
1073 END