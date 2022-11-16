#ifndef GT_PYTHONPREFERENCEPAGE_H
#define GT_PYTHONPREFERENCEPAGE_H

#include <gt_preferencespage.h>

#include <memory.h>

namespace Ui
{
class GtPythonPreferencePage;
} // namespace Ui

class GtPythonPreferencePage : public GtPreferencesPage
{
    Q_OBJECT

public:
    explicit GtPythonPreferencePage();
    ~GtPythonPreferencePage() override;

    void saveSettings(GtSettings &) const override;
    void loadSettings(const GtSettings &) override;

private slots:
    void onBtnSelectPyExePressed();
    void onBtnTestPyEnvPressed();

private:

    std::unique_ptr<Ui::GtPythonPreferencePage> ui;
};

#endif // GT_PYTHONPREFERENCEPAGE_H
