proj = pbr
srcs = main.cpp
builddir = build

nprocs = $(shell expr $(shell nproc) - 1) # Parallel compiling
ifeq ($(strip ${nprocs}), 0)
	nprocs = 1 # `-j` does not take 0 as a valid jobs count
endif
makeflags = -j${nprocs}

deafult: release

release: ${srcs} CMakeLists.txt
	cmake -S . -B ${builddir} -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
		-DCMAKE_BUILD_TYPE="Release"
	@make -C ${builddir} ${makeflags}
debug: ${srcs} CMakeLists.txt
	cmake -S . -B ${builddir} -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
		-DCMAKE_BUILD_TYPE="Debug"
	@make -C ${builddir} ${makeflags}
clean:
	@make -C ${builddir} clean
purge:
	-rm -rf ${builddir}

.PHONY: default clean debug purge release

# vim: set ft=make:

# Author: Blurgy <gy@blurgy.xyz>
# Date:   Jan 29 2021, 15:32 [CST]
