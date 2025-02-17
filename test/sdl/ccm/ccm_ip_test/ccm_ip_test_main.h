/*
 *   Copyright (c) Texas Instruments Incorporated 2022-2024
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

 /**
 *  \file     ccm_test_main.h
 *
 *  \brief    This file contains CCM main test defines.
 *
 *  \details  CCM unit tests
 **/
#ifndef CCM_TEST_MAIN_H
#define CCM_TEST_MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <string.h>
#include <kernel/dpl/DebugP.h>
#include <sdl/include/sdl_types.h>
#include <sdl/sdl_ccm.h>
#include <sdl/r5/v0/sdlr_vim.h>
#include <sdl/r5/v0/sdl_ip_ccm.h>
#if defined(SOC_AM263X)
#include <sdl/esm/v0/sdl_esm.h>
#endif
#if defined (SOC_AM263PX) || defined (SOC_AM261X)
#include <sdl/esm/v2/sdl_esm.h>
#endif
#if defined (SOC_AM263X)
#include <sdl/include/am263x/sdlr_intr_r5fss0_core0.h>
#endif
#if defined (SOC_AM263PX)
#include <sdl/include/am263px/sdlr_intr_r5fss0_core0.h>
#endif
#if defined (SOC_AM261X)
#include <sdl/include/am261x/sdlr_intr_r5fss0_core0.h>
#endif
#if defined (SOC_AM273X)
#include <sdl/esm/v1/sdl_esm.h>
#endif
#if defined (SOC_AWR294X)
#include <sdl/esm/v1/sdl_esm.h>
#endif
/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

#define  CCM_IP_API_TEST_ID       (0U)
#define  CCM_IP_ERROR_TEST_ID     (1U)
#define  CCM_TOTAL_NUM_TESTS      (2U)
#if defined (SOC_AM263X) || defined (SOC_AM263PX) || defined (SOC_AM261X)
#define  CCM_NUM_INSTANCE         (3U)
#define INSTANCE 		SDL_R5SS0_CCM
#endif
#if defined (SOC_AM273X) || (SOC_AWR294X)
#define  CCM_NUM_INSTANCE         (2U)
#define INSTANCE 		SDL_MSS_CCMR
#endif
/* ========================================================================== */
/*                 External Function Declarations                             */
/* ========================================================================== */
extern int32_t CCM_ipApiTest(void);
extern int32_t CCM_ipErrTest(void);

#ifdef __cplusplus
}
#endif

#endif /* CCM_TEST_MAIN_H */

/* Nothing past this point */
