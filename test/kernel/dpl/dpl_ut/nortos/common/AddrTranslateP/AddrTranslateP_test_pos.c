/* Copyright (c) 2023 Texas Instruments Incorporated
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
 *  \file     AddrTranslateP_null_test_pos.c
 *
 *  \brief    This file contains Nortos common API unit test code.
 *
 *  \details  nortos unit tests
 **/
#include <kernel/dpl/AddrTranslateP.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <kernel/dpl/SystemP.h>
#include <kernel/dpl/HwiP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/TaskP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/HeapP.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/EventP.h>
#include <kernel/dpl/QueueP.h>
#include <drivers/soc.h>
#include <unity.h>
#include "ti_drivers_open_close.h"

AddrTranslateP_Params  params;
uint64_t systemAddr = 0x11U;

/* positive test of AddrTranslateP_Params_init API */
void posTest_AddrTranslateP_Params_init(void *args)
{
    int32_t    testStatus = SystemP_SUCCESS;

    if (testStatus == SystemP_SUCCESS)
    {
        AddrTranslateP_Params_init(&params);
    }

	if (testStatus != SystemP_SUCCESS)
    {
        testStatus = SystemP_FAILURE;
        DebugP_log("nortos_common_pos_Test: failure on line no. %d \n", __LINE__);
    }
    TEST_ASSERT_EQUAL_INT32(testStatus,SystemP_SUCCESS);
}

/* positive test of AddrTranslateP_init API */
void posTest_AddrTranslateP_init(void *args)
{
	int32_t    testStatus = SystemP_SUCCESS;

    if (testStatus == SystemP_SUCCESS)
    {
		AddrTranslateP_init(&params);
    }
    if (testStatus != SystemP_SUCCESS)
    {
        testStatus = SystemP_FAILURE;
		DebugP_log("nortos_common_pos_Test: failure on line no. %d \n", __LINE__);
    }
	TEST_ASSERT_EQUAL_INT32(testStatus,SystemP_SUCCESS);
}

/* positive test of AddrTranslateP_init API by assigning numRegions to 1 */
void posTest_AddrTranslateP_initOne(void *args)
{
	int32_t    testStatus = SystemP_SUCCESS;
    AddrTranslateP_Params  *params = (AddrTranslateP_Params *)args;

    if (testStatus == SystemP_SUCCESS)
    {
        params->numRegions = 1;
		AddrTranslateP_init(params);
    }
    if (testStatus != SystemP_SUCCESS)
    {
        testStatus = SystemP_FAILURE;
		DebugP_log("nortos_common_pos_Test: failure on line no. %d \n", __LINE__);
    }
	TEST_ASSERT_EQUAL_INT32(testStatus,SystemP_SUCCESS);
}

/* positive test of AddrTranslateP_getLocalAddr API */
void posTest_AddrTranslateP_getLocalAddr(void *args)
{
	int32_t    testStatus = SystemP_SUCCESS;

    if (testStatus == SystemP_SUCCESS)
    {
		AddrTranslateP_getLocalAddr(systemAddr);
    }
    if (testStatus != SystemP_SUCCESS)
    {
        testStatus = SystemP_FAILURE;
		DebugP_log("nortos_common_pos_Test: failure on line no. %d \n", __LINE__);
    }
	TEST_ASSERT_EQUAL_INT32(testStatus,SystemP_SUCCESS);
}

/* positive test of AddrTranslateP_getLocalAddr API by assigning numRegions to 1 */
void posTest_AddrTranslateP_getLocalAddrOne(void *args)
{
	int32_t    testStatus = SystemP_SUCCESS;

    if (testStatus == SystemP_SUCCESS)
    {
         params.numRegions = 1;
		AddrTranslateP_getLocalAddr(systemAddr);
    }
    if (testStatus != SystemP_SUCCESS)
    {
        testStatus = SystemP_FAILURE;
		DebugP_log("nortos_common_pos_Test: failure on line no. %d \n", __LINE__);
    }
	TEST_ASSERT_EQUAL_INT32(testStatus,SystemP_SUCCESS);
}

void test_pos_main(void *args)
{
    RUN_TEST(posTest_AddrTranslateP_Params_init, 11139, NULL);
	RUN_TEST(posTest_AddrTranslateP_init, 11140, NULL);
    RUN_TEST(posTest_AddrTranslateP_initOne, 11141, NULL);
	RUN_TEST(posTest_AddrTranslateP_getLocalAddr, 11142, NULL);
    RUN_TEST(posTest_AddrTranslateP_getLocalAddrOne, 11143, NULL);
}
