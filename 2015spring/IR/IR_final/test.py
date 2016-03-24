import telnetlib
import sys
import re
import time
class User:
	def __init__(self, account, password):
		self.account = account
		self.password = password


def __processReadLine(self, line_p):
    '''
    remove non-printable characters from line <line_p>
    return a printable string.
    '''

    line, i, imax = '', 0, len(line_p)
    while i < imax:
        ac = ord(line_p[i])
        if (32<=ac<127) or ac in (9,10): # printable, \t, \n
            line += line_p[i]
        elif ac == 27:                   # remove coded sequences
            i += 1
            while i<imax and line_p[i].lower() not in 'abcdhsujkm':
                i += 1
        elif ac == 8 or (ac==13 and line and line[-1] == ' '): # backspace or EOL spacing
            if line:
                line = line[:-1]
        i += 1

    return line

u = User(sys.argv[1],sys.argv[2])
tn = telnetlib.Telnet('ptt.cc')
time.sleep(1)
page = tn.read_very_eager().decode('big5','ignore')
print("首頁顯示...")
if "請輸入代號" in page:
	print("輸入帳號...")
	tn.write((u.account+"\r\n").encode('big5') )
	time.sleep(1)
	page = tn.read_very_eager().decode('big5','ignore')

	print("輸入密碼...")
	tn.write((u.password+"\r\n").encode('big5'))
	time.sleep(2)
	page = tn.read_very_eager().decode('big5','ignore')

	if "密碼不對" in page:
		print("密碼不對或無此帳號。程式結束")
		sys.exit()
	if "您想刪除其他重複登入的連線嗎" in page:
		print("發現重複連線,不刪除...")
		tn.write("n\r\n".encode('big5') )
		time.sleep(7)
		page = tn.read_very_eager().decode('big5','ignore')
	print(page)
	while "任意鍵" in page:
		print("資訊頁面，按任意鍵繼續...")
		tn.write("\r\n".encode('big5') )
		time.sleep(2)
		page = tn.read_very_eager().decode('big5','ignore')

	if "要刪除以上錯誤嘗試" in page:
		print("發現嘗試登入卻失敗資訊，是否刪除?(Y/N)：",end= "")
		anser = input("")
		tn.write((anser+"\r\n").encode('big5') )
		time.sleep(1)
		page = tn.read_very_eager().decode('big5','ignore')
	print("----------------------------------------------")
	print("----------- 登入完成，顯示操作介面--------------")
	print("----------------------------------------------")
	print(page)
	time.sleep(1)
	tn.write(("sgossiping\r\n").encode('big5'))
	time.sleep(2)
	
	page = tn.read_very_eager().decode('big5','ignore')
	print("進入八卦版")
	print(page)
	time.sleep(3)
	while "任意鍵" in page:
		print("資訊頁面，按任意鍵繼續...")
		tn.write("\r\n".encode('big5') )
		time.sleep(2)
		page = tn.read_very_eager().decode('big5','ignore')
	# Arrow Down \u001b[B    Arrow Left  \u001b[D  Arrow Right  \u001b[C  Arrow Up  \u001b[A
	default = 100000


	t = open("test.txt","w")
	#lines = page.split(';2H')
	#lines = re.split('\x1b[[0-9;]*[mABCDHJKsu]', page)
	N = 155080 	
	tn.write((str(N)+"\r\n\x0c").encode('big5') )
	time.sleep(1)
	article = tn.read_very_eager().decode('big5','ignore')
	print(article)
	front = article
	for i in range(N,155000,-1):
		f = open(str(i)+".txt","w")
		#articletmp = tn.read_very_eager().decode('big5','ignore')
		
		topics = front.split('\n')
		title =""
		detetitle = 0
		for line in topics:
			if u'\u25CF' in line:
				#print(u'\u25CF')
				if line.find(str(i+1)) >= 0:
					title = line.split(str(i+1))[0]
				else:
					title = line.split(str(i+1)[1:])[0]

				t.write('\n>>>>[title:'+str(i)+'!!'+title+'\n')
				if'(本文已被刪除) ' in title:
					detetitle = 1
				break
			#t.write('>>'+line+'\n')
		if detetitle == 1:
			f.write('deleted.\n')
			tn.write("\x1b[A\x0c".encode("big5"))
			time.sleep(1)
			article = tn.read_very_eager().decode('big5','ignore')
			front = article
			print(article)
			time.sleep(3)
			continue


		tn.write("\x1b[C\x0c".encode("big5"))#<ESC>[{COUNT}D
		time.sleep(1)
		article = tn.read_very_eager().decode('big5','ignore')
		
		print(article)
		
		time.sleep(3)
		while '頁 (100%)' not in article:
			tn.write("\x1b[C\x0c".encode("big5"))#<ESC>[{COUNT}D
			time.sleep(1)
			prev = article
			article = tn.read_very_eager().decode('big5','ignore')
			for l  in range(0,len(prev)):
				s = prev[len(prev)-l:len(prev)]
				if article.startswith(s) == False:
					article = prev+article[l+1:]
					break
			
		f.write(article)
		time.sleep(3)
		print(article)
		tn.write("\x1b[D\x0c".encode("big5"))
		time.sleep(1)
		article = tn.read_very_eager().decode('big5','ignore')
		print(article)
		time.sleep(3)
		tn.write("\x1b[A\x0c".encode("big5"))
		time.sleep(1)
		article = tn.read_very_eager().decode('big5','ignore')
		front = article
		print(article)
		time.sleep(3)
		
		

		'''for line in lines:
		line = re.sub('\x1b[[0-9;]*[mABCDHJKsu]',"",line)
		if line[1:7] == '   ★  ':
			No = line[1:7]
		else:
			No = int(line[1:7])
			if No == 99999:
				default = 0
			if No < 100000:
				No += default
			No = str(No)
		f.write('['+No+']')
		

		#line = line.split(' ',1)[1]
		f.write('>>'+line+'\n')'''
		'''
		if '+' in line:
			tn.write("\u001b[C\r\n".encode('big5'))
			print(line)
			f = open(line, "w")
			time.sleep(1)
			article = tn.read_very_eager().decode('big5','ignore')
			f.write(article)
			while '頁 (100%)' not in article:
				tn.write("\u001b[C\r\n".encode('big5'))
				time.sleep(1)
				article = tn.read_very_eager().decode('big5','ignore')
				f.write(article)
			f.close()
			time.sleep(1)
			tn.write("\u001b[D\r\n".encode('big5'))
			time.sleep(1)
			tn.write("\u001b[A\r\n".encode('big5'))
			time.sleep(1)
			'''


	time.sleep(10)







	print("\n\n\n\n\n\n\n")
	print("----------------------------------------------")
	print("------------------- 登出----------------------")
	print("----------------------------------------------")
	tn.write("qqqqqqqqqg\r\ny\r\n".encode('big5') )
	time.sleep(1)
	page = tn.read_very_eager().decode('big5','ignore')
	print(page)
	tn.write("\r\n".encode('big5') )

else:
	print("沒有可輸入帳號的欄位，網站可能掛了")
