INSTALL_DIR ?= install
BIN_DIR = $(INSTALL_DIR)/bin
TEST_DIR = $(INSTALL_DIR)/tests

OUT1 = $(BIN_DIR)/Challenge1
OUT2 = $(BIN_DIR)/Challenge2
OUT3 = $(BIN_DIR)/Challenge3
OUT4 = $(BIN_DIR)/Challenge4
OUT6 = $(BIN_DIR)/Challenge6
OUT7 = $(BIN_DIR)/Challenge7

LINK=-Wl

CPP_FILES1  = src/Challenge1.cpp
CPP_FILES2  = src/Challenge2.cpp
CPP_FILES3  = src/Challenge3.cpp
CPP_FILES4  = src/Challenge4.cpp
CPP_FILES6  = src/Challenge6.cpp
CPP_FILES7  = src/Challenge7.cpp

# INCDIRS = -Iext

# C++ compiler flags (-g -O2 - Wall) & ignoring unknown-pragmas for cmdparser
CCFLAGS = $(INCDIRS) -g -O2 -Wall -Werror -std=c++17 -Wno-unknown-pragmas
# compiler
CCC = g++

$(OUT1): $(CPP_FILES1)# $(INCLUDES1)
	$(CCC) $(LINK),-rpath,\$$ORIGIN $(CCFLAGS) $(CPP_FILES1) -o $(OUT1)
$(OUT2): $(CPP_FILES2)# $(INCLUDES2)
	$(CCC) $(LINK),-rpath,\$$ORIGIN $(CCFLAGS) $(CPP_FILES2) -o $(OUT2)
$(OUT3): $(CPP_FILES3)# $(INCLUDES3)
	$(CCC) $(LINK),-rpath,\$$ORIGIN $(CCFLAGS) $(CPP_FILES3) -o $(OUT3)
$(OUT4): $(CPP_FILES4)# $(INCLUDES4)
	$(CCC) $(LINK),-rpath,\$$ORIGIN $(CCFLAGS) $(CPP_FILES4) -o $(OUT4)
$(OUT6): $(CPP_FILES6)# $(INCLUDES6)
	$(CCC) $(LINK),-rpath,\$$ORIGIN $(CCFLAGS) $(CPP_FILES6) -o $(OUT6)
$(OUT7): $(CPP_FILES7)# $(INCLUDES6)
	$(CCC) $(LINK),-rpath,\$$ORIGIN $(CCFLAGS) $(CPP_FILES7) -o $(OUT7)

clean:
	rm -f $(OUT1) $(OUT2) $(OUT3) $(OUT4) $(OUT6) $(OUT7)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_DIR):
	mkdir -p $(TEST_DIR)

install: $(BIN_DIR) $(TEST_DIR) $(OUT1) $(OUT2) $(OUT3) $(OUT4) $(OUT6) $(OUT7)

