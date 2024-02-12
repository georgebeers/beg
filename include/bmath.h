#ifndef BEG_BMATH_H
#define BEG_BMATH_H

#include <array>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <iostream>
#include <tuple>

namespace BEG {

using Number = float;

const float Pi = M_PIf32;

Number toRadians(Number degrees);
Number toDegrees(Number radians);

Number clamp(Number value, Number min, Number max);

template <size_t N>
class Vector {
protected:
    std::array<Number, N> mValues{};
public:
    static Number dot(const Vector& a, const Vector& b) {
        Number sum{ 0 };

        for (size_t i{ 0 }; i < N; ++i) {
            sum += a.mValues[i] * b.mValues[i];
        }

        return sum;
    }

    static Vector cross(const Vector& a, const Vector& b) {
        static_assert(N == 3, "cross products between 2 vectors only exist in the 3rd dimension");
        return {
            (a.y() * b.z()) - (a.z() * b.y()),
            (a.z() * b.x()) - (a.x() * b.z()),
            (a.x() * b.y()) - (a.y() * b.x())
        };
    }

    Vector(Number value = 0.0f) : mValues{} {
        static_assert(N > 0, "cannot have zero length vector");
        mValues.fill(value);
    }

    template<typename... T>
    Vector(T... values) : mValues{ values... } {
        static_assert(N > 0, "cannot have zero length vector");
        static_assert(sizeof...(values) == N, "invalid number of parameters");
    }

    Vector(const Vector& other) : mValues{ other.mValues } {}
    Vector& operator=(const Vector& other) {
        std::copy(other.mValues.begin(), other.mValues.end(), mValues.begin());
        
        return *this;
    }

    /* shrinking conversion */
    template<size_t M>
    Vector(const Vector<M>& other) : mValues{} {
        static_assert(M >= N, "cannot perform a shrinking vector conversion on a smaller vector");
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] = other[i];
        }
    }

    /* growing conversion */
    template<typename... T, size_t M>
    Vector(const Vector<M>& other, T... values) : mValues{} {
        static_assert(M + sizeof...(values) == N, "invalid number of parameters");
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] = other[i];
        }

        std::array<Number, sizeof...(values)> arr{ values... };
        std::copy(arr.begin(), arr.end(), mValues.begin() + M);
    }

    size_t size() const {
        return N;
    }

    const std::array<Number, N>& data() const {
        return mValues;
    }

    std::array<Number, N>& data() {
        return mValues;
    }

    Number x() const { static_assert(N >= 1, "vector has no member x"); return mValues[0]; }
    void x(Number n) { static_assert(N >= 2, "vector has no member x"); mValues[0] = n; }  

    Number y() const { static_assert(N >= 2, "vector has no member y"); return mValues[1]; }
    void y(Number n) { static_assert(N >= 2, "vector has no member y"); mValues[1] = n; }

    Number z() const { static_assert(N >= 3, "vector has no member z"); return mValues[2]; }
    void z(Number n) { static_assert(N >= 3, "vector has no member z"); mValues[2] = n; }

    Number w() const { static_assert(N >= 4, "vector has no member z"); return mValues[3]; }
    void w(Number n) { static_assert(N >= 4, "vector has no member z"); mValues[3] = n; }

    Vector operator+(const Vector& other) const {
        Vector result{};

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = mValues[i] + other.mValues[i];
        }

        return result;
    }

    Vector operator-(const Vector& other) const {
        Vector result{};

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = mValues[i] - other.mValues[i];
        }

        return result;
    }

    Vector operator*(const Vector& other) const {
        Vector result{};

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = mValues[i] * other.mValues[i];
        }

        return result;
    }

    Vector operator*(Number scalar) const {
        Vector result{};

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = mValues[i] * scalar;
        }

        return result;
    }

    Vector operator/(const Vector& other) const {
        Vector result{};

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = mValues[i] / other.mValues[i];
        }

        return result;
    }

    Vector operator/(Number scalar) const {
        Vector result{};

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = mValues[i] / scalar;
        }

        return result;
    }

    Vector operator+() const {
        return (*this);
    }

    Vector operator-() const {
        return (*this) * -1;
    }

    Vector& operator+=(const Vector& other) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] += other.mValues[i];
        }

        return *this;
    }

    Vector& operator+=(Number scalar) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] += scalar;
        }

        return *this;
    }

    Vector& operator-=(const Vector& other) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] -= other.mValues[i];
        }

        return *this;
    }

    Vector& operator-=(Number scalar) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] -= scalar;
        }

        return *this;
    }

    Vector& operator*=(const Vector& other) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] *= other.mValues[i];
        }

        return *this;
    }

    Vector& operator*=(Number scalar) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] *= scalar;
        }

        return *this;
    }

    Vector& operator/=(const Vector& other) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] /= other.mValues[i];
        }
        
        return *this;
    }

    Vector& operator/=(Number scalar) {
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] /= scalar;
        }

        return *this;
    }

    bool operator==(const Vector& other) {
        for (size_t i{ 0 }; i < N; ++i) {
            if (mValues[i] != other.mValues[i])
                return false;
        }

        return true;
    }

    bool operator!=(const Vector& other) {
        for (size_t i{ 0 }; i < N; ++i) {
            if (mValues[i] != other.mValues[i])
                return true;
        }

        return false;
    }

    const Number& operator[](size_t i) const {
        return mValues[i];
    }
    
    Number& operator[](size_t i) {
        return mValues[i];
    }

    Vector abs() const {
        Vector result{};

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = abs(mValues[i]);
        }

        return result;
    }

    Number magnitude() const {
        Number sum{ 0 };

        for (Number value : mValues) {
            sum += powf(value, 2.0f);
        }

        return sqrtf(sum);
    }

    Vector normalized() const {
        Vector result{};
        Number m = magnitude();

        for (size_t i{ 0 }; i < N; ++i) {
            result.mValues[i] = mValues[i] / m;
        }

        return result;
    }

    void normalize() {
        Number m = magnitude();
        for (size_t i{ 0 }; i < N; ++i) {
            mValues[i] /= m;
        }
    }

    Number dot(const Vector& other) const {
        return Vector::dot(*this, other);
    }

    Vector cross(const Vector& other) const {
        return Vector::cross(*this, other);
    }

    Vector lerp(const Vector& other, Number t) const {
        if (t < 0.0f)
            t = 0.0f;
        else if (t > 1.0f)
            t = 1.0f;
        
        return *this + ((other - *this) * t);
    }
};

template<size_t M, size_t N = M>
class Matrix {
private:
    std::array<Number, M * N> mValues{};
public:
    static Matrix identity() {
        static_assert(M == N, "cannot create rectangular identity matrix");
        Matrix result{};

        for (size_t i{ 0 }; i < M; ++i) {
            result.mValues[i * (M + 1)] = 1.0f;
        }

        return result;
    }

    Matrix(Number value = 0.0f) : mValues {} {
        static_assert(M > 0 && N > 0, "cannot have zero width matrix");
        mValues.fill(value);
    }

    template<typename... T>
    Matrix(T... values) : mValues{ values... } {
        static_assert(M > 0 && N > 0, "cannot have zero width matrix");
        static_assert(sizeof...(values) == M * N, "invalid number of parameters");
    }

    size_t size() const { return M * N; }
    size_t rows() const { return M; }
    size_t columns() const { return N; }

    const std::array<Number, M * N>& data() const {
        return mValues;
    }

    std::array<Number, N * N>& data() {
        return mValues;
    }

    Matrix operator+(const Matrix& other) const {
        Matrix result{};
        for (size_t i{ 0 }; i < M * N; ++i) {
            result.mValues[i] = mValues[i] + other.mValues[i];
        }

        return result;
    }

    Matrix operator-(const Matrix& other) const {
        Matrix result{};
        for (size_t i{ 0 }; i < M * N; ++i) {
            result.mValues[i] = mValues[i] - other.mValues[i];
        }

        return result;
    }

    template <size_t N2>
    Matrix<M, N2> operator*(const Matrix<N, N2> other) const {

        Matrix<M, N2> result{};

        for (size_t i{ 0 }; i < M * N2; ++i) {
            size_t r{ i / N2 }, c{ i % N2 };

            Number sum{ 0 };
            for (size_t j{ 0 }; j < N; ++j) {
                sum += at(r, j) * other.at(j, c);
            }

            result[i] = sum;
        }

        return result;
    }

    Vector<M> operator*(const Vector<N> other) const {
        Vector<M> result{};

        for (size_t i{ 0 }; i < M; ++i) {
            Number sum{ 0 };
            for (size_t j{ 0 }; j < N; ++j) {
                sum += at(i, j) * other[j];
            }

            result[i] = sum;
        }

        return result; 
    }

    Matrix operator*(Number scalar) const {
        Matrix result{};
        for (size_t i{ 0 }; i < M * N; ++i) {
            result.mValues[i] = mValues[i] * scalar;
        }

        return result;   
    }

    Matrix operator/(Number scalar) const {
        Matrix result{};
        for (size_t i{ 0 }; i < M * N; ++i) {
            result.mValues[i] = mValues[i] / scalar;
        }

        return result;   
    }

    Matrix& operator+=(Number scalar) {
        for (size_t i{ 0 }; i < M * N; ++i) {
            mValues[i] += scalar;
        }

        return *this;
    }

    Matrix& operator-=(Number scalar) {
        for (size_t i{ 0 }; i < M * N; ++i) {
            mValues[i] -= scalar;
        }

        return *this;
    }

    Matrix& operator*=(const Matrix<M, M>& other) {
        Matrix result{};

        for (size_t i{ 0 }; i < M * N; ++i) {
            size_t r{ i / N }, c{ i % N };

            Number sum{ 0 };
            for (size_t j{ 0 }; j < M; ++j) {
                sum += other.at(r, j) * at(j, c);
            }

            result[i] = sum;
        }

        *this = result;
        return *this;
    }

    Matrix& operator*=(Number scalar) {
        for (size_t i{ 0 }; i < M * N; ++i) {
            mValues[i] *= scalar;
        }

        return *this;
    }

    Matrix& operator/=(Number scalar) {
        for (size_t i{ 0 }; i < M * N; ++i) {
            mValues[i] /= scalar;
        }

        return *this;
    }

    Number& operator[](size_t i) {
        return mValues[i];
    }

    const Number& operator[](size_t i) const {
        return mValues[i];
    }

    Number at(size_t r, size_t c) const {
        return mValues[c + (r * N)];
    }

    Number& at(size_t r, size_t c) {
        return mValues[c + (r * N)];
    }

    Matrix<M-1, N-1> minor(size_t r, size_t c) const {
        static_assert(M > 1 && N > 1, "cannot take minor of single width matrix");

        Matrix<M-1, N-1> result{};

        for (size_t i{ 0 }; i < M * N; ++i) {
            size_t x{ i / N }, y { i % N };
            
            if (x == r || y == c)
                continue;

            if (x > r)
                --x;
            if (y > c)
                --y;

            result.at(x, y) = mValues[i];
        }

        return result;
    }
    
    Number determinant() const {
        static_assert(M == N, "cannot take determinant of rectangular matrix");

        if constexpr (M == 1) {
            return mValues[0];
        } else if constexpr (M == 2) {
            return (mValues[0] * mValues[3]) - (mValues[1] * mValues[2]);
        } else {
            Number sum{ 0 };

            for (size_t i{ 0 }; i < M; ++i) {
                sum += (minor(0, i).determinant() * (i % 2 == 0 ? 1.0f : -1.0f)) * mValues[i];
            }

            return sum;
        }
    }

    Matrix<N, M> transpose() const {
        Matrix<N, M> result{};

        for (size_t i{ 0 }; i < M * N; ++i) {
            result[i] = mValues[(N * (i % M)) + (i / M)];
        }

        return result;
    }

    Matrix<N, M> adjoint() const {
        Matrix result{};

        for (size_t i{ 0 }; i < N * M; ++i) {
            result.mValues[i] = minor(i / N, i % N).determinant() * ((i % 2 == 0 ? 1.0f : -1.0f) * ((i / N) % 2 == 0 ? 1.0f : -1.0f));
        }

        return result.transpose();
    }

    Matrix<N, M> inverse() const {
        return adjoint() / determinant();
    }
};

class Quaternion {
private:
    Number mW{}, mX{}, mY{}, mZ{};
public:
    Quaternion() : mW{ 1.0f }, mX{ 0.0f }, mY{ 0.0f }, mZ{ 0.0f } {}
    Quaternion(Number w, Number x, Number y, Number z) : mW{ w }, mX{ x }, mY{ y }, mZ{ z } {}

    Quaternion(Number x, Number y, Number z)
        : mW{ (cosf(x * 0.5f) * cosf(y * 0.5f) * cosf(z * 0.5f)) + (sinf(x * 0.5f) * sinf(y * 0.5f) * sinf(z * 0.5f)) },
          mX{ (sinf(x * 0.5f) * cosf(y * 0.5f) * cosf(z * 0.5f)) - (cosf(x * 0.5f) * sinf(y * 0.5f) * sinf(z * 0.5f)) },
          mY{ (cosf(x * 0.5f) * sinf(y * 0.5f) * cosf(z * 0.5f)) + (sinf(x * 0.5f) * cosf(y * 0.5f) * sinf(z * 0.5f)) },
          mZ{ (cosf(x * 0.5f) * cosf(y * 0.5f) * sinf(z * 0.5f)) - (sinf(x * 0.5f) * sinf(y * 0.5f) * cosf(z * 0.5f)) } {}
    Quaternion(Vector<3> euler) : Quaternion(euler.x(), euler.y(), euler.z()) {}

    Number w() const { return mW; }
    void w(Number value) { mW = value; }
    Number x() const { return mX; }
    void x(Number value) { mX = value; }
    Number y() const { return mY; }
    void y(Number value) { mY = value; }
    Number z() const { return mZ; }
    void z(Number value) { mZ = value; }

    Quaternion operator+(const Quaternion& other) const {
        return { mW + other.mW, mX + other.mX, mY + other.mY, mZ + other.mZ };
    }

    Quaternion operator+(Number scalar) const {
        return { mW + scalar, mX, mY, mZ };
    }

    Quaternion operator-(const Quaternion& other) const {
        return { mW - other.mW, mX - other.mX, mY - other.mY, mZ - other.mZ };
    }

    Quaternion operator-(Number scalar) const {
        return { mW - scalar, mX, mY, mZ };
    }

    Quaternion operator*(const Quaternion& other) const {
        return {
            (mW * other.mW) - (mX * other.mX) - (mY * other.mY) - (mZ * other.mZ),
            (mX * other.mW) + (mW * other.mX) + (mY * other.mZ) - (mZ * other.mY),
            (mW * other.mY) - (mX * other.mZ) + (mY * other.mW) + (mZ * other.mX),
            (mW * other.mZ) + (mX * other.mY) - (mY * other.mX) + (mZ * other.mW)
        };
    }

    Quaternion operator*(Number scalar) const {
        return {
            mW * scalar,
            mX * scalar,
            mY * scalar,
            mZ * scalar
        };
    }

    Quaternion operator/(const Quaternion& other) const {
        return *this * other.inverse();
    }

    Quaternion operator/(Number scalar) const {
        return {
            mW / scalar,
            mX / scalar,
            mY / scalar,
            mZ / scalar
        };
    }

    Quaternion& operator+=(const Quaternion& other) {
        mW += other.mW;
        mX += other.mX;
        mY += other.mY;
        mZ += other.mZ;

        return *this; 
    }

    Quaternion& operator+=(Number scalar) {
        mW += scalar;

        return *this;
    }
    
    Quaternion& operator-=(const Quaternion& other) {
        mW -= other.mW;
        mX -= other.mX;
        mY -= other.mY;
        mZ -= other.mZ;

        return *this; 
    }
    
    Quaternion& operator-=(Number scalar) {
        mW -= scalar;
        
        return *this;
    }
    

    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }

    Quaternion& operator*=(Number scalar) {
        mW *= scalar;
        mX *= scalar;
        mY *= scalar;
        mZ *= scalar;

        return *this;
    }
    
    Quaternion& operator/=(const Quaternion& other) {
        *this = *this / other;
        return *this;
    }

    Quaternion& operator/=(Number scalar) {
        mW /= scalar;
        mX /= scalar;
        mY /= scalar;
        mZ /= scalar;
        
        return *this;
    }

    Vector<3> toEuler() const {
        Number t0{ 2.0f * ((mW * mX) + (mY * mZ)) };
        Number t1{ 1.0f - (2.0f * ((mX * mX) + (mY * mY))) };

        Number x{ atan2f32(t0, t1) };

        Number t2{ 2.0f * ((mW * mY) - (mZ * mX)) };
        if (t2 > 1.0f)
            t2 = 1.0f;
        else if (t2 < -1.0f)
            t2 = -1.0f;
        
        Number y{ asinf32(t2) };

        Number t3{ 2.0f * ((mW * mZ) + (mX * mY)) };
        Number t4{ 1.0f - (2.0f * ((mY * mY) + (mZ * mZ))) };
        
        Number z{ atan2f32(t3, t4) };

        return { x, y, z };
    }

    Matrix<4> toMatrix() const {
        Number x2{ mX * mX }, y2{ mY * mY }, z2{ mZ * mZ };
        return {
            1.0f - (2.0f * y2) - (2.0f * z2), (2.0f * mX * mY) - (2.0f * mW * mZ), (2.0f * mX * mZ) + (2.0f * mW * mY), 0.0f,
            (2.0f * mX * mY) + (2.0f * mW * mZ), 1.0f - (2.0f * x2) - (2.0f * z2), (2.0f * mY * mZ) - (2.0f * mW * mX), 0.0f,
            (2.0f * mX * mZ) - (2.0f * mW * mY), (2.0f * mY * mZ) + (2.0f * mW * mX), 1.0f - (2.0f * x2) - (2.0f * y2), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    Number magnitude() const {
        return sqrtf((mW * mW) + (mX * mX) + (mY * mY) + (mZ * mZ));
    }

    Quaternion conjugate() const {
        return {
            mW,
            -mX,
            -mY,
            -mZ
        };
    }

    Quaternion normalized() const {
        return *this / magnitude();
    }

    void normalize() {
        Number m{ magnitude() };

        mW /= m;
        mX /= m;
        mY /= m;
        mZ /= m;
    }

    Quaternion inverse() const {
        return conjugate() / powf(magnitude(), 2.0f);
    }

    Number dot(const Quaternion& other) const {
        return (mW * other.mW) + (mX * other.mX) + (mY * other.mY) + (mZ * other.mZ);
    }
    
    Quaternion cross(const Quaternion& other) const {
        return (*this * other) + dot(other);
    }

    Quaternion lerp(const Quaternion& other, Number t) const {
        if (t < 0.0f)
            t = 0.0f;
        else if (t > 1.0f)
            t = 1.0f;

        Quaternion raw{ (*this * (1.0f - t)) + (other * t) };
        return raw / raw.magnitude();
    }
};

}

#endif