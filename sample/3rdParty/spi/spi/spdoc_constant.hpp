#ifndef _SPDOC_CONSTANT_HPP_
#define _SPDOC_CONSTANT_HPP_

/**
****************************************************************************
* Header file: spdoc_constant.hpp
****************************************************************************
*/

#include <spi/spi.hpp>
#include "spdoc_publicType.hpp"

SPDOC_BEGIN_NAMESPACE

SPI_DECLARE_OBJECT_CLASS(Constant);
SPI_DECLARE_OBJECT_CLASS(IntConstant);
SPI_DECLARE_OBJECT_CLASS(DateConstant);
SPI_DECLARE_OBJECT_CLASS(DateTimeConstant);
SPI_DECLARE_OBJECT_CLASS(DoubleConstant);
SPI_DECLARE_OBJECT_CLASS(CharConstant);
SPI_DECLARE_OBJECT_CLASS(StringConstant);
SPI_DECLARE_OBJECT_CLASS(BoolConstant);
SPI_DECLARE_OBJECT_CLASS(UndefinedConstant);

/**
****************************************************************************
* Interface class defining a constant scalar value.
****************************************************************************
*/
class SPI_IMPORT Constant : public spi::Object
{
public:

    /**
    ************************************************************************
    * returns the data type name for the scalar
    ************************************************************************
    */
    virtual std::string typeName() const = 0;

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    virtual std::string docString() const = 0;

    /**
    ************************************************************************
    * returns the integer value (where applicable) for the scalar
    ************************************************************************
    */
    virtual int getInt() const;

    /**
    ************************************************************************
    * returns the date value (where applicable) for the scalar
    ************************************************************************
    */
    virtual spi::Date getDate() const;

    /**
    ************************************************************************
    * returns the date time value (where applicable) for the scalar
    ************************************************************************
    */
    virtual spi::DateTime getDateTime() const;

    /**
    ************************************************************************
    * returns the double value (where applicable) for the scalar
    ************************************************************************
    */
    virtual double getDouble() const;

    /**
    ************************************************************************
    * returns the char value (where applicable) for the scalar
    ************************************************************************
    */
    virtual char getChar() const;

    /**
    ************************************************************************
    * returns the string value (where applicable) for the scalar
    ************************************************************************
    */
    virtual std::string getString() const;

    /**
    ************************************************************************
    * returns the bool value (where applicable) for the scalar
    ************************************************************************
    */
    virtual bool getBool() const;

    SPI_DECLARE_OBJECT_TYPE(Constant);

    virtual spi_boost::shared_ptr<Constant_Helper> make_base_helper() const = 0;

protected:

    Constant();
};

/**
****************************************************************************
* Integer constant defined in the configuration file.
****************************************************************************
*/
class SPI_IMPORT IntConstant : public Constant
{
public:
    static IntConstantConstSP Make(
        int value);

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns "int"
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the integer value for the scalar
    ************************************************************************
    */
    int getInt() const;

    /**
    ************************************************************************
    * returns the double value for the scalar
    ************************************************************************
    */
    double getDouble() const;

    SPI_DECLARE_OBJECT_TYPE(IntConstant);

    friend class IntConstant_Helper;
    spi_boost::shared_ptr<IntConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;

protected:

    IntConstant(
        int value);

public:

    const int value;
};

/**
****************************************************************************
* Date constant defined in the configuration file.
****************************************************************************
*/
class SPI_IMPORT DateConstant : public Constant
{
public:
    static DateConstantConstSP Make(
        spi::Date value);

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns "date"
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the date value for the scalar
    ************************************************************************
    */
    spi::Date getDate() const;

    SPI_DECLARE_OBJECT_TYPE(DateConstant);

    friend class DateConstant_Helper;
    spi_boost::shared_ptr<DateConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;

protected:

    DateConstant(
        spi::Date value);

public:

    const spi::Date value;
};

/**
****************************************************************************
* DateTime constant defined in the configuration file.
****************************************************************************
*/
class SPI_IMPORT DateTimeConstant : public Constant
{
public:
    static DateTimeConstantConstSP Make(
        spi::DateTime value);

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns "datetime"
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the date value for the scalar
    ************************************************************************
    */
    spi::DateTime getDateTime() const;

    SPI_DECLARE_OBJECT_TYPE(DateTimeConstant);

    friend class DateTimeConstant_Helper;
    spi_boost::shared_ptr<DateTimeConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;

protected:

    DateTimeConstant(
        spi::DateTime value);

public:

    const spi::DateTime value;
};

/**
****************************************************************************
* Double constant defined in the configuration file.
****************************************************************************
*/
class SPI_IMPORT DoubleConstant : public Constant
{
public:
    static DoubleConstantConstSP Make(
        double value);

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns "double"
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the double value for the scalar
    ************************************************************************
    */
    double getDouble() const;

    SPI_DECLARE_OBJECT_TYPE(DoubleConstant);

    friend class DoubleConstant_Helper;
    spi_boost::shared_ptr<DoubleConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;

protected:

    DoubleConstant(
        double value);

public:

    const double value;
};

/**
****************************************************************************
* Character constant defined in the configuration file.
****************************************************************************
*/
class SPI_IMPORT CharConstant : public Constant
{
public:
    static CharConstantConstSP Make(
        char value);

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns "char"
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the char value for the scalar
    ************************************************************************
    */
    char getChar() const;

    SPI_DECLARE_OBJECT_TYPE(CharConstant);

    friend class CharConstant_Helper;
    spi_boost::shared_ptr<CharConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;

protected:

    CharConstant(
        char value);

public:

    const char value;
};

/**
****************************************************************************
* String constant defined in the configuration file.
****************************************************************************
*/
class SPI_IMPORT StringConstant : public Constant
{
public:
    static StringConstantConstSP Make(
        const std::string& value);

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns "string"
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the string value for the scalar
    ************************************************************************
    */
    std::string getString() const;

    SPI_DECLARE_OBJECT_TYPE(StringConstant);

    friend class StringConstant_Helper;
    spi_boost::shared_ptr<StringConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;

protected:

    StringConstant(
        const std::string& value);

public:

    const std::string value;
};

/**
****************************************************************************
* Bool constant defined in the configuration file.
****************************************************************************
*/
class SPI_IMPORT BoolConstant : public Constant
{
public:
    static BoolConstantConstSP Make(
        bool value);

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns "bool"
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the bool value for the scalar
    ************************************************************************
    */
    bool getBool() const;

    SPI_DECLARE_OBJECT_TYPE(BoolConstant);

    friend class BoolConstant_Helper;
    spi_boost::shared_ptr<BoolConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;

protected:

    BoolConstant(
        bool value);

public:

    const bool value;
};

/**
****************************************************************************
* No description.
****************************************************************************
*/
class SPI_IMPORT UndefinedConstant : public Constant
{
public:
    static UndefinedConstantConstSP Make();

    /**
    ************************************************************************
    * returns the data type name for the scalar
    ************************************************************************
    */
    std::string typeName() const;

    /**
    ************************************************************************
    * returns the string which should appear in documentation
    ************************************************************************
    */
    std::string docString() const;

    /**
    ************************************************************************
    * returns the integer value (where applicable) for the scalar
    ************************************************************************
    */
    int getInt() const;

    /**
    ************************************************************************
    * returns the date value (where applicable) for the scalar
    ************************************************************************
    */
    spi::Date getDate() const;

    /**
    ************************************************************************
    * returns the date value (where applicable) for the scalar
    ************************************************************************
    */
    spi::DateTime getDateTime() const;

    /**
    ************************************************************************
    * returns the double value (where applicable) for the scalar
    ************************************************************************
    */
    double getDouble() const;

    /**
    ************************************************************************
    * returns the char value (where applicable) for the scalar
    ************************************************************************
    */
    char getChar() const;

    /**
    ************************************************************************
    * returns the string value (where applicable) for the scalar
    ************************************************************************
    */
    std::string getString() const;

    /**
    ************************************************************************
    * returns the bool value (where applicable) for the scalar
    ************************************************************************
    */
    bool getBool() const;

    SPI_DECLARE_OBJECT_TYPE(UndefinedConstant);

    friend class UndefinedConstant_Helper;
    spi_boost::shared_ptr<UndefinedConstant_Helper> make_helper() const;

    spi_boost::shared_ptr<Constant_Helper> make_base_helper() const;
};

SPDOC_END_NAMESPACE

#endif /* _SPDOC_CONSTANT_HPP_*/

