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

## License

The largest portion of the code is licensed under the `Apache 2.0 License`.

Smaller thirdparty party code part of code base uses other permissive licenses, such as the
`BSD` and `MIT` licenses. Please review the directory [LICENSES](https://github.com/dlr-gtlab/python-module/tree/master/LICENSES) and [.reuse](https://github.com/dlr-gtlab/python-module/tree/master/.reuse)
for a full overview on all licensed used.


## Compiling from Source

### Prerequisites

A working GTlab installation is required. This includes GTlab and the GTlab Logging library.
In addition, the Qt toolkit need to be installed to build the Python module.

The Python module uses the `PythonQt` library to embed the Python interpreter into GTlab
and provide access to GTlab's data model structure and the slots of all classes.
__PythonQt will be automatically downloaded and built by the Python module.__ 
If you want to use your own installation of PythonQt, use `BUILD_BUNDLED_PYTHONQT=OFF` 
in the CMake configuration.


| Library              |  Version  | Bundled | Where to get                                 |
| -------------------- | --------- | ------- | -------------------------------------------- |
| Qt                   |  5.15.x   | No      | https://download.qt.io/official_releases/qt/ |
| GTlab Core + Logging |  >= 2.0   | No      | https://github.com/dlr-gtlab/gtlab-core      |
| Python               |  3.9.x    | No      | https://www.python.org/downloads/            |
| PythonQt             | >= 3.5.0  | Yes     | https://github.com/MeVisLab/pythonqt         |

### Building

The python module requires a recent `CMake` (>3.15) to build. The configuration and build process is
similar to other CMake builds:

```
cmake -S . -B build_dir -DQt5_DIR=<path/to/cmake/Qt5> -DCMAKE_PREFIX_PATH=<path/to/gtlab_install> -DCMAKE_INSTALL_PREFIX=<path/to/gtlab_install>
cmake --build build_dir
cmake --build build_dir --target install
```
