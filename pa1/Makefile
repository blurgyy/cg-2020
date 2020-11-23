proj = pa1
srcs = *.cpp include/*
builddir = build

makeflags = -j$(shell expr $(shell nproc) - 1)

all: ${srcs} CMakeLists.txt
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	@make -C ${builddir} ${makeflags}
clean:
	@make -C ${builddir} clean
purge:
	-rm -rf ${builddir}
fresh: purge all

.PHONY: all clean fresh purge

# Author: Blurgy <gy@blurgy.xyz>
# Date:   Nov 18 2020, 16:54 [CST]
