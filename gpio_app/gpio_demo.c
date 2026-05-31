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
static void BoardGpioInit( void )
{
    /**
     * 1. GPIO Clock
     */
    CCM->CCGR1 |= CCM_CCGR1_CG13( 0x3 ); // for GPIO1
    CCM->CCGR1 |= CCM_CCGR1_CG6( 0x3 );
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

    // KEY
    IOMUXC_SetPinMux( IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0 );
    IOMUXC_SetPinConfig( IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, LED_PAD_CONFIG_DATA );

    GPIO1->GDIR |= (1<<4);  //设置GPIO1_04为输出模式
    GPIO1->DR |= (1<<4);    //设置GPIO1_04输出电平为高电平

    GPIO4->GDIR |= (1<<20);  //设置GPIO4_20为输出模式
    GPIO4->DR |= (1<<20);    //设置GPIO4_20输出电平为高电平

    GPIO4->GDIR |= (1<<19);  //设置GPIO4_19为输出模式
    GPIO4->DR |= (1<<19);    //设置GPIO4_19输出电平为高电平

    GPIO5->GDIR &= ~( 1 << 1 ); // GPIO5_1 input mode
    
}

static void delay( uint32_t count )
{
    while( --count )
    {
        __asm( "NOP" );
    }
}
int main(void)
{
    char ch;

    /* Init board hardware. */
    //BOARD_InitPins();
    //BOARD_BootClockRUN();
    //BOARD_InitMemory();
    //BOARD_InitDebugConsole();
    BoardGpioInit();
    while (1)
    {
        if( GPIO5->DR & ( 1 << 1 ) ){
            delay( 500 ); // flush shake
            if( GPIO5->DR & ( 1 << 1 ) )
            {
                //LED off
                if( GPIO1->DR & ( 1 << 4 ) ){
                    GPIO1->DR &= ~(1 << 4); // RED on
                    while( GPIO5->DR & ( 1 << 1 ) );
                }else
                {
                    GPIO1->DR |= (1 << 4); // RED off
                    while( GPIO5->DR & ( 1 << 1 ) );
                }

            }
        }
    }
}

