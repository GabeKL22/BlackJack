RUN = g++
CFLAGS = 0
OBJ =
MAIN = rungame.cpp
TEST = 0

run: $(MAIN) $(OBJ)
	$(RUN) -c -o $@ $<


.PHONY: clean
clean:
	rm run
