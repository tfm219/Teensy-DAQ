#ifndef ADS1258_H_
#define ADS1258_H_


#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <SPI.h>
//#include "hal.h"



//*****************************************************************************
//
// Constants
//
//*****************************************************************************
#define NUM_REGISTERS                   ( 10)


#define SPI_SPEED 8000000 //4000000 //2500000
//*****************************************************************************
//
// Command byte formatting
//
//*****************************************************************************

/* Command byte definition
 * ---------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
 * ---------------------------------------------------------------------------------
 * |            C[2:0]           |   MUL   |                A[3:0]                 |
 * ---------------------------------------------------------------------------------
 */

    /* SPI Commands */
    #define READ_DIRECT          (0x00)
    #define READ_COMMAND         (0x30)    // Includes MUL bit
    #define RREG                 (0x40)
    #define WREG                 (0x60)
    #define PULSE_CONVERT        (0x80)
    #define RESET                (0xC0)

    /* Commands byte masks */
    #define C_MASK        (0xE0)
    #define MUL_MASK      (0x10)
    #define A_MASK        (0x0F)

    /* Read mode enum */
    typedef enum { DIRECT, COMMAND } readMode;



//*****************************************************************************
//
// Status byte formatting
//
//*****************************************************************************

/* STATUS byte definition
 * ---------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
 * ---------------------------------------------------------------------------------
 * |   NEW   |   OVF   |  SUPPLY |                    CHID[4:0]                    |
 * ---------------------------------------------------------------------------------
 */

    /* STATUS byte field masks */
    #define STATUS_NEW_MASK                     (0x80)  /* Indicates new data */
    #define STATUS_OVF_MASK                     (0x40)  /* Indicates differential over-range condition */
    #define STATUS_SUPPLY_MASK                  (0x20)  /* Indicates low analog power-supply condition */
    #define STATUS_CHID_MASK                    (0x1F)  /* Channel ID bits */

    /* CHID field values */
    #define STATUS_CHID_DIFF0                   (0x00)
    #define STATUS_CHID_DIFF1                   (0x01)
    #define STATUS_CHID_DIFF2                   (0x02)
    #define STATUS_CHID_DIFF3                   (0x03)
    #define STATUS_CHID_DIFF4                   (0x04)
    #define STATUS_CHID_DIFF5                   (0x05)
    #define STATUS_CHID_DIFF6                   (0x06)
    #define STATUS_CHID_DIFF7                   (0x07)
    #define STATUS_CHID_AIN0                    (0x08)
    #define STATUS_CHID_AIN1                    (0x09)
    #define STATUS_CHID_AIN2                    (0x0A)
    #define STATUS_CHID_AIN3                    (0x0B)
    #define STATUS_CHID_AIN4                    (0x0C)
    #define STATUS_CHID_AIN5                    (0x0D)
    #define STATUS_CHID_AIN6                    (0x0E)
    #define STATUS_CHID_AIN7                    (0x0F)
    #define STATUS_CHID_AIN8                    (0x10)
    #define STATUS_CHID_AIN9                    (0x11)
    #define STATUS_CHID_AIN10                   (0x12)
    #define STATUS_CHID_AIN11                   (0x13)
    #define STATUS_CHID_AIN12                   (0x14)
    #define STATUS_CHID_AIN13                   (0x15)
    #define STATUS_CHID_AIN14                   (0x16)
    #define STATUS_CHID_AIN15                   (0x17)
    #define STATUS_CHID_OFFSET                  (0x18)
    #define STATUS_CHID_VCC                     (0x1A)
    #define STATUS_CHID_TEMP                    (0x1B)
    #define STATUS_CHID_GAIN                    (0x1C)
    #define STATUS_CHID_REF                     (0x1D)
    #define STATUS_CHID_FIXEDCHMODE             (0x1F)  /* ID for fixed-channel mode */



//*****************************************************************************
//
// Register definitions
//
//*****************************************************************************


/* Register0x00 (CONFIG0) definition
 * ---------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
 * ---------------------------------------------------------------------------------
 * |    0    |  SPIRST |  MUXMOD |  BYPAS  |  CLKENB |   CHOP  |   STAT  |    0    |
 * ---------------------------------------------------------------------------------
 */

    /** CONFIG0 register address */
    #define CONFIG0                    (0x00)

    /** CONFIG0 default (reset) value */
    #define CONFIG0_DEFAULT                     (0x0A)

    /* CONFIG0 register field masks */
    #define CONFIG0_SPIRST_MASK                 (0x40)
    #define CONFIG0_MUXMOD_MASK                 (0x20)
    #define CONFIG0_BYPAS_MASK                  (0x10)
    #define CONFIG0_CLKENB_MASK                 (0x08)
    #define CONFIG0_CHOP_MASK                   (0x04)
    #define CONFIG0_STAT_MASK                   (0x02)



/* Register0x01 (CONFIG1) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |  IDLMOD |           DLY[2:0]          |     SCBCS[1:0]    |     DRATE[0:1]    |
* ---------------------------------------------------------------------------------
*/

    /** CONFIG1 register address */
    #define CONFIG1                    (0x01)

    /** CONFIG1 default (reset) value */
    #define CONFIG1_DEFAULT                     (0x83)

    /* CONFIG1 register field masks */
    #define CONFIG1_IDLMOD_MASK                 (0x80)
    #define CONFIG1_DLY_MASK                    (0x70)
    #define CONFIG1_SCBCS_MASK                  (0x0C)
    #define CONFIG1_DRATE_MASK                  (0x03)

    /* DLY field values */
    #define CONFIG1_DLY_0us                     (0x00)
    #define CONFIG1_DLY_8us                     (0x10)
    #define CONFIG1_DLY_16us                    (0x20)
    #define CONFIG1_DLY_32us                    (0x30)
    #define CONFIG1_DLY_64us                    (0x40)
    #define CONFIG1_DLY_128us                   (0x50)
    #define CONFIG1_DLY_256us                   (0x60)
    #define CONFIG1_DLY_384us                   (0x70)

    /* SCBCS field values */
    #define CONFIG1_SCBCS_OFF                   (0x00)
    #define CONFIG1_SCBCS_1_5uA                 (0x40)
    #define CONFIG1_SCBCS_24uA                  (0xC0)

    /* DRATE field values (fixed-channel DRs shown) */
    #define CONFIG1_DRATE_1953SPS               (0x00)
    #define CONFIG1_DRATE_7813SPS               (0x01)
    #define CONFIG1_DRATE_31250SPS              (0x02)
    #define CONFIG1_DRATE_125000SPS             (0x03)



/* Register0x02 (MUXSCH) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |               AINP[3:0]               |               AINN[3:0]               |
* ---------------------------------------------------------------------------------
*/

    /** MUXSCH register address */
    #define MUXSCH                     (0x02)

    /** MUXSCH default (reset) value */
    #define MUXSCH_DEFAULT                      (0x00)

    /* MUXSCH register field masks */
    #define MUXSCH_AINP_MASK                    (0xF0)
    #define MUXSCH_AINN_MASK                    (0x0F)

    /* AINP field values */
    #define P_AIN0                    (0x00)
    #define P_AIN1                    (0x10)
    #define P_AIN2                    (0x20)
    #define P_AIN3                    (0x30)
    #define P_AIN4                    (0x40)
    #define P_AIN5                    (0x50)
    #define P_AIN6                    (0x60)
    #define P_AIN7                    (0x70)
    #define P_AIN8                    (0x80)
    #define P_AIN9                    (0x90)
    #define P_AIN10                   (0xA0)
    #define P_AIN11                   (0xB0)
    #define P_AIN12                   (0xC0)
    #define P_AIN13                   (0xD0)
    #define P_AIN14                   (0xE0)
    #define P_AIN15                   (0xF0)

    /* AINN field values */
    #define N_AIN0                    (0x00)
    #define N_AIN1                    (0x01)
    #define N_AIN2                    (0x02)
    #define N_AIN3                    (0x03)
    #define N_AIN4                    (0x04)
    #define N_AIN5                    (0x05)
    #define N_AIN6                    (0x06)
    #define N_AIN7                    (0x07)
    #define N_AIN8                    (0x08)
    #define N_AIN9                    (0x09)
    #define N_AIN10                   (0x0A)
    #define N_AIN11                   (0x0B)
    #define N_AIN12                   (0x0C)
    #define N_AIN13                   (0x0D)
    #define N_AIN14                   (0x0E)
    #define N_AIN15                   (0x0F)

    #define AIN0                    (0x00)
    #define AIN1                    (0x01)
    #define AIN2                    (0x02)
    #define AIN3                    (0x03)
    #define AIN4                    (0x04)
    #define AIN5                    (0x05)
    #define AIN6                    (0x06)
    #define AIN7                    (0x07)
    #define AIN8                    (0x08)
    #define AIN9                    (0x09)
    #define AIN10                   (0x0A)
    #define AIN11                   (0x0B)
    #define AIN12                   (0x0C)
    #define AIN13                   (0x0D)
    #define AIN14                   (0x0E)
    #define AIN15                   (0x0F)


/* Register0x03 (MUXDIF) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |  DIFF7  |  DIFF6  |  DIFF5  |  DIFF4  |  DIFF3  |  DIFF2  |  DIFF1  |  DIFF0  |
* ---------------------------------------------------------------------------------
*/

    /** MUXDIF register address */
    #define MUXDIF                     (0x03)

    /** MUXDIF default (reset) value */
    #define MUXDIF_DEFAULT                      (0x00)

    /* MUXDIF register field masks */
    #define DIFF7_ENABLE                 (0x80)
    #define DIFF6_ENABLE                 (0x40)
    #define DIFF5_ENABLE                 (0x20)
    #define DIFF4_ENABLE                 (0x10)
    #define DIFF3_ENABLE                 (0x08)
    #define DIFF2_ENABLE                 (0x04)
    #define DIFF1_ENABLE                 (0x02)
    #define DIFF0_ENABLE                 (0x01)



/* Register0x04 (MUXSG0) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |   AIN7  |   AIN6  |   AIN5  |   AIN4  |   AIN3  |   AIN2  |   AIN1  |   AIN0  |
* ---------------------------------------------------------------------------------
*/

    /** MUXSG0 register address */
    #define MUXSG0                     (0x04)

    /** MUXSG0 default (reset) value */
    #define MUXSG0_DEFAULT                      (0xFF)

    /* MUXSG0 register field masks */
    #define MUXSG0_AIN7_ENABLE                  (0x80)
    #define MUXSG0_AIN6_ENABLE                  (0x40)
    #define MUXSG0_AIN5_ENABLE                  (0x20)
    #define MUXSG0_AIN4_ENABLE                  (0x10)
    #define MUXSG0_AIN3_ENABLE                  (0x08)
    #define MUXSG0_AIN2_ENABLE                  (0x04)
    #define MUXSG0_AIN1_ENABLE                  (0x02)
    #define MUXSG0_AIN0_ENABLE                  (0x01)



/* Register0x05 (MUXSG1) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |  AIN15  |  AIN14  |  AIN13  |  AIN12  |  AIN11  |  AIN10  |   AIN9  |   AIN8  |
* ---------------------------------------------------------------------------------
*/

    /** MUXSG1 register address */
    #define MUXSG1                     (0x05)

    /** MUXSG1 default (reset) value */
    #define MUXSG1_DEFAULT                      (0xFF)

    /* MUXSG1 register field masks */
    #define MUXSG1_AIN15_ENABLE                 (0x80)
    #define MUXSG1_AIN14_ENABLE                 (0x40)
    #define MUXSG1_AIN13_ENABLE                 (0x20)
    #define MUXSG1_AIN12_ENABLE                 (0x10)
    #define MUXSG1_AIN11_ENABLE                 (0x08)
    #define MUXSG1_AIN10_ENABLE                 (0x04)
    #define MUXSG1_AIN9_ENABLE                  (0x02)
    #define MUXSG1_AIN8_ENABLE                  (0x01)



/* Register0x06 (SYSRED) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |    0    |    0    |   REF   |   GAIN  |   TEMP  |   VCC   |     0   |  OFFSET |
* ---------------------------------------------------------------------------------
*/

    /** SYSRED register address */
    #define SYSRED                     (0x06)

    /** SYSRED default (reset) value */
    #define SYSRED_DEFAULT                      (0x00)

    /* SYSRED register field masks */
    #define SYSRED_REF_ENABLE                   (0x20)
    #define SYSRED_GAIN_ENABLE                  (0x10)
    #define SYSRED_TEMP_ENABLE                  (0x08)
    #define SYSRED_VCC_ENABLE                   (0x04)
    #define SYSRED_OFFSET_ENABLE                (0x01)



/* Register0x07 (GPIOC) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |                                    CIO[7:0]                                   |
* ---------------------------------------------------------------------------------
*/

    /** GPIOC register address */
    #define GPIOC                      (0x07)

    /** GPIOC default (reset) value */
    #define GPIOC_DEFAULT                       (0xFF)

    /* GPIOC register field masks */
    #define GPIOC_GPIO7_INPUT                   (0x80)
    #define GPIOC_GPIO6_INPUT                   (0x40)
    #define GPIOC_GPIO5_INPUT                   (0x20)
    #define GPIOC_GPIO4_INPUT                   (0x10)
    #define GPIOC_GPIO3_INPUT                   (0x08)
    #define GPIOC_GPIO2_INPUT                   (0x04)
    #define GPIOC_GPIO1_INPUT                   (0x02)
    #define GPIOC_GPIO0_INPUT                   (0x01)



/* Register0x08 (GPIOD) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |                                    DIO[7:0]                                   |
* ---------------------------------------------------------------------------------
*/

    /** GPIOD register address */
    #define GPIOD                      (0x08)

    /** GPIOD default (reset) value */
    #define GPIOD_DEFAULT                       (0x00)

    /* GPIOD register field masks */
    #define GPIOD_GPIO7_HIGH                    (0x80)
    #define GPIOD_GPIO6_HIGH                    (0x40)
    #define GPIOD_GPIO5_HIGH                    (0x20)
    #define GPIOD_GPIO4_HIGH                    (0x10)
    #define GPIOD_GPIO3_HIGH                    (0x08)
    #define GPIOD_GPIO2_HIGH                    (0x04)
    #define GPIOD_GPIO1_HIGH                    (0x02)
    #define GPIOD_GPIO0_HIGH                    (0x01)



/* Register0x09 (ID) definition
* ---------------------------------------------------------------------------------
* |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
* ---------------------------------------------------------------------------------
* |                                    ID[7:0]                                    |
* ---------------------------------------------------------------------------------
*/

    /** ID register address */
    #define ID                         (0x09)
    #define ID_DEFAULT                 (0x8B)
    /* ID register field masks */
    #define ID_ID4_MASK                         (0x10)

    /* ID4 field values */
    #define ID_ID4_ADS1258                      (0x00)
    #define ID_ID4_ADS1158                      (0x10)



//*****************************************************************************
//
// Macros
//
//*****************************************************************************

/** Register bit checking macros...
 *  Return true if register bit is set (since last read or write).
 */
#define IS_MUXMOD_SET       ((bool) (getRegisterValue(CONFIG0) & CONFIG0_MUXMOD_MASK))
#define IS_STAT_SET         ((bool) (getRegisterValue(CONFIG0) & CONFIG0_STAT_MASK))



#endif /* ADS1258_H_ */
