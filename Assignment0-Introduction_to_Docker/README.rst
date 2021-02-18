Assignment 0 Introduction to Docker
===================================

.. note::

   Please feel free to skip this section if you are working on the MathLab
   machines @\ |MathLab|_ as they already have all the necessary software
   components installed.

.. |MathLab| replace:: mathlab.utsc.utoronto.ca
.. _MathLab: mathlab.utsc.utoronto.ca

.. contents::
   :local:


Introduction
------------

Docker runs our applications in a **lightweight** *container* by using the
*image* that has all the necessary software components installed. Think of it as
`conda` or `virtualenv` (both are used to as an containerized Python
environment), but at a broader scope for almost all software libraries. Compared
with the virtual machines, Docker has negligible performance overhead [1]_,
making itself an ideal solution for software development.

.. raw:: html

   <embed>
     <p align="middle">
       <img width="32.9%" src="https://docs.docker.com/images/Container%402x.png">
       <img width="32.9%" src="https://docs.docker.com/images/VM%402x.png">
     </p>
   </embed>

[2]_


Instructions
------------

Docker
""""""

- **Download and Install Docker**:
  
  :Windows: https://hub.docker.com/editions/community/docker-ce-desktop-windows
  :Mac: https://hub.docker.com/editions/community/docker-ce-desktop-mac
  :Ubuntu: https://docs.docker.com/install/linux/docker-ce/ubuntu/
- **Build the Docker image**:

  - An *image* is an isolated environment that includes all necessary software
    components.

  .. code-block:: bash

     # Build the docker image from file Dockerfile and tag (-t) it with name `cscd70:2021S`.
     cd Assignment0-Introduction_to_Docker
     docker build . -t cscd70:2021S

- **Create a Docker container**:

  - A *container* is an instantiation of an image. Multiple
    containers can be created out of the same image.

  .. code-block:: bash

     # Create a container for the first assignment.
     cd Assignment1-Introduction_to_LLVM/FunctionInfo
     docker run -it -v $(pwd):/mnt --rm --name cscd70_a1 cscd70:2021S
  
  -t      Allocate a pseudo terminal.
  -i      Connect to STDIN (i.e., keyboard).
  -v      Mount the current working directory (i.e., :code:`FunctionInfo`) to :code:`/mnt` in the container.
  --rm    (Optional) Cleanup the container when we exit.
  --name  (Optional) Name of the Container

  - Note that:

    #. :code:`-it` must be used jointly for an interactive bash shell,
    #. Any changes made natively in the mounted directory will be reflected in the container as well (and vice versa).
       The mounted directory must be in **absolute path**.
    #. The image name (i.e., :code:`cscd70:2021S`) should always come last.

- **Run the experiments**:

  .. code-block:: bash
     
     cd /mnt
     mkdir build && cd build
     cmake .. && make

Docker-Compose
""""""""""""""
 
Docker-Compose is a simple wrapper on top of the docker commands. There is a
docker-compose file provided in the root folder of the `GitHub repo`_. We can do
the same things as we have previously described, but much simpler:

.. _GitHub repo: https://github.com/UofT-EcoSystem/CSCD70

.. code-block:: bash

   # Install docker-compose via python-pip.
   pip3 install docker-compose

   docker-compose build cscd70-dev     # Build the image.
   docker-compose run --rm cscd70-dev  # Create a container.


References
----------

.. [1] https://stackoverflow.com/a/26149994/6320608
.. [2] https://docs.docker.com/get-started/
