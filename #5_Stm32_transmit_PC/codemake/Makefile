.PHONY: all build cmake clean format

PROJ_NAME := stm32_cpp
BUILD_DIR := build
BUILD_TYPE ?= Debug

all: build

${BUILD_DIR}/Makefile:
	cmake \
		-G "MinGW Makefiles" \
		-B${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DDUMP_ASM=OFF

cmake: ${BUILD_DIR}/Makefile

build: cmake
	$(MAKE) -C ${BUILD_DIR} --no-print-directory

SRCS := $(shell find . -name '*.[ch]' -or -name '*.[ch]pp')
%.format: %
	clang-format -i $<
format: $(addsuffix .format, ${SRCS})

flash:
	st-flash --reset write $(BUILD_DIR)/$(PROJ_NAME).bin 0x08000000

clean:
	rm -rf $(BUILD_DIR)
