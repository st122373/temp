#include "form.h"
#include "common_funcction.h"
#include "differentiation.h"

Form::Form(QWidget *parent): QWidget(parent)
{
    functionTextBox = new QLineEdit(this);
    resultTextBox = new QTextEdit(this);
    diffTextBox = new QTextEdit(this);
    calculateButton = new QPushButton("Calculate", this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Function:"));
    mainLayout->addWidget(functionTextBox);
    mainLayout->addWidget(calculateButton);
    mainLayout->addWidget(new QLabel("Result:"));
    mainLayout->addWidget(resultTextBox);
    mainLayout->addWidget(new QLabel("Derivative:"));
    mainLayout->addWidget(diffTextBox);

    setLayout(mainLayout);

    connect(calculateButton, &QPushButton::clicked, this, &Form::onCalculateClicked);
}

void Form::onCalculateClicked()
{
    QString func = functionTextBox->text();
    qDebug() << "Input function:" << func;
    QStringList arguments = parseExpression(func);
    qDebug() << "Parsed expression:" << arguments; // Отладочное сообщение
    QString derivative = computeDerivative(arguments);
    qDebug() << "Computed derivative:" << derivative; // Отладочное сообщение

    resultTextBox->setText(arguments.join(", "));
    diffTextBox->setText(derivative);
}

QStringList Form::parseExpression(const QString &expression)
{
    QStringList arguments;
    QString sourceExpression = expression.simplified().replace(" ", "");
    qDebug() << "Parsing expression:" << sourceExpression;

    int symbolNumber = 0;
    QString operand1 = "";
    QString operand2 = "";
    QString lastOperation = "";
    QStringList operations = { "(", "+", "-", "*", "/", "^", ")", "(", "()" };
    QStringList functions = { "sin", "cos", "exp", "tan", "ln", "lg", "sh", "ch" };

    while (symbolNumber < sourceExpression.length())
    {
        int currentIndex = arguments.count() - 1;

        if (sourceExpression[symbolNumber] == '(')
        {
            arguments.append("(");
            while (operations.contains(arguments[currentIndex]) &&
                   operations.lastIndexOf(sourceExpression[symbolNumber]) >
                       operations.indexOf(arguments[currentIndex]))
            {
                arguments[currentIndex + 1] = arguments[currentIndex];
                currentIndex--;
            }
            arguments[currentIndex + 1] = "(";
            lastOperation = "(";
            symbolNumber++;
            continue;
        }
        else if (sourceExpression[symbolNumber] == ')')
        {
            arguments[arguments.lastIndexOf("(")] = "()";
            lastOperation = "()";
            symbolNumber++;
            continue;
        }
        else if (operations.contains(sourceExpression[symbolNumber]))
        {
            if (lastOperation == "(")
            {
                int index = arguments.lastIndexOf(lastOperation);
                arguments.append(lastOperation);
                while (currentIndex >= index)
                {
                    arguments[currentIndex + 1] = arguments[currentIndex];
                    currentIndex--;
                }
                arguments[currentIndex + 1] = sourceExpression[symbolNumber];
            }
            else
            {
                arguments.append(lastOperation);
                while (operations.contains(arguments[currentIndex]) &&
                       operations.indexOf(sourceExpression[symbolNumber]) >
                           operations.indexOf(arguments[currentIndex]))
                {
                    arguments[currentIndex + 1] = arguments[currentIndex];
                    currentIndex--;
                }
                arguments[currentIndex + 1] = sourceExpression[symbolNumber];
            }
            lastOperation = sourceExpression[symbolNumber];
            symbolNumber++;
        }
        else if (operand1.isEmpty())
        {
            while (symbolNumber < sourceExpression.length() &&
                   !operations.contains(sourceExpression[symbolNumber]))
            {
                operand1 += sourceExpression[symbolNumber];
                symbolNumber++;
            }
            arguments.append(operand1);
            if (functions.contains(operand1))
            {
                arguments.append("Compose");
            }
            operand2.clear();
        }
        else if (operand2.isEmpty())
        {
            while (symbolNumber < sourceExpression.length() &&
                   !operations.contains(sourceExpression[symbolNumber]))
            {
                operand2 += sourceExpression[symbolNumber];
                symbolNumber++;
            }

            int index = arguments.lastIndexOf(lastOperation);
            int bracketIndex = arguments.indexOf("(");
            if (bracketIndex > 0 &&
                arguments.lastIndexOf(lastOperation, bracketIndex) > 0 &&
                arguments.lastIndexOf(lastOperation, bracketIndex) < index)
            {
                index = arguments.lastIndexOf(lastOperation, arguments.indexOf("("));
            }
            arguments.append(operand2);
            if (functions.contains(operand2))
            {
                arguments.append("Compose");
            }

            int currentIndex = arguments.count() - 1;
            while (currentIndex > index)
            {
                if (functions.contains(operand2))
                {
                    arguments[currentIndex] = arguments[currentIndex - 2];
                }
                else
                {
                    arguments[currentIndex] = arguments[currentIndex - 1];
                }
                currentIndex--;
            }

            arguments[index] = operand2;
            if (functions.contains(operand2))
            {
                arguments[index + 1] = "Compose";
            }
            operand2.clear();
        }
    }

    qDebug() << "Parsed arguments:" << arguments;
    return arguments;
}

QString Form::computeDerivative(const QStringList &arguments)
{
    qDebug() << "Computing derivative for arguments:" << arguments;
    QStringList localArguments = arguments;
    qDebug() << "Initial localArguments:" << localArguments;

    QStringList operations = { "+", "-", "*", "/", "^" };

    while (localArguments.size() > 0)
    {
        qDebug() << "Entering while loop with localArguments:" << localArguments;
        QString firstOper = "";
        int firstOperIndex = localArguments.size();
        qDebug() << "firstOperIndex = " << firstOperIndex;

        for (int i = 0; i < operations.size(); ++i)
        {
            QString str = operations[i];
            int index = localArguments.indexOf(str);
            qDebug() << "Checking operation:" << str << "Index found:" << index;

            if (index >= 0 && index < firstOperIndex)
            {
                firstOperIndex = index;
                firstOper = str;
            }
        }
        qDebug() << "firstOperIndex" << firstOperIndex;
        qDebug() << "firstOper" << firstOper;
        qDebug() << "End of for loop";

        QString f1, f2, df1, df2, oper, expression, diff;

        // Проверка границ
        if (firstOperIndex - 1 < 0 || firstOperIndex >= localArguments.size()) {
            return "Error: Invalid expression";
        }

        f1 = localArguments[firstOperIndex - 2];
        f2 = localArguments[firstOperIndex - 1];
        oper = localArguments[firstOperIndex];

        qDebug() << "f1 = " << f1;
        df1 = computeSimpleDerivative(f1);
        qDebug() << "df1 = " << df1;
        qDebug() << "f2 = " << f2;
        df2 = computeSimpleDerivative(f2);
        qDebug() << "df2 = " << df2;
        qDebug() << "oper = " << oper;

        // Обработка операций
        if (oper == "+")
        {
            expression = f1 + "+" + f2;
            diff = SimplifyAdd(df1, df2, true);
            qDebug() << "Operator + defined";
            return diff;
        }
        else if (oper == "-")
        {
            expression = f1 + "-" + f2;
            diff = SimplifyAdd(df1, df2, false);
            qDebug() << "Operator - defined";
            return diff;
        }
        else if (oper == "*")
        {
            expression = f1 + "*" + f2;
            diff = SimplifyAdd(SimplifyMult(df1, f2), SimplifyMult(f1, df2), true);
            qDebug() << "Operator * defined";
            return diff;
        }
        else if (oper == "/")
        {
            expression = f1 + "/" + f2;
            diff = "(" + SimplifyMult(df1, f2) + "-" + SimplifyMult(f1, df2) + ")/(" + f2 + "^2)";
            qDebug() << "Operator / defined";
            return diff;
        }
        else if (oper == "^")
        {
            expression = f1 + "^" + f2;
            if (X::IsIt(f1) && Constant::IsIt(f2))
            {
                double exponent = f2.toDouble();
                diff = QString::number(exponent) + "*" + f1 + "^" + QString::number(exponent - 1);
            }
            else
            {
                diff = "Unsupported operation for derivatives";
            }
            qDebug() << "Operator ^ defined";
            return diff;
        }
        else
        {
            diff = "Unsupported operation";
            return diff;
        }

        // Удаление обработанных элементов и добавление результата
        localArguments.removeAt(firstOperIndex + 1);
        localArguments.removeAt(firstOperIndex);
        localArguments[firstOperIndex - 1] = diff;

        if (!diffsList.contains(expression)) {
            diffsList.insert(expression, diff);
        }
    }

    qDebug() << "Final localArguments:" << localArguments;

    return localArguments.isEmpty() ? "Error computing derivative" : localArguments.first();
}

QString Form::computeSimpleDerivative(const QString &func)
{
    qDebug() << "Computing derivative for:" << func; // Отладочное сообщение
    if (Constant::IsIt(func))
    {
        qDebug() << "Identified as Constant";
        return Constant::Diff(func);
    }
    else if (X::IsIt(func))
    {
        qDebug() << "Identified as variable x";
        return X::Diff(func);
    }
    else if (Monome::IsIt(func))
    {
        qDebug() << "Identified as Monome";
        return Monome::Diff(func);
    }
    else if (Sinus::IsIt(func))
    {
        qDebug() << "Identified as Sinus";
        return Sinus::Diff(func);
    }
    else if (Cosinus::IsIt(func))
    {
        qDebug() << "Identified as Cosinus";
        return Cosinus::Diff(func);
    }
    else if (Exp::IsIt(func))
    {
        qDebug() << "Identified as Exp";
        return Exp::Diff(func);
    }
    else
    {
        qDebug() << "Unsupported function";
        return "Unsupported function";
    }
}
