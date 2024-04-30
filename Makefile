OUTPUT=main.out
MAIN=main.c
SOURCE=src
BINARIES=bin

all:
	@make -s clean;
	@make -s build;
	@make -s run;

build:
	@mkdir $(BINARIES)
	@gcc -o $(BINARIES)/$(OUTPUT) $(SOURCE)/$(MAIN) -lpng -lm
# @echo "project built"

clean:
	@rm -rf $(BINARIES)
#	@echo "binaries cleaned"

run:
	@chmod +x $(BINARIES)/$(OUTPUT)
	@./$(BINARIES)/$(OUTPUT)