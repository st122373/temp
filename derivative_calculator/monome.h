#ifndef MONOME_H
#define MONOME_H

#include <QString>

class Monome
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};

#endif // MONOME_H
