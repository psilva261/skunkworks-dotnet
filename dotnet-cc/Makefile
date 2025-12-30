.PHONY: all docker-build compile format run

all: docker-build compile-rv64 run

docker-build:
	./docker_build.sh

compile-rv64: docker-build
	make -C examples/basic basic.bin

compile: compile-rv64

run: compile
	make -C examples/basic run

format: Example.cs
	dotnet format

clean:
	rm -f out/mod0.c out/mod0.h
	make -C zkvm clean
