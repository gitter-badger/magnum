/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Configuration.h>

#include "Magnum/Math/Vector.h"

struct Vec3 {
    float x, y, z;
};

namespace Magnum { namespace Math {

namespace Implementation {

template<> struct VectorConverter<3, Float, Vec3> {
    #if !defined(__GNUC__) || defined(__clang__)
    constexpr /* See the convert() test case */
    #endif
    static Vector<3, Float> from(const Vec3& other) {
        return {other.x, other.y, other.z};
    }

    constexpr static Vec3 to(const Vector<3, Float>& other) {
        return {other[0], other[1], other[2]};
    }
};

}

namespace Test {

struct VectorTest: Corrade::TestSuite::Tester {
    explicit VectorTest();

    void construct();
    void constructFromData();
    void constructPad();
    void constructDefault();
    void constructNoInit();
    void constructOneValue();
    void constructOneComponent();
    void constructConversion();
    void constructCopy();
    void convert();

    void isZero();
    void isNormalized();

    void data();

    void negative();
    void addSubtract();
    void multiplyDivide();
    void multiplyDivideIntegral();
    void multiplyDivideComponentWise();
    void multiplyDivideComponentWiseIntegral();
    void modulo();
    void bitwise();

    void compare();
    void compareComponentWise();

    void dot();
    void dotSelf();
    void length();
    void lengthInverted();
    void normalized();
    void resized();

    void sum();
    void product();
    void min();
    void max();

    void projected();
    void projectedOntoNormalized();
    void angle();

    void subclassTypes();
    void subclass();

    void debug();
    void configuration();
};

typedef Math::Rad<Float> Rad;
typedef Vector<3, Float> Vector3;
typedef Vector<4, Float> Vector4;
typedef Vector<4, Int> Vector4i;

VectorTest::VectorTest() {
    addTests({&VectorTest::construct,
              &VectorTest::constructFromData,
              &VectorTest::constructPad,
              &VectorTest::constructDefault,
              &VectorTest::constructNoInit,
              &VectorTest::constructOneValue,
              &VectorTest::constructOneComponent,
              &VectorTest::constructConversion,
              &VectorTest::constructCopy,
              &VectorTest::convert,

              &VectorTest::isZero,
              &VectorTest::isNormalized,

              &VectorTest::data,

              &VectorTest::negative,
              &VectorTest::addSubtract,
              &VectorTest::multiplyDivide,
              &VectorTest::multiplyDivideIntegral,
              &VectorTest::multiplyDivideComponentWise,
              &VectorTest::multiplyDivideComponentWiseIntegral,
              &VectorTest::modulo,
              &VectorTest::bitwise,

              &VectorTest::compare,
              &VectorTest::compareComponentWise,

              &VectorTest::dot,
              &VectorTest::dotSelf,
              &VectorTest::length,
              &VectorTest::lengthInverted,
              &VectorTest::normalized,
              &VectorTest::resized,

              &VectorTest::sum,
              &VectorTest::product,
              &VectorTest::min,
              &VectorTest::max,

              &VectorTest::projected,
              &VectorTest::projectedOntoNormalized,
              &VectorTest::angle,

              &VectorTest::subclassTypes,
              &VectorTest::subclass,

              &VectorTest::debug,
              &VectorTest::configuration});
}

void VectorTest::construct() {
    constexpr Vector4 a = {1.0f, 2.0f, -3.0f, 4.5f};
    CORRADE_COMPARE(a, Vector4(1.0f, 2.0f, -3.0f, 4.5f));
}

void VectorTest::constructFromData() {
    Float data[] = { 1.0f, 2.0f, 3.0f, 4.0f };
    CORRADE_COMPARE(Vector4::from(data), Vector4(1.0f, 2.0f, 3.0f, 4.0f));
}

void VectorTest::constructPad() {
    constexpr Vector<2, Float> a{1.0f, -1.0f};
    constexpr Vector4 b = Vector4::pad(a);
    constexpr Vector4 c = Vector4::pad(a, 5.0f);
    CORRADE_COMPARE(b, Vector4(1.0f, -1.0f, 0.0f, 0.0f));
    CORRADE_COMPARE(c, Vector4(1.0f, -1.0f, 5.0f, 5.0f));

    constexpr Vector<5, Float> d{1.0f, -1.0f, 8.0f, 2.3f, -1.1f};
    constexpr Vector4 e = Vector4::pad(d);
    CORRADE_COMPARE(e, Vector4(1.0f, -1.0f, 8.0f, 2.3f));
}

void VectorTest::constructDefault() {
    constexpr Vector4 a;
    constexpr Vector4 b{ZeroInit};
    CORRADE_COMPARE(a, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    CORRADE_COMPARE(b, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
}

void VectorTest::constructNoInit() {
    Vector4 a{1.0f, 2.0f, -3.0f, 4.5f};
    new(&a) Vector4{NoInit};
    CORRADE_COMPARE(a, (Vector4{1.0f, 2.0f, -3.0f, 4.5f}));
}

void VectorTest::constructOneValue() {
    #ifndef CORRADE_MSVC2015_COMPATIBILITY
    /* Can't use delegating constructors with constexpr -- https://connect.microsoft.com/VisualStudio/feedback/details/1579279/c-constexpr-does-not-work-with-delegating-constructors */
    constexpr
    #endif
    Vector4 a(7.25f);

    CORRADE_COMPARE(a, Vector4(7.25f, 7.25f, 7.25f, 7.25f));

    /* Implicit conversion is not allowed */
    CORRADE_VERIFY(!(std::is_convertible<Float, Vector4>::value));
}

void VectorTest::constructOneComponent() {
    typedef Vector<1, Float> Vector1;

    /* Implicit constructor must work */
    constexpr Vector1 vec = 1.0f;
    CORRADE_COMPARE(vec, Vector1(1));
}

void VectorTest::constructConversion() {
    constexpr Vector4 a(1.3f, 2.7f, -15.0f, 7.0f);
    #ifndef CORRADE_MSVC2015_COMPATIBILITY
    /* Can't use delegating constructors with constexpr -- https://connect.microsoft.com/VisualStudio/feedback/details/1579279/c-constexpr-does-not-work-with-delegating-constructors */
    constexpr
    #endif
    Vector4i b(a);

    CORRADE_COMPARE(b, Vector4i(1, 2, -15, 7));

    /* Implicit conversion is not allowed */
    CORRADE_VERIFY(!(std::is_convertible<Vector4, Vector4i>::value));
}

void VectorTest::constructCopy() {
    constexpr Vector4 a(1.0f, 3.5f, 4.0f, -2.7f);
    constexpr Vector4 b(a);
    CORRADE_COMPARE(b, Vector4(1.0f, 3.5f, 4.0f, -2.7f));
}

void VectorTest::convert() {
    constexpr Vec3 a{1.5f, 2.0f, -3.5f};
    constexpr Vector3 b(1.5f, 2.0f, -3.5f);

    /* GCC 5.1 fills the result with zeros instead of properly calling
       delegated copy constructor if using constexpr. Reported here:
       https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66450
       MSVC 2015: Can't use delegating constructors with constexpr:
       https://connect.microsoft.com/VisualStudio/feedback/details/1579279/c-constexpr-does-not-work-with-delegating-constructors */
    #if (!defined(__GNUC__) || defined(__clang__)) && !defined(CORRADE_MSVC2015_COMPATIBILITY)
    constexpr
    #endif
    Vector3 c{a};
    CORRADE_COMPARE(c, b);

    #ifndef CORRADE_MSVC2015_COMPATIBILITY /* Why can't be conversion constexpr? */
    constexpr
    #endif
    Vec3 d(b);
    CORRADE_COMPARE(d.x, a.x);
    CORRADE_COMPARE(d.y, a.y);
    CORRADE_COMPARE(d.z, a.z);

    /* Implicit conversion is not allowed */
    CORRADE_VERIFY(!(std::is_convertible<Vec3, Vector3>::value));
    CORRADE_VERIFY(!(std::is_convertible<Vector3, Vec3>::value));
}

void VectorTest::isZero() {
    CORRADE_VERIFY(!Vector3(0.01f, 0.0f, 0.0f).isZero());
    CORRADE_VERIFY(Vector3(0.0f, 0.0f, 0.0f).isZero());
}

void VectorTest::isNormalized() {
    CORRADE_VERIFY(!Vector3(1.0f, 2.0f, -1.0f).isNormalized());
    CORRADE_VERIFY(Vector3(0.0f, 1.0f, 0.0f).isNormalized());
}

void VectorTest::data() {
    Vector4 vector(4.0f, 5.0f, 6.0f, 7.0f);
    vector[2] = 1.0f;
    vector[3] = 1.5f;

    CORRADE_COMPARE(vector[2], 1.0f);
    CORRADE_COMPARE(vector[3], 1.5f);
    CORRADE_COMPARE(vector, Vector4(4.0f, 5.0f, 1.0f, 1.5f));

    /* Pointer chasings, i.e. *(b.data()[3]), are not possible */
    constexpr Vector4 a(1.0f, 2.0f, -3.0f, 4.5f);
    constexpr Float f = a[3];
    #ifndef CORRADE_MSVC2015_COMPATIBILITY /* Apparently dereferencing pointer is verboten */
    constexpr
    #endif
    Float g = *a.data();
    CORRADE_COMPARE(f, 4.5f);
    CORRADE_COMPARE(g, 1.0f);
}

void VectorTest::compare() {
    CORRADE_VERIFY(Vector4(1.0f, -3.5f, 5.0f, -10.0f) == Vector4(1.0f + TypeTraits<Float>::epsilon()/2, -3.5f, 5.0f, -10.0f));
    CORRADE_VERIFY(Vector4(1.0f, -1.0f, 5.0f, -10.0f) != Vector4(1.0f, -1.0f + TypeTraits<Float>::epsilon()*2, 5.0f, -10.0f));

    CORRADE_VERIFY(Vector4i(1, -3, 5, -10) == Vector4i(1, -3, 5, -10));
    CORRADE_VERIFY(Vector4i(1, -3, 5, -10) != Vector4i(1, -2, 5, -10));
}

void VectorTest::compareComponentWise() {
    typedef BoolVector<3> BoolVector3;
    CORRADE_COMPARE(Vector3(1.0f, -1.0f, 5.0f) < Vector3(1.1f, -1.0f, 3.0f), BoolVector3(0x1));
    CORRADE_COMPARE(Vector3(1.0f, -1.0f, 5.0f) <= Vector3(1.1f, -1.0f, 3.0f), BoolVector3(0x3));
    CORRADE_COMPARE(Vector3(1.0f, -1.0f, 5.0f) >= Vector3(1.1f, -1.0f, 3.0f), BoolVector3(0x6));
    CORRADE_COMPARE(Vector3(1.0f, -1.0f, 5.0f) > Vector3(1.1f, -1.0f, 3.0f), BoolVector3(0x4));
}

void VectorTest::negative() {
    CORRADE_COMPARE(-Vector4(1.0f, -3.0f, 5.0f, -10.0f), Vector4(-1.0f, 3.0f, -5.0f, 10.0f));
}

void VectorTest::addSubtract() {
    Vector4 a(1.0f, -3.0f, 5.0f, -10.0f);
    Vector4 b(7.5f, 33.0f, -15.0f, 0.0f);
    Vector4 c(8.5f, 30.0f, -10.0f, -10.0f);

    CORRADE_COMPARE(a + b, c);
    CORRADE_COMPARE(c - b, a);
}

void VectorTest::multiplyDivide() {
    Vector4 vector(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 multiplied(-1.5f, -3.0f, -4.5f, -6.0f);

    CORRADE_COMPARE(vector*-1.5f, multiplied);
    CORRADE_COMPARE(-1.5f*vector, multiplied);
    CORRADE_COMPARE(multiplied/-1.5f, vector);

    /* Divide vector with number and invert */
    Vector4 divisor(1.0f, 2.0f, -4.0f, 8.0f);
    Vector4 result(1.0f, 0.5f, -0.25f, 0.125f);
    CORRADE_COMPARE(1.0f/divisor, result);
}

void VectorTest::multiplyDivideIntegral() {
    Vector4i vector(32, 10, -6, 2);
    Vector4i multiplied(-48, -15, 9, -3);

    CORRADE_COMPARE(vector*-1.5f, multiplied);
    CORRADE_COMPARE(-1.5f*vector, multiplied);

    CORRADE_COMPARE(multiplied/-1.5f, vector);
    /* Using integer vector as divisor is not supported */
}

void VectorTest::multiplyDivideComponentWise() {
    Vector4 vec(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 multiplier(7.0f, -4.0f, -1.5f, 1.0f);
    Vector4 multiplied(7.0f, -8.0f, -4.5f, 4.0f);

    CORRADE_COMPARE(vec*multiplier, multiplied);
    CORRADE_COMPARE(multiplied/multiplier, vec);
}

void VectorTest::multiplyDivideComponentWiseIntegral() {
    Vector4i vec(7, 2, -16, -1);
    Vector4 multiplier(2.0f, -1.5f, 0.5f, 10.0f);
    Vector4i multiplied(14, -3, -8, -10);

    CORRADE_COMPARE(vec*multiplier, multiplied);
    CORRADE_COMPARE(multiplier*vec, multiplied);

    CORRADE_COMPARE(multiplied/multiplier, vec);
    /* Using integer vector as divisor is not supported */
}

void VectorTest::modulo() {
    typedef Math::Vector<2, Int> Vector2i;

    const Vector2i a(4, 13);
    const Vector2i b(2, 5);
    CORRADE_COMPARE(a % 2, Vector2i(0, 1));
    CORRADE_COMPARE(a % b, Vector2i(0, 3));
}

void VectorTest::bitwise() {
    typedef Math::Vector<2, Int> Vector2i;

    const Vector2i a(85, 240);
    const Vector2i b(170, 85);
    CORRADE_COMPARE(~a, Vector2i(-86, -241));
    CORRADE_COMPARE(a & b, Vector2i(0, 80));
    CORRADE_COMPARE(a | b, Vector2i(255, 245));
    CORRADE_COMPARE(a ^ b, Vector2i(255, 165));

    const Vector2i c(7, 32);
    CORRADE_COMPARE(c << 2, Vector2i(28, 128));
    CORRADE_COMPARE(c >> 2, Vector2i(1, 8));
}

void VectorTest::dot() {
    CORRADE_COMPARE(Math::dot(Vector4{1.0f, 0.5f, 0.75f, 1.5f}, {2.0f, 4.0f, 1.0f, 7.0f}), 15.25f);
}

void VectorTest::dotSelf() {
    CORRADE_COMPARE(Vector4(1.0f, 2.0f, 3.0f, 4.0f).dot(), 30.0f);
}

void VectorTest::length() {
    CORRADE_COMPARE(Vector4(1.0f, 2.0f, 3.0f, 4.0f).length(), 5.4772256f);
}

void VectorTest::lengthInverted() {
    CORRADE_COMPARE(Vector4(1.0f, 2.0f, 3.0f, 4.0f).lengthInverted(), 0.182574f);
}

void VectorTest::normalized() {
    const auto vec = Vector4(1.0f, 1.0f, 1.0f, 1.0f).normalized();
    CORRADE_COMPARE(vec, Vector4(0.5f, 0.5f, 0.5f, 0.5f));
    CORRADE_COMPARE(vec.length(), 1.0f);
}

void VectorTest::resized() {
    const auto vec = Vector4(2.0f, 2.0f, 0.0f, 1.0f).resized(9.0f);
    CORRADE_COMPARE(vec, Vector4(6.0f, 6.0f, 0.0f, 3.0f));
    CORRADE_COMPARE(vec.length(), 9.0f);
}

void VectorTest::sum() {
    CORRADE_COMPARE(Vector3(1.0f, 2.0f, 4.0f).sum(), 7.0f);
}

void VectorTest::product() {
    CORRADE_COMPARE(Vector3(1.0f, 2.0f, 3.0f).product(), 6.0f);
}

void VectorTest::min() {
    /* Check also that initial value isn't initialized to 0 */
    CORRADE_COMPARE(Vector3(1.0f, -2.0f, 3.0f).min(), -2.0f);
}

void VectorTest::max() {
    /* Check also that initial value isn't initialized to 0 */
    CORRADE_COMPARE(Vector3(-1.0f, -2.0f, -3.0f).max(), -1.0f);
}

void VectorTest::projected() {
    Vector3 line(1.0f, -1.0f, 0.5f);
    Vector3 projected = Vector3(1.0f, 2.0f, 3.0f).projected(line);

    CORRADE_COMPARE(projected, Vector3(0.222222f, -0.222222f, 0.111111f));
    CORRADE_COMPARE(projected.normalized(), line.normalized());
}

void VectorTest::projectedOntoNormalized() {
    std::ostringstream o;
    Error::setOutput(&o);

    Vector3 vector(1.0f, 2.0f, 3.0f);
    Vector3 line(1.0f, -1.0f, 0.5f);
    vector.projectedOntoNormalized(line);
    CORRADE_COMPARE(o.str(), "Math::Vector::projectedOntoNormalized(): line must be normalized\n");

    Vector3 projected = vector.projectedOntoNormalized(line.normalized());
    CORRADE_COMPARE(projected, Vector3(0.222222f, -0.222222f, 0.111111f));
    CORRADE_COMPARE(projected.normalized(), line.normalized());
    CORRADE_COMPARE(projected, vector.projected(line));
}

void VectorTest::angle() {
    std::ostringstream o;
    Error::setOutput(&o);
    Math::angle(Vector3(2.0f, 3.0f, 4.0f).normalized(), {1.0f, -2.0f, 3.0f});
    CORRADE_COMPARE(o.str(), "Math::angle(): vectors must be normalized\n");

    o.str({});
    Math::angle({2.0f, 3.0f, 4.0f}, Vector3(1.0f, -2.0f, 3.0f).normalized());
    CORRADE_COMPARE(o.str(), "Math::angle(): vectors must be normalized\n");

    CORRADE_COMPARE(Math::angle(Vector3(2.0f,  3.0f, 4.0f).normalized(),
                                Vector3(1.0f, -2.0f, 3.0f).normalized()),
                    Rad(1.162514f));
}

template<class T> class BasicVec2: public Math::Vector<2, T> {
    public:
        /* MSVC 2015 can't handle {} here */
        template<class ...U> constexpr BasicVec2(U&&... args): Math::Vector<2, T>(args...) {}

        MAGNUM_VECTOR_SUBCLASS_IMPLEMENTATION(2, BasicVec2)
};

MAGNUM_VECTORn_OPERATOR_IMPLEMENTATION(2, BasicVec2)

typedef BasicVec2<Float> Vec2;
typedef BasicVec2<Int> Vec2i;

void VectorTest::subclassTypes() {
    Float* const data = nullptr;
    const Float* const cdata = nullptr;
    CORRADE_VERIFY((std::is_same<decltype(Vec2::from(data)), Vec2&>::value));
    CORRADE_VERIFY((std::is_same<decltype(Vec2::from(cdata)), const Vec2&>::value));

    Vector<1, Float> one;
    CORRADE_VERIFY((std::is_same<decltype(Vec2::pad(one)), Vec2>::value));

    /* Const operators */
    const Vec2 c;
    const Vec2 c2;
    CORRADE_VERIFY((std::is_same<decltype(-c), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c + c), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c*1.0f), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(1.0f*c), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c/1.0f), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(1.0f/c), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c*c2), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c/c2), Vec2>::value));

    /* Assignment operators */
    Vec2 a;
    CORRADE_VERIFY((std::is_same<decltype(a = c), Vec2&>::value));
    CORRADE_VERIFY((std::is_same<decltype(a += c), Vec2&>::value));
    CORRADE_VERIFY((std::is_same<decltype(a -= c), Vec2&>::value));
    CORRADE_VERIFY((std::is_same<decltype(a *= 1.0f), Vec2&>::value));
    CORRADE_VERIFY((std::is_same<decltype(a /= 1.0f), Vec2&>::value));
    CORRADE_VERIFY((std::is_same<decltype(a *= c), Vec2&>::value));
    CORRADE_VERIFY((std::is_same<decltype(a /= c), Vec2&>::value));

    /* Modulo operations */
    const Vec2i ci;
    Vec2i i;
    const Int j = {};
    CORRADE_VERIFY((std::is_same<decltype(ci % j), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(i %= j), Vec2i&>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci % ci), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(i %= ci), Vec2i&>::value));

    /* Bitwise operations */
    CORRADE_VERIFY((std::is_same<decltype(~ci), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci & ci), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci | ci), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci ^ ci), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci << 1), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci >> 1), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(i &= ci), Vec2i&>::value));
    CORRADE_VERIFY((std::is_same<decltype(i |= ci), Vec2i&>::value));
    CORRADE_VERIFY((std::is_same<decltype(i ^= ci), Vec2i&>::value));
    CORRADE_VERIFY((std::is_same<decltype(i <<= 1), Vec2i&>::value));
    CORRADE_VERIFY((std::is_same<decltype(i >>= 1), Vec2i&>::value));

    /* Integer multiplication/division */
    CORRADE_VERIFY((std::is_same<decltype(ci*1.0f), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(1.0f*ci), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(c*ci), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci*c), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(ci/c), Vec2i>::value));
    CORRADE_VERIFY((std::is_same<decltype(i *= c), Vec2i&>::value));
    CORRADE_VERIFY((std::is_same<decltype(i /= c), Vec2i&>::value));

    /* Functions */
    CORRADE_VERIFY((std::is_same<decltype(c.normalized()), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c.resized(1.0f)), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c.projected(c2)), Vec2>::value));
    CORRADE_VERIFY((std::is_same<decltype(c.projectedOntoNormalized(c2)), Vec2>::value));
}

void VectorTest::subclass() {
    Float data[] = {1.0f, -2.0f};
    CORRADE_COMPARE(Vec2::from(data), Vec2(1.0f, -2.0f));

    const Float cdata[] = {1.0f, -2.0f};
    CORRADE_COMPARE(Vec2::from(cdata), Vec2(1.0f, -2.0f));

    {
        constexpr Vector<1, Float> a = 5.0f;
        #ifndef CORRADE_MSVC2015_COMPATIBILITY /* Probably because copy is not constexpr */
        constexpr
        #endif
        Vec2 b = Vec2::pad(a);
        #ifndef CORRADE_MSVC2015_COMPATIBILITY /* Probably because copy is not constexpr */
        constexpr
        #endif
        Vec2 c = Vec2::pad(a, -1.0f);
        CORRADE_COMPARE(b, Vec2(5.0f, 0.0f));
        CORRADE_COMPARE(c, Vec2(5.0f, -1.0f));
    }

    /* Constexpr constructor */
    constexpr const Vec2 a{-2.0f, 5.0f};
    CORRADE_COMPARE(a[0], -2.0f);

    CORRADE_COMPARE(Vec2(-2.0f, 5.0f) + Vec2(1.0f, -3.0f), Vec2(-1.0f, 2.0f));
    CORRADE_COMPARE(Vec2(-2.0f, 5.0f) - Vec2(1.0f, -3.0f), Vec2(-3.0f, 8.0f));

    CORRADE_COMPARE(Vec2(-2.0f, 5.0f)*2.0f, Vec2(-4.0f, 10.0f));
    CORRADE_COMPARE(2.0f*Vec2(-2.0f, 5.0f), Vec2(-4.0f, 10.0f));
    CORRADE_COMPARE(Vec2(-2.0f, 5.0f)/0.5f, Vec2(-4.0f, 10.0f));
    CORRADE_COMPARE(2.0f/Vec2(-2.0f, 5.0f), Vec2(-1.0f, 0.4f));

    CORRADE_COMPARE(Vec2(-2.0f, 5.0f)*Vec2(1.5f, -2.0f), Vec2(-3.0f, -10.0f));
    CORRADE_COMPARE(Vec2(-2.0f, 5.0f)/Vec2(2.0f/3.0f, -0.5f), Vec2(-3.0f, -10.0f));

    /* Modulo operations */
    CORRADE_COMPARE(Vec2i(4, 13) % 2, Vec2i(0, 1));
    CORRADE_COMPARE(Vec2i(4, 13) % Vec2i(2, 5), Vec2i(0, 3));

    /* Bitwise operations */
    CORRADE_COMPARE(~Vec2i(85, 240), Vec2i(-86, -241));
    CORRADE_COMPARE(Vec2i(85, 240) & Vec2i(170, 85), Vec2i(0, 80));
    CORRADE_COMPARE(Vec2i(85, 240) | Vec2i(170, 85), Vec2i(255, 245));
    CORRADE_COMPARE(Vec2i(85, 240) ^ Vec2i(170, 85), Vec2i(255, 165));

    CORRADE_COMPARE(Vec2i(7, 32) << 2, Vec2i(28, 128));
    CORRADE_COMPARE(Vec2i(7, 32) >> 2, Vec2i(1, 8));

    /* Integral multiplication/division */
    CORRADE_COMPARE(Vec2i(2, 4)*1.5f, Vec2i(3, 6));
    CORRADE_COMPARE(1.5f*Vec2i(2, 4), Vec2i(3, 6));
    CORRADE_COMPARE(Vec2i(2, 4)/(2.0f/3.0f), Vec2i(3, 6));

    CORRADE_COMPARE(Vec2i(2, 4)*Vec2(-1.5f, 0.5f), Vec2i(-3, 2));
    CORRADE_COMPARE(Vec2(-1.5f, 0.5f)*Vec2i(2, 4), Vec2i(-3, 2));
    CORRADE_COMPARE(Vec2i(2, 4)/Vec2(-2.0f/3.0f, 2.0f), Vec2i(-3, 2));

    /* Functions */
    CORRADE_COMPARE(Vec2(3.0f, 0.0f).normalized(), Vec2(1.0f, 0.0f));
    CORRADE_COMPARE(Vec2(3.0f, 0.0f).resized(6.0f), Vec2(6.0f, 0.0f));
    CORRADE_COMPARE(Vec2(1.0f, 1.0f).projected({0.0f, 2.0f}), Vec2(0.0f, 1.0f));
    CORRADE_COMPARE(Vec2(1.0f, 1.0f).projectedOntoNormalized({0.0f, 1.0f}), Vec2(0.0f, 1.0f));
}

void VectorTest::debug() {
    std::ostringstream o;
    Debug(&o) << Vector4(0.5f, 15.0f, 1.0f, 1.0f);
    CORRADE_COMPARE(o.str(), "Vector(0.5, 15, 1, 1)\n");

    o.str({});
    Debug(&o) << "a" << Vector4() << "b" << Vector4();
    CORRADE_COMPARE(o.str(), "a Vector(0, 0, 0, 0) b Vector(0, 0, 0, 0)\n");
}

void VectorTest::configuration() {
    Corrade::Utility::Configuration c;

    Vector4 vec(3.0f, 3.125f, 9.0f, 9.55f);
    std::string value("3 3.125 9 9.55");

    c.setValue("vector", vec);
    CORRADE_COMPARE(c.value("vector"), value);
    CORRADE_COMPARE(c.value<Vector4>("vector"), vec);
}

}}}

CORRADE_TEST_MAIN(Magnum::Math::Test::VectorTest)
