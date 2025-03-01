
c++ --std=c++17 main.cpp src/*.cpp $(pkg-config --cflags --libs libmongocxx) -o app.out
./app.out