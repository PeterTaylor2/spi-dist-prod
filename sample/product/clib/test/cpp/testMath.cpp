/*
***************************************************************************
** Tests of all the functions and classes defined in math.cfg
***************************************************************************
*/

#include "clib_math.hpp"
#include "clib_clib_service.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>

#include <spi/TestRunner.hpp>
#include <spi/RuntimeError.hpp>

static void testAdd(void)
{
    double result = clib::Math::Add(1,2,3);
    SPI_POST_CONDITION(result == 6.0);
}

static void testBinaryOperator(void)
{
    using clib::Math::BinaryOp;

    SPI_POST_CONDITION(
        BinaryOp(2, clib::Math::BinaryOperator::Add, 2) == 4);
    SPI_POST_CONDITION(
        BinaryOp(4, clib::Math::BinaryOperator::Subtract, 1) == 3);
    SPI_POST_CONDITION(
        BinaryOp(2.5, clib::Math::BinaryOperator::Multiply, 4) == 10.0);
    SPI_POST_CONDITION(
        BinaryOp(10.0, clib::Math::BinaryOperator::Divide, 4.0) == 2.5);
    SPI_POST_CONDITION(
        BinaryOp(2.0, clib::Math::BinaryOperator::Power, 3.0) == 8.0);

    SPI_POST_CONDITION(
        BinaryOp(2, "+", 2) == 4);
    SPI_POST_CONDITION(
        BinaryOp(4, "-", 1) == 3);
    SPI_POST_CONDITION(
        BinaryOp(2.5, "*", 4) == 10.0);
    SPI_POST_CONDITION(
        BinaryOp(10.0, "/", 4.0) == 2.5);
    SPI_POST_CONDITION(
        BinaryOp(2.0, "**", 3.0) == 8.0);
}

static void testMatrix(void)
{
    spi::MatrixData<double> data(3,3);

    for (int i = 0; i < 3; ++i)
        data[i][i] = 1.0;

    clib::Math::MatrixConstSP matrix = clib::Math::Matrix::Make(data);

    // matrix->to_stream(std::cout);
    // std::cout << std::endl;

    SPI_POST_CONDITION(matrix->Cell(1,1) == 1.0);
    SPI_POST_CONDITION(matrix->Cell(1,2) == 0.0);
    SPI_POST_CONDITION(matrix->Rows() == 3);
    SPI_POST_CONDITION(matrix->Cols() == 3);

    spi::MatrixData<double> dataFunc = matrix->Data();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            SPI_POST_CONDITION(data[i][j] == dataFunc[i][j]);
        
}

int main(int argc, char* argv[])
{
    clib::clib_exported_service()->start_logging("math.log", "");
    return spi::TestRunner(argc, argv,
                           "Add", testAdd,
                           "BinaryOperator", testBinaryOperator,
                           "Matrix", testMatrix,
                           NULL);
}
