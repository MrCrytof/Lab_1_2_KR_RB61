#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>   // Бібліотека для зчитування клавіш консолі (використовуємо getch)


// Обчислення значення вихідної функції f(x) відповідно до Варіанту 7
// Формула: f(x) = -(0.5*x - 64)^3 - 3*x + 10
double calculate_f(double x) {
    return -pow(0.5 * x - 64, 3) - 3 * x + 10;
}

int main() {
    // --- Оголошення змінних ---
    int variant = 0;
    double x1, x2;
    double delta;
    int input_N;
    unsigned int N;

    // 1. Блок вибору та валідації режиму введення (1 або 2)
    // Цикл while буде запитувати режим, поки користувач не введе 1 або 2
    while (variant != 1 && variant != 2) {
        printf("Select data input mode:\n");
        printf("1 - X1, X2, N (points count)\n");
        printf("2 - X1, X2, delta (step)\n");
        printf("Your choice (1 or 2): ");

        // Перевіряємо, чи введено саме число. Якщо введені букви — scanf поверне 0
        if (scanf("%d", &variant) != 1) {
            while (getchar() != '\n');
        }

        // Якщо ввели число, але воно не 1 і не 2 — показуємо повідомлення
        if (variant != 1 && variant != 2) {
            printf("\nError! Please enter only 1 or 2.\n\n");
        }
    }

    // 2. Блок введення початкових даних та їх повна валідація
    while (1) { // Нескінченний цикл, з якого вийдемо через break при коректних даних
        printf("\nEnter X1 (start): ");
        scanf("%lf", &x1);

        printf("Enter X2 (end, must be greater than X1): ");
        scanf("%lf", &x2);

        // Перевірка умови відрізка: початок має бути строго меншим за кінець
        if (x1 >= x2) {
            printf("Error! X2 must be strictly greater than X1. Please try again.\n");
            continue;
        }

        // Обробка для Варіанту 1 (введення X1, X2, N)
        if (variant == 1) {
            printf("Enter N (points count, N > 1): ");
            scanf("%d", &input_N);

            if (input_N <= 1) {
                printf("Error! N must be greater than 1. Please try again.\n");
                continue;
            }
            N = (unsigned int)input_N;
            delta = (x2 - x1) / (N - 1);
        }

        // Обробка для Варіанту 2 (введення X1, X2, delta)
        else {
            printf("Enter delta (step, delta > 0): ");
            scanf("%lf", &delta);

            // Перевірка: крок має бути додатним І меншим за сам відрізок (x2 - x1)
            if (delta <= 0) {
                printf("Error! Step delta must be greater than 0. Please try again.\n");
                continue;
            }
            if (delta > (x2 - x1)) {
                printf("Error! Step delta (%.2lf) must not exceed range length (%.2lf).\n", delta, x2 - x1);
                continue;
            }

            N = (unsigned int)((x2 - x1) / delta) + 1;
        }
        break; // Усі вхідні дані провалідовано успішно, виходимо з циклу
    }

    // 3. Виведення підтвердження вхідних даних
    printf("\n========================================\n");
    printf(" INPUT DATA:\n");
    printf(" X1 = %.4lf, X2 = %.4lf\n", x1, x2);
    printf(" N = %u, delta = %.4lf\n", N, delta);
    printf("========================================\n\n");

    // 4. Друк шапки таблиці псевдографікою
    printf("+-------+---------------+---------------+\n");
    printf("|   N   |       X       |     f(X)      |\n");
    printf("+-------+---------------+---------------+\n");

    // Змінні для збереження поточного та попереднього точок (для зміни знаку та кроку)
    double current_x = x1;
    double prev_x = x1;
    double prev_y = calculate_f(x1);
    int lines_on_page = 0;

    // Масиви для збереження меж проміжків ізоляції (де f(x) змінює знак)
    double intervals_a[100];
    double intervals_b[100];
    int interval_steps[100]; // Масив для зберігання номера кроку N
    int interval_count = 0;  // Лічильник знайдених інтервалів

    // 5. Основний цикл обчислення значень та побудови таблиці
    for (unsigned int i = 1; i <= N; i++) {
        double current_y = calculate_f(current_x);

        // Перевірка зміни знаку
        if (i > 1 && (prev_y * current_y < 0)) {
            intervals_a[interval_count] = prev_x;
            intervals_b[interval_count] = current_x;
            interval_steps[interval_count] = i; // Зберігаємо номер кроку N
            interval_count++;
        }

        // Друк одного рядка таблиці з форматуванням ширини стовпчиків
        printf("| %5u | %13.4lf | %13.4lf |\n", i, current_x, current_y);
        lines_on_page++;

        // Посторінкова зупинка виводу (пагінація кожні 10 рядків)
        if (lines_on_page == 10 && i < N) {
            printf("+-------+---------------+---------------+");
            printf("\nPress Any Key to Continue...");

            getch();

            printf("\r                                        \r");
            lines_on_page = 0;
        }

        // Зберігаємо поточні значення як "попередні" перед наступним кроком
        prev_x = current_x;
        prev_y = current_y;
        current_x += delta;
    }
    printf("+-------+---------------+---------------+\n");

    // 6. Виведення знайдених інтервалів зміни знаку
    printf("\n========================================\n");
    printf(" SIGN CHANGE INTERVALS (ISOLATION INTERVALS):\n");
    if (interval_count == 0) {
        printf(" Function does not change sign on the given interval.\n");
    } else {
        for (int k = 0; k < interval_count; k++) {
            printf(" Interval %d (at step N = %d): [%.4lf ; %.4lf]\n",
                   k + 1, interval_steps[k], intervals_a[k], intervals_b[k]);
        }
    }
    printf("========================================\n");

    return 0; // Успішне завершення програми
}