// All credit to Carmine Noviello for this code
// https://github.com/cnoviello/mastering-stm32/blob/master/nucleo-f030R8/system/src/retarget/retarget.c

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include <../Inc/retarget.h>
#include <stdint.h>
#include <stdio.h>

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

USART_TypeDef *gHuart;
extern  uint8_t g_rxCarUSART2;

void RetargetInit(USART_TypeDef *huart) {
  gHuart = huart;

  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);
}

int _isatty(int fd) {
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 1;

  errno = EBADF;
  return 0;
}

int _write(int fd, char* ptr, int len) {

  if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
      for (int i=0; i<len;i++)
      {
        while(LL_USART_IsActiveFlag_TXE(USART2)==0);
        LL_USART_TransmitData8(USART2,*ptr);
        ptr++;
      }
    return len;
  }
  errno = EBADF;
  return -1;
}

int _close(int fd) {
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 0;

  errno = EBADF;
  return -1;
}

int _lseek(int fd, int ptr, int dir) {
  (void) fd;
  (void) ptr;
  (void) dir;

  errno = EBADF;
  return -1;
}

int _read(int fd, char* ptr, int len) {

  if (fd == STDIN_FILENO) {
      do
      {
    	if (g_rxCarUSART2==255)
    	{
    	  __WFI(); //wait for interrupt (for a key ==> data RX)
    	}
      }
      while(g_rxCarUSART2==255); //while no char received
      *ptr=g_rxCarUSART2;  //save received data
      g_rxCarUSART2=255; //clear char received
      return 1;
  }
  errno = EBADF;
  return -1;
}

int _fstat(int fd, struct stat* st) {
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
    st->st_mode = S_IFCHR;
    return 0;
  }

  errno = EBADF;
  return 0;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)

