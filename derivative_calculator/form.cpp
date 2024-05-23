#include "form.h"
#include "differentiation.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
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
    QStringList arguments = parseExpression(func);
    QString derivative = computeDerivative(arguments);

    resultTextBox->setText(arguments.join(", "));
    diffTextBox->setText(derivative);
}

QStringList Form::parseExpression(const QString &expression)
{
    QStringList arguments;
    QString sourceExpression = expression.simplified().replace(" ", "");

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

    return arguments;
}

QString Form::computeDerivative(const QStringList &arguments)
{
    QStringList localArguments = arguments;
    QStringList operations = { "(", "+", "-", "*", "/", "^", ")", "(", "()" };

    while (localArguments.size() > 1)
    {
        QString firstOper = "";
        int firstOperIndex = localArguments.size();
        for (const QString &str : operations)
        {
            int index = localArguments.indexOf(str);
            if (index > 0 && index < firstOperIndex)
            {
                firstOperIndex = index;
                firstOper = str;
            }
        }

        QString f1, f2, df1, df2, oper, expression, diff;

        if (firstOper == "()")
        {
            localArguments.removeAt(firstOperIndex);
            int ComposeIndex = localArguments.lastIndexOf("Compose", firstOperIndex);
            if (ComposeIndex > 0)
            {
                f1 = localArguments[ComposeIndex - 1];
                df1 = computeSimpleDerivative(f1);
                f2 = localArguments[ComposeIndex + 1];
                df2 = computeSimpleDerivative(f2);
                oper = "Compose";
                expression = f1 + "(" + f2 + ")";
                diff = computeSimpleDerivative(f1 + "(" + f2 + ")");

                localArguments.removeAt(ComposeIndex + 1);
                localArguments.removeAt(ComposeIndex);
                localArguments[ComposeIndex - 1] = expression;
            }
            else
            {
                continue;
            }
        }
        else
        {
            f1 = localArguments[firstOperIndex - 2];
            df1 = computeSimpleDerivative(f1);
            f2 = localArguments[firstOperIndex - 1];
            df2 = computeSimpleDerivative(f2);
            oper = localArguments[firstOperIndex];
            expression = f1 + oper + f2;
            diff = computeSimpleDerivative(f1 + oper + f2);

            localArguments.removeAt(firstOperIndex);
            localArguments.removeAt(firstOperIndex - 1);
            localArguments[firstOperIndex - 2] = expression;
        }

        if (!diffsList.contains(expression))
        {
            diffsList.insert(expression, diff);
        }
    }

    return localArguments.first();
}

QString Form::computeSimpleDerivative(const QString &func)
{
    // Реализуйте вычисление элементарных производных, аналогично C# версии

    return Diff(func);
}
