main:
先將training data和test data都讀入，x[i]的前九項是xi的九維向量，x[i][9]是xi的有雜訊的y。
對於每個維度，首先要針對那個維度去排序。然後取theta、s=1,s=-1等，用Ein記錄目前最小的Ein，
用一個stack記錄目前最小的Ein發生在哪些s,theta,維度上，若有同樣是最小Ein，加入stack，
若有比目前的Ein還小的，將stack清空，並加入這個。

跑完全部的x以及全部的維度後，得到一個stack裡面是有最小Ein的s,theta,維度，亂數抽一個當作要用的s,theta,維度。

接著跑test data看看有幾個錯誤，當成Eout。
