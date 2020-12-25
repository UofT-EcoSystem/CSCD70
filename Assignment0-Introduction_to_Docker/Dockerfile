FROM ubuntu:20.04

ENV LLVM_VERSION=12

RUN apt-get update && \
    env DEBIAN_FRONTEND=noninteractive \
    apt-get install -y \
        vim git build-essential python3-dev \
        wget ca-certificates \
        lsb-release software-properties-common gpg-agent && \
    rm -rf /var/lib/apt/lists/*

RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh ${LLVM_VERSION} && \
    apt-get install -y llvm llvm-dev clang clang-format clang-tidy && \
    rm -rf llvm.sh /var/lib/apt/lists/*

RUN wget https://bootstrap.pypa.io/get-pip.py && \
    python3 get-pip.py && rm -f get-pip.py

RUN pip3 install lit==0.11.0 cmake==3.18.4
