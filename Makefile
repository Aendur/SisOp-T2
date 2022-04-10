#####
CFLAGS = -std=c++17 -m64 -Wall -Wextra -Wpedantic
LIBS = $(patsubst source/%.cpp,obj/%.o,$(filter-out source/main.cpp,$(wildcard source/*.cpp)))

#LINKS = -lglfw -lGL
#LINKS = -lncurses
LINKS = -lSDL2 -lpthread

server: source/main.cpp dirs $(LIBS)
	g++ $(CFLAGS) -DSERVER -Iheader -oserver $< $(LIBS) $(LINKS)

client: source/main.cpp dirs $(LIBS)
	g++ $(CFLAGS) -DCLIENT -Iheader -oclient $< $(LIBS) $(LINKS)

tests/%: cpp/tests/%.cpp $(LIBS)
	g++ $(CFLAGS) -DUNIT_TEST -Iheader -obin/test $< $(LIBS)

obj/%.o: source/%.cpp header/%.h
	g++ $(CFLAGS) -Iheader -c -o$@ $<

.PHONY: env dirs clean cleanse

dirs:
	mkdir -p bin
	mkdir -p obj

# CUIDADO AO UTILIZAR AS RECEITAS ABAIXO POIS ELAS PODEM REMOVER OU SUBSTITUIR ARQUIVOS IMPORTANTES
env:
	mkdir -vp ./env/bin
	cp -v ./bin/unbsh ./env/bin
	rsync -av ./env/ ~

clean:
	rm -vfr obj

cleanse: clean
	rm -vfr bin
	rm -vfr env/bin

