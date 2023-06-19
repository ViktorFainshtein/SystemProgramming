gd = gcc
gc = gcc
vlg = valgrind --leak-check=yes --track-origins=yes

GDFLAGS = -ansi -pedantic-errors -Wall -Wextra -g $(DEPFLAGS)
GCFLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3 $(DEPFLAGS)
DEPFLAGS = -MP -MD

DIR = .

TARGET = foo.out
CFILES = $(foreach D,$(DIR),$(wildcard $(D)/*.c))
OBJECTS = $(patsubst %.c,%.o,$(CFILES))
DEPFILES = $(patsubst %.c,%.d,$(CFILES))


all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJECTS)
	$(gd) $(GDFLAGS) $^ -o $@
	
%.o : %.c
	$(gd) $(GDFLAGS) -c -o $@ $<
	
clean:
	rm -rf $(OBJECTS) $(DEPFILES) $(TARGET)

release: $(OBJECTS)
	$(gc) $(GCFLAGS) $^ -o $(TARGET)
	
vlg:	$(TARGET)
	$(vlg) ./$(TARGET)
	
debug:	$(TARGET)
	$(gd) ./$(TARGET)
	
-include $(DEPFILES)
