
.. contents:: Table of contents
   :backlinks: top
   :local:

Neon software components
========================

Main Neon components are:

* **Event Driven System** (``eds``) - provides event generation and handling,
  event queues, broadcasting services and execution of finite state machines.
  This component contains portable system and bare minimum headers needed by
  all other Neon components. The portable system offers consistent API to
  control the underlaying microcontroller and OS (if used). The Kernel contains
  core libraries, the cooperative scheduler, memory management, and virtual
  timers. 
* **Drivers** (``drivers``) - provide a higher level API to access
  microcontroller peripherals.
* **Library** (``lib``) - is a collection of higher level libraries intended to
  be used by the user application.
    
For more details about each software component please consult the README file 
for the component you are interested in.


Fetching sources
================

Neon sources consist of multiple git modules. To fetch all sources do the
following:

.. code:: bash

    git clone https://github.com/nradulovic/neon.git
    cd neon
    git submodule update --init --recursive

Support
=======

If you've found an error, please `file an issue
<https://github.com/nradulovic/neon/issues/new/>`_.

Patches are encouraged, and may be submitted by `forking this project 
<https://github.com/nradulovic/neon/fork/>`_ and submitting
a pull request through GitHub. Please see `CONTRIBUTING.rst
<https://github.com/nradulovic/neon/blob/master/CONTRIBUTING.rst/>`_ for more details.
