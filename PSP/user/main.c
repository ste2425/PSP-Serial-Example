//
// PSP Serial Example
// Hobbled together in 2025 - Stephen Cooper
// 
// This is a combination of code, in no specific order, by:
// PSP SERIAL
//    The OP-Ditto team https://github.com/Operation-DITTO
//    and TokyoDrift originally posted on the ACIDMOD forum later uploaded to GitHub
//       https://github.com/unraze/PSXControllerToPSP
//       https://acidmods.com/forum/index.php/topic,34966.0.html?PHPSESSID=83a0ebb0570430cac654db79d72e66dc
// PSP KUBRIDGE EXAMPLE
//    By Joel16 from the PSP Homebrew Discord and PR'd into the PSP SDK
//    https://github.com/pspdev/pspsdk/pull/258
//
// The init logic is by TokyoDrift the rest is OP-Ditto
//

#include <pspdebug.h>
#include <pspkernel.h>
#include <pspmodulemgr.h>
#include <pspctrl.h>

#include "kubridge.h"

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res) ((res) >= 0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)    ((res) < 0)

/* Kernel function prototype */
void pspUARTInit(int baud);
int pspUARTRead(void);
void pspUARTWrite(int ch);
void pspUARTResetBuff(void);
void pspUARTWriteString(const char *data, int len);

/* Define the module info section */
PSP_MODULE_INFO("user", PSP_MODULE_USER, 1, 0);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int done = 0;

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
    done = 1;
    return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
    int cbid = 0;
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
    int thid = 0;
    
    if (R_SUCCEEDED(thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0)))
        sceKernelStartThread(thid, 0, 0);
        
    return thid;
}

/* Load our kernel module */
static SceUID LoadStartModule(const char *path) {
    int ret = 0, status = 0;
    SceUID modID = 0;
    
    if (R_FAILED(ret = modID = kuKernelLoadModule(path, 0, NULL))) {
        pspDebugScreenPrintf("kuKernelLoadModule(%s) failed: %08x\n", path, ret);
        return ret;
    }
    
    if (R_FAILED(ret = sceKernelStartModule(modID, 0, NULL, &status, NULL))) {
        pspDebugScreenPrintf("sceKernelStartModule(%s) failed: %08x\n", path, ret);
        return ret;
    }
    
    return ret;
}

/* Unload our kernel module */
static void StopUnloadModules(SceUID modID) {
    sceKernelStopModule(modID, 0, NULL, NULL, NULL);
    sceKernelUnloadModule(modID);
}

int main(int argc, char *argv[]) {
    pspDebugScreenInit();
    SetupCallbacks();

    SceUID module = LoadStartModule("kernel.prx");
    
    pspDebugScreenPrintf("INIT UART\n");
    pspDebugScreenPrintf("Using Baud: 115200\n");
    pspUARTInit(115200);

    pspDebugScreenPrintf("DONE, waiting for serial data...\n");
    pspDebugScreenPrintf("Press X to exit\n");

    while(1) {
		SceCtrlData pad;
		int ch = -1;

		sceCtrlReadBufferPositive(&pad, 1);
		if(pad.Buttons & PSP_CTRL_CROSS)
		{
			break;
		}

        ch = pspUARTRead();

        if (ch > 0) {
		    pspDebugScreenPrintf("Received %d\n", ch);

            pspUARTWriteString("Hello from PSP\n", 15);
            pspUARTWriteString("Recieved: ", 10);

            pspUARTWrite(ch);
            pspUARTWrite('\n');
        }
        
        pspUARTResetBuff();
    }

    pspDebugScreenPrintf("Finished\n");

    StopUnloadModules(module);
    return 0;
}
