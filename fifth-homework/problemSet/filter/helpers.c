#include "helpers.h"
#include<math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    uint8_t grayImg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grayImg = ((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0) + 0.5;

            image[i][j].rgbtRed = grayImg;
            image[i][j].rgbtGreen = grayImg;
            image[i][j].rgbtBlue = grayImg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    uint8_t tmp[3];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            tmp[0] = image[i][j].rgbtRed;
            tmp[1] = image[i][j].rgbtGreen;
            tmp[2] = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            image[i][width - j - 1].rgbtRed = tmp[0];
            image[i][width - j - 1].rgbtGreen = tmp[1];
            image[i][width - j - 1].rgbtBlue = tmp[2];
        }
    }

    return;
}

int getBlur(int indexI, int indexJ, int height, int width, RGBTRIPLE image[height][width], int indexColor)
{
    int counter = 0;
    float sum = 0;
    for (int i = indexI - 1; i < indexI + 2; i++)
    {
        for (int j = indexJ - 1; j < indexJ + 2; j++)
        {
            if (i < 0 || j < 0 || i >= height || j >= width)
            {
                continue;
            }
            if (indexColor == 0)
            {
                sum += image[i][j].rgbtRed;
                counter++;
            }
            else if (indexColor == 1)
            {
                sum += image[i][j].rgbtGreen;
                counter++;
            }
            else if (indexColor == 2)
            {
                sum += image[i][j].rgbtBlue;
                counter++;
            }
        }
    }
    // sum = round(sum / counter);
    return round(sum / counter);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copyImage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copyImage[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = getBlur(i, j, height, width, copyImage, 0);
            image[i][j].rgbtGreen = getBlur(i, j, height, width, copyImage, 1);
            image[i][j].rgbtBlue = getBlur(i, j, height, width, copyImage, 2);
        }
    }

    return;
}

int sobelFilter(int indexI, int indexJ, int height, int width, RGBTRIPLE image[height][width], int indexColor)
{
    float sumX = 0;
    float sumY = 0;

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1,-2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = indexI - 1, x = 0; i < indexI + 2; i++, x++)
    {
        for (int j = indexJ - 1, y = 0; j < indexJ + 2; j++, y++)
        {
            if ( i < 0 || j < 0 || i >= height || j >= width)
            {
                continue;
            }
            if (indexColor == 0)
            {
                sumX += image[i][j].rgbtRed * gx[x][y];
                sumY += image[i][j].rgbtRed * gy[x][y];
            }
            else if (indexColor == 1)
            {
                sumX += image[i][j].rgbtGreen * gx[x][y];
                sumY += image[i][j].rgbtGreen * gy[x][y];
            }
            else if (indexColor == 2)
            {
                sumX += image[i][j].rgbtBlue * gx[x][y];
                sumY += image[i][j].rgbtBlue * gy[x][y];
            }
        }
    }

    int result = round(sqrt(pow(sumX, 2) + pow(sumY, 2)));

    return result < 255 ? result : 255;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copyImage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copyImage[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = sobelFilter(i, j, height, width, copyImage, 0);
            image[i][j].rgbtGreen = sobelFilter(i, j, height, width, copyImage, 1);
            image[i][j].rgbtBlue = sobelFilter(i, j, height, width, copyImage, 2);
        }
    }

    return;
}
