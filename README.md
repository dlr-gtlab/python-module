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
        <img src="/images/python_script_calculator.png" 
        alt="Python Script Calculator" width="600" height="450">
        <figcaption> <i>Fig. 2: Python Script Calculator</i></figcaption>
    </figure>

* <b>Python Task</b> 

    The Python Task enables the creation and execution of process chains in the 
    form of Python scripts. The wizard of the Python Task also provides an editor
    for defining Python scripts (Fig. 3). It offers the same functionality as 
    the editor of the Python Script Calculator. In addition it allows the 
    instantiation, configuration and execution of calculators. For this the 
    wizards of all available calculators can be called by using the
    'Add' button. If the configuration of a calculator via its wizard is 
    completed, the Python code corresponding to the configuration will be 
    automatically inserted into the editor. A calculator instance can be 
    executed by calling its run() method. During the evaluation (Ctrl+E) 
    the run() method returns False and the calculators are not executed. 
    The calculators are only executed when the Python Task is executed via
    process control. The Python task enables the flexible interconnection of 
    calculators. The calculators can be executed iteratively and depending on 
    previously calculated result data. All advantages of Python are available 
    for additional calculations and modifications of the data model.
    
    <figure class="image">
        <img src="/images/python_task.PNG" 
        alt="Python Task" width="600" height="450">
        <figcaption> <i>Fig. 3: Python Task</i></figcaption>
    </figure>
