
#include <math.h>
#include <stdio.h>

int main() 
{
    // Структура пикселя в формате BGR (Синий, Зелёный, Красный)
typedef struct
{
    unsigned char rgbtBlue;
    unsigned char rgbtGreen;
    unsigned char rgbtRed;
} RGBTRIPLE;

// 1. Фильтр Grayscale (Оттенки серого с учётом восприятия глаза)
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Считаем среднее округлённое значение
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            
            // Применяем ко всем трём каналам
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// 2. Фильтр Sepia (Старинное фото с лимитом в 255 байт)
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            // Формулы лектора
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            // Жёстко обрезаем всё, что вылетело за предел одного байта (255)
            image[i][j].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
            image[i][j].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
        }
    }
}

// 3. Фильтр Reflect (Зеркальное отражение строки до середины width / 2)
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Меняем пиксели местами слева направо через swap
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// 4. Фильтр Blur (Размытие матрицы 3х3 через обязательную временную копию)
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Создаём временный массив, чтобы не портить исходные пиксели при расчётах
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumRed = 0, sumGreen = 0, sumBlue = 0;
            float counter = 0.00;

            // Обходим окрестность пикселя 3х3
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    int current_i = i + k;
                    int current_j = j + l;

                    // Проверяем, не вылетели ли мы за границы картинки (края и углы)
                    if (current_i >= 0 && current_i < height && current_j >= 0 && current_j < width)
                    {
                        sumRed += image[current_i][current_j].rgbtRed;
                        sumGreen += image[current_i][current_j].rgbtGreen;
                        sumBlue += image[current_i][current_j].rgbtBlue;
                        counter++;
                    }
                }
            }

            // Записываем усреднённое значение во временную копию
            temp[i][j].rgbtRed = round(sumRed / counter);
            temp[i][j].rgbtGreen = round(sumGreen / counter);
            temp[i][j].rgbtBlue = round(sumBlue / counter);
        }
    }

    // Копируем готовый результат обратно в исходный массив изображения
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}
}
