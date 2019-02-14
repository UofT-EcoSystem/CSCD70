# LLVM Docker Image

This folder contains the LLVM *Docker* image which you could use to build the examples in any OS.

## Introduction

*Docker* has the 

## Instructions

- Download *Docker*: 
  [Windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows), 
  [MAC](https://hub.docker.com/editions/community/docker-ce-desktop-mac), 
  [Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/).
- Build the LLVM image.

```
# Build the docker image from file Dockerfile and tag (-t) it with name "llvm:6.0".
docker build . -t llvm:6.0
```

