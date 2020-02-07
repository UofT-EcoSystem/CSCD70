FROM ubuntu:18.04

ENV LLVM_VERSION=6.0

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        vim git ca-certificates \
        build-essential \
        llvm-${LLVM_VERSION} \
        llvm-${LLVM_VERSION}-dev \
        clang-${LLVM_VERSION} && \
    rm -rf /var/lib/apt/lists/*
