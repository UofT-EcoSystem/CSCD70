# LLVM Docker Image

This folder contains the LLVM *Docker* image which you could use to run experiments or build examples in any OS.

## Introduction

*Docker* runs our applications in a **lightweight** container by using the image that has all the necessary software components installed. Compared with the virtual machine solution, *Docker* has very low performance overhead ("*The general result is that Docker is **nearly identical to Native performance** and faster than KVM in every category*" as per the performance study [here](https://stackoverflow.com/a/26149994/6320608)).

<p align="middle">
  <img width="32.9%" src="https://docs.docker.com/images/Container%402x.png">
  <img width="32.9%" src="https://docs.docker.com/images/VM%402x.png">
</p>

## Instructions

- Download *Docker*: 
  [Windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows), 
  [Mac](https://hub.docker.com/editions/community/docker-ce-desktop-mac), 
  [Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/).
- Build the LLVM image.

```
# Build the docker image from file Dockerfile and tag (-t) it with name "llvm:6.0".
docker build . -t llvm:6.0
```

