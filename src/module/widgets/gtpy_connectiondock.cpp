#include "gtpy_connectiondock.h"

GtpyConnectionDock::GtpyConnectionDock()
{
    setAcceptDrops(false);

    setObjectName(tr("Python Connections"));
}

Qt::DockWidgetArea GtpyConnectionDock::getDockWidgetArea()
{
    return Qt::RightDockWidgetArea;
}
