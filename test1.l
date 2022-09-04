0000 label:
0000 ldc 0
0001 ldc -5
0002 ldc +5
0003 loop: br loop 
0004 br next
0005 next:
0005 ldc loop 
0006 ldc var1 
0007 var1: data 0 