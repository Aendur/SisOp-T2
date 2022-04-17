#####
CFLAGS = -std=c++17 -m64 -Wall -Wextra -Wpedantic
LIBS = $(patsubst source/%.cpp,obj/%.o,$(filter-out source/main.cpp,$(wildcard source/*.cpp source/*/*.cpp)))
LINKS = -lSDL2 -lpthread
DIRS=bin obj/settings obj/UI

all: server

server: source/main.cpp dirs $(LIBS)
	g++ $(CFLAGS) -Iheader -obin/server $< $(LIBS) $(LINKS)

tests/%: cpp/tests/%.cpp $(LIBS)
	g++ $(CFLAGS) -DUNIT_TEST -Iheader -obin/test $<  $(LIBS)

obj/%.o: source/%.cpp header/%.h
	g++ $(CFLAGS) -Iheader -c -o$@ $<

.PHONY: dirs clean cleanse server

dirs:
	mkdir -p $(DIRS)

# CUIDADO AO UTILIZAR AS RECEITAS ABAIXO POIS ELAS PODEM REMOVER OU SUBSTITUIR ARQUIVOS IMPORTANTES
clean:
	rm -vfr obj

cleanse: clean
	rm -vfr $(DIRS)

