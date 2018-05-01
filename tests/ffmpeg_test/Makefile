BIN1 := test-encode
BIN2 := test-rgb2yv
BINS := $(BIN1) $(BIN2)
OBJS1 := main.o
SRCS1 := main.c
OBJS2 := rgb2yuv.o
SRCS2 := rgb2yuv.c

$(BINS): $(BIN1) $(BIN2)

$(BIN1): $(OBJS1)
	gcc -o $@ $(OBJS1) -lavcodec -lavutil

$(BIN2): $(OBJS2)
	gcc -o $@ $(OBJS2) -lavcodec -lavutil -lswscale

%.o: %.c
	gcc -c -o $@ $<

clean:
	rm -f $(BINS)
