#ifndef DIFFERENTIATION_H
#define DIFFERENTIATION_H

#include <QString>
#include <QMap>

QString Diff(const QString &func);
QString SimplifyMult(const QString &func1, const QString &func2);
QString SimplifyAdd(const QString &func1, const QString &func2, bool isAdd);

#endif // DIFFERENTIATION_H
