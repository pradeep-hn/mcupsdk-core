/*
 *  Copyright (C) 2018-2023 Texas Instruments Incorporated
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
 */

#include <stdio.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/SystemP.h>
#include <security/security_common/drivers/hsmclient/hsmclient.h>
#include <security/security_common/drivers/secure_ipc_notify/sipc_notify.h>
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <kernel/dpl/SystemP.h>
#include <string.h>
#include <drivers/hw_include/csl_types.h>
#include <drivers/hw_include/cslr_soc.h>
#include <drivers/mpu_firewall.h>
#include <drivers/soc.h>

/* ========================================================================== */
/*                          Macros And Typedefs                               */
/* ========================================================================== */

#define APP_CLIENT_ID                  (0x02)
#define PARSED_VER_SIZE                (0X96)

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Prototypes                               */
/* ========================================================================== */

void getUIDApp(HsmClient_t *client);
void getVersionApp(HsmClient_t *client);
void HsmRngApp_start(HsmClient_t *client);
void HsmFirewallApp(HsmClient_t *client);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Demo Application code on R5 */
void HsmClientApp_start(void)
{
    int32_t status ;
    HsmClient_t client ;

    status = HsmClient_register(&client,APP_CLIENT_ID);
    DebugP_assert(status == SystemP_SUCCESS);

    /*Function call to the desired services*/
    getUIDApp(&client);
    getVersionApp(&client);
    HsmRngApp_start(&client);
    HsmFirewallApp(&client);
}
