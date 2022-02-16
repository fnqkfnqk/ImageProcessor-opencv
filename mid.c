
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef unsigned char uchar;

// �̹��� ũ�⸸ŭ �޸� ���� �Ҵ��ϴ� �Լ�
uchar** uc_alloc(int size_x, int size_y) {
    uchar** m;
    int i;

    // y���� ���� ����
    if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL) {
        printf("uc_alloc error 1\7\n");
        exit(0);
    }

    // y�࿡ x�� ���� ����
    for (i = 0; i < size_y; i++) {
        if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) {
            printf("uc_alloc error 2\7\n");
            exit(0);
        }
    }
    return m;
}

// �޸� �Ҵ� �����ϴ� �Լ�
void uc_free(int size_x, int size_y, uchar** ucmatrix) {
    int i;

    for (i = 0; i < size_y; i++) {
        free(ucmatrix[i]);
    }
    free(ucmatrix);

}

// �̹��� ���� �д� �Լ�
void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename) {
    int i;
    FILE* f;

    // ���̳ʸ� ���� �о����
    if (f = fopen(filename, "wb") != NULL) {
        printf("%s File open Error!\n", filename);
        exit(0);
    }

    // ������ ũ�� �Ҵ��� �Ϳ� �̹��� �ֱ�
    for (i = 0; i < size_y; i++) {
        if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) {
            printf("Data Read Error!\n");
            exit(0);
        }
    }
    fclose(f);
}

// �̹��� ���� ���� �Լ�
void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename) {
    int i;
    FILE* f;

    if (f = fopen(filename, "wb")  != NULL) {
        printf("%s File open Error!\n", filename);
        exit(0);
    }

    for (i = 0; i < size_y; i++) {
        if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x) {
            printf("Data Write Error!\n");
            exit(0);
        }
    }
    fclose(f);
}

// �̹��� ������ũ�ϴ� �Լ�
void mosaic(uchar** img, uchar** Result, int Row, int Col, int Block, int row_pos, int col_pos, int diameter) {

    int i, j, x, y, tmp, count;
    double d, xSquare, ySquare;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            tmp = 0;
            count = 0;

            // (col_pos,row_pos)�� �߽���ǥ �Ͽ� (x,y) ��ǥ�� ���Ѵ�.
            ySquare = (abs(row_pos - i)) * (abs(row_pos - i));
            xSquare = (abs(col_pos - j)) * (abs(col_pos - j));

            // (col_pos,row_pos)�� �߽��̰� �� ���� (x,y)�� ���� �������� ���Ѵ�.
            d = sqrt(ySquare + xSquare);

            // ������ ���� �������� �Է¹��� ���������� ���� ���
            if (d < diameter)
            {
                // �Է¹��� ������ ũ�⸸ŭ ������ �̹��� ���� ���� ���Ѵ�.
                for (y = 0; y < Block; y++) {
                    for (x = 0; x < Block; x++) {
                        tmp += img[i + y][j + x];
                        count++;
                    }
                }

                // �Է¹��� ������ ũ�⸸ŭ ������ �̹��� ���� ����� ���Ѵ�.
                tmp /= count;

                // �Է¹��� ������ ũ�⸸ŭ ������ �̹��� ���� ��հ����� �ٲ۴�.
                for (y = 0; y < Block; y++) {
                    for (x = 0; x < Block; x++) {
                        Result[i + y][j + x] = tmp;
                    }
                }
            }
            // ���� ���̰� �Է¹��� ���������� ū ���
            else
            {
                Result[i][j] = img[i][j];
            }
        }
    }
}

// �̹��� �� negative�� ��ȯ�ϴ� �Լ�
void Negative(uchar** img, uchar** Result, int Row, int Col, int row_pos, int col_pos, int diameter) {
    int i, j;
    double d, xSquare, ySquare;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            // (col_pos,row_pos)�� �߽���ǥ �Ͽ� (x,y) ��ǥ�� ���Ѵ�.
            ySquare = (abs(row_pos - i)) * (abs(row_pos - i));
            xSquare = (abs(col_pos - j)) * (abs(col_pos - j));

            // (col_pos,row_pos)�� �߽��̰� �� ���� (x,y)�� ���� �������� ���Ѵ�.
            d = sqrt(ySquare + xSquare);

            // ������ ���� �������� �Է¹��� ���������� ���� ���
            if (d < diameter)
            {
                Result[i][j] = 255 - img[i][j];
            }
            else
            {
                Result[i][j] = img[i][j];
            }
        }
    }
}

// �̹��� ������ ��� ���� ����� ����Ͽ� ��ȯ�ϴ� �Լ�
double average(uchar** img, int Row, int Col)
{
    double sum = 0, avg;
    int i, j;

    // ��� �̹��� ���� ���Ѵ�.
    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            sum += img[i][j];
        }
    }
    // ����� ���Ѵ�.
    avg = sum / ((double)Row * Col);
    // �̹��� ���� ����� �͹̳ο� ����Ѵ�.
    printf("Average of Image %lf \n", avg);

    return avg;
}

void Binary(uchar** img, uchar** Result, int Row, int Col, int row_pos, int col_pos, int diameter, double avg)
{
    int i, j;
    double d, xSquare, ySquare;

    for (i = 0; i < Row; i++)
    {
        for (j = 0; j < Col; j++)
        {
            // (col_pos,row_pos)�� �߽���ǥ �Ͽ� (x,y) ��ǥ�� ���Ѵ�.
            ySquare = (abs(row_pos - i)) * (abs(row_pos - i));
            xSquare = (abs(col_pos - j)) * (abs(col_pos - j));

            // (col_pos,row_pos)�� �߽��̰� �� ���� (x,y)�� ���� �������� ���Ѵ�.
            d = sqrt(ySquare + xSquare);

            // ������ ���� �������� �Է¹��� ���������� ���� ���
            if (d > diameter)
            {
                if (img[i][j] > avg) Result[i][j] = 255;
                else Result[i][j] = 0;
            }
            else
            {
                Result[i][j] = img[i][j];
            }
        }
    }
}

// mosaic.c �����Լ�
int main(int argc, char* argv[])
{
    int Col, Row, BlockSize, row_pos, col_pos, diameter;
    double avg;
    uchar** img, ** Result;

    if (argc != 9)
    {
        fprintf(stderr, "\n %s inImage outImage COL ROW Block_Size row_pos col_pos diameter\n", argv[0]);
        exit(-1);
    }

    // argument���� ���������� ��ȯ�Ͽ� ������ �����Ѵ�.
    Col = atoi(argv[3]);
    Row = atoi(argv[4]);
    BlockSize = atoi(argv[5]);
    col_pos = atoi(argv[7]);
    row_pos = atoi(argv[6]);
    diameter = atoi(argv[8]);

    // img, Result ���� �޸� ���� �Ҵ��Ѵ�.
    img = uc_alloc(Col, Row);
    Result = uc_alloc(Col, Row);

    // img �̹��� ���� �д´�.
    read_ucmatrix(Col, Row, img, argv[1]);

    // �̹��� ���� ������ũ�Ѵ�.
    mosaic(img, Result, Row, Col, BlockSize, row_pos, col_pos, diameter); 

    // �̹��� ���� ���� �����Ѵ�.
    //Negative(img, Result, Row, Col, row_pos, col_pos, diameter);

    // �̹��� ���� ���̳ʸ��Ѵ�.
    //avg = average(img, Row, Col);
    //Binary(img, Result, Row, Col, row_pos, col_pos, diameter, avg);
    write_ucmatrix(Col, Row, Result, argv[2]);

    // �޸� ���� ����
    uc_free(Col, Row, img);
    uc_free(Col, Row, Result);

    return 0;
}
