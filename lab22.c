#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

typedef double (*TFun)(double);

double Y(double);
double V(double);
void Out_Rez(TFun, double, double, double);
void calculate_value(TFun);
void tabulate_function(TFun);
void create_2d_array(TFun);
void print_menu();
TFun select_function();

// Глобальные переменные для хранения выбранной функции
TFun current_function;
char* current_function_name;

void main()
{
    int choice;
    setlocale(LC_ALL, "RUS");

    // Инициализация
    current_function = Y;
    current_function_name = "Y(x)";

    do {
        print_menu();
        printf("Выберите пункт меню: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            current_function = select_function();
            calculate_value(current_function);
            break;
        case 2:
            current_function = select_function();
            tabulate_function(current_function);
            break;
        case 3:
            current_function = select_function();
            create_2d_array(current_function);
            break;
        case 4:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }
        printf("\n");
    } while (choice != 4);
}

void print_menu()
{
    printf("=================================\n");
    printf("             МЕНЮ               \n");
    printf("=================================\n");
    printf("1. Вычислить значение\n");
    printf("2. Табулировать\n");
    printf("3. Сформировать двухмерный массив\n");
    printf("4. Выход\n");
    printf("=================================\n");
}

TFun select_function()
{
    int choice;
    printf("\nВыберите функцию:\n");
    printf("1. Y(x) = x³ + x/sin(x)\n");
    printf("2. V(x) - кусочно-заданная функция\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        current_function_name = "Y(x)";
        return Y;
    case 2:
        current_function_name = "V(x)";
        return V;
    default:
        printf("Неверный выбор! Используется Y(x) по умолчанию.\n");
        current_function_name = "Y(x)";
        return Y;
    }
}

void calculate_value(TFun f)
{
    double x;
    printf("\n--- Вычисление значения функции %s ---\n", current_function_name);
    printf("Введите значение x: ");
    scanf("%lf", &x);

    // Проверка особых случаев
    if (f == Y && fabs(sin(x)) < 1e-10) {
        printf("Ошибка: деление на ноль в функции Y(x) при x = %.3lf\n", x);
        return;
    }

    if (f == V && x == 2.0) {
        double znam = sin(3 * x) - 3 * x;
        if (fabs(znam) < 1e-10) {
            printf("Ошибка: деление на ноль в функции V(x) при x = 2.0\n");
            return;
        }
    }

    double result = f(x);
    printf("f(%.3lf) = %.3lf\n", x, result);
}

void tabulate_function(TFun f)
{
    double a, b, h;
    printf("\n--- Табулирование функции %s ---\n", current_function_name);
    puts("Введите начало интервала, конец и шаг:");
    scanf("%lf%lf%lf", &a, &b, &h);

    Out_Rez(f, a, b, h);
}

void create_2d_array(TFun f)
{
    double a, b, h;
    int n, i;

    printf("\n--- Формирование двухмерного массива для функции %s ---\n", current_function_name);
    puts("Введите начало интервала, конец и шаг:");
    scanf("%lf%lf%lf", &a, &b, &h);

    // Вычисление количества точек
    n = (int)((b - a) / h) + 1;

    // Создание двухмерного массива [n][2]
    double** array = (double**)malloc(n * sizeof(double*));
    for (i = 0; i < n; i++) {
        array[i] = (double*)malloc(2 * sizeof(double));
    }

    // Заполнение массива
    int count = 0;
    for (double x = a; x <= b + h / 2 && count < n; x += h, count++) {
        array[count][0] = x; // аргумент

        // Проверка особых случаев
        if (f == Y && fabs(sin(x)) < 1e-10) {
            array[count][1] = 0; // NaN для обозначения ошибки
        }
        else if (f == V && x == 2.0) {
            double znam = sin(3 * x) - 3 * x;
            if (fabs(znam) < 1e-10) {
                array[count][1] = 0;
            }
            else {
                array[count][1] = f(x);
            }
        }
        else {
            array[count][1] = f(x); // значение функции
        }
    }

    // Вывод массива
    printf("\nДвухмерный массив [аргумент, значение]:\n");
    printf("|-----------------------|\n");
    printf("|     x     |   f(x)    |\n");
    printf("|-----------------------|\n");

    for (i = 0; i < count; i++) {
        if (array[i][1] != array[i][1]) { // проверка на NaN
            printf("| %9.3lf |   ошибка  |\n", array[i][0]);
        }
        else {
            printf("| %9.3lf | %9.3lf |\n", array[i][0], array[i][1]);
        }
    }
    printf("|-----------------------|\n");

    // Освобождение памяти
    for (i = 0; i < n; i++) {
        free(array[i]);
    }
    free(array);
}

double Y(double x)
{
    return pow(x, 3) + x / sin(x);
}

double V(double x)
{
    if (x == 2.0)
    {
        return (cos(3 * x) + pow(x, 3)) / (sin(3 * x) - 3 * x);
    }
    else if (x > 2.0)
    {
        return (pow(sin(x), 2) + sqrt(pow(x, 2) + 2));
    }
    else
    {
        return pow(x, 3) * pow(log(fabs(atan(x) - exp(-x))) / log(2), 3);
    }
}

void Out_Rez(TFun f, double xn, double xk, double h)
{
    printf("|-----------------------|\n");
    printf("|     x     |    f(x)   |\n");
    printf("|-----------------------|\n");

    for (double x = xn; x <= xk + h / 2; x += h)
    {
        if (f == Y && fabs(sin(x)) < 1e-10)
        {
            printf("| %9.3lf |   ошибка  |\n", x);
            continue;
        }

        if (f == V && x == 2.0)
        {
            double znam = sin(3 * x) - 3 * x;
            if (fabs(znam) < 1e-10)
            {
                printf("| %9.3lf |   ошибка  |\n", x);
                continue;
            }
        }

        printf("| %9.3lf | %9.3lf |\n", x, f(x));
    }

    printf("|-----------------------|\n");
}