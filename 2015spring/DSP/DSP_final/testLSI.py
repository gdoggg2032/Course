#! -*- encoding=utf-8 -*-
from gensim import corpora, models, similarities
import logging
import sys


logging.basicConfig(format="%(asctime)s : %(levelname)s : %(message)s", level=logging.INFO)


#documents = ["Shipment of gold damaged in a fire","Delivery of silver arrived in a silver truck","Shipment of gold arrived in a truck"]

documents = []
Doc = []

with open(sys.argv[1],"r") as f:
	while True:
		Name = f.readline()
		if not Name:
			break
		Name = Name.split('/')[-1].split('.')[0]
		document = f.readline().strip()
		trash = f.readline()
		documents.append(document)
		Doc.append(Name)


#将英文单词小写化
#texts = [[word for word in document.lower().split()] for document in documents]
texts = [[word for word in document.split()] for document in documents]
'''
for text in texts:
	for word in text:
		print word.decode("utf-8")
'''
#将文档的token映射为id
dictionary = corpora.Dictionary(texts)
#print dictionary.token2id

#将用字符串表示的文档转换为用id表示的文档向量
corpus = [dictionary.doc2bow(text) for text in texts]
#print corpus

#build TF-IDF model
tfidf = models.TfidfModel(corpus)

#将上述用词频表示文档向量表示为一个用tf-idf值表示的文档向量
corpus_tfidf = tfidf[corpus]
#for doc in corpus_tfidf:
#	print doc
#print tfidf.dfs
#print tfidf.idfs

#训练一个LSI模型，设置topic数为2
#lsi = models.LsiModel(corpus_tfidf, id2word=dictionary, num_topics=3)
#lsi.print_topics(2)

#将文档映射到一个二维的topic空间中
#corpus_lsi = lsi[corpus_tfidf]
#for doc in corpus_lsi:
#	print doc

'''
#build LDA model
lda = models.LdaModel(corpus_tfidf, id2word=dictionary, num_topics=2)
lda.print_topics(2)'''


#建索引
index = similarities.MatrixSimilarity(tfidf[corpus])

#将Query其向量化
f = open(sys.argv[2],"r")

query = f.readline().strip()
query_bow = dictionary.doc2bow(query.split())
#print query_bow

#再用之前训练好的LSI模型将其映射到二维的topic空间
query_lsi = tfidf[query_bow]
#print query_lsi


#计算其和index中doc的余弦相似度
sims = index[query_lsi]
#print list(enumerate(sims))

#也可以按相似度进行排序
sort_sims = sorted(enumerate(sims), key=lambda item: -item[1])
print sort_sims

TOP_K = 100
for (d, sim) in sort_sims[0:TOP_K]:
	Name = Doc[d]
	print Name













