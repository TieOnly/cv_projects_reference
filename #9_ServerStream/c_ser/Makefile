OUT_DIR := out

MAIN_NAME := ser
MAIN_SRC := ./components/*.c ./utils/cjson/*.c ./utils/pool/*.c
MAIN_INC := -I ./components/include -I ./utils/cjson -I ./utils/pool -I ./utils
MAIN_LIB := -L -lpthread
MAIN_OPT := -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -Wundef -fno-common

all: build


build:
	gcc ./*.c $(MAIN_SRC) -o $(OUT_DIR)/$(MAIN_NAME) $(MAIN_OPT) $(MAIN_INC) $(MAIN_LIB)

run:
	$(OUT_DIR)/$(MAIN_NAME)