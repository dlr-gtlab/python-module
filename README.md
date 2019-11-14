# GTlab Python Module

This module embeds Python into GTlab. Python is an interpreted programming 
language that is suitable for script programming. Scripts are small 
programs that are usually implemented within one source code file. A Python 
Interpreter reads the source code file and executes the instructions defined in 
them. The Python Module offers the possibility to define and execute process chains 
by using Python. This increases the flexibility of process chains and enables 
the extension of GTlab's preliminary design calculation methods without source 
code access and C++ knowledge. To use these advantages the Python Module extends
GTlab by the following three interfaces. 

* <b>Python Console</b> 

    The Python Module integrates a Python Console into GTlab. This is 
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
        <figcaption> <i>Fig. 2: Python Script Calculator Wizard</i></figcaption>
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
    process control. The benefit of this task is to enable the flexible 
    interconnection of calculators. The calculators can be executed iteratively 
    and depending on previously calculated result data. All advantages of Python 
    are available for additional calculations and modifications of the data 
    model.
    
    <figure class="image">
        <img src="/images/python_task.PNG" 
        alt="Python Task" width="600" height="450">
        <figcaption> <i>Fig. 3: Python Task Wizard</i></figcaption>
    </figure>

### PythonQt for embedding Python 
For embedding Python into the Python Module the library 
[PythonQt](https://mevislab.github.io/pythonqt/ "This link takes you to the homepage of PythonQt.")
was used. It is designed to embed a Python Interpreter into an existing C++
application so users can control parts of the application using Python at 
runtime. PythonQt makes use of the 
[Meta-Object System](https://doc.qt.io/archives/qt-4.8/metaobjects.html "This link takes you to a description of the Meat-Object System.")
of Qt to transfer C++ objects to the Python environment.


### Compiling the Python Module
To compile the Python module, you must copy the file local_setting.pri from the 
features directory and paste it next to the project file gtlab_python.pro. 
The local_settings.pri contains the following three paths that you must adapt
for your local system. 

  * DEV_TOOLS  = path\to\your\local\GTlab-DevTools
    
  * GTLAB_REPO = path\to\a\local\stored\GTlab-Repository
    
  * GTLAB_PATH = path\to\the\installed\version\of\GTlab 

The path to the DevTools is always needed because the compiled version of 
PythonQt and Python is stored in the DevTools. Whether you have to specify the path 
to the repository or the path to the installed GTlab version depends on which 
compile mode you choose. You can specify the compile mode with the value of the
variable COMPILE_MODE. The compile mode determines whether the header files of 
the DevTools or a GTlab repository are used. It also determines in which build 
directory the binaries will be stored. 

There are four modes:

|   COMPILE_MODE  | description                                                                                                                                   | paths to specify         |
|  ---            |  ------                                                                                                                                       | ------                   |
| module          | header files of DevTools are used;<br> a build directory which contains the binaries will be created at the Python Module directory           | DEV_TOOLS                |
| moduleTests     | header files of a GTlab reposetory are used;<br> a build directory which contains the binaries will be created at the Python Module directory | DEV_TOOLS<br> GTLAB_REPO |
| gtlab           | header files of DevTools are used;<br> the binaries will be stored in the bin directory of the installed GTlab version                        | DEV_TOOLS<br> GTLAB_PATH |
| gtlabRepository | header files of a GTlab reposetory are used;<br> the binaries will be stored in the \build\modules\ directory of the GTlab repository         | DEV_TOOLS<br> GTLAB_REPO |