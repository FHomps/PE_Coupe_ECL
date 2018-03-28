import numpy as np
import matplotlib.pyplot as plt

encodes = [0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 726, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 799, 799, 799, 799, 799, 799, 799, 799, 799, 799, 799, 799, 799, 799, 799, 799, 835, 835, 835, 835, 835, 835, 835, 835, 835, 835, 835, 835, 835, 835, 835, 835, 871, 871, 871, 871, 871, 871, 871, 871, 871, 871, 871, 871, 871, 871, 871, 871, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 908, 964, 964, 964, 964, 964, 964, 964, 964, 964, 964, 964, 964, 964, 964, 964, 964, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1073, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1110, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1146, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1184, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1220, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1257, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1294, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1386, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1460, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1498, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1535, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1572, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1612, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1649, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1686, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1724, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1760, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1816, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1854, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893, 1893]

i = 0
N = len(encodes)
speeds = [0]*N

while i < N:
    k = i
    while k < N and encodes[i] == encodes[k]:
        k += 1

    if k == N:
        speeds[i:] = [speeds[i-1]] * (k-i+1)
    else:
        speed = (encodes[k] - encodes[i]) / (k-i+1)
        speeds[i:k] = [speed] * (k-i+1)
    i = k

speeds_raw = speeds[:]

middlePoints = [0]
i = 0
while i < N:
    k = i
    while k < N and speeds[k] == speeds[i]:
        k += 1
    middlePoints.append(int((k+i) / 2))
    i = k
middlePoints.append(N-1)

currentMiddlePoint = 0
for i in range(1, N):
    if i > middlePoints[currentMiddlePoint]:
        currentMiddlePoint += 1
    speeds[i] = speeds[middlePoints[currentMiddlePoint-1]] * (middlePoints[currentMiddlePoint] - i) / (middlePoints[currentMiddlePoint] - middlePoints[currentMiddlePoint-1]) + speeds[middlePoints[currentMiddlePoint]] * (i - middlePoints[currentMiddlePoint-1]) / (middlePoints[currentMiddlePoint] - middlePoints[currentMiddlePoint-1])

speeds_nofilter = speeds[:]

for j in range(50):
    i = 3
    while i < N - 3:
        speeds[i] = (speeds[i-3] + speeds[i-2] + speeds[i-1] + speeds[i] + speeds[i+1] + speeds[i+2] + speeds[i+3]) / 7
        i += 1

plt.plot(np.linspace(0, len(speeds) *  0.001, len(speeds)), speeds)
#plt.plot(np.linspace(0, len(speeds) *  0.001, len(speeds)), speeds_nofilter)
plt.plot(np.linspace(0, len(speeds) *  0.001, len(speeds)), speeds_raw)
plt.show()
