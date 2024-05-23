#include "differentiation.h"
#include "monome.h"
#include "constant.h"
#include "x.h"
#include "sinus.h"
#include "cosinus.h"
#include "exp.h"

QString Diff(const QString &func)
{
    QString result = "";
    if (Constant::IsIt(func))
    {
        result = Constant::Diff(func);
    }
    else if (X::IsIt(func))
    {
        result = X::Diff(func);
    }
    else if (Monome::IsIt(func))
    {
        result = Monome::Diff(func);
    }
    else if (Sinus::IsIt(func))
    {
        result = Sinus::Diff(func);
    }
    else if (Cosinus::IsIt(func))
    {
        result = Cosinus::Diff(func);
    }
    //else if (Exp::IsIt(func))
    //{
    //    result = Exp::Diff(func);
    //}

    return result;
}

QString SimplifyMult(const QString &func1, const QString &func2)
{
    QString result = "(" + func1 + ")*(" + func2 + ")";
    if (func1 == "0" || func2 == "0")
    {
        result = "0";
    }
    else if (func1 == "1")
    {
        result = func2;
    }
    else if (func2 == "1")
    {
        result = func1;
    }
    else if (Constant::IsIt(func1))
    {
        result = func1 + "*(" + func2 + ")";
    }
    else if (Constant::IsIt(func2))
    {
        result = func2 + "*(" + func1 + ")";
    }

    return result;
}

QString SimplifyAdd(const QString &func1, const QString &func2, bool isAdd)
{
    QString result = "";
    if (func2 == "0")
    {
        result = func1;
    }
    else if (func1 == "0")
    {
        result = (isAdd) ? func2 : "-" + func2;
    }
    else
    {
        result = (isAdd) ?
                     func1 + "+" + func2 :
                     func1 + "-" + func2;
    }

    return result;
}
