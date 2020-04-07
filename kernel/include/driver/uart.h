#pragma once 

void
setup_uart();

void
uart_write(char *);

void
uart_cwrite(char c);

char
uart_cread();
