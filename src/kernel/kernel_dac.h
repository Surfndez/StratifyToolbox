#ifndef KERNEL_DAC_H
#define KERNEL_DAC_H

int kernel_dac_request(void * args);
int kernel_dac_init();
void * kernel_dac_thread_function(void * args);

#endif // KERNEL_DAC_H
