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

RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh ${LLVM_VERSION} && \
    rm llvm.sh && \
    rm -rf /var/lib/apt/lists/*

RUN wget https://bootstrap.pypa.io/get-pip.py && \
    python3 get-pip.py && rm -f get-pip.py

RUN pip3 install lit==15.0.6 cmake
