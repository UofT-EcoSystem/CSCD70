FROM ubuntu:20.04

ENV LLVM_VERSION=12

RUN apt-get update && \
    env DEBIAN_FRONTEND=noninteractive \
    apt-get install -y \
        vim git build-essential python3-dev \
        wget ca-certificates \
        lsb-release software-properties-common gpg-agent && \
    rm -rf /var/lib/apt/lists/*

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    add-apt-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-${LLVM_VERSION} main" && \
    apt-get update && \
    apt-get install -y llvm-${LLVM_VERSION} llvm-${LLVM_VERSION}-dev \
                       clang-${LLVM_VERSION} \
                       clang-format-${LLVM_VERSION} \
                       clang-tidy-${LLVM_VERSION} && \
    rm -rf /var/lib/apt/lists/*

RUN wget https://bootstrap.pypa.io/get-pip.py && \
    python3 get-pip.py && rm -f get-pip.py

RUN pip3 install lit==0.11.0 cmake==3.18.4
