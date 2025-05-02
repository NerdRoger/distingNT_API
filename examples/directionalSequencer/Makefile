ifndef NT_API_PATH
	NT_API_PATH := ../..
endif

INCLUDE_PATHS := -I$(NT_API_PATH)/include -I./include

# Gather all local .cpp files and their matching .o filenames
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

# Final output location
FINAL_OUTPUT := ../plugins/directionalSequencer.o

all: $(FINAL_OUTPUT)

# Compile each .cpp file to a local .o file
%.o: %.cpp
	arm-none-eabi-c++ -std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -fno-exceptions -fno-rtti -Os -fPIC -Wall $(INCLUDE_PATHS) -c -o $@ $<

# Link all .o files into one final .o file
$(FINAL_OUTPUT): $(OBJ_FILES)
	mkdir -p $(@D)
	arm-none-eabi-ld -r -o $@ $(OBJ_FILES)
	rm -f $(OBJ_FILES)

clean:
	rm -f $(OBJ_FILES) $(FINAL_OUTPUT)
