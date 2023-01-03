FROM ubuntu:22.04

ENV LLVM_VERSION=16

RUN sed -i 's/#force_color_prompt=yes/force_color_prompt=yes/' ~/.bashrc

RUN apt-get update && \
    env DEBIAN_FRONTEND=noninteractive \
    apt-get install -y vim \
                       git \
                       build-essential \
                       python3-dev \
                       python-is-python3 \
                       wget \
                       ca-certificates \
                       lsb-release \
                       software-properties-common \
                       gpg-agent && \
    rm -rf /var/lib/apt/lists/*

RUN git config --global user.name  "ubuntu" && \
    git config --global user.email "ubuntu@cs.toronto.edu" && \
    git config --global --add safe.directory "*"

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy main" && \
    apt-get update && \
    apt-get install -y llvm-${LLVM_VERSION} \
                       llvm-${LLVM_VERSION}-dev \
                       clang-${LLVM_VERSION} \
                       clang-format-${LLVM_VERSION} \
                       clang-tidy-${LLVM_VERSION} && \
    rm -rf /var/lib/apt/lists/*

RUN wget https://bootstrap.pypa.io/get-pip.py && \
    python3 get-pip.py && rm -f get-pip.py

RUN pip3 install lit==0.11.1 cmake
