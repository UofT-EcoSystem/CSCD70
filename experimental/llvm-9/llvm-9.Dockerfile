FROM ubuntu:18.04

ENV LLVM_VERSION=9

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        wget gnupg ca-certificates \
        software-properties-common python3-dev

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-9 main"

RUN wget https://bootstrap.pypa.io/get-pip.py && \
    python3 get-pip.py && rm -f get-pip.py && \
    apt-get purge -y wget gnupg ca-certificates software-properties-common

RUN apt-get install -y --no-install-recommends \
        vim build-essential python3-dev \
        llvm-${LLVM_VERSION} clang-${LLVM_VERSION} && \
    rm -rf /var/lib/apt/lists/*

RUN pip3 install lit cmake==3.14.4
