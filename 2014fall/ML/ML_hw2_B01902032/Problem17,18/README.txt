main:
在5000次裡，每次都產生一組隨機的x在-1~1之間，然後用bubble sort將x從小排到大。
對於每個x，丟一個骰子，有20%的機率翻轉y。

然後印出來看看有沒有真的有y被翻轉。

設Ein=20，因為最多也就只可能有20個錯(size=20)，然後先從s=1開始，
theta取每個間隔，若在邊界就加上或減去一個小數字讓他在全部的外面，
若是在兩個x之間，就取兩x的中點，保證在兩x之間。

每次取完theta，就跑一次算算看有幾個錯誤，算法是看stmp*(x[j]-thetatmp)跟y[j]有沒有異號(<0)
	選最小錯誤的那個當作真正的s和theta，再用那個theta,s去算Eout。

	算出每輪的Ein、Eout後，更新aveEin、aveEout，印出Ein,Eout看看。

	重複5000次，再將平均印出。
