Syllabus
========


General Course Information
--------------------------

:Instructor: Professor Gennady Pekhimenko
:Teaching Assistant: `Bojian Zheng`_
:Lectures [#]_:
    | Mon. 1:00-3:00 p.m.
    | https://vectorinstitute.zoom.us/j/99214420711 (Passcode: 487511)
:Tutorials:
    | Mon. 10:00-11:00 a.m.
    | https://utoronto.zoom.us/j/85194477652 (Passcode: 040333)
:Discussion Board: https://piazza.com/utoronto.ca/winter2021/cscd70/home
:MarkUs:  https://markus.utsc.utoronto.ca/cscd70w21

.. _bojian Zheng: mailto:bojian@cs.toronto.edu
.. [#] Lectures and tutorials will be recorded and later uploaded to `UofT MyMedia`_ and YouTube.

.. _UofT MyMedia: https://mymedia.library.utoronto.ca/

Course Description
------------------

The goal of this course is introduce students to the theoretical and practical
aspects of building optimizing compilers that effectively exploit modern
architectures. The course will begin with the fundamentals of compiler
optimization, and will build upon these fundamentals to address issues in
state-of-the-art commercial and research machines. Topics include the
followings: intermediate representations, basic blocks and control flow graphs,
dataflow analysis, partial redundancy elimination, loop optimizations, register
allocation, instruction scheduling, inter-procedural analysis, memory hierarchy
optimizations, extracting parallelism, and dynamic optimizations. Students will
implement significant optimizations within LLVM, a modern research compiler framework.


Prerequisites
-------------

**This course is not supposed to be your first C/C++ course. Previous knowledge on compiler frontend is recommended but not required.**
If you feel uncertain about whether you are adequately prepared to take this class,
please discuss this with the instructor.

Course Work and Grading
-----------------------

* **Assignments (45%)**
  
  A major focus of this course is the assignments. We prefer that you work in
  **groups of two** on the assignments. The assignments will contain both
  programming and theoretical questions. There will be three assignments, each
  worth 15% of your final grade. The first assignment will be distributed on
  Jan. 11 and it will introduce you to the LLVM compiler framework.

* **Midterm (20%)** and **Final Exam (35%)**

  There will be a midterm exam covering the first half of the course material
  and a final covering the whole. Both exams will be closed book, closed notes
  [#]_.

.. [#] Please note that due to the situation of the pandemic, the exams will
       most likely happen online. We will announce more details on this later.
