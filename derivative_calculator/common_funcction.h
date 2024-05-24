#ifndef COMMON_FUNCCTION_H
#define COMMON_FUNCCTION_H

#include <QString>
//constant
class Constant
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};
//cosinus
class Cosinus
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};
//exp
class Exp
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};
//monome
class Monome
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};
//sinus
class Sinus
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};
//x
class X
{
public:
    static bool IsIt(const QString &func);
    static QString Diff(const QString &func);
};

#endif // COMMON_FUNCCTION_H
