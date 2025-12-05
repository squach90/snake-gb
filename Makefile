# ROM targets
CTARGET = snake-c.gb

# Source files
C_SRC   = src/main.c src/snake.c

# SDCC tools
LCC     = lcc


# --------------------
# C build
# --------------------
$(CTARGET): $(C_SRC)
	$(LCC) -o $(CTARGET) $(C_SRC)

# --------------------
# Run
# --------------------
run: $(CTARGET)
	java -jar ~/Emulicious/Emulicious.jar $(CTARGET)

# --------------------
# Clean
# --------------------
clean:
	rm -f $(CTARGET) *.o *.rel *.map
