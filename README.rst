=============
blockmatchgpu
=============

**Block Matcher GPU**

BlockMatchGPU is a mini-app implementation of the standard block-matching algorithm used to generate a disparity map given a pair of stereo images.

* GitHub repo: https://github.com/nasa-nccs-hpda/blockmatchgpu 
* Documentation: https://nasa-nccs-hpda.github.io/blockmatchgpu/

**Contents**

- `Objectives`_
- `Background`_
- `Getting Started`_
- `Manual Testing`_
- `Authors`_
- `Contributing`_
- `References`_

Objectives
============

BlockMatchGPU is a mini-app implementation of the standard block-matching algorithm used to generate a disparity map given a pair of stereo images.

The current implementation is very simple and was written as a proof-of-concept for accelerating disparity map calculation algorithms using GPUs.

Cost function used: sum-of-squares


Background
============

Developed as part of a NASA OpenHackathon (Sep 2023)

Getting Started
=================

Installation
--------------

Environment:
* Follow the instructions to create an env that contains the dependencies for Ames Stereo Pipeline (ASP). 

Additional dependencies:
* Support for nvcc (Nvidia's compiler)

Manual Testing
====================

To test both CPU and GPU implementations::

  git clone https://github.com/nasa-nccs-hpda/blockmatchgpu.git
  cd blockmatchgpu
  make
  ./install/bin/block_match_both


To run the block-match GPU implementation on rasters::

  ./install/bin/tif_block_match_gpu <left image path> <right image path>

Authors
====================

* Jordan Alexis Caraballo-Vega, jordan.a.caraballo-vega@nasa.gov
* Caleb S. Spradlin, caleb.s.spradlin@nasa.gov
* Jian Li, jian.li@nasa.gov


Contributing
====================

Contributions
are welcome, and they are greatly appreciated! Every little bit helps, and credit will
always be given.

You can contribute in many ways:

Report Bugs
-------------

Report bugs at https://github.com/nasa-cisto-ai/blockmatchgpu/issues.

If you are reporting a bug, please include:

* Your operating system name and version.
* Any details about your local setup that might be helpful in troubleshooting.
* Detailed steps to reproduce the bug.

Fix Bugs
-------------

Look through the GitHub issues for bugs. Anything tagged with "bug" and
"help wanted" is open to whoever wants to implement it.

Implement Features
--------------------

Look through the GitHub issues for features. Anything tagged with "enhancement" and "help wanted" is
open to whoever wants to implement it.

Write Documentation
------------------------

blockmatchgpu could always use more documentation, whether as part of the official blockmatchgpu docs,
in docstrings, or even on the web in blog posts, articles, and such.

Submit Feedback
--------------------

The best way to send feedback is to file an issue at https://github.com/nasa-cisto-ai/blockmatchgpu/issues.

If you are proposing a feature:

* Explain in detail how it would work.
* Keep the scope as narrow as possible, to make it easier to implement.
* Remember that this is a volunteer-driven project, and that contributions are welcome :)

References
============

* Beyer, R. A., Alexandrov, O., & McMichael, S. (2018). The Ames Stereo Pipeline: NASA's open source software for deriving and processing terrain data. Earth and Space Science, 5(9), 537-548.
* Caraballo-Vega, J., Carroll, M., Li, J., & Duffy, D. (2021, December). Towards Scalable & GPU Accelerated Earth Science Imagery Processing: An AI/ML Case Study. In AGU Fall Meeting 2021. AGU.
