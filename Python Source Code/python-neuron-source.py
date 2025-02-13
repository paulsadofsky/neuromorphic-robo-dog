import numpy as np, matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

plt.style.use(['custom','no-latex'])

tmax=int(10000)
t=np.arange(tmax)

# Iapp
Iapp = np.array([-1.5])

### Neurons parameters ###
# [af-,as+,as-,aus+]
alpha=np.array([-2,2,-1.5,1.5])
# [df-,ds+,ds-,dus+]
delta=np.array([0,0,-1.5,-1.5])


def dsdt(t,S):
    vm1, vf1, vs1, vus1 =S
    return [-vm1-alpha[0]*np.tanh(vf1-delta[0])-alpha[1]*np.tanh(vs1-delta[1])-alpha[2]*np.tanh(vs1-delta[2])-alpha[3]*np.tanh(vus1-delta[3])+Iapp[0],
            vm1-vf1,
            (vm1-vs1)/50,
            (vm1-vus1)/2500
            ]


vm1_0, vf1_0, vs1_0, vus1_0 = -1,0,0,0
S_0 = (vm1_0, vf1_0, vs1_0, vus1_0) 


sol_m1 = solve_ivp(dsdt,t_span=(0,tmax),y0=S_0,t_eval=t,method='BDF')


# ivp_solver
vm1=sol_m1.y[0]
vf1=sol_m1.y[1]
vs1=sol_m1.y[2]
vus1=sol_m1.y[3]

### Plot ###
plt.figure(tight_layout=True)
# N1 
plt.title("Neuron Behavior")
plt.plot(t,vm1,color='b',lw=0.7)
plt.xlim(0,tmax)
plt.xticks(np.arange(0,tmax+2000,2000),np.arange(0,(tmax//1000)+(2000//1000),2000//1000).astype(str))
plt.ylim(-4,4)
plt.ylabel("N1 [au]")
plt.xlabel("Timestep/1000 [au]")

plt.show()