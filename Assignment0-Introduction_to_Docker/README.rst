Assignment 0 Introduction to Docker
===================================

.. contents::
   :local:

Introduction
------------

Docker runs our applications in a **lightweight** *container* by using the
*image* that has all the necessary software components installed. Think of it as
`conda` or `virtualenv` (both are used to create an containerized Python
environment), but at a broader scope for almost all common software libraries.
Compared with the virtual machines, Docker has negligible performance
overhead [1]_, making itself an ideal solution for software development nowadays.

.. raw:: html

   <embed>
     <p align="middle">
       <img width="32.9%" src="https://docs.docker.com/images/Container%402x.png">
       <img width="32.9%" src="https://docs.docker.com/images/VM%402x.png">
     </p>
   </embed>
[2]_

Instructions

- **Download and Install Docker**:
  
  :Windows: https://hub.docker.com/editions/community/docker-ce-desktop-windows
  :Mac: https://hub.docker.com/editions/community/docker-ce-desktop-mac
  :Ubuntu: https://docs.docker.com/install/linux/docker-ce/ubuntu/
- **Build the docker image**:

  - An *image* is an isolated environment that includes all necessary software
    components.
  .. code-block:: bash

     # Build the docker image from file Dockerfile and tag (-t) it with name `cscd70:2021S`.
     docker build . -t cscd70:6.0


References
----------

.. [1] https://stackoverflow.com/a/26149994/6320608
.. [2] https://docs.docker.com/get-started/
 
