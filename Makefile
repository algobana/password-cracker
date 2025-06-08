# Derleyici ve bayraklar
CXX = g++
OPT_LEVEL = -O3
CXXFLAGS = -std=c++17 -Wall $(OPT_LEVEL) -pthread

# Dosyalar
SRC = main.cpp
OUT = cracker_v4
RUN_SCRIPT = ./run.sh

# Varsayılan hedef
all: $(OUT)

# Derleme
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Çalıştır
run: $(OUT)
	./$(OUT)

# run.sh scriptini çalıştır
profile: $(OUT)
	$(RUN_SCRIPT)

# Derleme seviyesi değiştirme (örn. make rebuild OLEVEL=O2)
OLEVEL ?= O3
rebuild:
	$(MAKE) clean
	$(MAKE) OPT_LEVEL=-$(OLEVEL)

# Temizleme (logs KALSIN!)
clean:
	rm -f $(OUT) cracker_v4_gprof gmon.out

