LIBSRC=\
	../lapi.c ../lobject.c   ../lstrlib.c \
	../lauxlib.c   ../ldebug.c  ../lopcodes.c  ../ltable.c   ../lundump.c \
	../lbaselib.c  ../ldo.c     ../llex.c      ../ltablib.c \
	../lcode.c     ../ldump.c   ../lmathlib.c  ../lparser.c  ../lvm.c \
	../lcorolib.c  ../lfunc.c   ../lmem.c      ../lstate.c    ../ltm.c ../lzio.c \
	../lctype.c    ../lgc.c     ../lstring.c

# LIBO=\
# 	./lapi.o ./ldblib.o  ./linit.o     ./lobject.o   ./lstrlib.o \
# 	./lauxlib.o   ./ldebug.o  ./liolib.o    ./lopcodes.o  ./ltable.o   ./lundump.o \
# 	./lbaselib.o  ./ldo.o     ./llex.o      ./loslib.o    ./ltablib.o  ./lutf8lib.o \
# 	./lcode.o     ./ldump.o   ./lmathlib.o  ./lparser.o   ./ltests.o   ./lvm.o \
# 	./lcorolib.o  ./lfunc.o   ./lmem.o      ./lstate.o    ./ltm.o      ./lzio.o \
# 	./lctype.o    ./lgc.o     ./loadlib.o   ./lstring.o

# lua-embed: lua-embed.c liblua.a
# 	gcc -O2 -ggdb2 -o lua-embed -I.. -L. lua-embed.c -llua -lm

# liblua.a: $(LIBSRC)
# 	gcc -c $(LIBSRC)
# 	ar rc ./liblua.a $(LIBO)
# 	rm $(LIBO)

lua-embed: lua-embed.c
	gcc -O2 -ggdb2 -o lua-embed -I.. -L. lua-embed.c $(LIBSRC) -lm
