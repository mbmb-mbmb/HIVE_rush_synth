NAME = synth
CC = cc

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS = -Wall -Wextra -Werror -I/opt/homebrew/include
	LDFLAGS = -L/opt/homebrew/lib -lportaudio -lm
else
	CFLAGS = -Wall -Wextra -Werror -I.
	LDFLAGS = libportaudio.a -lrt -lm -lasound -ljack -pthread
endif

SRCS = synth.c \
		set_note.c \
		waveforms.c \
		mixer.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
