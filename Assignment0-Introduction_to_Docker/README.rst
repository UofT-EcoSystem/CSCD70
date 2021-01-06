Assignment 0 Introduction to Docker
===================================

.. contents::
   :local:

Introduction
------------

Docker runs our applications in a **lightweight** *container* by using the
*image* that has all the necessary software components installed. Think of it as
`conda` or `virtualenv` (both are used to create an isolated Python
environment), but at a broader scope for almost all common software libraries.
Compared with the virtual machine solution, Docker has very low performance
overhead ("*The general result is that Docker is **nearly identical to Native
performance** and faster than KVM in every category*" [#], making itself an
ideal solution for software development nowadays.

.. [#] (https://stackoverflow.com/a/26149994/6320608)

<p align="middle">
  <img width="32.9%" src="https://docs.docker.com/images/Container%402x.png">
  <img width="32.9%" src="https://docs.docker.com/images/VM%402x.png">
  <a href="https://docs.docker.com/get-started/">[2]</a>
</p>
