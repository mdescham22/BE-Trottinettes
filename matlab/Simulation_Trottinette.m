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

    %T_Capteur de courant
    Kc = 0.10416;

%Lancement de la simulation
sim = sim('Model_Automatique_BF');

%Tracé
    %Consigne PWM
    subplot(2,1,1);
    plot(sim.sigma,'-b');

    %Courant capteur
    subplot(2,1,2);
    plot(sim.I,'r');

