proj = oglt
srcs = main.cpp CMakeLists.txt
builddir = build

makeflags = -j$(shell expr $(shell nproc) - 1)

all: ${srcs}
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	@make -C ${builddir} ${makeflags}

clean:
	@make -C ${builddir} clean

# Author: Blurgy <gy@blurgy.xyz>
# Date:   Nov 18 2020, 15:38 [CST]
