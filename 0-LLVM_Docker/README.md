# LLVM Docker Image

This folder contains the LLVM Docker image which you could use to run
experiments or build examples in almost any OS. You can skip this section if you
are using the UG workstations since they already have LLVM installed.

## Introduction

Docker runs our applications in a **lightweight** *container* by using the
*image* that has all the necessary software components installed. Think of it as
`conda` or `virtualenv` (both are used to create an isolated Python
environment), but at a broader scope for almost all common software libraries.
Compared with the virtual machine solution, Docker has very low performance
overhead ("*The general result is that Docker is **nearly identical to Native
performance** and faster than KVM in every category*"
[[1]](https://stackoverflow.com/a/26149994/6320608)).

<p align="middle">
  <img width="32.9%" src="https://docs.docker.com/images/Container%402x.png">
  <img width="32.9%" src="https://docs.docker.com/images/VM%402x.png">
  <a href="https://docs.docker.com/get-started/">[2]</a>
</p>

## Instructions

- **Download *Docker***: 
  [Windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows), 
  [Mac](https://hub.docker.com/editions/community/docker-ce-desktop-mac).

```bash
# Ubuntu (x86_64/amd64) Installation
sudo apt-get update && sudo apt-get install -y \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository \
    "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) \
    stable"
sudo apt-get update && sudo apt-get install -y \
    docker-ce \
    docker-ce-cli \
    containerd.io
```

- **Build the LLVM image**:

```bash
# Build the docker image from file Dockerfile and tag (-t) it with name "llvm:6.0".
docker build . -t llvm:6.0
```

- **Create a container**:

```bash
# Create a container for the first assignment.
cd ../Assignment1-Introduction_to_LLVM/FunctionInfo
docker run -it --rm -v $(pwd):/mnt --name CSCD70_A1 llvm:6.0 

# Options:
#    -it   : Allocate a pseudo tty (-t) and connect STDIN. 
#            These options must be used for interactive processes.
#   --rm   : Cleanup the container when we exit.
#   --name : Name of the Container
#    -v    : Mount the `FunctionInfo` directory to /mnt in the container.
#            This way any changes made natively will be reflected 
#            in the container as well (and vice versa).

# Note:
#   - The image name (llvm:6.0) should always come last.
#   - Directory path should be absolute path (`$(pwd)`).
#   - Additionally, you can append commands to the image name 
#     (e.g., `cd /mnt && make -f Optimize all`).
#     Otherwise the default command `/bin/bash` will run.
```

- **Run the experiments**:

```bash
cd /mnt && make -f Optimize.mk all
```

- **You can also combine the first two steps**:

```bash
cd ../Assignment1-Introduction_to_LLVM/FunctionInfo
docker run -it --rm -v $(pwd):/mnt --name CSCD70_A1 -w /mnt llvm:6.0 \
    make -f Optimize.mk all
# Pay attention to the additional `-w /mnt` (set working directory as `/mnt`)
# and the appended build command `make -f Optimize.mk all`.
```
