clear;
clc;

%Simulation Moteur

%Paramètres
    %Temps
    tfinal = 10e-3;
    
    %Alim
    Ubat = 24;

    %Step
    Yc = 0.5;
    
    %T_hacheur
    Kh = 2*Ubat;

    %T_Moteur
    R = 1;
    L = 2e-3;
    E = 0;
    tauElec = L/R;
    fElec = 1/(2*pi()*tauElec)
    
    %T_Capteur de courant
    Kc = 0.10416;
    
    %Filtre ordre 2
    R5 = 5.1e3;
    R8 = 10e3;
    R12 = 10e3;
    R18 = 12e3;
    R21 = 220;
    C2 = 22e-9;
    C7 = 22e-9;
    
    num = (R12+R18)*R8/(R12*(R5+R8));
    tau1 = R5*R8/(R5+R8)*C2;
    tau2 = R21*C7;
    den1 = tau1+tau2;
    den2 = tau1*tau2;
    f1 = 1/(2*pi()*tau1)
    f2 = 1/(2*pi()*tau2)
    
    %Integrateur
    Ki = 0.1;
    tauInt = Kc/Kp;
    
    
  
%Lancement de la simulation
sim = sim('Model_Automatique_BO_PI');

%Tracé
    %Consigne PWM
    plot(sim.sigma,'-b');
    hold on
    %Courant capteur
    plot(sim.Vretour,'-r');