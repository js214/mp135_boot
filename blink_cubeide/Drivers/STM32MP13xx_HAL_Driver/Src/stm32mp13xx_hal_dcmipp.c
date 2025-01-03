/**
  ******************************************************************************
  * @file    stm32mp13xx_hal_dcmipp.c
  * @author  MCD Application Team
  * @brief   DCMIPP HAL module driver
  *          This file provides firmware functions to manage the following
  *          functionalities of the DCMIPP (Digital Camera Interface Pixel Pipeline)  peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Error functions
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
     The sequence below describes how to use this driver to capture image
     from a camera module connected to the DCMI Interface.
     This sequence does not take into account the configuration of the
     camera module, which should be made before to configure and enable
     the DCMI to capture images.

     (#) Program the required configuration through the following parameters:
         the Format, the VSPolarity,the HSPolarity, the PCKPolarity, the ExtendedDataMode
         the SynchroMode, the SynchroCodes of the frame delimite, the SwapBits and the SwapCycles
         using HAL_DCMIPP_Init() and HAL_DCMIPP_SetParallelConfig
     (#) Program the required configuration through the following parameters:
         the FrameRate using HAL_DCMIPP_PIPE_Config function.

  *** Interrupt mode IO operation ***
  ===================================
     (#) Configure Pipe parameter, destination (one or two) and Capture Mode (Snapshot or Continuous) and enable
         the capture request using the following functions:
         HAL_DCMIPP_PIPE_Start() or HAL_DCMIPP_PIPE_DoubleBufferStart().

     (#) Use HAL_DCMIPP_IRQHandler() called under DCMIPP_IRQHandler() interrupt subroutine.
     (#) At the end of frame capture request HAL_DCMIPP_IRQHandler() function is executed and user can
         add his own function by customization of function pointer PipeFrameEventCallback (member
         of DCMIPP handle structure).
     (#) In case of common error, the HAL_DCMIPP_IRQHandler() function calls the callback
         ErrorCallback otherwise in case of Pipe error
         the HAL_DCMIPP_IRQHandler() function calls the callbackPipeErrorCallback.

    (#) The Pipe capture can be suspended and resumed using the following functions
        HAL_DCMIPP_PIPE_Suspend() and HAL_DCMIPP_PIPE_Resume().

    (#) For Snapshot Mode the capture can be re-enabled using the HAL_DCMIPP_PIPE_EnableCapture();

    (#) Optionally, Program the required configuration through the following parameters:
        Client, MemoryPageSize, Traffic, MaxOutstandingTransactions, DPREGStart, DPREGEnd
        and WLRURatio using HAL_DCMIPP_SetIPPlugConfig().

    (#) Optionally, configure and Enable the CROP feature to select a rectangular
        window from the received image using HAL_DCMIPP_PIPE_SetCropConfig()
        and HAL_DCMIPP_PIPE_EnableCrop() functions.

    (#) Optionally, configure and Enable the line and bytes decimation features
        using the following functions HAL_DCMIPP_PIPE_SetLinesDecimationConfig and
        HAL_DCMIPP_PIPE_SetBytesDecimationConfig.

    (#) Optionally, configure and enable the line event using the function HAL_DCMIPP_PIPE_EnableLineEvent().

    (#) Optionally, configure and enable the Limit event using the function HAL_DCMIPP_PIPE_EnableLimitEvent().

    (#) If needed, reconfigure and change the input pixel format value, the frame rate
        value, the capture Mode , the destination memory address , the syncunmask values,
        Multiline value and Limit value using respectively
        the following functions: HAL_DCMIPP_PIPE_SetInputPixelFormat(), HAL_DCMIPP_PIPE_SetFrameRate(),
        HAL_DCMIPP_PIPE_SetCaptureMode(), HAL_DCMIPP_PIPE_SetMemoryAddress(), HAL_DCMIPP_SetSyncUnmask(),
        HAL_DCMIPP_PIPE_EnableLineEvent() and HAL_DCMIPP_PIPE_EnableLimitEvent().

    (#) To read the transferred data counter , use the HAL_DCMIPP_PIPE_GetDataCounter()

    (#) To read and reset the Frame counter of the pipe, use the following functions:
        HAL_DCMIPP_PIPE_ReadFrameCounter() and HAL_DCMIPP_PIPE_ResetFrameCounter().

    (#) The Pipe capture can be Stopped using HAL_DCMIPP_PIPE_Stop() function.


    (#) To control the DCMIPP state, use the following function: HAL_DCMIPP_GetState().

    (#) To control the DCMIPP Pipe state, use the following function: HAL_DCMIPP_PIPE_GetState().

    (#) To read the DCMIPP error code, use the following function: HAL_DCMIPP_GetError().

  *** DCMIPP HAL driver macros list ***
  =============================================
  [..]
     Below the list of most used macros in DCMIPP HAL driver :

     (+) __HAL_DCMIPP_GET_FLAG: Get the DCMIPP pending flags.
     (+) __HAL_DCMIPP_CLEAR_FLAG: Clear the DCMIPP pending flags.
     (+) __HAL_DCMIPP_ENABLE_IT: Enable the specified DCMIPP interrupts.
     (+) __HAL_DCMIPP_DISABLE_IT: Disable the specified DCMIPP interrupts.
     (+) __HAL_DCMIPP_GET_IT_SOURCE: Check whether the specified DCMIPP interrupt is enabled or not.

  *** Callback registration ***
  ===================================
  [..]
     (#) The compilation define  USE_HAL_DCMIPP_REGISTER_CALLBACKS when set to 1
          allows the user to configure dynamically the driver callbacks.
          Use function @ref HAL_DCMIPP_RegisterCallback() to register a user callback.
          Use function @ref HAL_DCMIPP_PIPE_RegisterCallback() to register a user pipe callback.

     (#) Function @ref HAL_DCMIPP_RegisterCallback() allows to register following callbacks:
         (+) ErrorCallback          : callback for Error
         (+) MspInitCallback        : DCMIPP MspInit.
         (+) MspDeInitCallback      : DCMIPP MspDeInit.
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.
     (#) Function @ref HAL_DCMIPP_PIPE_RegisterCallback() allows to register following callbacks:
         (+) PipeFrameEventCallback : callback for Pipe Frame Event.
         (+) PipeVsyncEventCallback : callback for Pipe Vsync Event.
         (+) PipeLineEventCallback  : callback for Pipe Line Event.
         (+) PipeLimitEventCallback : callback for Pipe Limit Event.
         (+) PipeErrorCallback      : callback for Pipe Error
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.

     (#) Use function @ref HAL_DCMIPP_UnRegisterCallback() to reset a callback to the default
         weak (surcharged) function.
         @ref HAL_DCMIPP_UnRegisterCallback() takes as parameters the HAL peripheral handle,
         and the Callback ID.
         This function allows to reset following callbacks:
         (+) ErrorCallback          : callback for Error
         (+) MspInitCallback        : DCMIPP MspInit.
         (+) MspDeInitCallback      : DCMIPP MspDeInit.
     (#) Use function @ref HAL_DCMIPP_PIPE_UnRegisterCallback() to reset a pipe callback to the default
         weak (surcharged) function.
         @ref HAL_DCMIPP_PIPE_UnRegisterCallback() takes as parameters the HAL peripheral handle,
         and the Callback ID.
         This function allows to reset following callbacks:
         (+) PipeFrameEventCallback : callback for Pipe Frame Event.
         (+) PipeVsyncEventCallback : callback for Pipe Vsync Event.
         (+) PipeLineEventCallback  : callback for Pipe Line Event.
         (+) PipeLimitEventCallback : callback for Pipe Limit Event.
         (+) PipeErrorCallback      : callback for Pipe Error

     (#) By default, after the @ref HAL_DCMIPP_Init and if the state is HAL_DCMIPP_STATE_RESET
         all callbacks are reset to the corresponding legacy weak (surcharged) functions:
         examples @ref PipeFrameEventCallback(), @ref PipeVsyncEventCallback()
         Exception done for MspInit and MspDeInit callbacks that are respectively
         reset to the legacy weak (surcharged) functions in the @ref HAL_DCMIPP_Init
         and @ref HAL_DCMIPP_DeInit only when these callbacks are null (not registered beforehand)
         If not, MspInit or MspDeInit are not null, the @ref HAL_DCMIPP_Init and @ref HAL_DCMIPP_DeInit
         keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

         Callbacks can be registered/unregistered in READY state only.
         Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
         in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
         during the Init/DeInit.
         In that case first register the MspInit/MspDeInit user callbacks
         using @ref HAL_DCMIPP_RegisterCallback before calling @ref HAL_DCMIPP_DeInit
         or @ref HAL_DCMIPP_Init function.

         When The compilation define USE_HAL_DCMIPP_REGISTER_CALLBACKS is set to 0 or
         not defined, the callback registering feature is not available
         and weak (surcharged) callbacks are used.

     [..]
      (@) You can refer to the DCMIPP HAL driver header file for more useful macros
  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"

#ifdef HAL_DCMIPP_MODULE_ENABLED
#if defined (DCMIPP)

/** @addtogroup STM32MP13xx_HAL_Driver
  * @{
  */
/** @defgroup DCMIPP DCMIPP
  * @brief DCMIPP HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup DCMIPP_Private_Functions DCMIPP Private Functions
  * @{
  */
static void Pipe_Config(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, const DCMIPP_PipeConfTypeDef *pPipeConfig);
/**
  * @}
  */
/* Private functions ---------------------------------------------------------*/
/** @defgroup DCMIPP_Private_Functions DCMIPP Private Functions
  * @{
  */
/**
  * @brief  Configure the Pipe
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *                  the configuration information for DCMIPP.
  * @param  pPipeConfig pointer to the pipe configuration structure
  * @param  Pipe
  * @retval None
  */
static void Pipe_Config(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, const DCMIPP_PipeConfTypeDef *pPipeConfig)
{
  if (Pipe == DCMIPP_PIPE0)
  {
    /* Configure Pipe0 */
    /* Configure Frame Rate */
    MODIFY_REG(phdcmipp->Instance->P0FCTCR, DCMIPP_P0FCTCR_FRATE, pPipeConfig->FrameRate);
  }
}
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup DCMIPP_Exported_Functions DCMIPP Exported Functions
  * @{
  */

/** @addtogroup DCMIPP_Exported_Functions_Group1 Initialization and De-Initialization functions
  *  @brief   Initialization and De-Initialization Functions
  * @{
  */

/**
  * @brief  Initializes the DCMIPP according to the specified
  *         parameters in the DCMIPP_InitTypeDef and create the associated handle.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *                  the configuration information for DCMIPP.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_Init(DCMIPP_HandleTypeDef *phdcmipp)
{
  /* Check pointer validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* Check function parameters */
  assert_param(IS_DCMIPP_ALL_INSTANCE(phdcmipp->Instance));

  if (phdcmipp->State == HAL_DCMIPP_STATE_RESET)
  {
    /* Init the DCMIPP Callback settings */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
    /* Reset callback pointers to the weak predefined callbacks */
    phdcmipp->PIPE_FrameEventCallback = HAL_DCMIPP_PIPE_FrameEventCallback; /* Legacy weak PIPE_FrameEventCallback  */
    phdcmipp->PIPE_VsyncEventCallback = HAL_DCMIPP_PIPE_VsyncEventCallback; /* Legacy weak PIPE_VsyncEventCallback  */
    phdcmipp->PIPE_LineEventCallback  = HAL_DCMIPP_PIPE_LineEventCallback;  /* Legacy weak PIPE_LineEventCallback   */
    phdcmipp->PIPE_ErrorCallback      = HAL_DCMIPP_PIPE_ErrorCallback;      /* Legacy weak PIPE_ErrorCallback       */
    phdcmipp->ErrorCallback           = HAL_DCMIPP_ErrorCallback;           /* Legacy weak _ErrorCallback           */

    if (phdcmipp->MspInitCallback == NULL)
    {
      /* Legacy weak MspInit Callback        */
      phdcmipp->MspInitCallback = HAL_DCMIPP_MspInit;
    }
    /* Initialize the low level hardware (MSP) */
    phdcmipp->MspInitCallback(phdcmipp);
#else
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_DCMIPP_MspInit(phdcmipp);
#endif /* (USE_HAL_DCMIPP_REGISTER_CALLBACKS) */
  }

  /* Change the DCMIPP state */
  phdcmipp->State = HAL_DCMIPP_STATE_BUSY;

  /* Reset DCMIPP Pipe state */
  phdcmipp->PipeState[DCMIPP_PIPE0] = HAL_DCMIPP_PIPE_STATE_RESET;

  /* Update error code */
  phdcmipp->ErrorCode = HAL_DCMIPP_ERROR_NONE;

  /* Update the DCMIPP state*/
  phdcmipp->State = HAL_DCMIPP_STATE_INIT;

  return HAL_OK;
}

/**
  * @brief  De-initializes the DCMIPP peripheral registers to their default reset
  *         values.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_DeInit(DCMIPP_HandleTypeDef *phdcmipp)
{
  /* Check pointer validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* Disable the Parallel Interface */
  phdcmipp->Instance->PRCR &= ~DCMIPP_PRCR_ENABLE;

  /* Reset flow selection configuration register for the Pipe0 */
  phdcmipp->Instance->P0FSCR = 0;

#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
  if (phdcmipp->MspDeInitCallback == NULL)
  {
    phdcmipp->MspDeInitCallback = HAL_DCMIPP_MspDeInit;
  }

  /* DeInit the low level hardware */
  phdcmipp->MspDeInitCallback(phdcmipp);
#else
  /* DeInit the low level hardware */
  HAL_DCMIPP_MspDeInit(phdcmipp);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */

  /* Update error code */
  phdcmipp->ErrorCode = HAL_DCMIPP_ERROR_NONE;

  /* Initialize the DCMIPP state*/
  phdcmipp->State = HAL_DCMIPP_STATE_RESET;

  /* Reset DCMIPP Pipe state */
  phdcmipp->PipeState[DCMIPP_PIPE0] = HAL_DCMIPP_PIPE_STATE_RESET;

  return HAL_OK;
}

/**
  * @brief  Initializes the DCMIPP MSP.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @retval None
  */
__weak void HAL_DCMIPP_MspInit(DCMIPP_HandleTypeDef *phdcmipp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phdcmipp);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_MspInit could be implemented in the user file
   */
}

/**
  * @brief  De-Initializes the DCMIPP MSP.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @retval None
  */
__weak void HAL_DCMIPP_MspDeInit(DCMIPP_HandleTypeDef *phdcmipp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phdcmipp);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_MspDeInit could be implemented in the user file
   */
}
/**
  * @}
  */

/** @defgroup DCMIPP_Exported_Functions_Group2 Configuration Functions
  * @brief    Configuration Functions
  * @{
  */
/**
  * @brief  Configure the DCMIPP Parallel Interface according to the specified
  *         parameters in the pParallelConfig.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  pParallelConfig pointer to DCMIPP_ParallelConfTypeDef that contains
  *                         the parallel Interface configuration information for DCMIPP.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_SetParallelConfig(DCMIPP_HandleTypeDef *phdcmipp,
                                               const DCMIPP_ParallelConfTypeDef *pParallelConfig)
{
  uint32_t prcr_reg;
  uint32_t prescr_reg;

  /* Check parameters */
  if ((phdcmipp == NULL) || (pParallelConfig == NULL))
  {
    return HAL_ERROR;
  }

  /* Check function parameters */
  assert_param(IS_DCMIPP_FORMAT(pParallelConfig->Format));
  assert_param(IS_DCMIPP_VSPOLARITY(pParallelConfig->VSPolarity));
  assert_param(IS_DCMIPP_HSPOLARITY(pParallelConfig->HSPolarity));
  assert_param(IS_DCMIPP_PCKPOLARITY(pParallelConfig->PCKPolarity));
  assert_param(IS_DCMIPP_EXTENDED_DATA_MODE(pParallelConfig->ExtendedDataMode));
  assert_param(IS_DCMIPP_SYNC_MODE(pParallelConfig->SynchroMode));
  assert_param(IS_DCMIPP_SWAP_BITS(pParallelConfig->SwapBits));
  assert_param(IS_DCMIPP_SWAP_CYCLES(pParallelConfig->SwapCycles));

  /* Check DCMIPP state */
  if (phdcmipp->State != HAL_DCMIPP_STATE_INIT)
  {
    return HAL_ERROR;
  }
  else
  {

    /* Configures the Format, VS, HS, PCK polarity, ExtendedDataMode, SynchronisationMode, Swap Cycles and bits */
    prcr_reg  = ((pParallelConfig->Format)           | \
                 (pParallelConfig->VSPolarity)       | \
                 (pParallelConfig->HSPolarity)       | \
                 (pParallelConfig->PCKPolarity)      | \
                 (pParallelConfig->ExtendedDataMode) | \
                 (pParallelConfig->SynchroMode)      | \
                 (pParallelConfig->SwapCycles)       | \
                 (pParallelConfig->SwapBits));

    WRITE_REG(phdcmipp->Instance->PRCR, prcr_reg);

    if (pParallelConfig->SynchroMode == DCMIPP_SYNCHRO_EMBEDDED)
    {
      /* Set Embedded Sync codes */
      prescr_reg = (((uint32_t)pParallelConfig->SynchroCodes.FrameEndCode << DCMIPP_PRESCR_FEC_Pos)   | \
                    ((uint32_t)pParallelConfig->SynchroCodes.LineEndCode << DCMIPP_PRESCR_LEC_Pos)    | \
                    ((uint32_t)pParallelConfig->SynchroCodes.FrameStartCode << DCMIPP_PRESCR_FSC_Pos) | \
                    ((uint32_t)pParallelConfig->SynchroCodes.LineStartCode << DCMIPP_PRESCR_LSC_Pos));

      WRITE_REG(phdcmipp->Instance->PRESCR, prescr_reg);

      /* Set Embedded Sync Unmask codes : All codes are unmasked */
      WRITE_REG(phdcmipp->Instance->PRESUR, 0xFFFFFFFFU);
    }

    /* Enable the Synchronization error interrupt on parallel interface */
    __HAL_DCMIPP_ENABLE_IT(phdcmipp, DCMIPP_IT_PARALLEL_SYNC_ERROR);

    /* Enable Parallel interface enable */
    phdcmipp->Instance->PRCR |= DCMIPP_PRCR_ENABLE;
  }

  /* Update the DCMIPP state */
  phdcmipp->State = HAL_DCMIPP_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  Configure pipe
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for the DCMIPP.
  * @param  pPipeConfig pointer to pipe configuration structure
  * @param  Pipe  pipe to be configured
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_Config(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe,
                                         const DCMIPP_PipeConfTypeDef *pPipeConfig)
{
  HAL_DCMIPP_PipeStateTypeDef pipe_state;

  /* Check the DCMIPP peripheral handle parameter and pPipeConfig parameter*/
  if ((phdcmipp == NULL) || (pPipeConfig == NULL))
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_DCMIPP_ALL_INSTANCE(phdcmipp->Instance));
  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_FRAME_RATE(pPipeConfig->FrameRate));
  /* Get Pipe State */
  pipe_state = phdcmipp->PipeState[Pipe];

  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    if ((pipe_state == HAL_DCMIPP_PIPE_STATE_RESET) || \
        (pipe_state == HAL_DCMIPP_PIPE_STATE_ERROR))
    {
      /* Update the DCMIPP PIPE state */
      phdcmipp->PipeState[Pipe] = HAL_DCMIPP_PIPE_STATE_BUSY;

      /* Initialize the DCMIPP Pipe registers */
      Pipe_Config(phdcmipp, Pipe, pPipeConfig);

      /* Update the DCMIPP pipe state */
      phdcmipp->PipeState[Pipe] = HAL_DCMIPP_PIPE_STATE_READY;
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Configure the DCMIPP AXI master memory IP-Plug.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  pIPPlugConfig pointer to IPPLUG configuration structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_SetIPPlugConfig(DCMIPP_HandleTypeDef *phdcmipp,
                                             const DCMIPP_IPPlugConfTypeDef *pIPPlugConfig)
{
  uint32_t timeout = HAL_GetTick();

  /* Check handle validity */
  if ((phdcmipp == NULL) || (pIPPlugConfig == NULL))
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_DCMIPP_CLIENT(pIPPlugConfig->Client));
  assert_param(IS_DCMIPP_DPREG_END(pIPPlugConfig->DPREGEnd));
  assert_param(IS_DCMIPP_DPREG_START(pIPPlugConfig->DPREGStart));
  assert_param(IS_DCMIPP_MAX_OUTSTANDING_TRANSACTIONS(pIPPlugConfig->MaxOutstandingTransactions));
  assert_param(IS_DCMIPP_MEMORY_PAGE_SIZE(pIPPlugConfig->MemoryPageSize));
  assert_param(IS_DCMIPP_TRAFFIC(pIPPlugConfig->Traffic));
  assert_param(IS_DCMIPP_WLRU_RATIO(pIPPlugConfig->WLRURatio));


  if (phdcmipp->State != HAL_DCMIPP_STATE_RESET)
  {
    /* IP-PLUG Client1 configuration */
    if (pIPPlugConfig->Client == DCMIPP_CLIENT1)
    {
      /* Request to lock the IP-Plug, to allow reconfiguration */
      SET_BIT(phdcmipp->Instance->IPGR2, DCMIPP_IPGR2_PSTART);

      do
      {
        timeout--;
        if (timeout == 0U)
        {
          return HAL_ERROR;
        }
      } while ((phdcmipp->Instance->IPGR3 & DCMIPP_IPGR3_IDLE_Msk) != DCMIPP_IPGR3_IDLE);
      /* IP-Plug is currently locked and can be reconfigured */

      /* Set Memory page size */
      phdcmipp->Instance->IPGR1 = (pIPPlugConfig->MemoryPageSize);

      /* Set Traffic : Burst size and Maximum Outstanding transactions */
      phdcmipp->Instance->IPC1R1 = ((uint32_t)pIPPlugConfig->Traffic |
                                    ((uint32_t)pIPPlugConfig->MaxOutstandingTransactions));

      /* Set End word and Start Word of the FIFO of the Clientx */
      phdcmipp->Instance->IPC1R3 = (((uint32_t)pIPPlugConfig->DPREGStart << DCMIPP_IPC1R3_DPREGSTART_Pos) |
                                    ((uint32_t)pIPPlugConfig->DPREGEnd << DCMIPP_IPC1R3_DPREGEND_Pos));
      /* Set End word and Start Word of the FIFO of the Clientx */
      phdcmipp->Instance->IPC1R2 = ((uint32_t)pIPPlugConfig->WLRURatio << DCMIPP_IPC1R2_WLRU_Pos);

      /* No lock requested, IP-Plug runs on demand by background HW */
      CLEAR_BIT(phdcmipp->Instance->IPGR2, DCMIPP_IPGR2_PSTART);

      /* Enable DCMIPP_IT_AXI_TRANSFER_ERR */
      __HAL_DCMIPP_ENABLE_IT(phdcmipp, DCMIPP_IT_AXI_TRANSFER_ERROR);
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup DCMIPP_Exported_Functions_Group3 IO operation functions
  *  @brief   I/O operation functions
  * @{
  */
/**
  * @brief  Enables DCMIPP capture on the specified pipe
  * @param  phdcmipp    Pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe  pipe to be started
  * @param  DstAddress the destination address
  * @param  CaptureMode DCMIPP capture mode for the pipe can be a value from @ref DCMIPP_Capture_Mode.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_Start(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t DstAddress,
                                        uint32_t CaptureMode)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_CAPTURE_MODE(CaptureMode));

  /* Check pointer validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* Check DCMIPP pipe state */
  if (phdcmipp->PipeState[Pipe]  != HAL_DCMIPP_PIPE_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* Update the DCMIPP pipe State */
  phdcmipp->PipeState[Pipe] = HAL_DCMIPP_PIPE_STATE_BUSY;

  if (Pipe == DCMIPP_PIPE0)
  {
    /* Set the capture mode */
    phdcmipp->Instance->P0FCTCR |= CaptureMode;

    /* Set the destination address */
    WRITE_REG(phdcmipp->Instance->P0PPM0AR1, DstAddress);

    /* Enable all required interrupts lines for the PIPE0 */
    __HAL_DCMIPP_ENABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_FRAME | DCMIPP_IT_PIPE0_VSYNC | DCMIPP_IT_PIPE0_OVR);

    /* Activate the Pipe */
    SET_BIT(phdcmipp->Instance->P0FSCR, DCMIPP_P0FSCR_PIPEN);

    /* Start the capture */
    SET_BIT(phdcmipp->Instance->P0FCTCR, DCMIPP_P0FCTCR_CPTREQ);

  }
  else
  {
    /* Update the DCMIPP pipe State */
    phdcmipp->PipeState[DCMIPP_PIPE0] = HAL_DCMIPP_PIPE_STATE_READY;

    return HAL_ERROR;
  }

  return HAL_OK;
}


/**
  * @brief  Enables DCMIPP capture on the specified pipe with double buffering Mode Enabled
  * @param  phdcmipp    Pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe  pipe to be started
  * @param  DstAddress0 the first destination address
  * @param  DstAddress1 the second destination address
  * @param  CaptureMode DCMIPP capture mode for the pipe can be a value from @ref DCMIPP_Capture_Mode.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_DoubleBufferStart(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t DstAddress0,
                                                    uint32_t DstAddress1, uint32_t CaptureMode)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_CAPTURE_MODE(CaptureMode));

  /* Check pointer validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* Check DCMIPP pipe state */
  if (phdcmipp->PipeState[Pipe]  != HAL_DCMIPP_PIPE_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* Update the DCMIPP pipe State */
  phdcmipp->PipeState[Pipe] = HAL_DCMIPP_PIPE_STATE_BUSY;

  if (Pipe == DCMIPP_PIPE0)
  {
    /* Set the capture mode */
    phdcmipp->Instance->P0FCTCR |= CaptureMode;

    /* Set the destination address */
    WRITE_REG(phdcmipp->Instance->P0PPM0AR1, DstAddress0);

    /* Set the second destination address */
    WRITE_REG(phdcmipp->Instance->P0PPM0AR2, DstAddress1);

    /* Enable Double buffering Mode */
    SET_BIT(phdcmipp->Instance->P0PPCR, DCMIPP_P0PPCR_DBM);

    /* Enable all required interrupts lines for the Pipe0 */
    __HAL_DCMIPP_ENABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_FRAME | DCMIPP_IT_PIPE0_VSYNC | DCMIPP_IT_PIPE0_OVR);

    /* Activate the Pipe */
    SET_BIT(phdcmipp->Instance->P0FSCR, DCMIPP_P0FSCR_PIPEN);

    /* Start the capture */
    SET_BIT(phdcmipp->Instance->P0FCTCR, DCMIPP_P0FCTCR_CPTREQ);
  }
  else
  {
    /* Update the DCMIPP pipe State */
    phdcmipp->PipeState[0] = HAL_DCMIPP_PIPE_STATE_READY;

    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Stop DCMIPP capture on the specified pipe
  * @param  phdcmipp    Pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe    Pipe to be stopped
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_Stop(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  uint32_t timeout = HAL_GetTick();
  HAL_DCMIPP_PipeStateTypeDef pipe_state;

  assert_param(IS_DCMIPP_PIPE(Pipe));

  /* Check pointer validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* Get Pipe State */
  pipe_state = phdcmipp->PipeState[Pipe];
  /* Check DCMIPP Pipe state */
  if ((pipe_state == HAL_DCMIPP_PIPE_STATE_BUSY) || \
      (pipe_state == HAL_DCMIPP_PIPE_STATE_READY) || \
      (pipe_state == HAL_DCMIPP_PIPE_STATE_ERROR))
  {
    if (Pipe == DCMIPP_PIPE0)
    {
      /* Stop the capture */
      CLEAR_BIT(phdcmipp->Instance->P0FCTCR, DCMIPP_P0FCTCR_CPTREQ);

      /* Check that the Capture Request is effectively stopped */
      do
      {
        timeout--;
        if (timeout == 0U)
        {
          return HAL_ERROR;
        }
      } while ((phdcmipp->Instance->P0FCTCR & DCMIPP_P0FCTCR_CPTREQ) != 0U);

      /* Disable DBM when enabled */
      if ((phdcmipp->Instance->P0PPCR & DCMIPP_P0PPCR_DBM) == DCMIPP_P0PPCR_DBM)
      {
        CLEAR_BIT(phdcmipp->Instance->P0PPCR, DCMIPP_P0PPCR_DBM);
      }

      /* Disable the pipe */
      CLEAR_BIT(phdcmipp->Instance->P0FSCR, DCMIPP_P0FSCR_PIPEN);

      /* Disable all interrupts for this pipe */
      __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_FRAME | DCMIPP_IT_PIPE0_VSYNC | DCMIPP_IT_PIPE0_LINE | \
                              DCMIPP_IT_PIPE0_LIMIT | DCMIPP_IT_PIPE0_OVR);

    }
    else
    {
      return HAL_ERROR;
    }

    /* Update DCMIPP Pipe State */
    phdcmipp->PipeState[Pipe] = HAL_DCMIPP_PIPE_STATE_READY;
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Suspend DCMIPP Pipe capture
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe Pipe to be suspended
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_Suspend(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));

  uint32_t timeout = HAL_GetTick();

  if (phdcmipp == NULL)
  {
    /* Return Function Status */
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    /* Check Pipe0 State */
    if (phdcmipp->PipeState[0] == HAL_DCMIPP_PIPE_STATE_BUSY)
    {
      /* Disable Capture Request */
      phdcmipp->Instance->P0FCTCR &= ~DCMIPP_P0FCTCR_CPTREQ;
      /* Change Pipe State */
      phdcmipp->PipeState[0] = HAL_DCMIPP_PIPE_STATE_SUSPEND;

      /* Check if the DCMIPP capture effectively disabled */
      do
      {
        timeout-- ;
        if (timeout == 0U)
        {
          /* Change Pipe State */
          phdcmipp->PipeState[Pipe] = HAL_DCMIPP_PIPE_STATE_ERROR;

          return HAL_ERROR;
        }
      } while ((phdcmipp->Instance->P0FCTCR & DCMIPP_P0FCTCR_CPTREQ) != 0U);

    }
    else
    {
      /* Return Function Status */
      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  /* Return Function Status */
  return HAL_OK;
}

/**
  * @brief  Resume DCMIPP Pipe capture
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe Pipe to be resumed
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_Resume(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));

  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    /* Check Pipe0 State */
    if (phdcmipp->PipeState[0] == HAL_DCMIPP_PIPE_STATE_SUSPEND)
    {
      /* Enable Capture Request */
      phdcmipp->Instance->P0FCTCR |= DCMIPP_P0FCTCR_CPTREQ;

      /* Change Pipe State */
      phdcmipp->PipeState[0] = HAL_DCMIPP_PIPE_STATE_BUSY;
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup DCMIPP_Exported_Functions_Group4 IRQ and Callbacks functions
  * @brief    IRQ and Callbacks functions
  * @{
  */

/** @addtogroup DCMIPP_IRQHandler_functions IRQHandler functions
  * @{
  */
/**
  * @brief  Handles DCMIPP interrupt request.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for the DCMIPP.
  * @retval None
  */
void HAL_DCMIPP_IRQHandler(DCMIPP_HandleTypeDef *phdcmipp)
{
  uint32_t cmsr2flags = READ_REG(phdcmipp->Instance->CMSR2);
  uint32_t cmierflags = READ_REG(phdcmipp->Instance->CMIER);

  /* ========================= PIPE0 INTERRUPTS ==================== */
  /* Limit error on the PIPE0 ********************************************/
  if ((cmsr2flags & DCMIPP_FLAG_PIPE0_LIMIT) != 0U)
  {
    if ((cmierflags & DCMIPP_IT_PIPE0_LIMIT) != 0U)
    {
      /* Disable Limit error Interrupt for pipe0 */
      __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_LIMIT);

      /* Update error code */
      phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_PIPE0_LIMIT;

      /* Clear the Limit error flag */
      __HAL_DCMIPP_CLEAR_FLAG(phdcmipp, DCMIPP_FLAG_PIPE0_LIMIT);

      /* LIMIT Callback */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
      phdcmipp->PIPE_LimitEventCallback(phdcmipp, DCMIPP_PIPE0);
#else
      HAL_DCMIPP_PIPE_LimitEventCallback(phdcmipp, DCMIPP_PIPE0);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
    }
  }

  /* VSYNC interrupt management **********************************************/
  if ((cmsr2flags & DCMIPP_FLAG_PIPE0_VSYNC) != 0U)
  {
    if ((cmierflags & DCMIPP_IT_PIPE0_VSYNC) != 0U)
    {
      /* Clear the VSYNC flag for pipe0 */
      __HAL_DCMIPP_CLEAR_FLAG(phdcmipp, DCMIPP_FLAG_PIPE0_VSYNC);

      /* VSYNC Callback */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
      phdcmipp->PIPE_VsyncEventCallback(phdcmipp, DCMIPP_PIPE0);
#else
      HAL_DCMIPP_PIPE_VsyncEventCallback(phdcmipp, DCMIPP_PIPE0);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
    }
  }

  /* FRAME interrupt management ****************************/
  if ((cmsr2flags & DCMIPP_FLAG_PIPE0_FRAME) != 0U)
  {
    if ((cmierflags & DCMIPP_IT_PIPE0_FRAME) != 0U)
    {
      /* When snapshot mode, disable Vsync, Error and Overrun interrupts */
      if ((phdcmipp->Instance->P0FCTCR & DCMIPP_P0FCTCR_CPTMODE) == DCMIPP_MODE_SNAPSHOT)
      {
        __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_FRAME | DCMIPP_IT_PIPE0_VSYNC | DCMIPP_IT_PIPE0_OVR);

        /* Update Pipe State */
        phdcmipp->PipeState[0] = HAL_DCMIPP_PIPE_STATE_READY;
      }

      /* Clear the End of Frame flag */
      __HAL_DCMIPP_CLEAR_FLAG(phdcmipp, DCMIPP_FLAG_PIPE0_FRAME);

      /* Frame Callback */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
      phdcmipp->PIPE_FrameEventCallback(phdcmipp, DCMIPP_PIPE0);
#else
      HAL_DCMIPP_PIPE_FrameEventCallback(phdcmipp, DCMIPP_PIPE0);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
    }
  }

  /* LINE interrupt management **********************************************/
  if ((cmsr2flags & DCMIPP_FLAG_PIPE0_LINE) != 0U)
  {
    if ((cmierflags & DCMIPP_IT_PIPE0_LINE) != 0U)
    {
      /* Clear the LINE flag */
      __HAL_DCMIPP_CLEAR_FLAG(phdcmipp, DCMIPP_FLAG_PIPE0_LINE);

      /* LINE Callback */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
      phdcmipp->PIPE_LineEventCallback(phdcmipp, DCMIPP_PIPE0);
#else
      HAL_DCMIPP_PIPE_LineEventCallback(phdcmipp, DCMIPP_PIPE0);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
    }
  }

  /* Overrun error interrupt for Pipe0 ***************************************/
  if ((cmsr2flags & DCMIPP_FLAG_PIPE0_OVR) != 0U)
  {
    if ((cmierflags & DCMIPP_IT_PIPE0_OVR) != 0U)
    {
      /* Disable Overrun Error Interrupt for pipe0 */
      __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_OVR);

      /* Update error code */
      phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_PIPE0_OVR;

      /* Clear the overrun error flag */
      __HAL_DCMIPP_CLEAR_FLAG(phdcmipp, DCMIPP_FLAG_PIPE0_OVR);

      /* Change DCMIPP Pipe state */
      phdcmipp->PipeState[0] = HAL_DCMIPP_PIPE_STATE_ERROR;

      /* Error Callback */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
      phdcmipp->PIPE_ErrorCallback(phdcmipp, DCMIPP_PIPE0);
#else
      HAL_DCMIPP_PIPE_ErrorCallback(phdcmipp, DCMIPP_PIPE0);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
    }
  }


  /* Synchronization Error Interrupt on the parallel interface  **************/
  if ((cmsr2flags & DCMIPP_FLAG_PARALLEL_SYNC_ERROR) != 0U)
  {
    if ((cmierflags & DCMIPP_IT_PARALLEL_SYNC_ERROR) != 0U)
    {
      /* Disable Synchronization error interrupt on parallel interface */
      __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_PARALLEL_SYNC_ERROR);

      /* Update error code */
      phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_PARALLEL_SYNC;

      /* Clear the synchronization error flag */
      __HAL_DCMIPP_CLEAR_FLAG(phdcmipp, DCMIPP_FLAG_PARALLEL_SYNC_ERROR);

      /* Change DCMIPP state */
      phdcmipp->State = HAL_DCMIPP_STATE_ERROR;

      /* Error Callback */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
      phdcmipp->ErrorCallback(phdcmipp);
#else
      HAL_DCMIPP_ErrorCallback(phdcmipp);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
    }
  }

  /* IPPLUG AXI transfer Error Interrupt     *********************************/
  if ((cmsr2flags & DCMIPP_FLAG_AXI_TRANSFER_ERROR) != 0U)
  {
    if ((cmierflags & DCMIPP_IT_AXI_TRANSFER_ERROR) != 0U)
    {
      /* Disable IPPLUG AXI transfer Error Interrupt */
      __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_AXI_TRANSFER_ERROR);

      /* Update error code */
      phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_AXI_TRANSFER;

      /* Clear the AXI transfer error flag */
      __HAL_DCMIPP_CLEAR_FLAG(phdcmipp, DCMIPP_FLAG_AXI_TRANSFER_ERROR);

      /* Change DCMIPP state */
      phdcmipp->State = HAL_DCMIPP_STATE_ERROR;

      /* Error Callback */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
      phdcmipp->ErrorCallback(phdcmipp);
#else
      HAL_DCMIPP_ErrorCallback(phdcmipp);
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
    }
  }
}
/**
  * @}
  */

/** @addtogroup DCMIPP_Callback_functions Callback functions
  * @{
  */
/**
  * @brief  Frame Event callback on the pipe
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param  Pipe  pipe to be configured
  * @retval None
  */
__weak void HAL_DCMIPP_PIPE_FrameEventCallback(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_FrameEventDumpPipeCallback could be implemented in the user file
   */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  UNUSED(phdcmipp);
}

/**
  * @brief  Vsync Event callback on pipe
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param  Pipe  pipe to be configured
  * @retval None
  */
__weak void HAL_DCMIPP_PIPE_VsyncEventCallback(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_VsyncEventDumpPipeCallback could be implemented in the user file
   */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  UNUSED(phdcmipp);
}


/**
  * @brief  Line Event callback on the pipe
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param  Pipe  pipe to be configured
  * @retval None
  */
__weak void HAL_DCMIPP_PIPE_LineEventCallback(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_LineEventMainPipeCallback could be implemented in the user file
   */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  UNUSED(phdcmipp);
}

/**
  * @brief  Limit callback on the Pipe0
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param  Pipe  pipe to be configured
  * @retval None
  */
__weak void HAL_DCMIPP_PIPE_LimitEventCallback(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_LimitEventDumpPipeCallback could be implemented in the user file
   */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  UNUSED(phdcmipp);
}

/**
  * @brief  Error callback on the pipe
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param  Pipe  pipe to be configured
  * @retval None
  */
__weak void HAL_DCMIPP_PIPE_ErrorCallback(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_SyncErrorEventCallback could be implemented in the user file
   */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  UNUSED(phdcmipp);
}


/**
  * @brief  Error callback on DCMIPP
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @retval None
  */
__weak void HAL_DCMIPP_ErrorCallback(DCMIPP_HandleTypeDef *phdcmipp)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMIPP_ErrorCallback could be implemented in the user file
   */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phdcmipp);
}
/**
  * @}
  */

/** @addtogroup DCMIPP_RegisterCallback_functions Register Callback functions
  * @{
  */
#if (USE_HAL_DCMIPP_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User DCMIPP Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param phdcmipp DCMIPP handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref  HAL_DCMIPP_ERROR_CB_ID DCMIPP Error callback ID
  *          @arg @ref  HAL_DCMIPP_MSPINIT_CB_ID DCMIPP MspInit callback ID
  *          @arg @ref  HAL_DCMIPP_MSPDEINIT_CB_ID DCMIPP MspDeInit callback ID
  * @param pCallback pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_DCMIPP_RegisterCallback(DCMIPP_HandleTypeDef *phdcmipp, HAL_DCMIPP_CallbackIDTypeDef CallbackID,
                                              pDCMIPP_CallbackTypeDef pCallback)
{

  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* Update the error code */
    phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
    return HAL_ERROR;
  }

  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_DCMIPP_MSPINIT_CB_ID :
        phdcmipp->MspInitCallback = pCallback;
        break;

      case HAL_DCMIPP_MSPDEINIT_CB_ID :
        phdcmipp->MspDeInitCallback = pCallback;
        break;

      case HAL_DCMIPP_ERROR_CB_ID :
        phdcmipp->ErrorCallback = pCallback;
        break;

      default :
        /* Update the error code */
        phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
        /* update return status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (phdcmipp->State == HAL_DCMIPP_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_DCMIPP_MSPINIT_CB_ID :
        phdcmipp->MspInitCallback = pCallback;
        break;

      case HAL_DCMIPP_MSPDEINIT_CB_ID :
        phdcmipp->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
        /* update return status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
    /* update return status */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  Unregister a DCMIPP Callback
  *         DCMIPP Callback is redirected to the weak (surcharged) predefined callback
  * @param phdcmipp DCMIPP handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref  HAL_DCMIPP_ERROR_CB_ID DCMIPP Error callback ID
  *          @arg @ref  HAL_DCMIPP_MSPINIT_CB_ID DCMIPP MspInit callback ID
  *          @arg @ref  HAL_DCMIPP_MSPDEINIT_CB_ID DCMIPP MspDeInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_DCMIPP_UnRegisterCallback(DCMIPP_HandleTypeDef *phdcmipp, HAL_DCMIPP_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_DCMIPP_MSPINIT_CB_ID :
        phdcmipp->MspInitCallback = HAL_DCMIPP_MspInit; /* Legacy weak (surcharged) Msp Init */
        break;

      case HAL_DCMIPP_MSPDEINIT_CB_ID :
        phdcmipp->MspDeInitCallback = HAL_DCMIPP_MspDeInit; /* Legacy weak (surcharged) Msp DeInit */
        break;

      default :
        /* Update the error code */
        phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
        /* update return status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (phdcmipp->State == HAL_DCMIPP_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_DCMIPP_MSPINIT_CB_ID :
        phdcmipp->MspInitCallback = HAL_DCMIPP_MspInit;   /* Legacy weak (surcharged) Msp Init */
        break;

      case HAL_DCMIPP_MSPDEINIT_CB_ID :
        phdcmipp->MspDeInitCallback = HAL_DCMIPP_MspDeInit;  /* Legacy weak (surcharged) Msp DeInit */
        break;

      default :
        /* Update the error code */
        phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
        /* update return status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
    /* update return status */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  Register a User DCMIPP Pipe Callback
  *         To be used instead of the weak (surcharged) predefined callback
  * @param phdcmipp DCMIPP handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref  HAL_DCMIPP_PIPE_FRAME_EVENT_CB_ID DCMIPP Pipe Frame event callback ID
  *          @arg @ref  HAL_DCMIPP_PIPE_VSYNC_EVENT_CB_ID DCMIPP Pipe Vsync event callback ID
  *          @arg @ref  HAL_DCMIPP_PIPE_LINE_EVENT_CB_ID DCMIPP Pipe Line event callback ID
  *          @arg @ref  HAL_DCMIPP_PIPE_LIMIT_EVENT_CB_ID DCMIPP Pipe Limit event callback ID
  *          @arg @ref  HAL_DCMIPP_PIPE_ERROR_CB_ID DCMIPP Pipe Error callback ID
  * @param pCallback pointer to the Pipe Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_RegisterCallback(DCMIPP_HandleTypeDef *phdcmipp,
                                                   HAL_DCMIPP_PIPE_CallbackIDTypeDef CallbackID,
                                                   pDCMIPP_PIPE_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* Update the error code */
    phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
    return HAL_ERROR;
  }

  if (HAL_DCMIPP_STATE_READY == phdcmipp->State)
  {
    switch (CallbackID)
    {
      case HAL_DCMIPP_PIPE_FRAME_EVENT_CB_ID :
        phdcmipp->PIPE_FrameEventCallback = pCallback;
        break;

      case HAL_DCMIPP_PIPE_VSYNC_EVENT_CB_ID :
        phdcmipp->PIPE_VsyncEventCallback = pCallback;
        break;

      case HAL_DCMIPP_PIPE_LINE_EVENT_CB_ID :
        phdcmipp->PIPE_LineEventCallback = pCallback;
        break;

      case HAL_DCMIPP_PIPE_LIMIT_EVENT_CB_ID :
        phdcmipp->PIPE_LimitEventCallback = pCallback;
        break;

      case HAL_DCMIPP_PIPE_ERROR_CB_ID :
        phdcmipp->PIPE_ErrorCallback = pCallback;
        break;

      default :
        /* Update the error code */
        phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
        /* update return status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
    /* update return status */
    status =  HAL_ERROR;
  }

  return status;
}

HAL_StatusTypeDef HAL_DCMIPP_PIPE_UnRegisterCallback(DCMIPP_HandleTypeDef *phdcmipp,
                                                     HAL_DCMIPP_PIPE_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_DCMIPP_PIPE_FRAME_EVENT_CB_ID :
        phdcmipp->PIPE_FrameEventCallback = HAL_DCMIPP_PIPE_FrameEventCallback;
        break;

      case HAL_DCMIPP_PIPE_VSYNC_EVENT_CB_ID :
        phdcmipp->PIPE_VsyncEventCallback = HAL_DCMIPP_PIPE_VsyncEventCallback;
        break;

      case HAL_DCMIPP_PIPE_LINE_EVENT_CB_ID :
        phdcmipp->PIPE_LineEventCallback = HAL_DCMIPP_PIPE_LineEventCallback;
        break;

      case HAL_DCMIPP_PIPE_LIMIT_EVENT_CB_ID :
        phdcmipp->PIPE_LimitEventCallback = HAL_DCMIPP_PIPE_LimitEventCallback;
        break;

      case HAL_DCMIPP_PIPE_ERROR_CB_ID :
        phdcmipp->PIPE_ErrorCallback = HAL_DCMIPP_PIPE_ErrorCallback;
        break;

      default :
        /* Update the error code */
        phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
        /* update return status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    phdcmipp->ErrorCode |= HAL_DCMIPP_ERROR_INVALID_CALLBACK;
    /* update return status */
    status =  HAL_ERROR;
  }

  return status;
}
#endif /* USE_HAL_DCMIPP_REGISTER_CALLBACKS */
/**
  * @}
  */
/**
  * @}
  */

/** @addtogroup DCMIPP_Exported_Functions_Group5 Peripheral Control Functions
  * @brief    Peripheral Control functions
  * @{
  */
/** @defgroup DCMIPP_Control_Functions_Group1 Control Functions
  * @{
  */
/**
  * @brief  Configure the DCMIPP CROP coordinate.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param pCropConfig pointer to DCMIPP_CropConfTypeDef structure that contains
  *        the configuration information for Crop
  * @param  Pipe pipe where crop is configured
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_SetCropConfig(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe,
                                                const DCMIPP_CropConfTypeDef *pCropConfig)
{
  uint32_t tmp1;
  uint32_t tmp2;

  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_PIPE_CROP_AREA(pCropConfig->PipeArea));
  assert_param(IS_DCMIPP_PIPE_CROP_HSTART(pCropConfig->HStart));
  assert_param(IS_DCMIPP_PIPE_CROP_HSIZE(pCropConfig->HSize));
  assert_param(IS_DCMIPP_PIPE_CROP_VSIZE(pCropConfig->VSize));
  assert_param(IS_DCMIPP_PIPE_CROP_VSTART(pCropConfig->VStart));

  /* Check handle validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    tmp1 = (phdcmipp->Instance->P0SCSZR & DCMIPP_P0SCSZR_ENABLE_Msk);
    tmp2 = (phdcmipp->Instance->PRCR & DCMIPP_PRCR_FORMAT_Msk);

    /* Verify no Crop Enabled and This bit must be kept cleared if the input format is JPEG */
    if ((tmp1 != DCMIPP_P0SCSZR_ENABLE) || (tmp2 == DCMIPP_FORMAT_BYTE))
    {
      /* Set Cropping horizontal and vertical start for Pipe0 */
      MODIFY_REG(phdcmipp->Instance->P0SCSTR, DCMIPP_P0SCSTR_HSTART | DCMIPP_P0SCSTR_VSTART,
                 (pCropConfig->HStart << DCMIPP_P0SCSTR_HSTART_Pos) |
                 (pCropConfig->VStart << DCMIPP_P0SCSTR_VSTART_Pos));

      /* Set Cropping horizontal and vertical width for Pipe0 */
      /* Set crop Area (Inner or outer) for Pipe0 */
      MODIFY_REG(phdcmipp->Instance->P0SCSZR, DCMIPP_P0SCSZR_HSIZE | DCMIPP_P0SCSZR_VSIZE | DCMIPP_P0SCSZR_POSNEG,
                 (pCropConfig->HSize << DCMIPP_P0SCSZR_HSIZE_Pos) | (pCropConfig->VSize << DCMIPP_P0SCSZR_VSIZE_Pos) |
                 (pCropConfig->PipeArea));
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Disable the Crop feature.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param  Pipe pipe where crop is disabled
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_DisableCrop(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* Check handle validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    phdcmipp->Instance->P0SCSZR &= ~(uint32_t)DCMIPP_P0SCSZR_ENABLE;
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Enable the Crop feature.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param  Pipe pipe where crop is enabled
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_EnableCrop(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));

  /* Check handle validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* This bit must be kept cleared if the input format is JPEG */
  if ((phdcmipp->Instance->PRCR & DCMIPP_PRCR_FORMAT_Msk) != DCMIPP_FORMAT_BYTE)
  {
    if (Pipe == DCMIPP_PIPE0)
    {
      SET_BIT(phdcmipp->Instance->P0SCSZR, DCMIPP_P0SCSZR_ENABLE);
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Configure the Bytes decimation.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe pipe where bytes decimation is enabled
  * @param  SelectStart can a be value from @ref DCMIPP_Byte_Start_Mode
  * @param  SelectMode can be a value from @ref DCMIPP_Byte_Select_Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_SetBytesDecimationConfig(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe,
                                                           uint32_t SelectStart, uint32_t SelectMode)
{
  uint32_t tmp;

  assert_param(IS_DCMIPP_BYTE_SELECT_START(SelectStart));
  assert_param(IS_DCMIPP_BYTE_SELECT_MODE(SelectMode));

  /* Check handle validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    /* OEBS : This bit works in conjunction with BSM field (BSM != 00) */
    /* Modes 10 and 11 work only with EDM [2:0] = 000 into the DCMIPP_PRCR */
    tmp = (phdcmipp->Instance->PRCR & DCMIPP_PRCR_EDM_Msk);
    if (((SelectStart == DCMIPP_OEBS_EVEN) && (SelectMode == DCMIPP_BSM_ALL)) ||
        ((SelectStart == DCMIPP_OEBS_ODD) && (SelectMode == DCMIPP_BSM_ALL)) ||
        ((SelectMode > DCMIPP_BSM_DATA_OUT_2) && (tmp != DCMIPP_INTERFACE_8BITS)))
    {
      return HAL_ERROR;
    }
    else
    {
      /* Set Bytes select Start and Bytes select Mode */
      MODIFY_REG(phdcmipp->Instance->P0PPCR, DCMIPP_P0PPCR_BSM | DCMIPP_P0PPCR_OEBS, (SelectStart | SelectMode));
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Configure the Lines decimation.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe pipe where Lines decimation is enabled
  * @param  SelectStart can a be value from @ref DCMIPP_Line_Start_Mode
  * @param  SelectMode can be a value from @ref DCMIPP_Line_Select_Mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_SetLinesDecimationConfig(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe,
                                                           uint32_t SelectStart, uint32_t SelectMode)
{
  assert_param(IS_DCMIPP_LINE_SELECT_MODE(SelectMode))  ;
  assert_param(IS_DCMIPP_LINE_SELECT_START(SelectStart)) ;

  /* Check handle validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    /* This bit works in conjunction with LSM field (LSM = 1) */
    if ((SelectStart == DCMIPP_OELS_EVEN) && (SelectMode == DCMIPP_LSM_ALTERNATE_2))
    {
      return HAL_ERROR;
    }
    else
    {
      /* Set Lines select Start and Bytes select Mode */
      MODIFY_REG(phdcmipp->Instance->P0PPCR, DCMIPP_P0PPCR_LSM | DCMIPP_P0PPCR_OELS, (SelectStart | SelectMode));
    }
  }
  else
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}
/**
  * @}
  */

/** @defgroup DCMIPP_Control_Functions_Group2 Runtime Functions
  * @{
  */
/**
  * @brief  Reconfigure the Frame Rate for the selected pipe
  * @param  phdcmipp   pointer to a DCMIPP_HandleTypeDfef structure
  * @param  FrameRate  the new Frame Rate value.
  * @param  Pipe       Pipe to be reconfigured
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_SetFrameRate(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t FrameRate)
{
  /* Check Parameters */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }
  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_FRAME_RATE(FrameRate));

  /* Set Frame Rate for the Pipe */
  if (Pipe == DCMIPP_PIPE0)
  {
    phdcmipp->Instance->P0FCTCR &= ~DCMIPP_P0FCTCR_FRATE;
    phdcmipp->Instance->P0FCTCR |= FrameRate;
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Reconfigure Capture Mode for the selected pipe
  * @param  phdcmipp   pointer to a DCMIPP_HandleTypeDfef structure
  * @param  CaptureMode  the new Capture Mode value.
  * @param  Pipe       Pipe to be reconfigured
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_SetCaptureMode(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t CaptureMode)
{
  /* Check Parameters */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_CAPTURE_MODE(CaptureMode));

  /* Set Capture Mode */
  if (Pipe == DCMIPP_PIPE0)
  {
    MODIFY_REG(phdcmipp->Instance->P0FCTCR, DCMIPP_P0FCTCR_CPTMODE_Msk, CaptureMode);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Re-Enable Capture for the selected pipe
  * @param  phdcmipp   pointer to a DCMIPP_HandleTypeDfef structure
  * @param  Pipe       Pipe to be re-enable capture request
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_EnableCapture(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* Check Parameters */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* Request Capture for the chosen Pipe */

  if (Pipe == DCMIPP_PIPE0)
  {
    SET_BIT(phdcmipp->Instance->P0FCTCR, DCMIPP_P0FCTCR_CPTREQ);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Reconfigure the destination memory address for the selected pipe
  * @param  phdcmipp     pointer to a DCMIPP_HandleTypeDfef structure
  * @param  Memory  the destination address to be changed can be value from DCMIPP.
  * @param  DstAddress the new destination address
  * @param  Pipe     Pipe to be reconfigured
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_SetMemoryAddress(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t Memory,
                                                   uint32_t DstAddress)
{
  /* Check Parameters */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_MEMORY_ADDRESS(Memory));

  /* Request Capture for the chosen Pipe */

  if (Pipe == DCMIPP_PIPE0)
  {
    if (Memory == DCMIPP_MEMORY_ADDRESS_0)
    {
      /* Set Memory0 destination addresses for pipe0 */
      WRITE_REG(phdcmipp->Instance->P0PPM0AR1, DstAddress);
    }
    else
    {
      if ((phdcmipp->Instance->P0PPCR & DCMIPP_P0PPCR_DBM_Msk) == DCMIPP_P0PPCR_DBM)
      {
        /* Set Memory1 destination addresses for pipe0 */
        WRITE_REG(phdcmipp->Instance->P0PPM0AR2, DstAddress);
      }
      else
      {
        return HAL_ERROR;
      }
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;

}

/**
  * @brief  Define the Data dump limit for the Pipe0.
  * @param  phdcmipp   pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe pipe where Data limit is enabled
  * @param  Limit  Data dump Limit.
  * @note   User application may resort to HAL_DCMIPP_PIPE_LimitCallback() at Limit interrupt generation.
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_EnableLimitEvent(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe,
                                                   uint32_t Limit)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_DATA_LIMIT(Limit));

  /* Check Parameters */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    /* Set and enable data limit on Pipe 0 */
    WRITE_REG(phdcmipp->Instance->P0DCLMTR, (Limit << DCMIPP_P0DCLMTR_LIMIT_Pos) | DCMIPP_P0DCLMTR_ENABLE);

    /* Enable Limit Interrupt for pipe0 */
    __HAL_DCMIPP_ENABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_LIMIT);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Disable the the Limit interrupt.
  * @param  phdcmipp   pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe pipe where Limit interrupt is disabled
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_DisableLimitEvent(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));

  /* Check Parameters */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    /* Disable data limit on Pipe 0 */
    phdcmipp->Instance->P0DCLMTR &= ~DCMIPP_P0DCLMTR_ENABLE;

    /* Disable Limit Interrupt for pipe0 */
    __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_LIMIT);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Reconfigure the input pixel format for the selected pipe
  * @param  phdcmipp     pointer to a DCMIPP_HandleTypeDfef structure
  * @param  InputPixelFormat  new pixel format value.
  * @param  Pipe     Pipe to be reconfigured
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_SetInputPixelFormat(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe,
                                                      uint32_t InputPixelFormat)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_FORMAT(InputPixelFormat));

  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  if (Pipe == DCMIPP_PIPE0)
  {
    /* Set Frame Rate */
    MODIFY_REG(phdcmipp->Instance->PRCR, DCMIPP_PRCR_FORMAT_Msk, InputPixelFormat);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}
/**
  * @brief  Set embedded synchronization delimiters unmasks.
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  SyncUnmask pointer to a DCMIPP_SyncUnmaskTypeDef structure that contains
  *                    the embedded synchronization delimiters unmasks.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_SetSyncUnmask(DCMIPP_HandleTypeDef *phdcmipp,
                                           const DCMIPP_EmbeddedSyncUnmaskTypeDef *SyncUnmask)
{
  uint32_t presur_reg;

  /* Check pointer validity */
  if ((phdcmipp == NULL) || (SyncUnmask == NULL))
  {
    return HAL_ERROR;
  }

  /* Check the DCMIPP State */
  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    /* Check that Embedded synchronisation mode is configured */
    if ((phdcmipp->Instance->PRCR & DCMIPP_PRCR_ESS_Msk) == DCMIPP_SYNCHRO_EMBEDDED)
    {
      /* Configure DCMIPP embedded synchronization unmask register */
      presur_reg = (((uint32_t)SyncUnmask->FrameStartUnmask << DCMIPP_PRESUR_FSU_Pos) | \
                    ((uint32_t)SyncUnmask->LineStartUnmask << DCMIPP_PRESUR_LSU_Pos)  | \
                    ((uint32_t)SyncUnmask->LineEndUnmask << DCMIPP_PRESUR_LEU_Pos)    | \
                    ((uint32_t)SyncUnmask->FrameEndUnmask << DCMIPP_PRESUR_FEU_Pos));

      WRITE_REG(phdcmipp->Instance->PRESUR, presur_reg);
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}


/**
  * @brief  Define the position of the line interrupt.
  * @param  phdcmipp   pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe pipe where Line event is enabled
  * @param  Line    Line Interrupt Position.
  * @note   User application may resort to HAL_DCMIPP_PIPE_LineEventCallback() at line interrupt generation.
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_EnableLineEvent(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t Line)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));
  assert_param(IS_DCMIPP_PIPE_MULTILINE(Line));

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  /* Set MultiLine configuration */
  MODIFY_REG(phdcmipp->Instance->P0PPCR, DCMIPP_P0PPCR_LINEMULT, Line);

  /* Enable Multiline Interrupt */
  __HAL_DCMIPP_ENABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_LINE);

  return HAL_OK;
}

/**
  * @brief  Disable the the line event interrupt.
  * @param  phdcmipp   pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe pipe where Line event is disabled
  * @retval  HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_DisableLineEvent(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  assert_param(IS_DCMIPP_PIPE(Pipe));

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  /* Enable Multiline Interrupt */
  __HAL_DCMIPP_DISABLE_IT(phdcmipp, DCMIPP_IT_PIPE0_LINE);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup DCMIPP_Control_Functions_Group3 Functions
  * @{
  */

/**
  * @brief  Reset the DCMIPP Pipe frame counter
  * @param  Pipe     Pipe selected to reset frame
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_ResetFrameCounter(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* Check parameters */
  assert_param(IS_DCMIPP_PIPE(Pipe));

  /* Check pointer validity */
  if (phdcmipp == NULL)
  {
    return HAL_ERROR;
  }

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  /* Check the DCMIPP State */
  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    /* Clear Frame counter */
    SET_BIT(phdcmipp->Instance->CMCR, DCMIPP_CMCR_CFC);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Read the DCMIPP frame counter
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure
  * @param  Pipe     Pipe selected to read frame counter
  * @param  pCounter pointer to store the value of the frame counter
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_ReadFrameCounter(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t *pCounter)
{

  /* Check parameters */
  assert_param(IS_DCMIPP_PIPE(Pipe));

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  /* Check pointer validity */
  if ((phdcmipp == NULL) || (pCounter == NULL))
  {
    return HAL_ERROR;
  }

  /* Check the DCMIPP State */
  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    /* Read frame counter */
    *pCounter = READ_REG(phdcmipp->Instance->CMFRCR);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Read Number of data dumped during the frame for the Pipe0
  * The counter saturates at 0x3FFFFFF. Granularity is 32-bit for all the
  * formats except for the byte stream formats (e.g. JPEG) having byte granularity
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for the DCMIPP.
  * @param  Pipe     Pipe selected to get data counter
  * @param  pCounter pointer to amount of word transferred
  * @retval Status
  */
HAL_StatusTypeDef HAL_DCMIPP_PIPE_GetDataCounter(DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe, uint32_t *pCounter)
{
  /* Check parameters */
  assert_param(IS_DCMIPP_PIPE(Pipe));

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Pipe);
  /* Check pointer validity */
  if ((phdcmipp == NULL) || (pCounter == NULL))
  {
    return HAL_ERROR;
  }

  /* Check the DCMIPP State */
  if (phdcmipp->State == HAL_DCMIPP_STATE_READY)
  {
    /* Read  Pipe0 dump counter register */
    *pCounter = READ_REG(phdcmipp->Instance->P0DCCNTR);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}


/**
  * @}
  */
/** @defgroup DCMIPP_Exported_Functions_Group6 Peripheral State and error functions
  * @{
  */

/**
  * @brief  Return the DCMIPP state
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @retval HAL state
  */
HAL_DCMIPP_StateTypeDef HAL_DCMIPP_GetState(const DCMIPP_HandleTypeDef *phdcmipp)
{
  return phdcmipp->State;
}

/**
  * @brief  Return the DCMIPP error code
  * @param  phdcmipp  pointer to a DCMIPP_HandleTypeDef structure that contains
  *              the configuration information for DCMIPP.
  * @retval DCMIPP Error Code
  */
uint32_t HAL_DCMIPP_GetError(const DCMIPP_HandleTypeDef *phdcmipp)
{
  return phdcmipp->ErrorCode;
}

/**
  * @brief  Return the DCMIPP state
  * @param  phdcmipp pointer to a DCMIPP_HandleTypeDef structure that contains
  *               the configuration information for DCMIPP.
  * @param Pipe Pipe selected to get state
  * @retval HAL state
  */
HAL_DCMIPP_PipeStateTypeDef HAL_DCMIPP_PIPE_GetState(const DCMIPP_HandleTypeDef *phdcmipp, uint32_t Pipe)
{
  /* Check Parameters */
  assert_param(IS_DCMIPP_PIPE(Pipe));

  return phdcmipp->PipeState[Pipe];
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* DCMIPP */
#endif /* HAL_DCMIPP_MODULE_ENABLED */
/**
  * @}
  */

