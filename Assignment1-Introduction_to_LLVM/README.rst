Assignment 1 Introduction to LLVM |ready|
=========================================

.. |ready| image:: https://img.shields.io/badge/ready%3F-no-red

.. contents::
   :local:

Abstract
--------

Welcome to CSCD70 Compiler Optimization. We will be using the Low Level Virtual
Machine (LLVM) Compiler infrastructure from University of Illinois
Urbana-Champaign (UIUC) for our programming assignments.

The objective of this first assignment is to introduce you to LLVM and some ways
that it can be used to make your programs run faster. In particular, you will be
using LLVM to analyze code to output interesting properties about your program
(Section `Function Information`_) and to perform simple optimizations (Section
`Local Optimizations`_).

Policy
------

Collaboration
`````````````

You will work in groups of **two** for the assignments in this course. Please turn
in a single submission per group.

Submission
``````````

Please submit all your source code and writeup to `MarkUs`_. Make sure to
include the followings:

.. _MarkUs: https://markus.utsc.utoronto.ca/cscd70w21

Problem Statement
-----------------


Function Information
````````````````````

Local Optimizations
```````````````````

FAQs
----

*Given below is the questions asked during previous offering of the class.
Please make sure to read them carefully* **BEFORE** *approaching the
assignment. If you do not think they fully answer your question, always
feel free to open a new thread on* Piazza_ *.*

.. _Piazza: https://piazza.com/utoronto.ca/winter2021/cscd70/home

:Q: **Can I work in group of one?**
:A: | Yes. You can. However, we would like to remind you that working in group
      of one would **NOT** give you any advantage in terms of grading. 
    | For people working in group of two, although it is up to you to decide how
      to distribute the work evenly between the group members, **BOTH** of you
      are responsible for knowing all the assignment materials as they will be
      tested in the exams.

:Q: **Are we allowed to include headers other than those that are provided by LLVM (e.g., STL)?**
:A: Yes. You can, but we strongly doubt whether this is necessary to use
    libraries beyond those of LLVM and STL in this course.


:Q: **How do we know what each type of instruction does?**
:A: For most instructions you can directly infer from their names. Please refer
    to the `LLVM Language Reference Manual`_ for more detailed information.

:Q: **Suppose that we have the statement** :math:`i = i + 1` **. Should we treat
    expression** :math:`i + 1` **as available after the statement?**

.. _LLVM Language Reference Manual: https://llvm.org/docs/LangRef.html