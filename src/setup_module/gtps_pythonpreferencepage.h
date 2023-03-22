#ifndef GTPS_PYTHONPREFERENCEPAGE_H
#define GTPS_PYTHONPREFERENCEPAGE_H

#include "gt_preferencespage.h"

#include <memory.h>

namespace Ui
{
class GtPythonPreferencePage;
} // namespace Ui

/**
 * @brief The GtPythonPreferencePage class
 */
class GtPythonPreferencePage : public GtPreferencesPage
{
    Q_OBJECT

public:
    /**
     * @brief GtPythonPreferencePage
     */
    explicit GtPythonPreferencePage();

    /**
     * @brief Destructor.
     */
    ~GtPythonPreferencePage() override;

    /**
     * @brief saveSettings
     */
    void saveSettings(GtSettings &) const override;

    /**
     * @brief loadSettings
     */
    void loadSettings(const GtSettings &) override;

private slots:
    /**
     * @brief onBtnSelectPyExePressed
     */
    void onBtnSelectPyExePressed();
    /**
     * @brief onBtnTestPyEnvPressed
     */
    void onBtnTestPyEnvPressed();

    void setStatusTextColor(const QColor& color);

private:
    std::unique_ptr<Ui::GtPythonPreferencePage> ui;
};

#endif // GTPS_PYTHONPREFERENCEPAGE_H
