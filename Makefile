CC = gcc
CFLAGS += -std=c11 -Wall -Werror -g3 -O0
LDFLAGS = -D_POSIX_C_SOURCE=200809L -lm
ODIR=obj
TARGETS = smallsh
DEPS = smallsh.h

_OBJ = expand.o built_in.o sig_handlers.o main.o error_warn.o run.o \
	process_args.o fg_only_mode.o redirection.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $<

smallsh: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(TARGETS) $(ODIR)/*.o

.PHONY: clean
