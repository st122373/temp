#include "cosinus.h"

bool Cosinus::IsIt(const QString &func)
{
    return (func == "cos");
}

QString Cosinus::Diff(const QString &func)
{
    return "-sin";
}
