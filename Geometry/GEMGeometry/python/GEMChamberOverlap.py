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

expected_phi1 = np.arange(0.000, 360.000, 10.000)
expected_phi2 = np.arange(5.000, 365.000, 20.000)

overlap_11rm = np.zeros(36)
overlap_11r = np.zeros(36)
overlap_21rm = np.zeros(18)
overlap_21r = np.zeros(18)

for i in range(35):
    if (i%2 == 0):
        overlap_11rm[i] = abs(ge11_rm[i][0][383] - ge11_rm[i+1][0][383])
        overlap_11r[i] = abs(ge11_rm[i][0][383] - ge11_rm[i + 1][0][383])
    if (i%2 == 1):
        overlap_11rm[i] = abs(ge11_rm[i][0][0] - ge11_rm[i+1][0][0])
        overlap_11r[i] = abs(ge11_rm[i][0][0] - ge11_rm[i + 1][0][0])
overlap_11rm[35] = abs(ge11_rm[35][0][0] - ge11_rm[0][0][0]) - 360.
overlap_11r[35] = abs(ge11_r[35][0][383] - ge11_r[0][0][383]) - 360.

for i in range(17):
    if (i%2 == 0):
        overlap_21rm[i] = abs(ge21_rm[i][0][383*2-1] - ge21_rm[i+1][0][383*2-1])
        overlap_21r[i] = abs(ge21_r[i+1][0][0] - ge21_r[i][0][0])
    if (i%2 == 1):
        overlap_21rm[i] = abs(ge21_rm[i][0][0] - ge21_rm[i+1][0][0])
        overlap_21r[i] = abs(ge21_r[i + 1][0][383*2-1] - ge21_r[i][0][383*2-1])
overlap_21rm[17] = abs(ge21_rm[17][0][0] - ge21_rm[0][0][0]) - 360.
overlap_21r[17] = abs(ge21_r[17][0][383*2-1] - ge21_r[0][0][383*2-1]) - 180.

"""chamberNum_1rm = np.arange(0, 36, 1)
chamberNum_1r = np.arange(0, 36, 1)
chamberNum_2rm = np.arange(0, 18, 1)
chamberNum_2r = np.arange(0, 18, 1)

plt.plot(chamberNum_1rm, overlap_11rm, c='k')
plt.ylim(0.1195, 0.122)
plt.title('GE1/1 Region-1 Chamber Overlapped Portion Phi Coverage')
plt.xlabel('Chamber Number')
plt.ylabel('Phi Coverage')
plt.show()

plt.plot(chamberNum_1r, overlap_11r, c='k')
plt.ylim(0.1195, 0.122)
plt.title('GE1/1 Region1 Chamber Overlapped Portion Phi Coverage')
plt.xlabel('Chamber Number')
plt.ylabel('Phi Coverage')
plt.show()

plt.plot(chamberNum_2rm, overlap_21rm, c='k')
plt.ylim(0.0, 0.20)
plt.title('GE2/1 Region-1 Chamber Overlapped Portion Phi Coverage')
plt.xlabel('Chamber Number')
plt.ylabel('Phi Coverage')
plt.show()

plt.plot(chamberNum_2r, overlap_21r, c='k')
plt.ylim(0.0, 0.20)
plt.title('GE2/1 Region1 Chamber Overlapped Portion Phi Coverage')
plt.xlabel('Chamber Number')
plt.ylabel('Phi Coverage')
plt.show()"""

"""y = np.arange(0, 1.1, 0.1)
plt.axvline(x = expected_phi1[i], color = "k", alpha =0.5, linewidth = 0.1)
for i in range(35):
    if (i%2 == 0):
        if((overlap_11rm[i] > 0.1205 - .001) & (overlap_11rm[i] < 0.1205 + .001)):
            plt.fill_betweenx(y, ge11_rm[i+1][0][383], ge11_rm[i][0][383], alpha = 0.5, color="b", edgecolor="none")
        else:
            plt.fill_betweenx(y, ge11_rm[i + 1][0][383], ge11_rm[i][0][383], alpha=0.5, color="r", edgecolor="none")
    if (i%2 == 1):
        if((overlap_11rm[i] > 0.1205 - .001) & (overlap_11rm[i] < 0.1205 + .001)):
            plt.fill_betweenx(y, ge11_rm[i+1][0][0], ge11_rm[i][0][0], alpha = 0.5, color="b", edgecolor="none")
        else:
            plt.fill_betweenx(y, ge11_rm[i + 1][0][0], ge11_rm[i][0][0], alpha=0.5, color="r", edgecolor="none")
if ((overlap_11rm[35] > 0.1205 - .001) & (overlap_11rm[35] < 0.1205 + .001)):
    plt.fill_betweenx(y, ge11_rm[35][0][0], ge11_rm[0][0][0] + 360., alpha = 0.5, color="b", edgecolor="none")
else: plt.fill_betweenx(y, ge11_rm[35][0][0], ge11_rm[0][0][0] + 360., alpha = 0.5, color="r", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE1/1 Region-1 Chamber Overlapped $\phi$ Portion')
plt.xlabel('$\phi$')
plt.ylabel('Overlapped Area')
plt.show()"""

y = np.arange(0, 1.1, 0.1)
#plt.axvline(x = expected_phi1[i], color = "k", alpha =0.5, linewidth = 0.1)
for i in range(35):
    if (i%2 == 0):
        if((overlap_11r[i] > 0.1205 - .001) & (overlap_11r[i] < 0.1205 + .001)):
            plt.fill_betweenx(y, ge11_r[i+1][0][0], ge11_r[i][0][0], alpha = 0.5, color="b", edgecolor="none")
        else:
            plt.fill_betweenx(y, ge11_r[i + 1][0][0], ge11_r[i][0][0], alpha=0.5, color="r", edgecolor="none")
    if (i%2 == 1):
        if ((overlap_11r[i] > 0.1205 - .001) & (overlap_11r[i] < 0.1205 + .001)):
            plt.fill_betweenx(y, ge11_r[i + 1][0][383], ge11_r[i][0][383], alpha=0.5, color="b", edgecolor="none")
        else:
            plt.fill_betweenx(y, ge11_r[i + 1][0][383], ge11_r[i][0][383], alpha=0.5, color="r", edgecolor="none")
if ((overlap_11r[35] > 0.1205 - .001) & (overlap_11r[35] < 0.1205 + .001)):
    plt.fill_betweenx(y, ge11_r[35][0][383], ge11_r[0][0][383] + 360., alpha = 0.5, color="b", edgecolor="none")
else: plt.fill_betweenx(y, ge11_r[35][0][383], ge11_r[0][0][383] + 360., alpha = 0.5, color="r", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE1/1 Region1 Chamber Overlapped $\phi$ Portion')
plt.xlabel('$\phi$')
plt.ylabel('Overlapped Area')
plt.show()

"""y = np.arange(0, 1.1, 0.1)
#plt.axvline(x = expected_phi1[i], color = "k", alpha =0.5, linewidth = 0.1)
for i in range(17):
    if (i%2 == 0):
        plt.fill_betweenx(y, ge21fl_rm[i+1][1]-(overlap_21rm[i]-overlap_21rm[i+1])*10, ge21fl_rm[i][1]+(overlap_21rm[i]-overlap_21rm[i+1])*10, alpha = 0.5, color="b", edgecolor="none")
    if (i%2 == 1):
        plt.fill_betweenx(y, ge21fl_rm[i + 1][0], ge21fl_rm[i][0], alpha=0.5, color="b", edgecolor="none")
plt.fill_betweenx(y, ge21fl_rm[0][0], ge21fl_rm[17][0], alpha = 0.5, color="b", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE2/1 Region-1 Chamber Overlapped $\phi$ Portion_exaggerated')
plt.xlabel('$\phi$')
plt.ylabel('Exaggerated Overlapped Area')
plt.show()"""

"""y = np.arange(0, 1.1, 0.1)
#plt.axvline(x = expected_phi1[i], color = "k", alpha =0.5, linewidth = 0.1)
for i in range(17):
    if (i%2 == 0):
        plt.fill_betweenx(y, ge21fl_r[i+1][0]-abs(overlap_21r[i]-overlap_21r[i+1])*10, ge21fl_r[i][0]+abs(overlap_21r[i]-overlap_21r[i+1])*10, alpha = 0.5, color="b", edgecolor="none")
    if (i%2 == 1):
        plt.fill_betweenx(y, ge21fl_r[i + 1][1], ge21fl_r[i][1], alpha=0.5, color="b", edgecolor="none")
plt.fill_betweenx(y, ge21fl_r[17][1], ge21fl_r[0][1], alpha = 0.5, color="b", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE2/1 Region1 Chamber Overlapped $\phi$ Portion_exaggerated')
plt.xlabel('$\phi$')
plt.ylabel('Exaggerated Overlapped Area')
plt.show()"""