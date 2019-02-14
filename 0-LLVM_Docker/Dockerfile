FROM ubuntu:18.04

ENV LLVM_VERSION=6.0

RUN apt-get update && apt-get install -y --no-install-recommends \
    vim g++ make libboost-all-dev \
    llvm-${LLVM_VERSION} \
    clang-${LLVM_VERSION} && \
    rm -rf /var/lib/apt/lists/*
