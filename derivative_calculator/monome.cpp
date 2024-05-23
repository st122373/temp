#include "monome.h"
#include <QRegularExpression>

bool Monome::IsIt(const QString &func)
{
    // Проверка, является ли строка мономом вида ax^b, где a и b - числа
    QRegularExpression re("^-?\\d*\\.?\\d*x\\^?-?\\d*\\.?\\d*$");
    QRegularExpressionMatch match = re.match(func);
    return match.hasMatch();
}

QString Monome::Diff(const QString &func)
{
    // Регулярное выражение для разбора монома вида ax^b
    QRegularExpression re("(-?\\d*\\.?\\d*)x\\^(-?\\d*\\.?\\d*)");
    QRegularExpressionMatch match = re.match(func);

    if (!match.hasMatch())
    {
        return "0";  // Если не удается разобрать, возвращаем 0
    }

    // Получаем коэффициенты a и b
    QString aStr = match.captured(1);
    QString bStr = match.captured(2);

    // Преобразуем строки в числа
    double a = aStr.isEmpty() ? 1 : aStr.toDouble();
    double b = bStr.isEmpty() ? 1 : bStr.toDouble();

    // Вычисляем производную
    double newA = a * b;
    double newB = b - 1;

    // Формируем результат
    if (newB == 0)
    {
        return QString::number(newA);
    }
    else if (newB == 1)
    {
        return QString::number(newA) + "x";
    }
    else
    {
        return QString::number(newA) + "x^" + QString::number(newB);
    }
}
