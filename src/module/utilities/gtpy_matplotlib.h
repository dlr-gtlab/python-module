/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_matplotlib.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 12.09.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_MATPLOTLIB_H
#define GTPY_MATPLOTLIB_H

namespace gtpy
{

namespace matplotlib
{

///Backend name
constexpr const char* backendName = "gtlab_svg_backend";

constexpr const char* customBackend = R"(
import os
import threading
import matplotlib.pyplot as plt
from matplotlib.backend_bases import Gcf
from matplotlib.backends.backend_svg import FigureCanvasSVG, FigureManagerSVG

FigureCanvas = FigureCanvasSVG
FigureManager = FigureManagerSVG

global gt_temp

backend_lock = threading.Lock()


def set_temp_dir(d):
    global gt_temp
    gt_temp = d


def show(*args, **kwargs):
    with backend_lock:
        global gt_temp

        thread_path = os.path.join(gt_temp, str(threading.current_thread().ident))
        if not os.path.exists(thread_path):
            os.makedirs(thread_path, exist_ok=True)

        for num, fig_manager in enumerate(Gcf.get_all_fig_managers()):
            output_path = os.path.join(thread_path, f"figure_{num}.svg")

            fig = fig_manager.canvas.figure
            fig.set_size_inches(6, 4)
            fig.tight_layout()

            fig.savefig(output_path, format='svg')
            plt.close(fig)
)";

constexpr const char* setCustomBackend = R"(
try:
    import os
    import matplotlib
    import tempfile

    from gtlab_svg_backend import set_temp_dir

    pid = os.getpid()
    dir_name = os.path.join(tempfile.gettempdir(), f"pid_{pid}")
    if not os.path.exists(dir_name):
        os.makedirs(dir_name, exist_ok=True)

    set_temp_dir(dir_name)
    matplotlib.use("module://gtlab_svg_backend")
except ImportError:
    pass
)";

} // namespace constants

} // namespace gtpy

#endif // GTPY_MATPLOTLIB_H
