# GTlab Python Module

This module integrates Python into GTlab. Python is an interpreted programming 
language and is therefore suitable for script programming. Scripts are small 
programs that are usually implemented within one source code file. A Python 
interpreter reads the source files and executes the instructions defined in 
them. The GTlab Python Module integrates three interfaces into GTlab, which make
it possible to pass Python scripts to a Python interpreter. 

* <b>Python Console</b> 

    The GTlab Python Module integrates a Python Console into GTlab. This is 
    available via the Python Console tab next to the Application Console tab. 
    The Python Console allows you to navigate through projects and read and 
    modify data from the data model using Python code. It provides an automatic 
    completion that suggests available objects and functions. The Fig. 1
    illustrates how the navigation through the data model works. 

    <figure class="image">
        <img src="/images/python_console.png" alt="Python Console">
        <figcaption> <i>Fig. 1: Python Console</i></figcaption>
    </figure>

* <b>Python Calculator</b> 

    The Python Calculator is a calculator element which can be integrated into 
    a process chain. The wizard of the Python Calculator provides an editor for 
    defining Python scripts.  When creating scripts using the editor, all data 
    model packages can be accessed. The editor offers the possibility to 
    evaluate the script (Ctrl+E), whereby errors can be identified directly. 
    It should be noted that all changes to data model objects are not 
    transferred to the central data model until the Python Calculator is
    executed as part of a process chain. The output that results from the
    evaluation is displayed on the console below the editor. When the Python 
    calculator is executed as part of a process chain, the output is displayed
    on the Python Console described above (Fig. 1). The Python Calculator is 
    helpful to integrate and test new calculation methods during the runtime 
    of GTlab without having to recompile some program parts.

* <b>Python Task</b> 

    The Python Task enables the creation and execution of process chains in the 
    form of Python scripts. The Python Calculator wizard also provides an editor
    for defining Python scripts. In addition to data model access, calulators 
    can be instantiated, configured and executed. For this the wizards of all
    available calculators can be called. If the configuration of a calculator 
    via the wizard is completed, the Python code corresponding to the 
    configuration is automatically inserted into the editor. A calculator can
    be executed by calling the run() method. When evaluating (Ctrl+E) the
    script, the run() method returns False and the calculator is not executed. 
    The run() method is only executed when the Python Task is executed via 
    process control. The output of the evaluation is displayed on the console 
    below the editor. When executing the Python Task via process control, the 
    output is displayed on the Python Console described above (Fig. 1). 
    The Python task enables the flexible interconnection of calculators. 
    In addition, data from the central data model can be read and modified.
