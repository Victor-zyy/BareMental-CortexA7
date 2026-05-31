#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
#define LED_PAD_CONFIG_DATA            (IOMUXC_SW_PAD_CTL_PAD_SRE( 0 )| \
                                          IOMUXC_SW_PAD_CTL_PAD_DSE( 6 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_SPEED( 1 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_ODE( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_PKE( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_PUE( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_PUS( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_HYS( 0 ))

#define BUTTON_PAD_CONFIG_DATA            (IOMUXC_SW_PAD_CTL_PAD_SRE( 0 )| \
                                          IOMUXC_SW_PAD_CTL_PAD_DSE( 6 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_SPEED( 1 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_ODE( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_PKE( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_PUE( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_PUS( 0 ) | \
                                          IOMUXC_SW_PAD_CTL_PAD_HYS( 1 ))
#define button2_GPIO               GPIO5
#define button2_GPIO_PIN           (1U)

static int button = 0;
static void EXAMPLE_GPIO_IRQHandler(void);
static void buttonInterruptInit( void )
{
    volatile uint32_t *icr;  
    uint32_t icrShift;      

    icrShift = button2_GPIO_PIN;
    /**
     * Register interrupt function into IrqTable
     */
    SystemInstallIrqHandler(GPIO5_Combined_0_15_IRQn, \
      (system_irq_handler_t)EXAMPLE_GPIO_IRQHandler, NULL);

    /**
     * Enable IRq
     */
    GIC_EnableIRQ(GPIO5_Combined_0_15_IRQn);

    /**
     * Enable GPIO Clock
     */

    CCM->CCGR1 |= CCM_CCGR1_CG15(0x3); 

    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, BUTTON_PAD_CONFIG_DATA);

    /**
     * GPIO_IMR mask interrupt
     * Set input direction
     */
    GPIO5->IMR &= ~(1 << button2_GPIO_PIN);  
    GPIO5->GDIR &= ~(1<<1);                 

    /**
     * Set GPIO edge interrupt type falling or rising edge clear the bit 
     * ICR determines the edge type
     */
    GPIO5->EDGE_SEL &= ~(1U << button2_GPIO_PIN);

    /**
     * GPIO ICR1/2 for each GPIO pin for select edge type 
     * need to calculate the register
     */
    if(button2_GPIO_PIN < 16)
    {
        icr = &(GPIO5->ICR1);
    }
    else
    {
        icr = &(GPIO5->ICR2);
        icrShift -= 16;
    }
     /**
      * GPIO Key default value is low , and rising edge trigger
      */
     *icr = (*icr & (~(3U << (2 * icrShift)))) | (2U << (2 * icrShift));
     /**
      * Enable GPIO pin interrupt 
      */
     button2_GPIO->IMR |= (1 << button2_GPIO_PIN); 
}

static void EXAMPLE_GPIO_IRQHandler(void)
{
    /**
     * Clear the ISR interrupt status
     */
    button2_GPIO->ISR = 1U << button2_GPIO_PIN;
    if(button > 0)
    {
        button = 0;
    }
    else
    {
        button = 1;
    }
}

static void BoardGpioInit( void )
{
    /**
     * 1. GPIO Clock
     */
    CCM->CCGR1 |= CCM_CCGR1_CG13( 0x3 ); // for GPIO1
    CCM->CCGR3 |= CCM_CCGR1_CG6( 0x3 );  // for GPIO4
    CCM->CCGR1 |= CCM_CCGR1_CG15( 0x3 ); //for GPIO5 -- input key
    /**
     * 2. Set IOMUXC
     */
    // RED
    IOMUXC_SetPinMux( IOMUXC_GPIO1_IO04_GPIO1_IO04, 0 );
    IOMUXC_SetPinConfig( IOMUXC_GPIO1_IO04_GPIO1_IO04, LED_PAD_CONFIG_DATA );
    // GREEN
    IOMUXC_SetPinMux( IOMUXC_CSI_HSYNC_GPIO4_IO20, 0 );
    IOMUXC_SetPinConfig( IOMUXC_CSI_HSYNC_GPIO4_IO20, LED_PAD_CONFIG_DATA );
    // BLUE
    IOMUXC_SetPinMux( IOMUXC_CSI_VSYNC_GPIO4_IO19, 0 );
    IOMUXC_SetPinConfig( IOMUXC_CSI_VSYNC_GPIO4_IO19, LED_PAD_CONFIG_DATA );

    GPIO1->GDIR |= (1<<4);  //设置GPIO1_04为输出模式
    GPIO1->DR |= (1<<4);    //设置GPIO1_04输出电平为高电平

    GPIO4->GDIR |= (1<<20);  //设置GPIO4_20为输出模式
    GPIO4->DR |= (1<<20);    //设置GPIO4_20输出电平为高电平

    GPIO4->GDIR |= (1<<19);  //设置GPIO4_19为输出模式
    GPIO4->DR |= (1<<19);    //设置GPIO4_19输出电平为高电平
    
}

static void delay( uint32_t count )
{
    while( --count )
    {
        __asm( "NOP" );
    }
}
#define LED_RED_ON  GPIO1->DR &= ~(1 << 4) // RED on
#define LED_RED_OFF GPIO1->DR |= (1 << 4) // RED off
int main(void)
{
    int num = 0;
    /* Init board hardware. */
    BOARD_InitPins();           // Configure UART PAD and Pin
    BOARD_BootClockRUN();
    //BOARD_InitMemory();
    BOARD_InitDebugConsole();
    BoardGpioInit();
    buttonInterruptInit();
    PRINTF("hello world");
    while (1)
    {
        SCANF("%d", &num);
        if( num > 10 ){
            LED_RED_ON;
        }else
        {
            LED_RED_OFF;
        }
    }
}

