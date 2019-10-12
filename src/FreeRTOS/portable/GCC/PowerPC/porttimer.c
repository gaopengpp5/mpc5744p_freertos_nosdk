/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>

//#include "device_registers.h"
#include "FreeRTOSConfig.h"

#include "PIT.h"
#include "MPC5744P_drv.h"
//#include "interrupt_manager.h"

/* functions required by port.c */
//extern void prvPortTimerSetup(void* paramF, uint32_t tick_interval);
extern void prvPortTimerSetup(void);
extern void prvPortTimerReset(void);

/* Workaround for MPC574xP platforms where PIT is PIT_0 defined in header */
#if defined(PIT_0)
#define PIT PIT_0
#endif

void prvPortTimerSetup(void)
{
	PIT_Setup(0, PIT_CLK/1000000, 1000);  //1msec period
	PIT_EnableInt(0);

	PIT_EnableChannel(0);
}


void prvPortTimerReset(void)
{
	PIT_ClearFlag(0);
}
