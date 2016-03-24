import sys
import math
# input = XX.cnt
import operator
x = {1: 2, 3: 4, 4: 3, 2: 1, 0: 0}
sorted_x = sorted(x.items(), key=operator.itemgetter(1))

f= open(sys.argv[1],"r")
p = open(sys.argv[2],"r")
t = open(sys.argv[3],"w")

TT = []
F = {}
TF = 0
for line in f:
	l = line.strip()
	string = l.split('\t')[0]
	freq = int(l.split('\t')[1])
	F[string] = freq
	TF += freq
	TT.append(string)

P = {}
PF = 0
for line in p:
	l = line.strip()
	string = l.split('\t')[0]
	freq = int(l.split('\t')[1])
	P[string] = freq
	PF += freq
	TT.append(string)
TT = list(set(TT))
T = {}
for w in TT:
	'''if w in F and w not in P:
		w1 = w
		w2 = w
		P[w2] = 0.01
		prob1 = float(F[w1])/float(TF)
		prob2 = float(P[w2])/float(PF)
		cross1 = prob1*math.log(prob1/prob2)
		cross2 = prob2*math.log(prob2/prob1)
		KL = 0.5*(cross1+cross2)

		T[(w1,w2)] = KL
	elif w not in F and w in P:
		w1 = w
		w2 = w
		F[w1] = 0.01
		prob1 = float(F[w1])/float(TF)
		prob2 = float(P[w2])/float(PF)
		cross1 = prob1*math.log(prob1/prob2)
		cross2 = prob2*math.log(prob2/prob1)
		KL = 0.5*(cross1+cross2)
		T[(w1,w2)] = KL

	else:
		w1 = w
		w2 = w
		prob1 = float(F[w1])/float(TF)
		prob2 = float(P[w2])/float(PF)
		cross1 = prob1*math.log(prob1/prob2)
		cross2 = prob2*math.log(prob2/prob1)
		KL = 0.5*(cross1+cross2)
		T[(w1,w2)] = KL'''
	if w in F and w in P:
		w1 = w
		w2 = w
		prob1 = float(F[w1])/float(TF)
		prob2 = float(P[w2])/float(PF)
		cross1 = prob1*math.log(prob1/prob2)
		cross2 = prob2*math.log(prob2/prob1)
		KL = 0.5*(cross1+cross2)
		T[(w1,w2)] = KL
sorted_T = sorted(T.items(), key=operator.itemgetter(1),reverse=True)

for w in sorted_T:
	w1 = w[0][0]
	#w2 = w[0][1]	
	KL = w[1]

	print w1,KL


