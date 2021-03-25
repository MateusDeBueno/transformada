#include "peripheral_setup.h"
#include "math.h"

/**
 * main.c
 */

int Va = 0;
int Vb = 0;
int Vc = 0;
float angulo = 0;
int Vds = 0;
int Vqs = 0;

//variaveis para plotar
#define N 500
#define inc 0.0628
int vetor1[N];
int *var_vetor1 = &Vds;
int vetor2[N];
int *var_vetor2 = &Vqs;
uint32_t index = 0;


__interrupt void isr_cpu_timer0(void);

int main(void)
{
    InitSysCtrl();                  //Initialize System Control
    DINT;                           //Disable CPU interrupts
    InitPieCtrl();                  //Initialize the PIE control register to their default state
    IER = 0x0000;                   //Disable CPU interrupts
    IFR = 0x0000;                   //Clear all CPU interrupt flags
    InitPieVectTable();             //Initialize the PIE vector table

    Setup_GPIO();
    //Setup_ePWM();

    EALLOW;
    PieVectTable.TIMER0_INT = &isr_cpu_timer0;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  //habilita coluna 7
    EDIS;

    IER |= M_INT1;  //habilita linha 1

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 200, 200); //(timer, clock do dsp, periodo em micro s)
    CpuTimer0Regs.TCR.all = 0x4001;

    EINT;                           //Enable Global interrupt INTM
    ERTM;                           //Enable Global realtime interrupt DBGM

    while(1){
        //
    }
}



__interrupt void isr_cpu_timer0(void){

    angulo += inc;
    if (angulo > 9.42477) //angulo > 3pi   -> reseta
        angulo = 0;

    //gera as tensoes trifasica
    Va = (int) 220*sin(angulo);
    Vb = (int) 220*sin(angulo-2.0944);
    Vc = (int) 220*sin(angulo+2.0944);

    Vds = (int) (Va*cos(angulo) + Vb*cos(angulo - 2.0944) + Vc*cos(angulo + 2.0944))*0.6667;
    Vqs = (int) (Va*sin(angulo) + Vb*sin(angulo - 2.0944) + Vc*sin(angulo + 2.0944))*0.6667;

    // Plot
    vetor1[index] = *var_vetor1;
    vetor2[index] = *var_vetor2;
    index = (index == N) ? 0 : (index+1);


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

