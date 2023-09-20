CXX = /nobackup/jacaraba/.conda/envs/asp_deps/bin/g++
CXXFLAGS = -std=c++17 -Wall -I/nobackup/jacaraba/.conda/envs/asp_deps/include
LDFLAGS = -lgdal # Linker flags for GDAL

SRCS = src/stereoCorrelator.cc src/stereoCorrelator.h src/BlockMatcherGPU.cc src/BlockMatcherGPU.h src/blockmatcher_test.cc
OBJS = $(SRCS:.cc=.o)

EXEC = stereo_corr

INSTALL_DIR = ./install

.PHONY: all clean install

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(INSTALL_DIR)/bin/$(EXEC) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

install:
	mkdir -p $(INSTALL_DIR)/bin
	cp $(EXEC) $(INSTALL_DIR)/bin

clean:
	rm -f $(INSTALL_DIR)/bin/$(EXEC)
