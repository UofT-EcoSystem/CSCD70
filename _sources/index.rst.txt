.. CSCD70 documentation master file.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

CSCD70 Compiler Optimization
============================

.. toctree::
   :hidden:

   Slides
   Assignments/index


Welcome everyone to CSCD70. The first lecture and tutorial will be on **Jan.
9**. We will be using Quercus and Piazza for important announcements and this
webpage for uploading course materials (|:point_right:| the right sidebar), so
please make sure that you check them regularly for any updates.


Syllabus
--------

General Course Information
^^^^^^^^^^^^^^^^^^^^^^^^^^

:Instructor: Professor Gennady Pekhimenko
:Teaching Assistant: Bojian Zheng
:Lectures: Mon. 1:00-3:00 p.m. AA 206
:Tutorials: Mon. 11:00-12:00 a.m. HW 402 [#]_
:Discussion Board: https://piazza.com/utoronto.ca/winter2023/cscd70/home

.. [#] Please make sure that you are enrolled in the tutorials. If, after Jan.
    22 (the add/drop deadline) you are still not in a tutorial, please send an
    email to Jenn Chaskavich (`jenn.chaskavich@utoronto.ca`_) and she will
    enroll you.

.. _jenn.chaskavich@utoronto.ca: mailto:jenn.chaskavich@utoronto.ca

Course Description
^^^^^^^^^^^^^^^^^^

The goal of this course is to introduce students to the theoretical and
practical aspects of building optimizing compilers that effectively exploit
modern architectures. The course will begin with the fundamentals of compiler
optimization, and will build upon these fundamentals to address issues in
state-of-the-art commercial and research machines. Topics include the
followings: intermediate representations, basic blocks and control flow graphs,
dataflow analysis, partial redundancy elimination, loop optimizations, register
allocation, instruction scheduling, inter-procedural analysis, memory hierarchy
optimizations, extracting parallelism, and dynamic optimizations. Students will
implement significant optimizations within LLVM, a modern research compiler
framework.

Prerequisites
^^^^^^^^^^^^^

**This course is not supposed to be your first C/C++ course. Previous knowledge
on compiler frontend is recommended but not required.** If you feel uncertain
about whether you are adequately prepared to take this class, please discuss
this with the instructor.

Course Works and Grading
^^^^^^^^^^^^^^^^^^^^^^^^

* **Assignments (45%)**
  
  A major focus of this course is the assignments. We prefer that you work in
  **groups of two** on the assignments. The assignments will contain both
  programming and theoretical questions. There will be three assignments, each
  worth 15% of your final grade. The first assignment will be distributed on
  Jan. 9 and it will introduce you to the LLVM compiler framework.

* **Midterm (20%)** and **Final Exam (35%)**

  There will be a midterm exam covering the first half of the course material
  and a final covering the whole. Both exams will be closed book, closed notes.
