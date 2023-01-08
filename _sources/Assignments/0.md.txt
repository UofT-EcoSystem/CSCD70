# Assignment 0 Introduction to Docker

Please feel free to skip this section if you are working on the MathLab machines
@`mathlab.utsc.utoronto.ca` as they <del>already</del> will have all the
necessary software components installed.

## Introduction

Docker runs our applications in a *lightweight* **container** by using the
**image** that has all the necessary software components installed. Think of it
as `conda` or `virtualenv` (both are used to as an containerized Python
environment), but at a broader scope for almost all software libraries. Compared
with the virtual machines, Docker has negligible performance overhead, making
itself an ideal solution for software development.

## Instructions

### Docker

- **Download and Install Docker**:
  [Windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows),
  [Mac](https://hub.docker.com/editions/community/docker-ce-desktop-mac),
  [Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
- **Build the Docker image**:
  - An **image** is an isolated environment that includes all necessary software
    components.
  ```Bash
  # Build the docker image from file Dockerfile and tag (-t) it with name `cscd70:2023S`.
  cd Assignment0-Introduction_to_Docker
  docker build . -t cscd70:2023S
  ```
- **Create a Docker container**:
  - A **container** is an instantiation of an image. Multiple containers can be
    created out of the same image.
  ```Bash
  # Create a container for the first assignment.
  cd Assignment1-Introduction_to_LLVM/FunctionInfo
  docker run -it -v $(pwd):/mnt --rm --name cscd70_a1 cscd70:2023S
  ```
  - `-t`: Allocate a pseudo terminal; `-i`: Connect to STDIN (i.e., keyboard).
    `-it` must be used jointly for an interactive bash shell.
  - `-v`: Mount the current working directory (i.e., `FunctionInfo`) to `/mnt`
    in the container. Any changes made to the mounted directory outside the
    container will be reflected in the container as well (and vice versa). The
    mounted directory must be in **absolute path**.
  - `--rm`: \[Optional\] Automatically clean the container when we exit.
  - `--name`: \[Optional\] Name of the container
  - The image name (i.e., `cscd70:2023S`) should always come last.

- **Run the experiments**:
  ```Bash
  cd /mnt
  mkdir build && cd build
  cmake .. && make
  ```

### Docker-Compose
 
Docker-Compose is a simple wrapper on top of the docker commands. There is a
docker-compose file provided in the root folder of this repository. We can do
the same things as we have previously described, but much simpler:

```Bash
# Install docker-compose via python-pip.
pip3 install docker-compose

docker-compose build dev     # Build the image.
docker-compose run --rm dev  # Create a container.
```
