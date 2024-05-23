#include "constant.h"

bool Constant::IsIt(const QString &func)
{
    bool ok;
    func.toDouble(&ok);
    return ok;
}

QString Constant::Diff(const QString &func)
{
    return "0";
}
