clean:
	rm -rf install/*
	rm -rf src/*~
	rm -rf src/player_bis.c

toto : src/graph_pattern.c src/server.c src/player.c
	cp src/player.c src/player_bis.c
	cc -shared -fPIC src/player.c -o install/libplayer_1.so
	cc -shared -fPIC src/player_bis.c -o install/libplayer_2.so
	cc src/annex_function.c src/graph_pattern.c install/libplayer_1.so install/libplayer_2.so src/server.c -o install/server `gsl-config --cflags --libs` -ldl
