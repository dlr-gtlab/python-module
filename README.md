# GTlab Python Module

This module integrates Python into GTlab. Python is an interpreted programming 
language and is therefore suitable for script programming. Scripts are small 
programs that are usually implemented within one source code file. A Python 
interpreter reads the source files and executes the instructions defined in 
them. The GTlab Python Module integrates three interfaces into GTlab, which make
it possible to pass Python scripts to a Python interpreter. 

* <b>Python Console</b> 

    The GTlab Python Module integrates a Python console into GTlab. This is 
    available via the Python console tab next to the application console tab. 
    The Python console allows you to navigate through projects and read and 
    modify data from the data model using Python code. It provides an automatic 
    completion that suggests available objects and functions. The figure 1
    illustrates how the navigation through the data model works. 

    <figure class="image">
        <img src="/images/python_console.png" alt="Python Console">
        <figcaption> <i>Figure 1: Python Console</i></figcaption>
    </figure>

* <b>Python Calculator</b> 

    The Python Calculator is a calculator element which can be integrated into 
    a process chain. The wizard of the Python Calculator provides an editor for 
    defining Python scripts.  When creating scripts using the editor, all data 
    model packages can be accessed. The editor offers the possibility to 
    evaluate the script (Ctrl+E), whereby errors can be identified directly. 
    It should be noted that all changes to data model objects are not 
    transferred to the central data model until the Python calculator is
    executed as part of a process chain.  The Python calculator is helpful to 
    integrate and test new calculation methods during the runtime of GTlab
    without having to recompile some program parts.  






* <b>Python Task</b> 