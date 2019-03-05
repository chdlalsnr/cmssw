import numpy as np
import matplotlib.pyplot as plt

f = open("C:\\Users\chdlalsnr\Documents\Python Scripts\GEMtestOutput_mcphi.out", 'r')
d = f.read()
data = d.split()

ge11rm = np.zeros((36, 2, 3))
for i in range(36):
    for j in range(2):
        for k in range(3):
            ge11rm[i][j][k] = data[i*2*3 + j*3 + k]
            if (ge11rm[i][j][k] < -0.1): ge11rm[i][j][k] += 360.

ge11rm_192 = np.zeros((36, 2))
ge11rm_193 = np.zeros((36, 2))
cphi_rm = np.zeros((36, 2))
for i in range(36):
    for j in range(2):
        cphi_rm[i][j] = ge11rm[i][j][0]
        ge11rm_192[i][j] = ge11rm[i][j][1]
        ge11rm_193[i][j] = ge11rm[i][j][2]
        if (abs(cphi_rm[i][j]) < 10E-5): cphi_rm[i][j] = 0.

ge11r = np.zeros((36, 2, 3))
for i in range(36):
    for j in range(2):
        for k in range(3):
            ge11r[i][j][k] = data[i*2*3 + j*3 + k + 36*2*3 + 18*2*3]
            if (ge11r[i][j][k] < -0.1): ge11r[i][j][k] += 360.

ge11r_192 = np.zeros((36, 2))
ge11r_193 = np.zeros((36, 2))
cphi_r = np.zeros((36, 2))
for i in range(36):
    for j in range(2):
        cphi_r[i][j] = ge11r[i][j][0]
        ge11r_192[i][j] = ge11r[i][j][1]
        ge11r_193[i][j] = ge11r[i][j][2]
        if (abs(cphi_r[i][j]) < 10E-5): cphi_r[i][j] = 0.

mphi_rm = np.zeros((36, 2))
for i in range(36):
    for j in range(2):
        mphi_rm[i][j] = (ge11rm_192[i][j] + ge11rm_193[i][j])/2.000

mphi_r = np.zeros((36, 2))
for i in range(36):
    for j in range(2):
        mphi_r[i][j] = (ge11r_192[i][j] + ge11r_193[i][j])/2.000

expected_phi1 = np.arange(0.000, 360.000, 10.000)
expected_phi2 = np.arange(5.000, 365.000, 20.000)

for i in range (36):
    print(cphi_r[i][0], ge11r_192[i][0], ge11r_193[i][0])

y = np.arange(0, 1.1, 0.1)
for i in range (36):
    #plt.axvline(x=expected_phi1[i], color="k", alpha=0.5, linewidth=0.1)
    if ((cphi_rm[i][0] > expected_phi1[i] - 0.001) & (cphi_rm[i][0] < expected_phi1[i] + 0.001) & (mphi_rm[i][0] > expected_phi1[i] - 0.001) & (mphi_rm[i][0] < expected_phi1[i] + 0.001)):
        plt.fill_betweenx(y, expected_phi1[i] - 1., expected_phi1[i] + 1., color="b", edgecolor="none")
    else:
        plt.fill_betweenx(y, expected_phi1[i] - 1., expected_phi1[i] + 1., color="r", edgecolor="none")
    plt.ylim(0, 1)
    plt.title('GE1/1 Region-1 Cphi Alignment Status in $\phi$ direction')
    plt.xlabel('$\phi$')
    plt.ylabel('Goodness of Alignment')
for i in range(35):
    if ((mphi_rm[i+1][0] - mphi_rm[i][0] > 10.- 0.001) & (mphi_rm[i+1][0] - mphi_rm[i][0] < 10.+ 0.001)):
        plt.fill_betweenx(y, expected_phi1[i]+.1, expected_phi1[i+1]-.1, alpha = 0.5, color="b", edgecolor="none")
    else:
        plt.fill_betweenx(y, expected_phi1[i]+.1, expected_phi1[i + 1]-.1, alpha=0.5, color="r", edgecolor="none")
if ((mphi_rm[0][0] + 360. - mphi_rm[35][0] > 10. - 0.001) & (mphi_rm[0][0] + 360. - mphi_rm[35][0] < 10. + 0.001)):
    plt.fill_betweenx(y, expected_phi1[35]+.1, expected_phi1[35]+5., alpha = 0.5, color="b", edgecolor="none")
    plt.fill_betweenx(y, expected_phi1[0]-5., expected_phi1[0]-.1, alpha = 0.5, color="b", edgecolor="none")
else:
    plt.fill_betweenx(y, expected_phi1[35] + .1, expected_phi1[35] + 5., alpha=0.5, color="r", edgecolor="none")
    plt.fill_betweenx(y, expected_phi1[0] - 5., expected_phi1[0] - .1, alpha=0.5, color="r", edgecolor="none")
plt.show()

y = np.arange(0, 1.1, 0.1)
for i in range (36):
    #plt.axvline(x=expected_phi1[i], color="k", alpha=0.5, linewidth=0.1)
    if ((cphi_r[i][0] > expected_phi1[i] - 0.001) & (cphi_r[i][0] < expected_phi1[i] + 0.001) & (mphi_r[i][0] > expected_phi1[i] - 0.001) & (mphi_r[i][0] < expected_phi1[i] + 0.001)):
        plt.fill_betweenx(y, expected_phi1[i] - 1., expected_phi1[i] + 1., color="b", edgecolor="none")
    else:
        plt.fill_betweenx(y, expected_phi1[i] - 1., expected_phi1[i] + 1., color="r", edgecolor="none")
    plt.ylim(0, 1)
    plt.title('GE1/1 Region1 Cphi Alignment Status in $\phi$ direction')
    plt.xlabel('$\phi$')
    plt.ylabel('Goodness of Alignment')
for i in range(35):
    if ((mphi_r[i+1][0] - mphi_r[i][0] > 10.- 0.001) & (mphi_r[i+1][0] - mphi_r[i][0] < 10.+ 0.001)):
        plt.fill_betweenx(y, expected_phi1[i]+.1, expected_phi1[i+1]-.1, alpha = 0.5, color="b", edgecolor="none")
    else:
        plt.fill_betweenx(y, expected_phi1[i]+.1, expected_phi1[i + 1]-.1, alpha=0.5, color="r", edgecolor="none")
if ((abs(mphi_r[0][0] + 360. - mphi_r[35][0]) > 10. - 0.001) & (abs(mphi_r[0][0] + 360. - mphi_r[35][0]) < 10. + 0.001)):
    plt.fill_betweenx(y, expected_phi1[35]+.1, expected_phi1[35]+5., alpha = 0.5, color="b", edgecolor="none")
    plt.fill_betweenx(y, expected_phi1[0]-5., expected_phi1[0]-.1, alpha = 0.5, color="b", edgecolor="none")
else:
    plt.fill_betweenx(y, expected_phi1[35] + .1, expected_phi1[35] + 5., alpha=0.5, color="r", edgecolor="none")
    plt.fill_betweenx(y, expected_phi1[0] - 5., expected_phi1[0] - .1, alpha=0.5, color="r", edgecolor="none")
plt.show()