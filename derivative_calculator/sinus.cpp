#include "sinus.h"

bool Sinus::IsIt(const QString &func)
{
    return (func == "sin");
}

QString Sinus::Diff(const QString &func)
{
    return "cos";
}
