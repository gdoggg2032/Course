# -*- coding: utf-8 -*-

import sys
import random
import math
import jieba
import jieba.posseg as pseg


f = open(sys.argv[1],"r")
p = open(sys.argv[2],"w")

for line in f:
	l = jieba.cut(line)
	W = ""
	W = " ".join(l)
	p.write(W.encode("utf-8"))
