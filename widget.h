#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QDebug>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void on_cbox_ReH_currentIndexChanged(int index);
    void on_pb_caiculus_clicked();
    void on_le_beamResistance_textEdited(const QString &arg1);

private:
    Ui::Widget *ui;

    double coefficientNu{};
    double limitFlow{};
    double flowNormalStrain{};
    double flowShearStrain{};
    double normalStrain{};
    double shearStrain{};
    double serviceLife{};
    double thicknessElem{};
    double safetyMargin{};
    double beamResistance{};

    void calculationsStrain();

    bool validateInputs();
};

#endif // WIDGET_H
