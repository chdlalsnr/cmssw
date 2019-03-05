import numpy as np
import matplotlib.pyplot as plt

f = open("C:\\Users\chdlalsnr\Documents\Python Scripts\GEMtestOutput_noAbs.out", 'r')
d = f.read()
data = d.split()

ge11_rm = np.zeros((36, 2, 384))
ge11_r = np.zeros((36, 2, 384))
ge21_rm = np.zeros((18, 2, 384*2))
ge21_r = np.zeros((18, 2, 384*2))

for i in range (36):
    for j in range (2):
        for k in range(384):
            ge11_rm[i][j][k] = data[i*2*384 + j*384 + k]
            ge11_r[i][j][k] = data[i*2*384 + j*384 + k + 36*2*384 + 18*2*384*2]
            if ( (i != 0) & (ge11_rm[i][j][k] < 0) ):
                ge11_rm[i][j][k] += 360.
            if ( (i != 0) & (ge11_r[i][j][k] < 0) ):
                ge11_r[i][j][k] += 360.

for i in range(18):
    for j in range(2):
        for k in range(384*2):
            ge21_rm[i][j][k] = data[i*2*384*2 + j*384*2 + k + 36*2*384]
            ge21_r[i][j][k] = data[i * 2 * 384 + j * 384 + k + 36 * 2 * 384 + 18 * 2 * 384 * 2]
            if ((i != 0) & (ge21_rm[i][j][k] < 0)):
                ge21_rm[i][j][k] += 360.
            if ((i != 0) & (ge21_r[i][j][k] < 0)):
                ge21_r[i][j][k] += 360.

strip_num = np.arange(1, 384, 1)
residual_num = np.arange(1, 193, 1)
stripCover11_rm = np.zeros((36, 2, 383))
stripCover11_r = np.zeros((36, 2, 383))
for i in range (36):
    for j in range (2):
        for k in range (383):
            stripCover11_rm[i][j][k] = ge11_rm[i][j][k+1] - ge11_rm[i][j][k]
            stripCover11_r[i][j][k] = ge11_r[i][j][k+1] - ge11_r[i][j][k]

#print((stripCover11_rm[0][0][191] + stripCover11_rm[0][0][192])/2)
#print(np.max(stripCover11_rm[0][0][0]))
plt.plot(strip_num, stripCover11_rm[0][0], c='k')
plt.title('GE1/1 Region-1 Layer 1 StripPhi Coverage Distribution')
plt.xlabel('Strip Number')
plt.ylabel('StripPhi Coverage')
plt.show()

"""residual11_rm = np.zeros((36, 2, 192))
residual11_r = np.zeros((36, 2, 192))
for i in range(36):
    for j in range (2):
        for k in range (192):
            residual11_rm[i][j][k] = stripCover11_rm[i][j][k] - stripCover11_rm[i][j][382 - k]
            residual11_r[i][j][k] = stripCover11_r[i][j][k] - stripCover11_r[i][j][382 - k]

for i in range (192):
    print(residual11_rm[0][0][i])

plt.plot(residual_num, residual11_rm[0][0], c = 'k')
plt.ylim(-0.0000000001, 0.0000000001)
plt.show()"""
