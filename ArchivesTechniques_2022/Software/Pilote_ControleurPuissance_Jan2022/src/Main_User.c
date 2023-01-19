
/*
	!!!! NB : ALIMENTER LA CARTE AVANT DE CONNECTER L'USB !!!

VERSION 16/12/2021 :
- ToolboxNRJ V4
- Driver version 2021b (synchronisation de la mise à jour Rcy -CCR- avec la rampe)
- Validé Décembre 2021

*/


/*
STRUCTURE DES FICHIERS

COUCHE APPLI = Main_User.c : 
programme principal à modifier. Par défaut hacheur sur entrée +/-10V, sortie 1 PWM
Attention, sur la trottinette réelle, l'entrée se fait sur 3V3.
Attention, l'entrée se fait avec la poignée d'accélération qui va de 0.6V à 2.7V !

COUCHE SERVICE = Toolbox_NRJ_V4.c
Middleware qui configure tous les périphériques nécessaires, avec API "friendly"

COUCHE DRIVER =
clock.c : contient la fonction Clock_Configure() qui prépare le STM32. Lancée automatiquement à l'init IO
lib : bibliothèque qui gère les périphériques du STM : Drivers_STM32F103_107_Jan_2015_b
*/



#include "ToolBox_NRJ_v4.h"




//=================================================================================================================
// 					USER DEFINE
//=================================================================================================================





// Choix de la fréquence PWM (en kHz)
#define FPWM_Khz 20.0
						


//==========END USER DEFINE========================================================================================

// ========= Variable globales indispensables et déclarations fct d'IT ============================================

void IT_Principale(void);
//=================================================================================================================


/*=================================================================================================================
 					FONCTION MAIN : 
					NB : On veillera à allumer les diodes au niveau des E/S utilisée par le progamme. 
					
					EXEMPLE: Ce progamme permet de générer une PWM (Voie 1) à 20kHz dont le rapport cyclique se règle
					par le potentiomètre de "l'entrée Analogique +/-10V"
					Placer le cavalier sur la position "Pot."
					La mise à jour du rapport cyclique se fait à la fréquence 1kHz.

//=================================================================================================================*/


float Te,Te_us;
float Epsilon, Epsilon_1, Sigma, Sigma_1, tau_3, tau_4;
float Alpha;
float Vretour,Cons_In,Yc;
int main (void)
{
// !OBLIGATOIRE! //	
Conf_Generale_IO_Carte();	
	
//Yc = 1.75;
//Vretour = 1.65;
Epsilon_1 = 0;
Sigma_1 = 0;
tau_3=0.002;
tau_4=0.0028;

// ------------- Discret, choix de Te -------------------	
Te=	2.5e-4;; // en seconde
Te_us=Te*1000000.0; // conversion en µs pour utilisation dans la fonction d'init d'interruption
	

//______________ Ecrire ici toutes les CONFIGURATIONS des périphériques ________________________________	
// Paramétrage ADC pour entrée analogique
Conf_ADC();
// Configuration de la PWM avec une porteuse Triangle, voie 1 & 2 activée, inversion voie 2
Triangle (FPWM_Khz);
Active_Voie_PWM(1);	
Active_Voie_PWM(2);	
Inv_Voie(2);

Start_PWM;
R_Cyc_1(2048);  // positionnement à 50% par défaut de la PWM
R_Cyc_2(2048);

// Activation LED
LED_Courant_On;
LED_PWM_On;
LED_PWM_Aux_Off;
LED_Entree_10V_Off;
LED_Entree_3V3_On;
LED_Codeur_Off;

// Conf IT
Conf_IT_Principale_Systick(IT_Principale, Te_us);

	while(1)
	{}

}





//=================================================================================================================
// 					FONCTION D'INTERRUPTION PRINCIPALE SYSTICK
//=================================================================================================================


void IT_Principale(void)
{
	
	//	Cons_In= Entree_10V();
//		Yc = Entree_3V3();
	//	Vretour =I1();
//	ConsinN = (float) Cons_In * 3,3/4096;
//	VretourN = (float) Vretour * 3,3/4096;
//	Epsilon_N = ConsinN - VretourN;
	
  Yc = (float) Entree_3V3()* 3.3/4096;
	Vretour = (float) I1()* 3.3/4096;
	
	Epsilon = Yc - Vretour;
	Sigma = Sigma_1 + (Te+2*tau_3)/(2*tau_4)*Epsilon + (Te-2*tau_3)/(2*tau_4)*Epsilon_1;
	
	if (Sigma>0.45){Sigma = 0.45;}
	if (Sigma <-0.45){Sigma = -0.45;}
	
	Sigma_1 = Sigma;
	Epsilon_1 = Epsilon;
	
	Alpha = (Sigma + 0.5) * 4096;
	
	R_Cyc_1((int) Alpha);
	R_Cyc_2((int) Alpha);
	
	
}

