Q=10
OS := $(shell uname)
ifeq ($(OS), Darwin)
DARWIN=true
else
DARWIN=false
endif

TAR=/usr/bin/tar
TEACHER=true
DEPS=false
CP=/bin/cp
CC=gcc
CFLAGS=-g -Wall -DTEACHER=$(TEACHER) -DDARWIN=$(DARWIN) -DQ=$(Q)
LDFLAGS=-g -DTEACHER=$(TEACHER) -pthread

PRESOURCES_1=\
cond_blocking_queue.c\
cond_blocking_queue.h\
main_executor.c\
executor.c\
executor.h\
future.c\
future.h\
blocking_queue.h\
blocking_queue.c\
sem_blocking_queue.h\
sem_blocking_queue.c\
thread_pool.c\
thread_pool.h\
scenario.c\
scenario.h\
task.c\
task.h\
utils.c\
utils.h\

SOURCES_1 = \
bounded_buffer.h\
bounded_buffer.c\

OBJECTS_1 = \
bounded_buffer.o\
cond_blocking_queue.o\
executor.o\
future.o\
main_executor.o\
blocking_queue.o\
scenario.o\
sem_blocking_queue.o\
task.o\
thread_pool.o\
utils.o\

PRESOURCES = \
$(PRESOURCES_1)\

SOURCES = \
$(SOURCES_1)\

OBJECTS = \
$(OBJECTS_1)\

PROGS = \
main_executor\

%.c: %.p.c
	awk -f presources.awk -v TEACHER=$(TEACHER) $< >$@
	clang-format -i $@

%.h: %.p.h
	awk -f presources.awk -v TEACHER=$(TEACHER) $< >$@
	clang-format -i $@

.c.o:
	$(CC) -c $(CFLAGS) $<

default : $(PROGS)

clean : 
	$(RM) $(OBJECTS) $(PROGS) *~

prepro-src-clean: clean
	$(RM) $(PRESOURCES)

main_executor : $(PRESOURCES_1) $(OBJECTS_1)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_1) 

student:
	@make prepro-src-clean
	@make TEACHER=false $(PRESOURCES)

teacher:
	@make prepro-src-clean
	@make TEACHER=true $(PRESOURCES)

index.html: index.texi
	makeinfo \
	        --no-headers --html --ifinfo --no-split \
		--css-include=style.css $< > $@

error :
	$(error "PREFIX variable not set")

install : index.html
	@if test -z "$(PREFIX)"; then \
	   make error; \
	fi
	@make student Q=10
	-mkdir -p $(PREFIX)
	chmod og=u-w $(PREFIX)
	$(TAR) zcf src.tar.gz `cat MANIFEST`
	chmod og=u-w style.css index.html src.tar.gz
	cp -p style.css index.html src.tar.gz $(PREFIX)

deps: $(SOURCES) $(PRESOURCES)
	$(CC) -M $(SOURCES) $(PRESOURCES) >deps

-include deps
