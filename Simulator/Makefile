TARGET = challenge
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I. -I../Lib_Croix

SRC = ../Lib_Croix/main.cpp ../Lib_Croix/CroixPharma.cpp

LIB = libsim.a

all: challenge heart matrix_rain date annonce specter

challenge: $(SRC) $(LIB) wiringPi.h
	$(CXX) $(CXXFLAGS) $(SRC) $(LIB) -o challenge

heart: ../Lib_Croix/heart.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) wiringPi.h
	$(CXX) $(CXXFLAGS) ../Lib_Croix/heart.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) -o heart

matrix_rain: ../Lib_Croix/matrix_rain.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) wiringPi.h
	$(CXX) $(CXXFLAGS) ../Lib_Croix/matrix_rain.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) -o matrix_rain

date: ../Lib_Croix/date.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) wiringPi.h
	$(CXX) $(CXXFLAGS) ../Lib_Croix/date.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) -o date

annonce: ../Lib_Croix/annonce.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) wiringPi.h
	$(CXX) $(CXXFLAGS) ../Lib_Croix/annonce.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) -o annonce

specter: ../Lib_Croix/specter.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) wiringPi.h
	$(CXX) $(CXXFLAGS) ../Lib_Croix/specter.cpp ../Lib_Croix/CroixPharma.cpp $(LIB) -o specter

clean:
	rm -f challenge bright_test heart matrix_rain date annonce specter

fclean: clean

re: fclean all