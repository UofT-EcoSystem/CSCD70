# LLVM Docker Image

This folder contains the LLVM *Docker* image which you could use to build the examples in any OS.

## Introduction

*Docker* has the 

Compared with the virtual machine solution, *Docker* has very low performance overhead ("*The general result is that Docker is **nearly identical to Native performance** and faster than KVM in every category*" as per the performance study [here](https://stackoverflow.com/a/26149994/6320608)).

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

