/**
  ******************************************************************************
  * @file    getch.h
  * @brief   This file contains the headers of getch functions
  * (code in stm32l0xx_it.c)
  ******************************************************************************
  */
void getchInit(void);
uint8_t getch(void);
int kbhit(void);
