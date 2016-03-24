#! -*- encoding=utf-8 -*-
import gensim
import sys
 
model = gensim.models.Word2Vec.load(sys.argv[1])
 
result = model.most_similar(u"馬英九")
 
for e in result:
  print e[0], e[1]

 
print model.similarity(u"藍", u"死魚")
print model.similarity(u"馬英九", u"反對")

 
print model.similarity(u"女人", u"男人")

 
model.doesnt_match(u"馬英九 晚餐 午餐 中心".split())
print model.doesnt_match(u"馬英九 總統 diefishfish".split())