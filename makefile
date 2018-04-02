FLAGS = -std=gnu++11

%.o: %.cc
	export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
	g++ -MMD `pkg-config exiv2 --cflags` -c -Iexiv2-trunk/include -Iexiv2-trunk/include/exiv2 -std=gnu++11 -o $@ $<

fixer: fixer.o
	#g++ $(FLAGS) $< -o $@
	export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
	exiv2-trunk/libtool --mode=link g++ `pkg-config exiv2 --libs` -rpath /usr/local/lib -L/usr/local/lib -o $@ $<

.PHONY: clean
clean:
	-rm -rf fixer
