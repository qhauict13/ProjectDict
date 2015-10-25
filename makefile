CC          = gcc
CFLAGS      = `pkg-config --cflags gtk+-3.0 gmodule-2.0`
LIBS        = `pkg-config --libs   gtk+-3.0 gmodule-2.0` -lbt
INC         = ./inc
LIB         = ./lib
SRCS        = model.c  main.c callbacks.c stack.c
OBJS        = $(SRCS:.c=.o)
EXECUTABLE  = dict
DB          = dict_db soundex_db

all: $(SRCS) $(EXECUTABLE)

debug: CFLAGS += -g -Wall
debug: clean $(SRCS) $(EXECUTABLE)

profiling: CFLAGS += -pg
profiling: all

$(EXECUTABLE): $(OBJS)
	$(CC) -I$(INC) $(CFLAGS) $(OBJS) -o $@  -L$(LIB) $(LIBS)

.c.o:
	$(CC) -I$(INC) $(CFLAGS) -c $< -o $@ -L$(LIB) $(LIBS)

clean:
	rm -rf *.o $(EXECUTABLE) $(DB)

remake: clean all
reprofiling: clean profiling
