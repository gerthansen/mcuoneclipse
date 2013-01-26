/*
 * Application.c
 *
 *  Created on: Oct 14, 2012
 *      Author: Erich Styger
 */

#include "Application.h"
#include "BT1.h"
#include "UTIL1.h"
#include "FSSH1.h"
#include "Shell.h"

static void Test(const FSSH1_StdIOType *io) {
  FSSH1_SendStr((unsigned char*)"\r\nBluetooth Module Test:\r\n----------------------\r\n", io->stdOut);

  FSSH1_SendStr((unsigned char*)"Test UART: ", io->stdOut);
  if (BT1_btTestUART()!=ERR_OK) {
    FSSH1_SendStr((unsigned char*)"failed\r\n", io->stdOut);
    return;
  } else {
    FSSH1_SendStr((unsigned char*)"OK\r\n", io->stdOut);
  }

#if 0
  FSSH1_SendStr((unsigned char*)"Reset Device: ", io->stdOut);
  if (BT1_btResetDevice()!=ERR_OK) {
    FSSH1_SendStr((unsigned char*)"failed\r\n", io->stdOut);
  } else {
    FSSH1_SendStr((unsigned char*)"OK\r\n", io->stdOut);
  }
  FSSH1_SendStr((unsigned char*)"Wait some time to reset device...\r\n", io->stdOut);
  WAIT1_Waitms(BT1_TIMEOUT_MS_AFTER_RESET);
#endif

#if 0
  FSSH1_SendStr((unsigned char*)"Reset to factory defaults: ", io->stdOut);
  if (BT1_btRestorFactoryDefault()!=ERR_OK) {
    FSSH1_SendStr((unsigned char*)"failed\r\n", io->stdOut);
  } else {
    FSSH1_SendStr((unsigned char*)"OK\r\n", io->stdOut);
  }
  WAIT1_Waitms(BT1_TIMEOUT_MS_AFTER_FACTORY_DEFAULT); /* give device time to boot up again. Did not find a value in the data sheet */
#endif
  
#if 0
  FSSH1_SendStr((unsigned char*)"Setting device name: ", io->stdOut);
  if (BT1_btSetDeviceName((unsigned char*)"INTRO")!=ERR_OK) {
    FSSH1_SendStr((unsigned char*)"failed\r\n", io->stdOut);
  } else {
    FSSH1_SendStr((unsigned char*)"OK\r\n", io->stdOut);
  } 
#endif
  
}

static void PrintHelp(const FSSH1_StdIOType *io) {
  /* list your local help here */
  FSSH1_SendHelpStr((unsigned char*)"bt", (unsigned char*)"Group of bt commands\r\n", io->stdOut);
  FSSH1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows trace help or status\r\n", io->stdOut);
}

/*!
 * \brief Prints the status text to the console
 * \param io I/O channel to be used
 */
static void PrintStatus(const FSSH1_StdIOType *io) {
  /* list your local status here */
  Test(io);
}

uint8_t APP_ParseCommand(const unsigned char *cmd, bool *handled, const FSSH1_StdIOType *io) {
  /* handling our own commands */
  if (UTIL1_strcmp((char*)cmd, FSSH1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "bt help")==0) {
    PrintHelp(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, FSSH1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, "bt status")==0) {
    PrintStatus(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, "bt test")==0) {
    Test(io);
    *handled = TRUE;
  }
  return ERR_OK;
}

void APP_Run(void) {
  unsigned char buf[32];
  Serial1_TComData rx;
  
  SHELL_Init();
  buf[0]='\0';
  SHELL_ParseLine((unsigned char*)"help");
  for(;;) {
    SHELL_ReadAndParseLine(buf, sizeof(buf));
    if (Serial1_GetCharsInRxBuf()>0) {
      if (Serial1_RecvChar(&rx)==ERR_OK) {
        FSSH1_GetStdio()->stdOut(rx);
      }
    }
  }
}