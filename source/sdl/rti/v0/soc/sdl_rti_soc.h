/*
 * SDL RTI
 *
 * SDL SoC Header file for RTI
 *
 *  Copyright (c) Texas Instruments Incorporated 2022
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

#ifndef INCLUDE_SDL_RTI_SOC_H_
#define INCLUDE_SDL_RTI_SOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined (SOC_AM64X)
#include <sdl/rti/v0/soc/am64x/sdl_soc_rti.h>
#endif /* SOC_AM64X */

#if defined (SOC_AM243X)
#include <sdl/rti/v0/soc/am243x/sdl_soc_rti.h>
#endif /* SOC_AM243X */

#if defined (SOC_AM263X)
#include <sdl/rti/v0/soc/am263x/sdl_soc_rti.h>
#endif /* SOC_AM263X */

#if defined (SOC_AM263PX)
#include <sdl/rti/v0/soc/am263px/sdl_soc_rti.h>
#endif /* SOC_AM263PX */

#if defined (SOC_AM261X)
#include <sdl/rti/v0/soc/am261x/sdl_soc_rti.h>
#endif /* SOC_AM261X */

#if defined (SOC_AM273X)
#include <sdl/rti/v0/soc/am273x/sdl_soc_rti.h>
#endif /* SOC_AM273X */

#if defined (SOC_AWR294X)
#include <sdl/rti/v0/soc/awr294x/sdl_soc_rti.h>
#endif /* SOC_AWR294X */

#if defined (SOC_AM64X)
#include <sdl/rti/v0/soc/am64x/sdl_soc_rti.h>
#endif /* SOC_AM64X */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_RTI_SOC_H_ */
