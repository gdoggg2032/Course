# coding=utf-8
from BeautifulSoup import BeautifulSoup
import urllib, urllib2
import re
import random #匯入需要的模組
import os

url = "http://www.diyifanwen.com/sicijianshang/index.htm"
#encode 
ENCODE = "windows-1252"



data = urllib2.urlopen(url)
soup = BeautifulSoup(data)
print soup.originalEncoding
#T = u"臺灣通史/[\u4e00-\u9fa5]+[0-9]*"

#N = u"臺灣通史/卷1"
#a = re.match(T,N)
tf=urllib.URLopener()
pf = u"./Ture/"
if not os.path.exists(pf):
	os.makedirs(pf)
#prefix = u"http://zh.wikisource.org/wiki/"
prefix = u"http://www.diyifanwen.com/"
for article in soup.findAll("dl",{'class':'IndexDl'}):
	#print 'title' in article
	#print article
	for daticle in article.findAll("dd"):
		#print daticle
		T = "Title: " 
		S = daticle.a.contents[0]
		print S.encode(ENCODE)
		#print T		
		h = pf+S
		if not os.path.exists(h.encode("utf-8")): #在這做IF查詢, 確定沒有同名的檔案再建立
			os.makedirs(h.encode("utf-8")) #利用os.makedirs()產生資料夾
		#print T
		print "href: " + daticle.a['href']
		href = prefix+daticle.a['href']
		fn = (pf+S+u'/'+S+'.html').encode("utf-8")
		tf.retrieve(href, fn)

		#tf.retrieve(prefix+daticle.a['href'],pf+S+'/'+S)

	'''print article.dd
	print article.dd.dd
	if article.has_key('dd'):
		for dd in article['dd']:
			print dd'''
	'''if article.has_key('title'):
		N = article['title']
		a = re.match(T,N)
		if a != None:
			print "Title: " + article['title']
			print "href: " + article['href']
			fn = u"./"+article['title']
			href = prefix+article['title']
			href = href.encode("utf-8")
			tf.retrieve(href, fn)
'''
		
			
			

	#'(?<=<a class="aposted" href=")http://blog.roodo.com/lakatos/archives/\d+\.html(?=">)'