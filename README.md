
Neon software components
========================

Main Neon components are:
* eds (Event Driven System) - provides event generation and handling, event 
    queues, broadcasting services and execution of finite state machines. This
    component contains portable system and bare minimum headers needed by all 
    other Neon components. The portable system offers consistent API to control 
    the underlaying microcontroller and OS (if used). The Kernel contains core 
    libraries, the cooperative scheduler, memory management, and virtual timers. 
* drivers - provide a higher level API to access microcontroller peripherals.
* lib - is a collection of higher level libraries intended to be used by the
    user application.
    
For more details about each software component please consult the README file 
for the component you are interested in.


Fetching sources
----------------
Neon sources consist of multiple git modules. To fetch all sources do the following:

    git clone https://github.com/nradulovic/neon.git
    cd neon
    git submodule update --init --recursive

Support
-------
If you've found an error, please [file an issue](https://github.com/nradulovic/neon/issues/new).

Patches are encouraged, and may be submitted by [forking this project](https://github.com/nradulovic/neon/fork) and
submitting a pull request through GitHub. Please see [CONTRIBUTING.md](CONTRIBUTING.md) for more details.
