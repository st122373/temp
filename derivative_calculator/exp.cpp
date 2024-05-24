#include "exp.h"

#include <QString>

bool Exp::IsIt(const QString &func)
{
    return (func == "exp");
}

QString Exp::Diff(const QString &func)
{
    return "exp";
}
