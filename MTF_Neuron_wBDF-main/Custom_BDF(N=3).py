import numpy as np, matplotlib.pyplot as plt
# Iapp
Iapp = [-1.8,-1.8,-1.8]

plt.style.use(['custom','no-latex'])

### Neurons parameters ###
# [af-,as+,as-,aus+]
alpha=[-2,2,-1.5,1.5]
# [df-,ds+,ds-,dus+]
delta=[0,0,-1.5,-1.5]

Tf,Ts,Tus=1,50,2500

# Synapse 
### Synapse Parameters ###
# synapse is not linearized function
asyn=np.array([[0,-0.3,-0.3],
               [-0.3,0,-0.3],
               [-0.3,-0.3,0]])
dsyn=np.array([[0,-1,-1],
               [-1,0,-1],
               [-1,-1,0]])
b=5

# Synapse Function (Sigmoid) X: is voltage (in our case is set at Vs), b=Steepness of the funxtion, ds= center position in the x axis)
def sf(x,b,ds):
    k=b*(x-ds)
    return 1/(1+np.exp(-k))

def dsf(x,b,ds):
    k=b*(x-ds)
    return (b*np.exp(-k))/pow(1+np.exp(-k),2)

# dsdt= f(t,vm,vf,vs,vus)
def f(S):
    vm1,vf1,vs1,vus1,vm2,vf2,vs2,vus2,vm3,vf3,vs3,vus3 = S
    Isyn1=Iapp[0]+asyn[1][0]*sf(vs2,b,dsyn[1][0])+asyn[2][0]*sf(vs3,b,dsyn[2][0])
    Isyn2=Iapp[1]+asyn[0][1]*sf(vs1,b,dsyn[0][1])+asyn[2][1]*sf(vs3,b,dsyn[2][1])
    Isyn3=Iapp[2]+asyn[0][2]*sf(vs1,b,dsyn[0][2])+asyn[1][2]*sf(vs2,b,dsyn[1][2])
    return [-vm1-alpha[0]*np.tanh(vf1-delta[0])-alpha[1]*np.tanh(vs1-delta[1])-alpha[2]*np.tanh(vs1-delta[2])-alpha[3]*np.tanh(vus1-delta[3])+Isyn1,(vm1-vf1)/Tf,(vm1-vs1)/Ts,(vm1-vus1)/Tus,
            -vm2-alpha[0]*np.tanh(vf2-delta[0])-alpha[1]*np.tanh(vs2-delta[1])-alpha[2]*np.tanh(vs2-delta[2])-alpha[3]*np.tanh(vus2-delta[3])+Isyn2,(vm2-vf2)/Tf,(vm2-vs2)/Ts,(vm2-vus2)/Tus,
            -vm3-alpha[0]*np.tanh(vf3-delta[0])-alpha[1]*np.tanh(vs3-delta[1])-alpha[2]*np.tanh(vs3-delta[2])-alpha[3]*np.tanh(vus3-delta[3])+Isyn3,(vm3-vf3)/Tf,(vm3-vs3)/Ts,(vm3-vus3)/Tus]

def df(S):
    vm1,vf1,vs1,vus1,vm2,vf2,vs2,vus2,vm3,vf3,vs3,vus3 = S
    return [[-1,-alpha[0]*(1-np.tanh(vf1-delta[0])**2),-alpha[1]*(1-np.tanh(vs1-delta[1])**2)-alpha[2]*(1-np.tanh(vs1-delta[2])**2),-alpha[3]*(1-np.tanh(vus1-delta[3])**2),0,0,asyn[1][0]*dsf(vs2,b,dsyn[1][0]),0,0,0,asyn[2][0]*dsf(vs3,b,dsyn[2][0]),0],
            [(1/Tf),(-1/Tf),0,0,0,0,0,0,0,0,0,0],
            [(1/Ts),0,(-1/Ts),0,0,0,0,0,0,0,0,0],
            [(1/Tus),0,0,(-1/Tus),0,0,0,0,0,0,0,0],
            [0,0,asyn[0][1]*dsf(vs1,b,dsyn[0][1]),0,-1,-alpha[0]*(1-np.tanh(vf2-delta[0])**2),-alpha[1]*(1-np.tanh(vs2-delta[1])**2)-alpha[2]*(1-np.tanh(vs2-delta[2])**2),-alpha[3]*(1-np.tanh(vus2-delta[3])**2),0,0,asyn[2][1]*dsf(vs3,b,dsyn[2][1]),0],
            [0,0,0,0,(1/Tf),(-1/Tf),0,0,0,0,0,0],
            [0,0,0,0,(1/Ts),0,(-1/Ts),0,0,0,0,0],
            [0,0,0,0,(1/Tus),0,0,(-1/Tus),0,0,0,0],
            [0,0,asyn[0][2]*dsf(vs1,b,dsyn[0][2]),0,0,0,asyn[1][2]*dsf(vs3,b,dsyn[1][2]),0,-1,-alpha[0]*(1-np.tanh(vf3-delta[0])**2),-alpha[1]*(1-np.tanh(vs3-delta[1])**2)-alpha[2]*(1-np.tanh(vs3-delta[2])**2),-alpha[3]*(1-np.tanh(vus3-delta[3])**2)],
            [0,0,0,0,0,0,0,0,(1/Tf),(-1/Tf),0,0],
            [0,0,0,0,0,0,0,0,(1/Ts),0,(-1/Ts),0],
            [0,0,0,0,0,0,0,0,(1/Tus),0,0,(-1/Tus)]]

def newton(f,Jf,x0,epsilon,max_iter):
    '''Approximate solution of f(x)=0 by Newton's method.

    Parameters
    ----------
    f : function
        Function for which we are searching for a solution f(x,y,z,...)=0.
    Df : function
        Jacobian of f(x,y,z,...).
    x0 : number
        Initial guess for a solution f(x,y,z,...)=0.
    epsilon : number
        Stopping criteria is norm(Df(x,y,z,...)) < epsilon.
    max_iter : integer
        Maximum number of iterations of Newton's method.

    Returns
    -------
    xn : number
        Implement Newton's method: compute the linear approximation
        of f(x) at xn and find x intercept by the formula
            x = xn - f(xn)/Df(xn)
        Continue until abs(f(xn)) < epsilon and return xn.
        If Df(xn) == 0, return None. If the number of iterations
        exceeds max_iter, then return None.

    Examples
    --------
    >>> f = lambda x: x**2 - x - 1
    >>> Df = lambda x: 2*x - 1
    >>> newton(f,Df,1,1e-8,10)
    Found solution after 5 iterations.
    1.618033988749989
    '''
    
    xn = np.array(x0)
    delta=1
    CF=False
    for n in range(0,max_iter):
        fxn = f(xn)
        
        if abs(delta) < epsilon:
            CF=True
            # print('Found solution after',n,'iterations.')
            return [xn,CF]
        Dfxn = Jf(xn)
        # Coovergence condition (if the norm of the ||delta||<epsilon)
        delta=np.linalg.norm(np.matmul(np.linalg.inv(Dfxn),fxn))
        
        # Det check
        if np.linalg.det(Dfxn) == 0:
            CF=False
            # print('Zero derivative. No solution found.')
            return [None,CF]
        
        # Calculation
        xn = xn - np.matmul(np.linalg.inv(Dfxn),fxn)
    print('Exceeded maximum iterations. No solution found.')
    return [None,CF]

def bdf_2nd(f, y0, h, ti,tf):
    # Initilizing
    N=int((tf-ti)//h) # Number of steps 
    t=np.linspace(ti,tf,N) # Timestep vector
    y=np.zeros([int(len(y0)),N])
    for k in range(int(len(y0))):
        y[k][0]=y0[k]
    
    # Calculate the second initial condition with BDF (1st order) ===============================================================
    # BW Euler
    O=1 # for 2nd BDF 
    for i in range(0,O):
        # g, Next value function. 
        def g(S):
            # Next value of state variables, vmn = vm(k+1)
            vmn1,vfn1,vsn1,vusn1,vmn2,vfn2,vsn2,vusn2,vmn3,vfn3,vsn3,vusn3=S
            # Next value of state variable, fvmn = f(t[i+1],S) 
            fvmn1,fvfn1,fvsn1,fvusn1,fvmn2,fvfn2,fvsn2,fvusn2,fvmn3,fvfn3,fvsn3,fvusn3=f(S)
            
            return np.array([vmn1-(h*fvmn1)-y[0][i],vfn1-(h*fvfn1)-y[1][i],vsn1-(h*fvsn1)-y[2][i],vusn1-(h*fvusn1)-y[3][i],
                             vmn2-(h*fvmn2)-y[4][i],vfn2-(h*fvfn2)-y[5][i],vsn2-(h*fvsn2)-y[6][i],vusn2-(h*fvusn2)-y[7][i],
                             vmn3-(h*fvmn3)-y[8][i],vfn3-(h*fvfn3)-y[9][i],vsn3-(h*fvsn3)-y[10][i],vusn3-(h*fvusn3)-y[11][i]])
        
        def Jg(S):
            # next value of state variables, vmn = vm(n+1)
            Jf=df(S)
            return np.array([[1-h*Jf[0][0],-h*Jf[0][1],-h*Jf[0][2],-h*Jf[0][3],-h*Jf[0][4],-h*Jf[0][5],-h*Jf[0][6],-h*Jf[0][7],-h*Jf[0][8],-h*Jf[0][9],-h*Jf[0][10],-h*Jf[0][11]],
                             [-h*Jf[1][0],1-h*Jf[1][1],-h*Jf[1][2],-h*Jf[1][3],-h*Jf[1][4],-h*Jf[1][5],-h*Jf[1][6],-h*Jf[1][7],-h*Jf[1][8],-h*Jf[1][9],-h*Jf[1][10],-h*Jf[1][11]],
                             [-h*Jf[2][0],-h*Jf[2][1],1-h*Jf[2][2],-h*Jf[2][3],-h*Jf[2][4],-h*Jf[2][5],-h*Jf[2][6],-h*Jf[2][7],-h*Jf[2][8],-h*Jf[2][9],-h*Jf[2][10],-h*Jf[2][11]],
                             [-h*Jf[3][0],-h*Jf[3][1],-h*Jf[3][2],1-h*Jf[3][3],-h*Jf[3][4],-h*Jf[3][5],-h*Jf[3][6],-h*Jf[3][7],-h*Jf[3][8],-h*Jf[3][9],-h*Jf[3][10],-h*Jf[3][11]],
                             [-h*Jf[4][0],-h*Jf[4][1],-h*Jf[4][2],-h*Jf[4][3],1-h*Jf[4][4],-h*Jf[4][5],-h*Jf[4][6],-h*Jf[4][7],-h*Jf[4][8],-h*Jf[4][9],-h*Jf[4][10],-h*Jf[4][11]],
                             [-h*Jf[5][0],-h*Jf[5][1],-h*Jf[5][2],-h*Jf[5][3],-h*Jf[5][4],1-h*Jf[5][5],-h*Jf[5][6],-h*Jf[5][7],-h*Jf[5][8],-h*Jf[5][9],-h*Jf[5][10],-h*Jf[5][11]],
                             [-h*Jf[6][0],-h*Jf[6][1],-h*Jf[6][2],-h*Jf[6][3],-h*Jf[6][4],-h*Jf[6][5],1-h*Jf[6][6],-h*Jf[6][7],-h*Jf[6][8],-h*Jf[6][9],-h*Jf[6][10],-h*Jf[6][11]],
                             [-h*Jf[7][0],-h*Jf[7][1],-h*Jf[7][2],-h*Jf[7][3],-h*Jf[7][4],-h*Jf[7][5],-h*Jf[7][6],1-h*Jf[7][7],-h*Jf[7][8],-h*Jf[7][9],-h*Jf[7][10],-h*Jf[7][11]],
                             [-h*Jf[8][0],-h*Jf[8][1],-h*Jf[8][2],-h*Jf[8][3],-h*Jf[8][4],-h*Jf[8][5],-h*Jf[8][6],-h*Jf[8][7],1-h*Jf[8][8],-h*Jf[8][9],-h*Jf[8][10],-h*Jf[8][11]],
                             [-h*Jf[9][0],-h*Jf[9][1],-h*Jf[9][2],-h*Jf[9][3],-h*Jf[9][4],-h*Jf[9][5],-h*Jf[9][6],-h*Jf[9][7],-h*Jf[9][8],1-h*Jf[9][9],-h*Jf[9][10],-h*Jf[9][11]],
                             [-h*Jf[10][0],-h*Jf[10][1],-h*Jf[10][2],-h*Jf[10][3],-h*Jf[10][4],-h*Jf[10][5],-h*Jf[10][6],-h*Jf[10][7],-h*Jf[10][8],-h*Jf[10][9],1-h*Jf[10][10],-h*Jf[10][11]],
                             [-h*Jf[11][0],-h*Jf[11][1],-h*Jf[11][2],-h*Jf[11][3],-h*Jf[11][4],-h*Jf[11][5],-h*Jf[11][6],-h*Jf[11][7],-h*Jf[11][8],-h*Jf[11][9],-h*Jf[11][10],1-h*Jf[11][11]]])
        
        # initial condition for Newthon's method
        x0=[y[k][i] for k in range(int(len(y0)))]
        # x0=[y[0][i],y[1][i],y[2][i],y[3][i]]
        root=newton(g,Jg,x0,1e-10,20)
        # root[1] = bolean flag to make sure that the newthon was successful.
        if root[1]:
            for k in range(int(len(y0))):
                y[k][i+1] = root[0][k]
        else:
            print('No solution found')
    #============================================================================================================================
    
    
    # Main BDF ==================================================================================================================
    for i in range(0,N-2):
        # print(i)
        # g, Next value function. 
        def g(S):
            # Next value of state variables, vmn = vm(k+2)
            vmn1,vfn1,vsn1,vusn1,vmn2,vfn2,vsn2,vusn2,vmn3,vfn3,vsn3,vusn3=S
            # Next value of state variable, fvmn = f(t[i+2],S) 
            fvmn1,fvfn1,fvsn1,fvusn1,fvmn2,fvfn2,fvsn2,fvusn2,fvmn3,fvfn3,fvsn3,fvusn3=f(S)
            
            return np.array([vmn1-((2/3)*h*fvmn1)-(4/3)*y[0][i+1]+(1/3)*y[0][i],vfn1-((2/3)*h*fvfn1)-(4/3)*y[1][i+1]+(1/3)*y[1][i],vsn1-((2/3)*h*fvsn1)-(4/3)*y[2][i+1]+(1/3)*y[2][i],vusn1-((2/3)*h*fvusn1)-(4/3)*y[3][i+1]+(1/3)*y[3][i],
                             vmn2-((2/3)*h*fvmn2)-(4/3)*y[4][i+1]+(1/3)*y[4][i],vfn2-((2/3)*h*fvfn2)-(4/3)*y[5][i+1]+(1/3)*y[5][i],vsn2-((2/3)*h*fvsn2)-(4/3)*y[6][i+1]+(1/3)*y[6][i],vusn2-((2/3)*h*fvusn2)-(4/3)*y[7][i+1]+(1/3)*y[7][i],
                             vmn3-((2/3)*h*fvmn3)-(4/3)*y[8][i+1]+(1/3)*y[8][i],vfn3-((2/3)*h*fvfn3)-(4/3)*y[9][i+1]+(1/3)*y[9][i],vsn3-((2/3)*h*fvsn3)-(4/3)*y[10][i+1]+(1/3)*y[10][i],vusn3-((2/3)*h*fvusn3)-(4/3)*y[11][i+1]+(1/3)*y[11][i]])
        
        def Jg(S):
            # next value of state variables, vmn = vm(n+1)
            Jf=df(S)
            # BDF Coefficient
            CF=2/3
            return np.array([[1-CF*h*Jf[0][0],-CF*h*Jf[0][1],-CF*h*Jf[0][2],-CF*h*Jf[0][3],-CF*h*Jf[0][4],-CF*h*Jf[0][5],-CF*h*Jf[0][6],-CF*h*Jf[0][7],-CF*h*Jf[0][8],-CF*h*Jf[0][9],-CF*h*Jf[0][10],-CF*h*Jf[0][11]],
                             [-CF*h*Jf[1][0],1-CF*h*Jf[1][1],-CF*h*Jf[1][2],-CF*h*Jf[1][3],-CF*h*Jf[1][4],-CF*h*Jf[1][5],-CF*h*Jf[1][6],-CF*h*Jf[1][7],-CF*h*Jf[1][8],-CF*h*Jf[1][9],-CF*h*Jf[1][10],-CF*h*Jf[1][11]],
                             [-CF*h*Jf[2][0],-CF*h*Jf[2][1],1-CF*h*Jf[2][2],-CF*h*Jf[2][3],-CF*h*Jf[2][4],-CF*h*Jf[2][5],-CF*h*Jf[2][6],-CF*h*Jf[2][7],-CF*h*Jf[2][8],-CF*h*Jf[2][9],-CF*h*Jf[2][10],-CF*h*Jf[2][11]],
                             [-CF*h*Jf[3][0],-CF*h*Jf[3][1],-CF*h*Jf[3][2],1-CF*h*Jf[3][3],-CF*h*Jf[3][4],-CF*h*Jf[3][5],-CF*h*Jf[3][6],-CF*h*Jf[3][7],-CF*h*Jf[3][8],-CF*h*Jf[3][9],-CF*h*Jf[3][10],-CF*h*Jf[3][11]],
                             [-CF*h*Jf[4][0],-CF*h*Jf[4][1],-CF*h*Jf[4][2],-CF*h*Jf[4][3],1-CF*h*Jf[4][4],-CF*h*Jf[4][5],-CF*h*Jf[4][6],-CF*h*Jf[4][7],-CF*h*Jf[4][8],-CF*h*Jf[4][9],-CF*h*Jf[4][10],-CF*h*Jf[4][11]],
                             [-CF*h*Jf[5][0],-CF*h*Jf[5][1],-CF*h*Jf[5][2],-CF*h*Jf[5][3],-CF*h*Jf[5][4],1-CF*h*Jf[5][5],-CF*h*Jf[5][6],-CF*h*Jf[5][7],-CF*h*Jf[5][8],-CF*h*Jf[5][9],-CF*h*Jf[5][10],-CF*h*Jf[5][11]],
                             [-CF*h*Jf[6][0],-CF*h*Jf[6][1],-CF*h*Jf[6][2],-CF*h*Jf[6][3],-CF*h*Jf[6][4],-CF*h*Jf[6][5],1-CF*h*Jf[6][6],-CF*h*Jf[6][7],-CF*h*Jf[6][8],-CF*h*Jf[6][9],-CF*h*Jf[6][10],-CF*h*Jf[6][11]],
                             [-CF*h*Jf[7][0],-CF*h*Jf[7][1],-CF*h*Jf[7][2],-CF*h*Jf[7][3],-CF*h*Jf[7][4],-CF*h*Jf[7][5],-CF*h*Jf[7][6],1-CF*h*Jf[7][7],-CF*h*Jf[7][8],-CF*h*Jf[7][9],-CF*h*Jf[7][10],-CF*h*Jf[7][11]],
                             [-CF*h*Jf[8][0],-CF*h*Jf[8][1],-CF*h*Jf[8][2],-CF*h*Jf[8][3],-CF*h*Jf[8][4],-CF*h*Jf[8][5],-CF*h*Jf[8][6],-CF*h*Jf[8][7],1-CF*h*Jf[8][8],-CF*h*Jf[8][9],-CF*h*Jf[8][10],-CF*h*Jf[8][11]],
                             [-CF*h*Jf[9][0],-CF*h*Jf[9][1],-CF*h*Jf[9][2],-CF*h*Jf[9][3],-CF*h*Jf[9][4],-CF*h*Jf[9][5],-CF*h*Jf[9][6],-CF*h*Jf[9][7],-CF*h*Jf[9][8],1-CF*h*Jf[9][9],-CF*h*Jf[9][10],-CF*h*Jf[9][11]],
                             [-CF*h*Jf[10][0],-CF*h*Jf[10][1],-CF*h*Jf[10][2],-CF*h*Jf[10][3],-CF*h*Jf[10][4],-CF*h*Jf[10][5],-CF*h*Jf[10][6],-CF*h*Jf[10][7],-CF*h*Jf[10][8],-CF*h*Jf[10][9],1-CF*h*Jf[10][10],-CF*h*Jf[10][11]],
                             [-CF*h*Jf[11][0],-CF*h*Jf[11][1],-CF*h*Jf[11][2],-CF*h*Jf[11][3],-CF*h*Jf[11][4],-CF*h*Jf[11][5],-CF*h*Jf[11][6],-CF*h*Jf[11][7],-CF*h*Jf[11][8],-CF*h*Jf[11][9],-CF*h*Jf[11][10],1-CF*h*Jf[11][11]]])
        
        # initial condition for Newthon's method
        x0=[y[k][i+1] for k in range(int(len(y0)))]
        # x0=[y[0][i+1],y[1][i+1],y[2][i+1],y[3][i+1]]
        root=newton(g,Jg,x0,1e-10,20)
        # root[1] = bolean flag to make sure that the newthon was successful.
        if root[1]:
            for k in range(int(len(y0))):
                y[k][i+2] = root[0][k]
        else:
            print('No solution found')
    #============================================================================================================================
    return y,t


vm1_0, vf1_0, vs1_0, vus1_0 = -1,0,0,0
vm2_0, vf2_0, vs2_0, vus2_0 = -1,0,0,0
vm3_0, vf3_0, vs3_0, vus3_0 = -1,0,0,0
S_0=[vm1_0, vf1_0, vs1_0, vus1_0,
    vm2_0, vf2_0, vs2_0, vus2_0,
    vm3_0, vf3_0, vs3_0, vus3_0]
# bw_Euler(function, intial condition as a vector, timestep, intial time, final time)
# out[0][:] = Vm, out[1][:] = Vf, out[2][:] = Vs, out[3][:] = Vus
[out,t]=bdf_2nd(f,S_0,0.1,0,20000)

fig, ax = plt.subplots(1,1,tight_layout=True)
ax.set_xlim(0,20000)
ax.set_ylim(-5,5)
ax.set_xticks([0,5000,10000,15000,20000])
ax.set_yticks(np.arange(-5,5,1))
ax.set_xlabel('Timestep [a.u]')
ax.set_ylabel('Output [a.u]')


ax.plot(t,out[0][:],'-b',lw=0.5,label='Vm1')
ax.plot(t,out[4][:],'-r',lw=0.5,label='Vm2')
ax.plot(t,out[8][:],'-g',lw=0.5,label='Vm3')
ax.legend(loc='upper left', ncol=3, fontsize=5)

plt.show()