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
Compared with virtual machines, Docker has negligible performance
overhead [1]_, making itself an ideal solution for software development nowadays.

.. raw:: html

    <embed>
      <p align="middle">
        <img width="32.9%" src="https://docs.docker.com/images/Container%402x.png">
        <img width="32.9%" src="https://docs.docker.com/images/VM%402x.png">
      </p>
    </embed>
[2]_

.. [1] https://stackoverflow.com/a/26149994/6320608
.. [2] https://docs.docker.com/get-started/
