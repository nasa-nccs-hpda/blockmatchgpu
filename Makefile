CXX=nvcc
CXXFLAGS=-std=c++17 -x cu
LDFLAGS=-lgdal # Linker flags for GDAL

SRCS=src/BlockMatcherGPU.cu src/BlockMatcherCPU.cc

EXEC1 = block_match_cpu
EXEC2 = block_match_gpu
EXEC3 = block_match_both

INSTALL_DIR=./install

.PHONY: all clean install

all: $(EXEC1) $(EXEC2) $(EXEC3)

$(EXEC1): src/test_blockmatcherCPU.cc
	$(CXX) $(CXXFLAGS) src/test_blockmatcherCPU.cc $(SRCS) -o $(INSTALL_DIR)/bin/$(EXEC1)

$(EXEC2): src/test_blockmatcherGPU.cc
	$(CXX) $(CXXFLAGS) src/test_blockmatcherGPU.cc $(SRCS) -o $(INSTALL_DIR)/bin/$(EXEC2)

$(EXEC3): src/blockmatcher_test.cc
	$(CXX) $(CXXFLAGS) src/blockmatcher_test.cc $(SRCS) -o $(INSTALL_DIR)/bin/$(EXEC3)

clean:
	rm -rf src/*.o $(INSTALL_DIR)/bin/$(EXEC1) $(INSTALL_DIR)/bin/$(EXEC2) $(INSTALL_DIR)/bin/$(EXEC3)

install: $(EXEC1) $(EXEC2) $(EXEC3)
	mkdir -p $(INSTALL_DIR)/bin
	cp $(EXEC1) $(INSTALL_DIR)/bin/$(EXEC1)
	cp $(EXEC2) $(INSTALL_DIR)/bin/$(EXEC2)
	cp $(EXEC3) $(INSTALL_DIR)/bin/$(EXEC3)
