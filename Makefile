# Build for Linux/POSIX with g++. Headers are included by basename, so every
# Includes/ subdir goes on the include path.
#
# The game loads resources with paths like "../Resources/map.txt", so it
# expects to run one directory below the project root. The binary is emitted
# into build/ and `make run` launches it from there.

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall $(addprefix -I,$(shell find Includes -type d))

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/ForTheKing

# Dead/legacy files that are not part of the build (e.g. Passive.cpp
# includes a header that does not exist). Exclude them here.
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
