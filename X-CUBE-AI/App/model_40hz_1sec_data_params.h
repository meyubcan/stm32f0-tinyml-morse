/**
  ******************************************************************************
  * @file    model_40hz_1sec_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-12-19T09:32:37+0300
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef MODEL_40HZ_1SEC_DATA_PARAMS_H
#define MODEL_40HZ_1SEC_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_MODEL_40HZ_1SEC_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_model_40hz_1sec_data_weights_params[1]))
*/

#define AI_MODEL_40HZ_1SEC_DATA_CONFIG               (NULL)


#define AI_MODEL_40HZ_1SEC_DATA_ACTIVATIONS_SIZES \
  { 1040, }
#define AI_MODEL_40HZ_1SEC_DATA_ACTIVATIONS_SIZE     (1040)
#define AI_MODEL_40HZ_1SEC_DATA_ACTIVATIONS_COUNT    (1)
#define AI_MODEL_40HZ_1SEC_DATA_ACTIVATION_1_SIZE    (1040)



#define AI_MODEL_40HZ_1SEC_DATA_WEIGHTS_SIZES \
  { 3504, }
#define AI_MODEL_40HZ_1SEC_DATA_WEIGHTS_SIZE         (3504)
#define AI_MODEL_40HZ_1SEC_DATA_WEIGHTS_COUNT        (1)
#define AI_MODEL_40HZ_1SEC_DATA_WEIGHT_1_SIZE        (3504)



#define AI_MODEL_40HZ_1SEC_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_model_40hz_1sec_activations_table[1])

extern ai_handle g_model_40hz_1sec_activations_table[1 + 2];



#define AI_MODEL_40HZ_1SEC_DATA_WEIGHTS_TABLE_GET() \
  (&g_model_40hz_1sec_weights_table[1])

extern ai_handle g_model_40hz_1sec_weights_table[1 + 2];


#endif    /* MODEL_40HZ_1SEC_DATA_PARAMS_H */
