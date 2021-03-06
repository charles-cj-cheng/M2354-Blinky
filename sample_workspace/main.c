/* =============================================================================
** ACBEL POLYTECH INC.
** Copyright (c) 2020 AcBel. All right reserved.
** =============================================================================
**
**		FILE NAME            : main.c
**		DESCRIPTION          :
**    AUTHOR               :
**    VERSION              : V1.00
**    DATE                 :
**		SOFTWARE DEPENDANCIES:
**		HARDWARE DEPENDANCIES:
**		NOTES:
**
** ===========================================================================*/

/******************************************************************************/
/*             System header includes                                         */
/******************************************************************************/
#include <stdio.h>


/******************************************************************************/
/*             Project header includes                                        */
/******************************************************************************/
#include "NuMicro.h"
#include "clk.h"
#include "uart.h"
#include "gpio.h"
#include "sys.h"
#include "Debug.h"

/******************************************************************************/
/*             Local defined constants                                        */
/******************************************************************************/
#define LED_YELLOW  PD2
#define LED_GREEN   PD3

/******************************************************************************/
/*             Local struct, union, enum definitions and typedefs             */
/******************************************************************************/


/******************************************************************************/
/*             Local macro definitions                                        */
/******************************************************************************/
#define LED_INIT()  (PD->MODE = (PD->MODE &(~(0xful << 2*2))) | (0x5ul << 2 * 2))


/******************************************************************************/
/*             Static variable definitions                                    */
/******************************************************************************/


/******************************************************************************/
/*             Global variable definitions                                    */
/******************************************************************************/


/******************************************************************************/
/*             Static function declarations(prototypes)	                      */
/******************************************************************************/
static void SYS_Init(void);
static void UART0_Init(void);


/******************************************************************************/
/*             Public functions                                               */
/******************************************************************************/

int main()
{
    SYS_UnlockReg();

    SYS_Init();

    UART0_Init();

    DEBUG_PRINTF("\n");
    DEBUG_PRINTF("+-------------------------------------------------------------+\n");
    DEBUG_PRINTF("|              Simple Blinky Demo                             |\n");
    DEBUG_PRINTF("+-------------------------------------------------------------+\n");

    /* Init GPIO for LED toggle */
    LED_INIT();
    LED_YELLOW = 1;
    LED_GREEN = 0;
    while(1)
    {
        LED_YELLOW ^= 1;
        LED_GREEN ^= 1;
        CLK_SysTickLongDelay(200000);
        LED_YELLOW ^= 1;
        LED_GREEN ^= 1;
        CLK_SysTickLongDelay(200000);
    }
}

/******************************************************************************/
/*             Private functions                                              */
/******************************************************************************/

void SYS_Init(void)
{
    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set power level to 0 */
    SYS_SetPowerLevel(SYS_PLCTL_PLSEL_PL0);

    /* Set core clock to 96MHz */
    CLK_SetCoreClock(96000000);

    /* Enable SRAM module clock */
    CLK_EnableModuleClock(SRAM1_MODULE);
    CLK_EnableModuleClock(SRAM2_MODULE);

    /* Enable GPIO module clock */
    CLK_EnableModuleClock(GPA_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);
    CLK_EnableModuleClock(GPC_MODULE);
    CLK_EnableModuleClock(GPD_MODULE);
    CLK_EnableModuleClock(GPE_MODULE);
    CLK_EnableModuleClock(GPF_MODULE);
    CLK_EnableModuleClock(GPG_MODULE);
    CLK_EnableModuleClock(GPH_MODULE);

    /* Select IP clock source */
    CLK->CLKSEL2 = CLK_CLKSEL2_UART0SEL_HIRC;

    /* Enable IP clock */
    CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk;


    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set multi-function pins for UART0 RXD and TXD */
    SYS->GPA_MFPL = (SYS->GPA_MFPL & (~(UART0_RXD_PA6_Msk | UART0_TXD_PA7_Msk))) | UART0_RXD_PA6 | UART0_TXD_PA7;

}

void UART0_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    UART0->LINE = UART_PARITY_NONE | UART_STOP_BIT_1 | UART_WORD_LEN_8;
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC, 115200);

}

////////////// End /////////////////////////////////////////////////////////////
