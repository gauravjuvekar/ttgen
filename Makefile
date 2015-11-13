.PHONY: all
all:
	$(MAKE) -C src
	mv src/project .


.PHONY:clean
clean:
	$(MAKE) -C src clean
