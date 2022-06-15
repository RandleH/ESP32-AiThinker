


#ifndef GPIO_H
#define GPIO_H

void rh_gpio__init   (void);
void rh_gpio__set    ( char gpio, int pin, int value);
int  rh_gpio__get    ( char gpio, int pin);
void rh_gpio__toggle ( char gpio, int pin);


#endif