#ifndef GTPYCONNECTIONDOCK_H
#define GTPYCONNECTIONDOCK_H

#include "gt_dockwidget.h"

class GtpyConnectionDock: public GtDockWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyConnectionDock
     */
    Q_INVOKABLE GtpyConnectionDock();

    /** Returns the recommended dock widget area in main windows.
        @return dock widget area */
    Qt::DockWidgetArea getDockWidgetArea() override;
};

#endif // GTPYCONNECTIONDOCK_H
