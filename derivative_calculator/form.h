#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMap>


class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);

private slots:
    void onCalculateClicked();

private:
    QLineEdit *functionTextBox;
    QTextEdit *resultTextBox;
    QTextEdit *diffTextBox;
    QPushButton *calculateButton;

    QStringList parseExpression(const QString &expression);
    QString computeDerivative(const QStringList &arguments);
    QString computeSimpleDerivative(const QString &func);

    QMap<QString, QString> diffsList;
};

#endif // FORM_H
