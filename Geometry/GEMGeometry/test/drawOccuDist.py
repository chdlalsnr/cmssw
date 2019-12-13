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
    print(re[i],st[i],gx[i],gy[i],deg[i])

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

for i in range(len(data)/5):
    if (re[i] == -1):
        if (st[i] == 1):
            if ( (deg[i]>-5. and deg[i]<5.) | (deg[i]>25. and deg[i]<35.) | (deg[i]>45. and deg[i]<55.) | (deg[i]>65. and deg[i]<75.) | (deg[i]>85. and deg[i]<95.) | (deg[i]>105. and deg[i]<115.) | (deg[i]>125. and deg[i]<135.) | (deg[i]>145. and deg[i]<155.) | (deg[i]>165. and deg[i]<175.) | (deg[i]>185. and deg[i]<195.) | (deg[i]>205. and deg[i]<215.) | (deg[i]>225. and deg[i]<235.) | (deg[i]>245. and deg[i]<255.) | (deg[i]>265. and deg[i]<275.) | (deg[i]>285. and deg[i]<295.) | (deg[i]>305. and deg[i]<315.) | (deg[i]>325. and deg[i]<335.) | (deg[i]>345. and deg[i]<355.) ):
                if (((gx[i]**2+gy[i]**2)**.5 > 130.2) & ((gx[i]**2+gy[i]**2)**.5 < 236.3)):
                    x_r_1s1t.append(gx[i])
                    y_r_1s1t.append(gy[i])
                else:
                    x_r_1s1f.append(gx[i])
                    y_r_1s1f.append(gy[i])
            else:
                if (((gx[i]**2+gy[i]**2)**.5 > 130.2) & ((gx[i]**2+gy[i]**2)**.5 < 251.1)):
                    x_r_1s1t.append(gx[i])
                    y_r_1s1t.append(gy[i])
                else:
                    x_r_1s1f.append(gx[i])
                    y_r_1s1f.append(gy[i])
        elif (st[i] == 2):
            if (((gx[i]**2+gy[i]**2)**.5 > 136.55) & ((gx[i]**2+gy[i]**2)**.5 < 319.68)):
                x_r_1s2t.append(gx[i])
                y_r_1s2t.append(gy[i])
            else:
                x_r_1s2f.append(gx[i])
                y_r_1s2f.append(gy[i])

    elif (re[i] == 1):
        if (st[i] == 1):
            if ( (deg[i]>-5. and deg[i]<5.) | (deg[i]>25. and deg[i]<35.) | (deg[i]>45. and deg[i]<55.) | (deg[i]>65. and deg[i]<75.) | (deg[i]>85. and deg[i]<95.) | (deg[i]>105. and deg[i]<115.) | (deg[i]>125. and deg[i]<135.) | (deg[i]>145. and deg[i]<155.) | (deg[i]>165. and deg[i]<175.) | (deg[i]>185. and deg[i]<195.) | (deg[i]>205. and deg[i]<215.) | (deg[i]>225. and deg[i]<235.) | (deg[i]>245. and deg[i]<255.) | (deg[i]>265. and deg[i]<275.) | (deg[i]>285. and deg[i]<295.) | (deg[i]>305. and deg[i]<315.) | (deg[i]>325. and deg[i]<335.) | (deg[i]>345. and deg[i]<355.) ):
                if (((gx[i]**2+gy[i]**2)**.5 > 130.2) & ((gx[i]**2+gy[i]**2)**.5 < 236.3)):
                    x_r1s1t.append(gx[i])
                    y_r1s1t.append(gy[i])
                else:
                    x_r1s1f.append(gx[i])
                    y_r1s1f.append(gy[i])
            else:
                if (((gx[i]**2+gy[i]**2)**.5 > 130.2) & ((gx[i]**2+gy[i]**2)**.5 < 251.1)):
                    x_r1s1t.append(gx[i])
                    y_r1s1t.append(gy[i])
                else:
                    x_r1s1f.append(gx[i])
                    y_r1s1f.append(gy[i])
        elif (st[i] == 2):
            if (((gx[i]**2+gy[i]**2)**.5 > 136.55) & ((gx[i]**2+gy[i]**2)**.5 < 319.68)):
                x_r1s2t.append(gx[i])
                y_r1s2t.append(gy[i])
            else:
                x_r1s2f.append(gx[i])
                y_r1s2f.append(gy[i])

plt.grid(True)
plt.title('GEMRecHit Occupancy Distribution GE2/1 re1',pad=10)
plt.xlabel('GlobalX',labelpad=10)
plt.ylabel('GlobalY',labelpad=10)
plt.scatter(x_r1s2t,y_r1s2t,s=0.5,c='g')
plt.scatter(x_r1s2f,y_r1s2f,s=0.5,c='r')
print(len(x_r1s2t),len(y_r1s2f))

plt.show()
