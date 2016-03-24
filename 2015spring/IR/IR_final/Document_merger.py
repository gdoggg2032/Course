import sys

N = len(sys.argv)
S = (N-3)/2
f1 = open(sys.argv[N-2],"w")
f2 = open(sys.argv[N-1],"w")
for i in range(1,N-2):
	if i < 1+S:
		f = open(sys.argv[i],"r")
		print sys.argv[i] + " to "+sys.argv[N-2]
		d = f.read()
		f1.write(d)
		f.close()
	else:
		f = open(sys.argv[i],"r")
		print sys.argv[i] + " to "+sys.argv[N-1]

		d = f.read()
		f2.write(d)
		f.close()
f1.close()
f2.close()