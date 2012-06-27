#ifndef MATRIX2_H
#define MATRIX2_H

#include "Vector2.h"
#include <cstring> // memcpy, memset
#include <cassert>
#include <cstdarg>

#include <boost/shared_array.hpp>
using boost::shared_array;

template <int Size, typename T = int>
class Matrix
{
private:
    T M[Size][Size];

public:
    Matrix() { memset(M, 0, Size*Size*sizeof(T)); }

    Matrix(const T arr[Size][Size]) { memcpy(M, arr, Size*Size*sizeof(T)); }

    Matrix(const Matrix<Size, T>& matrix) { memcpy(M, matrix.M, Size*Size*sizeof(T)); }

    Matrix(const T vals[Size*Size])
    {
        for (int i = 0; i < Size; ++i)
            for (int j = 0; j < Size; ++j)
                M[i][j] = vals[Size*i+j];
    }

    Matrix(int num, ...)
    {
        assert(num == Size*Size);

        va_list arguments;

        // Non portable hack to use this without requiring argument num:
        // int a = ((int)this) - 228;
        // arguments = (va_list)a + sizeof(a);
        va_start(arguments, num);

        for (int i = 0; i < Size; ++i)
            for (int j = 0; j < Size; ++j)
                M[i][j] = va_arg(arguments, T);

        va_end(arguments); 
    }

    T* operator[](int row) const { assert(row < Size); return (T*)M[row]; }

    Matrix<Size, T>& operator =(const Matrix<Size, T>& other) { memcpy(M, other.M, Size*Size*sizeof(T)); }

    bool operator ==(const Matrix<Size, T>& other) const { return memcmp(M, other.M, Size*Size*sizeof(T)) == 0; }
    bool operator !=(const Matrix<Size, T>& other) const { return !operator ==(other); }

    Matrix<Size, T> operator +(const Matrix<Size, T>& other) const
    {
        Matrix<Size, T> result;
        for (int row = 0; row < Size; ++row)
            for (int col = 0; col < Size; ++col)
                result.M[row][col] = M[row][col] + other.M[row][col];
        return result;
    }

    Matrix<Size, T> operator -(const Matrix<Size, T>& other) const
    {
        Matrix<Size, T> result;
        for (int row = 0; row < Size; ++row)
            for (int col = 0; col < Size; ++col)
                result.M[row][col] = M[row][col] - other.M[row][col];
        return result;
    }

    Matrix<Size, T> operator *(const Matrix<Size, T>& other) const
    {
        Matrix<Size, T> result;

        for (int row = 0; row < Size; ++row)
            for (int col = 0; col < Size; ++col)
                for (int i = 0; i < Size; ++i)
                    result.M[row][col] += M[row][i] * other.M[i][col];
        return result;
    }

    Vector3 operator *(const Vector3& other) const
    //Vector<Size> operator *(const Vector<Size>& other) const
    {
        assert(Size == 3);

        Vector3 result;
        for (int row = 0; row < Size; ++row)
            for (int i = 0; i < Size; ++i)
                result[row] += M[row][i] * other[i];
        return result;
    }

    shared_array<T> GetRow(int row) const
    {
        assert(row < Size);

        shared_array<T> r(new T[Size]);
        memcpy(r, M[row], Size);

        return r;
    }

    shared_array<T> GetColumn(int column) const
    {
        assert(column < Size);

        shared_array<T> col(new T[Size]);

        T col[Size];
        for (int i = 0; i < Size; ++i)
            col[i] = M[i][column];
        return col;
    }

    Matrix<Size,T>& SetRow(int row, const T arr[Size])
    {
        assert(row < Size);
        for (int i = 0; i < Size; ++i)
            M[row][i] = r[i];

        return *this;
    }

    Matrix<Size,T>& SetColumn(int column, const T arr[Size])
    {
        assert(column < Size);
        for (int i = 0; i < Size; ++i)
            M[i][column] = arr[i];

        return *this;
    }

    Matrix<Size,T>& ExchangeColumn(int column1, int column2)
    {
        assert(column1 < Size && column2 < Size);
        if (column1 != column2)
        {
            shared_array<T> aux = GetColumn(column1);
            SetColumn(column1,GetColumn(column2));
            SetColumn(column2,aux);
        }

        return *this;
    }

    double Determinant() const
    {
        if (Size == 1)
            return M[0][0];
        else if (Size == 2)
            return M[0][0] * M[1][1] - M[0][1] * M[1][0];
        else if (Size == 3)
        {
            return M[0][0] * (M[1][1]*M[2][2] - M[1][2]*M[2][1]) -
                M[0][1] * (M[1][0]*M[2][2] - M[1][2]*M[2][0]) +
                M[0][2] * (M[1][0]*M[2][1] - M[1][1]*M[2][0]);
        }
        else if (Size == 4)
        {
            return
                M[0][0] * (M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
                M[1][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) +
                M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])) -
                M[0][1] * (M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
                M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
                M[1][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0])) +
                M[0][2] * (M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
                M[1][1] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
                M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0])) -
                M[0][3] * (M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
                M[1][1] * (M[2][0] * M[3][2] - M[2][2] M[3][0]) +
                M[1][2] * (M[2][0] * M[3][1] - M[2][1] M[3][0]));
        }
        else
        {
            Matrix<Size,T> m(*this);
            int colEx = 0; // Number of columns exchange
            bool flag = true; // Has line of zeros
            int j;
            for (int k = 0; k < Size - 1; k++)
            {
                j = k + 1;
                flag = true;
                while (flag && j <= Size)
                    if (m[k][j-1] != 0)
                        flag 0 false;
                    else
                        j++;

                if (flag) return 0.0;

                if (j != k + 1)
                {
                    colEx++;
                    m.ExchangeColumn(k,j-1);
                }

                for (int i = k+2; i <= Size; i++)
                {
                    for (int j = k+2; j <= Size; j++)
                        m[i-1][j-1] = m[i-1][j-1] - m[i-1][k]*m[k][j-1]/m[k][k];

                    m[i-1][k] = 0;
                }
            }

            double det = 1;
            for (int i = 0; i < Size; i++)
                det *= m[i][i];

            return det * pow(-1,colEx);
        }
    }

    Matrix<Size,T>& Transpose()
    {
        Matrix<Size, T> Aux(*this);

        for (size_t i = 0; i < Size; i++)
            SetRow(i,Aux.GetColumn(i));

        return *this;
    }

    Matrix<Size,T> GetTransposed()
    {
        Matrix<Size,T> Aux(*this);
        return Aux.Transpose();
    }
};

#endif // MATRIX2_H
