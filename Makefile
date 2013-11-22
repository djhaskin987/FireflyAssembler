.PHONY: build clean test
build:
	scons -Q --debug=stacktrace
clean:
	scons --clean
test:
	scons test
memtest:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes \
	build-src/FireflyAssembler -p firefly -d hamming resources/sequences-1.fasta.out oo
