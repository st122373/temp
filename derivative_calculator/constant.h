#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>

class Constant
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};

#endif // CONSTANT_H
