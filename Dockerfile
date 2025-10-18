FROM ubuntu:22.04

# Install build essentials
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    mingw-w64 \
    g++-mingw-w64-x86-64-posix \
    && rm -rf /var/lib/apt/lists/*

# need posix variant compilers apparently
RUN update-alternatives \
    --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix && \
    update-alternatives \
    --set x86_64-w64-mingw32-gcc /usr/bin/x86_64-w64-mingw32-gcc-posix

# confirm toolchain properties
# RUN cmake --version
# RUN x86_64-w64-mingw32-g++ -v | grep -i Thread

WORKDIR /app
COPY src/ src/
COPY CMakeLists.txt .
COPY mingw_toolchain.cmake .
COPY external/ external/
COPY assets/ assets/

RUN rm -rf build && mkdir build 
WORKDIR /app/build

# build
RUN cmake -DCMAKE_TOOLCHAIN_FILE=../mingw_toolchain.cmake .. && make

# debug:
# RUN cmake -DCMAKE_TOOLCHAIN_FILE=../mingw_toolchain.cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON .. \
#  && make VERBOSE=1

# Bundle exe and runtime libraries
RUN rm -rf /app/bundle
RUN mkdir -p /app/bundle && \
    cp /app/build/SnakeGame.exe /app/bundle/ && \
    cp -r /app/build/assets /app/bundle/ && \
    cp /app/external/SDL2/bin/SDL2.dll /app/bundle/ && \
    cp /app/external/SDL2_ttf/bin/SDL2_ttf.dll /app/bundle/ && \
    cp /usr/lib/gcc/x86_64-w64-mingw32/10-posix/libstdc++-6.dll /app/bundle/ && \
    cp /usr/lib/gcc/x86_64-w64-mingw32/10-posix/libgcc_s_seh-1.dll /app/bundle/ && \
    cp /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll /app/bundle/



