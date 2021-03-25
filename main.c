#include "peripheral_setup.h"
#include "math.h"

/**
 * main.c
 */

uint32_t count = 0;
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

    GpioDataRegs.GPATOGGLE.bit.GPIO31    =1;
    GpioDataRegs.GPBTOGGLE.bit.GPIO34    =1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

