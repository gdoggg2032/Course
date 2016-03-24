import sys
import numpy
import math
def get_Index(INDEX):

	Index = {}
	with open(INDEX,"r") as f:
		for line in f:
			line = line.strip()
			if len(line.split()) < 2:
				Num = int(line)
			else:
				string = line.split()[0]
				index = int(line.split()[1])
				Index[string] = index
	return Index, Num

def get_Doc_Index(DOC_INDEX):
	Doc_Index = {}
	with open(DOC_INDEX,"r") as f:
		for line in f:
			line = line.strip()
			if len(line.split()) < 2:
				Doc_Index_Num = int(line)
			else:
				string = line.split()[0]
				index = int(line.split()[1])
				Doc_Index[index] = string
	return Doc_Index, Doc_Index_Num

def get_Data(DATA, Index_Num, Doc_Index_Num):
	
	Data = []
	DF = numpy.zeros((Index_Num,1))
	IDF = numpy.zeros((Index_Num,1))

	with open(DATA,"r") as f:
		for line in f:
			line = line.strip()
			Vec = numpy.zeros((Index_Num,1))
			for word_index in line.split():
				w = int(word_index)
				Vec[w] += 1
			for w_once in set(line.split()):
				w = int(w_once)
				DF[w] += 1
			Data.append(Vec)
	
	for i in xrange(len(DF)):
		IDF[i] = math.log((Doc_Index_Num+0.5)/(DF[i]+0.5))
		
	return Data, IDF

def get_Query_Index(QUERY, Index, Index_Num):

	Query = []

	with open(QUERY,"r") as f:
		for line in f:
			line = line.strip()
			Vec = numpy.zeros((Index_Num,1))
			
			for word_index in line.strip().split():
				w = int(Index[word_index])
				Vec[w] += 1
			Query.append(Vec)
	return Query

def VSM(Data, Query, IDF, TOP_K, TOP_P):
	Result = []
	for query in Query:

		qlen = numpy.sqrt(numpy.dot(query.T,query))
		qresult = []
		for i in xrange(len(Data)):

			data = numpy.multiply(Data[i],IDF) #TF-IDF
			dlen = numpy.sqrt(numpy.dot(data.T,data))
			sim = numpy.dot(data.T,query)/(qlen*dlen)
			
			qresult.append((i,sim))
		qresult = sorted(qresult, key=lambda tup: tup[1], reverse=True)

		#pseudo feedback
		#pquery = numpy.copy(query)
		pquery = numpy.zeros((query.shape[0],1))
		for i in xrange(TOP_P):
			doc = qresult[i][0]
			data = numpy.multiply(Data[doc],IDF)
			pquery = numpy.add(pquery,data)
		query = 0.8*query + 0.2* pquery/TOP_P
		#get query expansion


		qlen = numpy.sqrt(numpy.dot(query.T,query))
		qresult = []
		for i in xrange(len(Data)):

			data = numpy.multiply(Data[i],IDF) #TF-IDF
			dlen = numpy.sqrt(numpy.dot(data.T,data))
			sim = numpy.dot(data.T,query)/(qlen*dlen)
			
			qresult.append((i,sim))
		qresult = sorted(qresult, key=lambda tup: tup[1], reverse=True)



		result = []
		for i in xrange(TOP_K):
			(d,sim) = qresult[i]
			rank = i
			if sim <= 0:
				break
			result.append((d,sim[0][0],rank))
		Result.append(result)
	return Result
		
def Dump_Result(RESULT, Result, Doc_Index):
	#qid iter   docno      rank  sim   run_id 
	with open(RESULT,"w") as f:
		for i in xrange(len(Result)):
			query_id = i+1
			second = 0
			forth = 1
			result = Result[i]
			for (doc_id,sim,rank) in result:
				doc_name = Doc_Index[doc_id]
				output = " ".join([str(query_id),str(second),doc_name,str(rank),str(sim),str(forth)])+'\n'
				f.write(output)

if __name__ == "__main__":
	INDEX = sys.argv[1]
	DOC_INDEX = sys.argv[2]
	DATA = sys.argv[3]
	QUERY = sys.argv[4]
	RESULT = sys.argv[5]

	Index, Index_Num = get_Index(INDEX)

	print "get_Index: Index Num: ", Index_Num

	Doc_Index, Doc_Index_Num = get_Doc_Index(DOC_INDEX)
	print "get_Doc_Index: Index Num: ", Doc_Index_Num

	Data, IDF = get_Data(DATA, Index_Num, Doc_Index_Num)
	print "get_Data"
	

	Query = get_Query_Index(QUERY, Index, Index_Num)
	print "get_Query_Index"

	TOP_K = 1000
	print "TOP_K: ", TOP_K
	TOP_P = 8
	print "TOP_P: ", TOP_P,", assume TOP_P is relevant."


	Result = VSM(Data, Query, IDF, TOP_K, TOP_P)
	print "VSM"

	Dump_Result(RESULT, Result, Doc_Index)
	print "Dump_Result"
	







