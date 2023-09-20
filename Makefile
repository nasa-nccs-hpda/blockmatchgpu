CXX=nvcc
CXXFLAGS =-std=c++17 -I/discover/nobackup/projects/akmosaic/micromamba/envs/aspv2/include -L/discover/nobackup/projects/akmosaic/micromamba/envs/aspv2/lib -shared
LDFLAGS=-lgdal # Linker flags for GDAL

SRCS=src/stereoCorrelator.cc src/BlockMatcherGPU.cc
OBJS=$(SRCS:.cc=.o)

EXEC=stereo_corr

INSTALL_DIR=./install

.PHONY: all clean install

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(INSTALL_DIR)/bin/$(EXEC)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

install:
	mkdir -p $(INSTALL_DIR)/bin
	cp $(EXEC) $(INSTALL_DIR)/bin

clean:
	rm -f $(OBJS) $(INSTALL_DIR)/bin/$(EXEC)
