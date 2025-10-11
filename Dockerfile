FROM ubuntu:22.04

# Install build essentials
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    mingw-w64 \ 
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY src/. .
COPY CMakeLists.txt .
COPY mingw_toolchain.cmake .


RUN mkdir build 

RUN cd build && \
    cmake -DCMAKE_TOOLCHAIN_FILE=../mingw_toolchain.cmake .. &&\
    make

