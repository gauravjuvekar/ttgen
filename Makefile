project: src/project
	mv src/project .

src/project:
	$(MAKE) -C src

.PHONY:clean
clean:
	$(MAKE) -C src clean
