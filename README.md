# STM32MP135 Boot Experiments

This repo contains simple code examples booting code on the
[STM32MP135](https://www.st.com/en/microcontrollers-microprocessors/stm32mp135.html)
processors, as implemented on the official [eval
board](https://www.st.com/en/evaluation-tools/stm32mp135f-dk.html).

The official documentation and code examples by ST is extensive, but in many
cases overly complicated. These examples aim to be self-contained minimum
working examples.

The files are as follows:

- `blink_cubeide`: "Blink" program done as an STM32CubeIDE project and uploaded
  to the processor using the on-board STLink debugger in "test boot" mode

- `stm32_header`: Jupyter notebook to convert an ELF object file to an
  executable with the correct header file required by the boot ROM

- `uart_boot`: Jupyter notebook implementing all the commands used by the device
  boot ROM to write an executable to the internal SRAM of the device.

- `blink_noide`: "Blink" program done without an IDE, compiled using arm-gcc
  with a simple Makefile

### Author

Jakob Kastelic, Stanford Research Systems
