import matplotlib.pyplot as plt

Vc = [0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50]
Vm = [0,0.054,0.105,0.155,0.206,0.257,0.307,0.358,0.408,0.458,0.510]
plt.title("Vitesse (Encode / ms) efd Puissance (robotC)")
plt.plot(Vc,Vm,'-x')
plt.show()