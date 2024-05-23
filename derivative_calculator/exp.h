#ifndef EXP_H
#define EXP_H

#include <QString>

class Exp
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};

#endif // EXP_H
