import sys

#open p2.train.txt
with open(sys.argv[1], "r") as train_f:
    #open the output train file
    f = open(sys.argv[2],"w")

    for line in train_f:
        tokens = line.strip().split('\t')
        ID = tokens[0]#ID
        SW = list(tokens[1].decode('utf-8'))#wrong sentence
        SR = list(tokens[2].decode('utf-8'))#right sentence
        
        f.write(ID+'\t')
        for c in SW:
            f.write(c.encode('utf-8')+' ')
        f.write('\t')
        
        #see where the redundant words are
        i = 0
        j = 0
        while i < len(SW):
            if SW[i] != SR[j]:#redundant
                i+=1
                f.write(str(i)+' ')
            else:#right
                i+=1
                j+=1

            #the remaining words are all the redunant words
            if j == len(SR):
                while i < len(SW):
                    i+=1
                    f.write(str(i)+' ')
        f.write('\n')
            
