# STM32MP135 Boot Experiments

This repo contains simple code examples booting code on the
[STM32MP135](https://www.st.com/en/microcontrollers-microprocessors/stm32mp135.html)
processors, as implemented on the official [eval
board](https://www.st.com/en/evaluation-tools/stm32mp135f-dk.html).

The official documentation and code examples by ST is extensive, but in many
cases overly complicated. These examples aim to be self-contained minimum
working examples.

The files are as follows:

- `uart_boot.ipynb`: Jupyter notebook implementing all the commands used by the
device boot ROM to write a "bitstream" (such as the STM32DDRFW-UTIL utility
taken from the STMicroelectronics
[repository](https://github.com/STMicroelectronics/STM32DDRFW-UTIL/tree/main))
to the internal SRAM of the device.