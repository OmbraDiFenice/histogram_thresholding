CXX=g++
CXXFLAGS=-std=c++0x -O3
INCLUDES=-I./fastflow-2.0.4
LDFLAGS=-lpthread
FF_FLAGS=-DTRACE_FASTFLOW -DNO_DEFAULT_MAPPING

SRC_DIR=src
OBJ_DIR=bin

SOURCES=$(filter-out Par.cpp Seq.cpp, $(notdir $(wildcard $(SRC_DIR)/*.cpp)))
OBJECTS=$(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))

SEQ_EXE=seq
PAR_EXE=par

.PHONY: clean all $(SEQ_EXE) $(PAR_EXE) clean_graphs clean_output

.DEFAULT_GOAL := all

all: $(SEQ_EXE) $(PAR_EXE)

$(OBJECTS): $(OBJ_DIR)/%.o: $(addprefix $(SRC_DIR)/, %.cpp %.hpp)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(FF_FLAGS)

# Sequential
$(SEQ_EXE): $(OBJ_DIR)/Seq.o $(filter-out $(addprefix $(OBJ_DIR)/, farm_worker.o streamer.o), $(OBJECTS))
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS) $(FF_FLAGS)

$(OBJ_DIR)/Seq.o: $(SRC_DIR)/Seq.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 

# Parallel
$(PAR_EXE): $(OBJ_DIR)/Par.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS) $(FF_FLAGS)

$(OBJ_DIR)/Par.o: $(SRC_DIR)/Par.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(FF_FLAGS)

# Other
clean:
	rm -f $(OBJ_DIR)/* $(SEQ_EXE) $(PAR_EXE)

clean_graphs:
	rm -f graphs/*.pbm graphs/data*.txt

clean_output:
	rm -f output/*
