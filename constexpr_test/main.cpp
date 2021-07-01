// main.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <array>


//******************************************************************
// constexpr 関数
// *****************************************************************
// constexpr 関数は
// - コンパイル時定数を与えられ、呼び出された時にコンパイル時定数を返す。
// - コンパイル時点では値が決定されておらず、実行時までに未知な値が与えられた場合は、
//   実行時にのみ使用可能な値を返す。
// - 関数の１つ以上の引数がコンパイル時に未知な値を含んでいた場合、通常の関数として
//   ふるまい、結果は実行時に算出される。
//******************************************************************
// std::pow はconstexpr 関数ではないため、std::pow の戻り値はコンパイル時定数として利用できない。
// 以下の自作した pow 関数なら、それが可能であり、戻り値は汎整数型である。（std::arrayの要素数指定にも使用可能）
constexpr int pow(int base, int exp) noexcept
{
    // C++14 から constexpr 関数にif-else,for, 変数宣言などが可能に
    auto result = 1;
    for (int i = 0; i < exp; ++i) result *= base;

    return result;
}

class Point
{
public:
    // 実引数がコンパイル時に既知であれば、コンストラクタでも constexpr 使用可能
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
    : x(xVal)
    , y(yVal)
    { }

    constexpr double xValue() const noexcept { return x;}
    constexpr double yValue() const noexcept { return y;}

    constexpr void setX(double newX) noexcept { x = newX; } // C++14 から void がリテラル型に対応
    constexpr void setY(double newY) noexcept { y = newY; } // C++11 までは constexpr メンバ関数は暗黙的に const になっていた。


private:
    double x, y;
    
};

constexpr Point midPoint(const Point& p1, const Point& p2) noexcept
{
    return {(p1.xValue() + p2.xValue()) / 2, (p1.yValue() + p2.yValue()) / 2};
}

constexpr Point reflection(const Point& p) noexcept
{
    Point result;

    result.setX(-p.xValue());
    result.setY(-p.yValue());

    return result;
}


int main()
{
    int sz = 1;


    //******************************************************************
    // constexpr 定数
    // *****************************************************************
    
    //constexpr auto arraySize1 = sz;   // Error
    //std::array<int, sz> data1;        // Error
    
    constexpr auto arraySize2 = 10;     // Fine
    std::array<int, arraySize2> data2;  // Fine

    // 配列要素数、汎整数のテンプレート実引数、列挙子の値、アラインメント指定子などの
    // 汎整数定数式を必要とする場面で使用できる。

    // コンパイラがコンパイル時に使用可能な値であると保証できる。


    //******************************************************************
    // const 定数
    //******************************************************************

    const auto arraySize = sz;          // Fine
    //std::array<int, arraySize> data;    // Error

    const auto arraySize3 = 10;         // Fine
    std::array<int, arraySize3> data3;  // Fine

    // constexpr -> const になりうるが、
    // const     -> constexpr になるとは限らない。
    // const は必ずしもコンパイル時定数であると保証することはできない。
    // 汎整数型を求められている場合は constexpr 定数を用いるほうが無難である

    constexpr Point p1(9.4, 27.7);
    constexpr Point p2(28.8, 5.3);


    constexpr auto mid = midPoint(p1, p2);
    constexpr auto reflectedMid = reflection(mid);



    return 1;
}
