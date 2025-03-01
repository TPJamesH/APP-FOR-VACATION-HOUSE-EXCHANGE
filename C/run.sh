rm -f /private/var/folders/xd/gyvx2hpn23q5bkql_911wlqh0000gn/T/main-*.o
rm -f /private/var/folders/xd/gyvx2hpn23q5bkql_911wlqh0000gn/T/system-*.o

c++ --std=c++17 main.cpp src/*.cpp $(pkg-config --cflags --libs libmongocxx) -o app.out
./app.out