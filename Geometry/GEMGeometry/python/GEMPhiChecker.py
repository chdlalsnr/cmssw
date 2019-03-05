import numpy as np
import matplotlib.pyplot as plt
#from ROOT import TCanvas, TF1

f = open("C:\\Users\chdlalsnr\Documents\Python Scripts\GEMtestOutput.out", 'r')
d = f.read()
data = d.split()

ge11_rm = np.zeros((36, 2, 8))
ge11_r = np.zeros((36, 2, 8))
ge21_rm = np.zeros((18, 2, 8))
ge21_r = np.zeros((18, 2, 8))

flag11_rm = np.ones((36, 2))
flag11_r = np.ones((36, 2))
flag21_rm = np.ones((18, 2))
flag21_r = np.ones((18, 2))

for i in range(36):
	for j in range(2):
		for k in range(8):
			ge11_rm[i][j][k] = data[i*16 + j*8 + k]
			ge11_r[i][j][k] = data[i*16 + j*8 + k + 72*8 + 36*8]
for i in range(36):
	for j in range(2):
		for k in range(8):
			if (ge11_r[i][j][k] == 360):
				ge11_r[i][j][k] = 0

for i in range(18):
	for j in range(2):
		for k in range(8):
			ge21_rm[i][j][k] = data[i*16 + j*8 + k + 72*8]
			ge21_r[i][j][k] = data[i*16 + j*8 + k + 72*8*2 + 36*8]

expected_phi1 = np.arange(0, 360, 10)
expected_phi2 = np.arange(5, 365, 20)
for i in range(36):
	for j in range(2):
		for k in range(8):
			if ((ge11_rm[i][j][k] > expected_phi1[i] + 0.001) | (ge11_rm[i][j][k] < expected_phi1[i] - 0.001)):
				flag11_rm[i][j] = 0
			if ((ge11_r[i][j][k] > expected_phi1[i] + 0.001) | (ge11_r[i][j][k] < expected_phi1[i] - 0.001)):
				flag11_r[i][j] = 0

for i in range(18):
	for j in range(2):
		for k in range(8):
			if ((ge21_rm[i][j][k] > expected_phi2[i] + 0.001) | (ge21_rm[i][j][k] < expected_phi2[i] - 0.001)):
				flag21_rm[i][j] = 0
			if ((ge21_r[i][j][k] > expected_phi2[i] + 0.001) | (ge21_r[i][j][k] < expected_phi2[i] - 0.001)):
				flag21_r[i][j] = 0
"""
y = np.arange(0, 1.1, 0.1)
for i in range(36):
	plt.axvline(x = expected_phi1[i], color = "k", linewidth = 0.2)
	if (flag11_rm[i][0] == 1):
		plt.fill_betweenx(y, expected_phi1[i]-1, expected_phi1[i]+1, alpha = 0.3, color="g", edgecolor="none")
	if (flag11_rm[i][0] == 0):
		plt.fill_betweenx(y, expected_phi1[i]-1, expected_phi1[i]+1, alpha = 0.3, color="r", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE1/1 Region-1 Chamber Alignment Status in $\phi$ direction')
plt.xlabel('$\phi$')
plt.ylabel('Goodness of Alignment')
plt.show()

y = np.arange(0, 1.1, 0.1)
for i in range(36):
	plt.axvline(x = expected_phi1[i], color = "k", linewidth = 0.2)
	if (flag11_r[i][1] == 1):
		plt.fill_betweenx(y, expected_phi1[i]-1, expected_phi1[i]+1, alpha = 0.3, color="g", edgecolor="none")
	if (flag11_r[i][1] == 0):
		plt.fill_betweenx(y, expected_phi1[i]-1, expected_phi1[i]+1, alpha = 0.3, color="r", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE1/1 Region1 Chamber Alignment Status in $\phi$ direction')
plt.xlabel('$\phi$')
plt.ylabel('Goodness of Alignment')
plt.show()

y = np.arange(0, 1.1, 0.1)
for i in range(18):
	plt.axvline(x = expected_phi2[i], color = "k", linewidth = 0.2)
	if (flag21_rm[i][0] == 1):
		plt.fill_betweenx(y, expected_phi2[i]-1, expected_phi2[i]+1, alpha = 0.3, color="g", edgecolor="none")
	if (flag21_rm[i][0] == 0):
		plt.fill_betweenx(y, expected_phi2[i]-1, expected_phi2[i]+1, alpha = 0.3, color="r", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE2/1 Region-1 Chamber Alignment Status in $\phi$ direction')
plt.xlabel('$\phi$')
plt.ylabel('Goodness of Alignment')
plt.show()
"""
y = np.arange(0, 1.1, 0.1)
for i in range(18):
	plt.axvline(x = expected_phi2[i], color = "k", linewidth = 0.2)
	if (flag21_r[i][1] == 1):
		plt.fill_betweenx(y, expected_phi2[i]-1, expected_phi2[i]+1, alpha = 0.3, color="g", edgecolor="none")
	if (flag21_r[i][1] == 0):
		plt.fill_betweenx(y, expected_phi2[i]-1, expected_phi2[i]+1, alpha = 0.3, color="r", edgecolor="none")
plt.ylim(0, 1)
plt.title('GE2/1 Chamber Alignment Status in $\phi$ direction')
plt.xlabel('$\phi$')
plt.ylabel('Goodness of Alignment')
plt.show()

