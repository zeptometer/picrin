CFLAGS=-Wall -Wextra -std=c99
EXTLIB=-lxfile

ifeq ($(findstring CYGWIN,$(shell uname -s)), CYGWIN)
  XFILE_LIB=cygxfile.dll
  PICRIN_LIB=cygpicrin.dll
else
  XFILE_LIB=libxfile.so
  PICRIN_LIB=libpicrin.so
endif

all: deps release

deps:
	git submodule update --init
	$(CC) $(CFLAGS) -shared -fPIC extlib/xfile/*.c -o lib/$(XFILE_LIB) -I./extlib/xfile
	$(CC) $(CFLAGS) -shared -fPIC extlib/GomiHiroi/server/gclog.c -o lib/libgomihiroi.so -I./extlib/GomiHiroi/server

release: CFLAGS += -DDEBUG=0 -O3
release: build

debug: CFLAGS += -g -DDEBUG=1 -O0
debug: build

gc-visualize: CFLAGS += -g -DGC_VISUALIZE
gc-visualize: EXTLIB += -lgomihiroi
gc-visualize: build visualizer

build: build-lib build-main

build-main:
	$(CC) $(CFLAGS) -D_GNU_SOURCE -Wl,-rpath lib tools/main.c -o bin/picrin -I./include -I./extlib -L./lib -lreadline -lm $(EXTLIB) -lpicrin

build-lib:
	cd src; \
	  yacc -d parse.y; \
	  flex scan.l
	$(CC) $(CFLAGS) -shared -fPIC src/*.c -o lib/$(PICRIN_LIB) -I./include -I./extlib -L./lib -lm $(EXTLIB)

clean:
	rm -f src/y.tab.c src/y.tab.h src/lex.yy.c
	rm -f lib/$(PICRIN_LIB)
	rm -f bin/picrin

run:
	bin/picrin

tak: release
	bin/picrin etc/tak.scm

visualizer:
	processing-java --sketch=extlib/GomiHiroi/visualizer --output=extlib/GomiHiroi/visualizer/output --export --force

lines: clean
	wc -l `find . -name "*.[chyl]"`

no-act:
	bin/picrin -e '' > /dev/null
