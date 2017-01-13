
objs = main.o mg.o map.o mapstyle.o screen.o general.o \
		game.o
bin = jy-rogue
ccflags = -c -g `sdl-config --cflags`
linkflags = -g `sdl-config --libs` -lSDL_ttf -lSDL_image

$(bin): $(objs)
	g++ -o $(bin) $(objs) $(linkflags)

main.o: main.cpp
	g++ $(ccflags) main.cpp

mg.o: mg.h mg.cpp
	g++ $(ccflags) mg.cpp

map.o: map.cpp map.h
	g++ $(ccflags) map.cpp

map_style.o: mapstyle.cpp graphic.h
	g++ $(ccflags) mapstyle.cpp

screen.o: screen.cpp screen.h
	g++ $(ccflags) screen.cpp

general.o: general.cpp general.h
	g++ $(ccflags) general.cpp

game.o: game.cpp game.h
	g++ $(ccflags) game.cpp

clean:
	-rm -f *.o $(bin)
