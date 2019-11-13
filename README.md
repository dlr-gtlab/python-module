# GTlab Python Module

This module integrates Python into GTlab. Python is an interpreted programming 
language that is suitable for script programming. Scripts are small 
programs that are usually implemented within one source code file. A Python 
interpreter reads the source files and executes the instructions defined in 
them. The GTlab Python Module integrates three interfaces into GTlab which make
it possible to execute Python scripts using a Python interpreter. By using the 
interfaces the central data model can be modified and the preliminary engine
design can be controlled with Python scripts.

* <b>Python Console</b> 

    The GTlab Python Module integrates a Python Console into GTlab. This is 
    available via the Python Console tab next to the Application tab (Fig. 1). 
    The Python Console allows you to navigate through projects and read and 
    modify data from the central data model using Python code. It provides an 
    automatic completion that suggests available objects and functions.

    <figure class="image">
        <img src="/images/python_console.png" alt="Python Console">
        <figcaption> <i>Fig. 1: Python Console</i></figcaption>
    </figure>

* <b>Python Script Calculator</b> 

    The Python Script Calculator is a calculator element which can be integrated
    into process chains. The wizard of the Python Script Calculator provides an 
    editor for defining Python scripts (Fig. 2). When creating scripts using this
    editor all data model packages can be accessed. So it is possible to read
    and modify data from the central data model. To identify errors the editor 
    offers the possibility to evalute the script (Ctrl+E). The evaluation is 
    also necessary to register newly defined variables, classes and methods in 
    the Python context. After evaluation the newly defined components are 
    available via automatic completion. Output messages that results from the 
    evaluation are displayed on the console below the editor. Note that any 
    changes to data model objects that result from the evaluation are not 
    transferred to the central data model. The data transfer will only happen, 
    if the Python Script Calculator is executed as part of a process chain. In 
    this case the output messages are displayed on the Python Console described 
    above (Fig. 1). The benefit of the Python Script Calculator is to integrate 
    and test new calculation methods during the runtime of GTlab. No recompiling
    of program parts is necessary.
    
    <figure class="image">
        <img src="/images/python_script_calculator.png" alt="Python Script Calculator" width="600" height="450">
        <figcaption> <i>Fig. 2: Python Script Calculator</i></figcaption>
    </figure>

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
