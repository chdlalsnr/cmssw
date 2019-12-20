import numpy as np
import matplotlib.pyplot as plt
from ROOT import TCanvas, TF1

f = open("GEMtestOutput.out", 'r')
d = f.read()
data = d.split()

re = np.zeros(len(data)/5)
st = np.zeros(len(data)/5)
gx = np.zeros(len(data)/5)
gy = np.zeros(len(data)/5)
deg = np.zeros(len(data)/5)

for i in range(len(data)/5):
    re[i] = data[5*i]
    st[i] = data[5*i+1]
    gx[i] = data[5*i+2]
    gy[i] = data[5*i+3]
    deg[i] = data[5*i+4]
    #print(re[i],st[i],gx[i],gy[i],deg[i])

x_r1s1t = []
y_r1s1t = []
x_r1s2t = []
y_r1s2t = []
x_r_1s1t = []
y_r_1s1t = []
x_r_1s2t = []
y_r_1s2t = []

x_r1s1f = []
y_r1s1f = []
x_r1s2f = []
y_r1s2f = []
x_r_1s1f = []
y_r_1s1f = []
x_r_1s2f = []
y_r_1s2f = []

df_r1s1 = []
df_r1s2 = []
df_r_1s1 = []
df_r_1s2 = []

r = 130.2
Rs = 236.3
Rl = 251.1
r2 = 136.55
R2 = 319.68
for i in range(len(data)/5):
    if (re[i] == -1):
        if (st[i] == 1):
            if ( (deg[i]>-5. and deg[i]<5.) | (deg[i]>15. and deg[i]<25.) | (deg[i]>35. and deg[i]<45.) | (deg[i]>55. and deg[i]<65.) | (deg[i]>75. and deg[i]<85.) | (deg[i]>95. and deg[i]<105.) | (deg[i]>115. and deg[i]<125.) | (deg[i]>135. and deg[i]<145.) | (deg[i]>155. and deg[i]<165.) | (deg[i]>175. and deg[i]<185.) | (deg[i]>195. and deg[i]<205.) | (deg[i]>215. and deg[i]<225.) | (deg[i]>235. and deg[i]<245.) | (deg[i]>255. and deg[i]<265.) | (deg[i]>275. and deg[i]<285.) | (deg[i]>295. and deg[i]<305.) | (deg[i]>315. and deg[i]<325.) | (deg[i]>335. and deg[i]<345.) ):
                if (((gx[i]**2+gy[i]**2)**.5 > r) & ((gx[i]**2+gy[i]**2)**.5 < Rs)):
                    x_r_1s1t.append(gx[i])
                    y_r_1s1t.append(gy[i])
                else:
                    x_r_1s1f.append(gx[i])
                    y_r_1s1f.append(gy[i])
                    df_r_1s1.append(deg[i])
            else:
                if (((gx[i]**2+gy[i]**2)**.5 > r) & ((gx[i]**2+gy[i]**2)**.5 < Rl)):
                    x_r_1s1t.append(gx[i])
                    y_r_1s1t.append(gy[i])
                else:
                    x_r_1s1f.append(gx[i])
                    y_r_1s1f.append(gy[i])
                    df_r_1s1.append(deg[i])
        elif (st[i] == 2):
            if (((gx[i]**2+gy[i]**2)**.5 > r2) & ((gx[i]**2+gy[i]**2)**.5 < R2)):
                x_r_1s2t.append(gx[i])
                y_r_1s2t.append(gy[i])
            else:
                x_r_1s2f.append(gx[i])
                y_r_1s2f.append(gy[i])
                df_r_1s2.append(deg[i])

    elif (re[i] == 1):
        if (st[i] == 1):
            if ( (deg[i]>-5. and deg[i]<5.) | (deg[i]>15. and deg[i]<25.) | (deg[i]>35. and deg[i]<45.) | (deg[i]>55. and deg[i]<65.) | (deg[i]>75. and deg[i]<85.) | (deg[i]>95. and deg[i]<105.) | (deg[i]>115. and deg[i]<125.) | (deg[i]>135. and deg[i]<145.) | (deg[i]>155. and deg[i]<165.) | (deg[i]>175. and deg[i]<185.) | (deg[i]>195. and deg[i]<205.) | (deg[i]>215. and deg[i]<225.) | (deg[i]>235. and deg[i]<245.) | (deg[i]>255. and deg[i]<265.) | (deg[i]>275. and deg[i]<285.) | (deg[i]>295. and deg[i]<305.) | (deg[i]>315. and deg[i]<325.) | (deg[i]>335. and deg[i]<345.) ):
                if (((gx[i]**2+gy[i]**2)**.5 > r) & ((gx[i]**2+gy[i]**2)**.5 < Rs)):
                    x_r1s1t.append(gx[i])
                    y_r1s1t.append(gy[i])
                else:
                    x_r1s1f.append(gx[i])
                    y_r1s1f.append(gy[i])
                    df_r1s1.append(deg[i])
            else:
                if (((gx[i]**2+gy[i]**2)**.5 > r) & ((gx[i]**2+gy[i]**2)**.5 < Rl)):
                    x_r1s1t.append(gx[i])
                    y_r1s1t.append(gy[i])
                else:
                    x_r1s1f.append(gx[i])
                    y_r1s1f.append(gy[i])
                    df_r1s1.append(deg[i])
        elif (st[i] == 2):
            if (((gx[i]**2+gy[i]**2)**.5 > r2) & ((gx[i]**2+gy[i]**2)**.5 < R2)):
                x_r1s2t.append(gx[i])
                y_r1s2t.append(gy[i])
            else:
                x_r1s2f.append(gx[i])
                y_r1s2f.append(gy[i])
                df_r1s2.append(deg[i])

for i in range(len(x_r_1s2f)):
    rad = (x_r_1s2f[i]**2 + y_r_1s2f[i]**2)**.5
    print(x_r_1s2f[i], y_r_1s2f[i],rad, df_r_1s2[i])

plt.grid(True)
plt.title('GEMRecHit Occupancy Distribution GE2/1 re-1',pad=10)
plt.xlabel('GlobalX',labelpad=10)
plt.ylabel('GlobalY',labelpad=10)
plt.scatter(x_r_1s2t,y_r_1s2t,s=0.5,c='g')
plt.scatter(x_r_1s2f,y_r_1s2f,s=0.5,c='r')
print(len(x_r_1s2f))

plt.show()
