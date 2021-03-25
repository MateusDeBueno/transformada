/*
 * peripheral_setup.c
 *
 *  Created on: 21 de mar de 2021
 *      Author: Mateus
 */

#include "peripheral_setup.h"

void Setup_GPIO(void){
    EALLOW;
    //Table 8-7. GPIO Muxed Pins - explica qual a funcao que cada pino pode receber
    /*
    GpioCtrlRegs.GPxMUXy.bit.GPIOz = 0;
    x = A,B,C...
    y = 1,2
    z = 0...15, 16...31 (A)
    z = 32...47, 48...31 (B)
    */

    // LED BLUE 31 A, 2
    // LED RED 34 B, 1

    GpioCtrlRegs.GPAGMUX2.bit.GPIO31 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;     //configura como saida, so é necessario se for um uso generico, feito GPIO
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 1;     //adiciona pull-down

    GpioCtrlRegs.GPBGMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 1;

/*
    //PWM
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
*/
    EDIS;
}



void Setup_ePWM(void){
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;       //desativo o pwm para poder configurar

    //configura portadora
    EPwm1Regs.TBPRD = 50000;            //1khz //PRD=MCclock/(2*fs), porem se updown PRD = MCclock/(4*fs)
    EPwm1Regs.TBPHS.bit.TBPHS = 0;     //defasagem 0
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    EPwm1Regs.TBCTR = 0x0000;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; //count up/down
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       //pre scale do clock do pwm, util para baixar frequencias, para o TBPRD não estourar seu valor maximo
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;




    //configura shadow (comparadores so atualizam apos um evento)
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;  //escolhe o evento para PWM1A
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;  //escolhe o evento para PWM1B



    //configura as ações para PWM1A
    EPwm1Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;

    EPwm1Regs.CMPA.bit.CMPA = EPwm1Regs.TBPRD/2;

    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; //EPWM B é complementar do EPWM A
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  //habilita dead band
    EPwm1Regs.DBFED.bit.DBFED = 100;                //rising edges - 100 equivale a 1 micro s
    EPwm1Regs.DBRED.bit.DBRED = 100;                //falling edges

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;       //ativa o pwm
    EDIS;
}





