#####
CFLAGS = -std=c++17 -m64 -Wall -Wextra -Wpedantic
LIBS1 = $(patsubst source/%.cpp,obj/%.o,$(filter-out source/main.cpp,$(wildcard source/*.cpp)))
LIBS2 = $(patsubst source/settings/%.cpp,obj/settings/%.o,$(wildcard source/settings/*.cpp))

#LINKS = -lglfw -lGL
#LINKS = -lncurses
LINKS = -lSDL2 -lpthread

all: server client

server: source/main.cpp dirs $(LIBS1) $(LIBS2)
	g++ $(CFLAGS) -DSERVER -Iheader -obin/server $< $(LIBS1) $(LIBS2) $(LINKS)

client: source/main.cpp dirs $(LIBS1) $(LIBS2)
	g++ $(CFLAGS) -DCLIENT -Iheader -obin/client $<  $(LIBS1) $(LIBS2) $(LINKS)

tests/%: cpp/tests/%.cpp $(LIBS1) $(LIBS2)
	g++ $(CFLAGS) -DUNIT_TEST -Iheader -obin/test $<  $(LIBS1) $(LIBS2)

obj/%.o: source/%.cpp header/%.h
	g++ $(CFLAGS) -Iheader -c -o$@ $<

obj/settings/%.o: source/settings/%.cpp header/settings/%.h
	g++ $(CFLAGS) -Iheader -c -o$@ $<

.PHONY: ipcrm dirs clean cleanse server client

dirs:
	mkdir -p bin
	mkdir -p obj/settings

# CUIDADO AO UTILIZAR AS RECEITAS ABAIXO POIS ELAS PODEM REMOVER OU SUBSTITUIR ARQUIVOS IMPORTANTES
ipcrm:
	ipcrm -M 0x005da900 -S 0x005da900 -S 0x005da901 -S 0x005da902

clean:
	rm -vfr obj

cleanse: clean
	rm -vfr bin
	rm -vfr env/bin

