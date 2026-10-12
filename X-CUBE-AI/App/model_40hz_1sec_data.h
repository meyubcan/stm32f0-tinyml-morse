/**
  ******************************************************************************
  * @file    model_40hz_1sec_data.h
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

#ifndef MODEL_40HZ_1SEC_DATA_H
#define MODEL_40HZ_1SEC_DATA_H

#include "model_40hz_1sec_config.h"
#include "model_40hz_1sec_data_params.h"

AI_DEPRECATED
#define AI_MODEL_40HZ_1SEC_DATA_ACTIVATIONS(ptr_)  \
  ai_model_40hz_1sec_data_activations_buffer_get(AI_HANDLE_PTR(ptr_))

AI_DEPRECATED
#define AI_MODEL_40HZ_1SEC_DATA_WEIGHTS(ptr_)  \
  ai_model_40hz_1sec_data_weights_buffer_get(AI_HANDLE_PTR(ptr_))


AI_API_DECLARE_BEGIN


extern const ai_u64 s_model_40hz_1sec_weights_array_u64[438];



/*!
 * @brief Get network activations buffer initialized struct.
 * @ingroup model_40hz_1sec_data
 * @param[in] ptr a pointer to the activations array storage area
 * @return an ai_buffer initialized struct
 */
AI_DEPRECATED
AI_API_ENTRY
ai_buffer ai_model_40hz_1sec_data_activations_buffer_get(const ai_handle ptr);

/*!
 * @brief Get network weights buffer initialized struct.
 * @ingroup model_40hz_1sec_data
 * @param[in] ptr a pointer to the weights array storage area
 * @return an ai_buffer initialized struct
 */
AI_DEPRECATED
AI_API_ENTRY
ai_buffer ai_model_40hz_1sec_data_weights_buffer_get(const ai_handle ptr);

/*!
 * @brief Get network weights array pointer as a handle ptr.
 * @ingroup model_40hz_1sec_data
 * @return a ai_handle pointer to the weights array
 */
AI_DEPRECATED
AI_API_ENTRY
ai_handle ai_model_40hz_1sec_data_weights_get(void);


/*!
 * @brief Get network params configuration data structure.
 * @ingroup model_40hz_1sec_data
 * @return true if a valid configuration is present, false otherwise
 */
AI_API_ENTRY
ai_bool ai_model_40hz_1sec_data_params_get(ai_network_params* params);


AI_API_DECLARE_END

#endif /* MODEL_40HZ_1SEC_DATA_H */

