
Neon software components
========================

Main Neon components are:
* base - contains portable system and bare minimum headers needed by all other
    Neon components. The portable system offers consistent API to control the
    underlaying microcontroller and OS (if used).
* kernel - contains core libraries, the cooperative scheduler, memory management, 
    and virtual timers. 
* eds (Event Driven System) - provides event generation and handling, event 
    queues, broadcasting services and execution of finite state machines.
* drivers - provide a higher level API to access microcontroller peripherals.
* lib - is a collection of higher level libraries intended to be used by the
    user application.
    
For more details about each software component please consult the README file 
for the component you are interested in.


