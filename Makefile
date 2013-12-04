.PHONY: build clean test
mode=release
build:
	scons -Q --debug=stacktrace mode=$(mode)
clean:
	scons --clean
test:
	scons test
memtest:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes \
	build-src/FireflyAssembler -p firefly -d hamming resources/sequences-1.fasta.out oo
