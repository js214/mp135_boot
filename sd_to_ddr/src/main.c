#include <stdio.h> // printf()
#include <ctype.h> // isprint()

#include "stm32mp13xx_hal.h"
#include "stm32mp13xx_hal_etzpc.h"
#include "stm32mp13xx_disco.h"
#include "stm32mp13xx_disco_stpmic1.h"

#include "setup.h"

#define READ_TIMEOUT       3000

// global variables
__IO uint8_t RxCplt;
SD_HandleTypeDef SDHandle;

void clear_ddr(const int num_words)
{
   for (int i=0; i<num_words; i++) {
      uint32_t *p = (uint32_t*)(DRAM_MEM_BASE + 4*i);
      *p = 0;
   }
}

void print_ddr(const int num_words)
{
   for (int i=0; i<num_words/4; i+=4) {
      // print address
      printf("0x%08x : ", 4*i);

      // print in hex
      for (int j=0; j<4; j++) {
         uint32_t *p = (uint32_t*)(DRAM_MEM_BASE + 4*(i+j));
         const int i0 = *p;
         for (int k=0; k<4; k++) {
            const char c = (i0 & (0xff << k*8)) >> k*8;
            printf("%02x ", c);
         }
         printf(" ");
      }

      // print as ASCII
      for (int j=0; j<4; j++) {
         uint32_t *p = (uint32_t*)(DRAM_MEM_BASE + 4*(i+j));
         const int i0 = *p;
         for (int k=0; k<4; k++) {
            const char c = (i0 & (0xff << k*8)) >> k*8;
            if (isprint(c))
               printf("%c", c);
            else
               printf(".");
         }
      }

      printf("\r\n");
   }
}


void setup_sd(void)
{
   // unsecure SYSRAM so that SDMMC1 (which we configure as non-secure) can access it
   LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

   // unsecure SDMMC1
   LL_ETZPC_Set_SDMMC1_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_WRITE_NONSECURE);

   // initialize SDMMC1
   SDHandle.Instance = SDMMC1;
   HAL_SD_DeInit(&SDHandle);

   // SDMMC IP clock xx MHz, SDCard clock xx MHz
   SDHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
   SDHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
   SDHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
   SDHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
   SDHandle.Init.ClockDiv            = SDMMC_NSPEED_CLK_DIV;

   if (HAL_SD_Init(&SDHandle) != HAL_OK) {
      printf("Error in HAL_SD_Init()\r\n");
      Error_Handler();
   }

   while (HAL_SD_GetCardState(&SDHandle) != HAL_SD_CARD_TRANSFER)
      ;
}


void read_sd_blocking(const int app_offset, const int num_blocks)
{
   if (HAL_SD_ReadBlocks(&SDHandle, (uint8_t*)DRAM_MEM_BASE, app_offset, num_blocks, READ_TIMEOUT) != HAL_OK) {
      printf("Error in HAL_SD_ReadBlocks()\r\n");
      Error_Handler();
   }
}

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
   while (HAL_SD_GetCardState(&SDHandle) != HAL_SD_CARD_TRANSFER) {}
   RxCplt=1;
}


void read_sd_nonblocking(const int app_offset, const int num_blocks)
{
//
//   /* Read application from the SD */
//   RxCplt = 0;
//   uint32_t exampleOffset = 0x00;
//   if (HAL_SD_ReadBlocks_DMA(&SDHandle, (uint8_t*)DRAM_MEM_BASE, app_offset, num_blocks) != HAL_OK) {
//      printf("Error in HAL_SD_ReadBlocks_DMA()\r\n");
//      Error_Handler();
//   }
//
//   /* Wait until Application if loaded into DDR from SD storage */
//   while (RxCplt == 0)
//      ;
}


int main(void)
{
   HAL_Init();
   SystemClock_Config();
   BSP_PMIC_Init();
   BSP_PMIC_InitRegulators();
   PeriphCommonClock_Config();
   MX_UART4_Init();
   BSP_LED_Init(LED_RED);
   BSP_LED_Init(LED_BLUE);
   setup_ddr();
   setup_sd();

   for (int i=1; i<=5; i++) {
      printf("Hello World %d\r\n", i);
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_14);
      HAL_Delay(1000);
   }

   // optional: clear the DDR memory
   clear_ddr(BLOCKSIZE / sizeof(uint32_t) * 255);

   // copy from SD card to DDR memory
   const int prog_loc = 0x4400;
   const int prog_size = 67228;
   read_sd_blocking(prog_loc/BLOCKSIZE, 132);

   // print out the first block
   print_ddr(BLOCKSIZE / 4);

   // jump to application we just loaded into DDR
   printf("Jumping to app...\r\n");
   void (*app_entry)(void);
   app_entry = (void (*)(void))(0xc0000000);
   app_entry();
}

// end file main.c

