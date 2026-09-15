CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRCS     = main.cpp Admin.cpp Student.cpp Utils.cpp
HEADERS  = User.h Admin.h Student.h Utils.h
TARGET   = attendance

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: clean
