#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->sb_coefficientNu->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->sb_coefficientNu->setReadOnly(true);

    ui->le_serviceLife->setPlaceholderText("Если срок службы не устанавливается, принимаем T = 25");
    ui->le_serviceLife->setClearButtonEnabled(true);
    ui->le_thicknessElem->setClearButtonEnabled(true);
    ui->le_beamResistance->setClearButtonEnabled(true);

    ui->pb_caiculus->setEnabled(false);
    ui->le_beamResistance->setEnabled(false);

    connect(ui->le_serviceLife, &QLineEdit::textChanged, this, &Widget::validateInputs);
    connect(ui->le_thicknessElem, &QLineEdit::textChanged, this, &Widget::validateInputs);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_cbox_ReH_currentIndexChanged(int index)
{
    qDebug() << Q_FUNC_INFO << "index = " << index;

    if (index == 0)
    {
        ui->lb_flowNormalStrain->setText("0.00");
        ui->lb_flowShearStrain->setText("0.00");
        ui->lb_normalStrain->setText("0.00");
        ui->lb_shearStrain->setText("0.00");
        return;
    }
    else if (index == 1)
    {
        coefficientNu = 1.0;
    }
    else if (index == 2)
    {
        coefficientNu = 0.78;
    }
    else if (index == 3)
    {
        coefficientNu = 0.72;
    }
    else if (index == 4)
    {
        coefficientNu = 0.68;
    }

    QString str = ui->cbox_ReH->currentText();
    str.remove('>');
    str.remove('=');

    bool ok{};
    limitFlow = str.toDouble(&ok);

    if (!ok)
    {
        QMessageBox::warning(this, "Ошибка", "Неверный формат числа в 'Верхний предел текучести'");
        qWarning() << "Error! Incorrect number format!";
        return;
    }
    ui->sb_coefficientNu->setValue(coefficientNu);
    calculationsStrain();
}

void Widget::calculationsStrain()
{
    flowNormalStrain = 235 / coefficientNu;
    flowShearStrain = 0.57 * flowNormalStrain;

    normalStrain = 0.8 * flowNormalStrain;
    shearStrain = 0.8 * flowShearStrain;

    ui->lb_flowNormalStrain->setText(QString::number(flowNormalStrain));
    ui->lb_flowShearStrain->setText(QString::number(flowShearStrain));

    ui->lb_normalStrain->setText(QString::number(normalStrain));
    ui->lb_shearStrain->setText(QString::number(shearStrain));
}


void Widget::on_pb_caiculus_clicked()
{
    safetyMargin = thicknessElem * (serviceLife - 12);
    ui->lb_safetyMargin->setText(QString::number(safetyMargin));
    ui->le_beamResistance->setEnabled(true);
}

void Widget::on_le_beamResistance_textEdited(const QString &arg1)
{
    if (safetyMargin >0)
    {
        bool ok_beamResistance{};
        beamResistance = arg1.toDouble(&ok_beamResistance);

        if (!ok_beamResistance && arg1 != "")
        {
            qWarning() << "Error! Incorrect number format!";
            ui->lb_correctionForWear->setText("Ошибка ввода");
            return;
        }

        double Ak{};

        if (beamResistance >= 200)
        {
            Ak = (1/ 0.15) * (0.01 + (1 / beamResistance));
        }
        else
        {
            Ak = 0.07 +(6 / beamResistance);

            if (Ak > 0.25)
                Ak = 0.25;
        }

        double correctionForWear = 1 + (Ak * safetyMargin);
        ui->lb_correctionForWear->setText(QString::number(correctionForWear));
    }
}


bool Widget::validateInputs()
{
    bool serviceLifeValid, thicknessElemValid;
    serviceLife = ui->le_serviceLife->text().toDouble(&serviceLifeValid);
    thicknessElem = ui->le_thicknessElem->text().toDouble(&thicknessElemValid);

    bool isValid = serviceLifeValid && thicknessElemValid;
    ui->pb_caiculus->setEnabled(isValid);

    return isValid;
}
