# -*- encoding=utf-8 -*-
import sys
import os

#C_num = number of classes
DestDir = sys.argv[1]
C_num = int(sys.argv[2])
C_list = []
for c in range(0,C_num):
	C_list.append(sys.argv[c+3])
TESTFILE = sys.argv[3+C_num]
print DestDir, C_num, C_list, TESTFILE

# train LM for each class
for Class in C_list:
	LMCNT = Class+'.cnt'
	LMORDER = str(4)
	LM = Class+'.lm'
	os.system(DestDir+'ngram-count -text '+Class+' -write '+LMCNT+' -order '+LMORDER)
	#./ngram-count –read lm.cnt –lm bigram.lm –unk –order 2
	os.system(DestDir+'ngram-count -read '+LMCNT+' -lm '+LM+' -unk -order '+LMORDER)
	

	OUTPUTPORB = TESTFILE+"."+Class.split('/')[-1]
	os.system(DestDir+'ngram -lm '+LM+' -ppl '+TESTFILE+' -debug 1 > '+OUTPUTPORB)	
	print OUTPUTPORB


