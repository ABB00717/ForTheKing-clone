# Build for Linux/POSIX. There is no more Visual Studio project; this is the
# build system. Headers are included by basename, so every Includes/ subdir
# goes on the include path (mirrors the old .vcxproj AdditionalIncludeDirectories).
#
# The game loads resources with paths like "../Resources/map.txt", i.e. it
# expects to run from one directory below the project root. So the binary is
# emitted into build/ and `make run` launches it from there.

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall $(addprefix -I,$(shell find Includes -type d))

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/ForTheKing

# These .cpp exist on disk but were never part of the Visual Studio build:
# they are dead/legacy files (e.g. Passive.cpp includes a header that does not
# exist) whose symbols live in the compiled sources. Exclude them here.
ORPHANS := Sources/Gadget/Armor.cpp Sources/Gadget/Contain.cpp \
           Sources/Gadget/Passive.cpp Sources/Gadget/Weapon.cpp

SRCS := main.cpp $(filter-out $(ORPHANS),$(shell find Sources -name '*.cpp'))
OBJS := $(addprefix $(BUILD_DIR)/,$(SRCS:.cpp=.o))

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run from build/ so the "../Resources" and "../Sources/Display" paths resolve.
run: $(TARGET)
	cd $(BUILD_DIR) && ./ForTheKing

clean:
	rm -rf $(BUILD_DIR)

.PHONY: run clean
