# coding=utf-8
from BeautifulSoup import BeautifulSoup
import urllib, urllib2
import re

url = "http://zh.wikisource.org/wiki/臺灣通史"

u = "臺灣通史/1 臺灣通史/2 臺灣通史/3"

data = urllib2.urlopen(url)
soup = BeautifulSoup(data)
T = u"臺灣通史/[\u4e00-\u9fa5]+[0-9]*"

N = u"臺灣通史/卷1"
a = re.match(T,N)
tf=urllib.URLopener()

prefix = u"http://zh.wikisource.org/wiki/"
for article in soup.findAll("a"):
	#print 'title' in article

	if article.has_key('title'):
		N = article['title']
		a = re.match(T,N)
		if a != None:
			print "Title: " + article['title']
			print "href: " + article['href']
			fn = u"./"+article['title']
			href = prefix+article['title']
			href = href.encode("utf-8")
			tf.retrieve(href, fn)

		
			
			

	#'(?<=<a class="aposted" href=")http://blog.roodo.com/lakatos/archives/\d+\.html(?=">)'