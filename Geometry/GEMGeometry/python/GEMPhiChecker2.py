import numpy as np
import matplotlib.pyplot as plt
import time
starttime = time.time()

f = open("C:\\Users\chdlalsnr\Documents\Python Scripts\GEMtestOutput_noAbs.out", 'r')
d = f.read()
data = d.split()

ge11_rm = np.zeros((36, 2, 384))
ge11_r = np.zeros((36, 2, 384))
ge21_rm = np.zeros((18, 2, 384*2))
ge21_r = np.zeros((18, 2, 384*2))

flag11_rm = np.ones((36, 2, 384))
flag11_r = np.ones((36, 2, 384))
flag21_rm = np.ones((18, 2, 384*2))
flag21_r = np.ones((18, 2, 384*2))

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

expected_phi1 = np.arange(0.000, 360.0000, 10.0000)
expected_phi2 = np.arange(5.000, 365.0000, 20.0000)

expected_phi1rm = np.zeros((36, 384))
expected_phi1r = np.zeros((36, 384))
expected_phi2rm = np.zeros((18, 384*2))
expected_phi2r = np.zeros((18, 384*2))

ge11_phicov = 10.1207                                                                       # GE1/1 chamber stripPhi coverage: 10.1207 <- need to be checked
"""normalization_const = 0.000204/36672.25
def sum (strip_num):
    sum = 0.
    for i in range(strip_num):
        sum += -((strip_num-191.5)**2)*normalization_const + 0.02649355
    return sum
"""
for i in range (36):
    if (i%2 == 1):
        for j in range (384):
            expected_phi1rm[i][j] = expected_phi1[i] + ge11_phicov/2.000 - j*(ge11_phicov/383)
            expected_phi1r[i][j] = expected_phi1[i] - ge11_phicov/2.000 + j*(ge11_phicov/383)
    if (i%2 == 0):
        for j in range (384):
            expected_phi1rm[i][j] = expected_phi1[i] - ge11_phicov/2.000 + j*(ge11_phicov/383)
            expected_phi1r[i][j] = expected_phi1[i] + ge11_phicov/2.000 - j*(ge11_phicov/383)

for i in range(36):
    for j in range(2):
        for k in range(384):
            if ((ge11_rm[i][j][k] > expected_phi1rm[i][k] + 0.003) | (ge11_rm[i][j][k] < expected_phi1rm[i][k] - 0.003)):
                flag11_rm[i][j][k] = 0
            if ((ge11_r[i][j][k] > expected_phi1r[i][k] + 0.003) | (ge11_r[i][j][k] < expected_phi1r[i][k] - 0.003)):
                flag11_r[i][j][k] = 0

for i in range(36):
    for k in range(384):
        print(expected_phi1rm[i][k], ge11_rm[i][0][k])

y = np.arange(0, 1.1, 0.1)
for i in range(36):
    for j in range(2):
        for k in range(384):
            plt.axvline(x = expected_phi1[i], color = "k", alpha =0.5, linewidth = 0.1)
            if (flag11_rm[i][0][k] == 1):
                plt.fill_betweenx(y, expected_phi1rm[i][k]-ge11_phicov/384, expected_phi1rm[i][k]+ge11_phicov/384, alpha = 0.5, color="g", edgecolor="none")
            if (flag11_rm[i][0][k] == 0):
                plt.fill_betweenx(y, expected_phi1rm[i][k]-ge11_phicov/384, expected_phi1rm[i][k]+ge11_phicov/384, alpha = 0.5, color="r", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE1/1 Region-1 Layer 1 Chamber Alignment Status in $\phi$ direction')
plt.xlabel('$\phi$')
plt.ylabel('Goodness of Alignment')
plt.show()

runtime = time.time() - starttime
print(runtime)

"""for i in range (18):
    if (i%2 == 1):
        for j in range (384*2):
            expected_phi2rm[i][j] = expected_phi2[i] + 20.783/2.000 - j*20.783/(384.000*2.000)
            expected_phi2r[i][j] = expected_phi2[i] - 20.783/2.000 + j*20.783/(384.000*2.000)           # GE2/1 chamber stripPhi coverage: 20.783 <- need to be checked
    if (i%2 == 0):
        for j in range (384*2):
            expected_phi2rm[i][j] = expected_phi2[i] - 20.783/2.000 + j*20.783/(384.000*2.000)
            expected_phi2r[i][j] = expected_phi2[i] + 20.783/2.000 - j*20.783/(384.000*2.000)
            
for i in range(18):
    for j in range(2):
        for k in range(384*2):
            if ((ge21_rm[i][j][k] > expected_phi2rm[i][k] + 0.0264) | (ge21_rm[i][j][k] < expected_phi2rm[i][k] - 0.0264)):
                flag21_rm[i][j][k] = 0
            if ((ge21_r[i][j][k] > expected_phi2r[i][k] + 0.0264) | (ge21_r[i][j][k] < expected_phi2r[i][k] - 0.0264)):
                flag21_r[i][j][k] = 0"""