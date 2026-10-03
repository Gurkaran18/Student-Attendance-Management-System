# Respect CXX from the environment or command line, otherwise use g++.
ifeq ($(origin CXX),default)
CXX = g++
endif

CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
TARGET    = main
SOURCES   = main.cpp AttendanceSystem.cpp Admin.cpp Student.cpp User.cpp Util.cpp
HEADERS   = AttendanceSystem.h Admin.h Student.h User.h Util.h

# Some macOS installs ship a stale, partial libc++ in the Command Line Tools
# that shadows the complete one in the SDK, so every #include <string> fails.
# Probe by actually compiling: if the standard library is reachable, this adds
# nothing and the build uses the plain command below.
PROBE := $(shell printf '\043include <string>\nint main(){return 0;}' > /tmp/.cxx_probe.cpp 2>/dev/null && \
                 $(CXX) -std=c++17 -fsyntax-only /tmp/.cxx_probe.cpp >/dev/null 2>&1 && echo ok)
ifneq ($(PROBE),ok)
SDK_PATH := $(shell xcrun --show-sdk-path 2>/dev/null)
ifneq ($(wildcard $(SDK_PATH)/usr/include/c++/v1/string),)
STDLIB_FIX = -nostdinc++ -isystem $(SDK_PATH)/usr/include/c++/v1
endif
endif

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(STDLIB_FIX) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean
