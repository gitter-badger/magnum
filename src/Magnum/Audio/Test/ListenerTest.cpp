/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2015 Jonathan Hale <squareys@googlemail.com>

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

#include <Corrade/TestSuite/Tester.h>

#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/MatrixTransformation2D.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

#include "Magnum/Audio/Playable.h"
#include "Magnum/Audio/Context.h"
#include "Magnum/Audio/Listener.h"
#include "Magnum/Audio/PlayableGroup.h"

namespace Magnum { namespace Audio { namespace Test {

typedef SceneGraph::Scene<SceneGraph::MatrixTransformation2D> Scene2D;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation2D> Object2D;

typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;

struct ListenerTest: TestSuite::Tester {
    explicit ListenerTest();

    void testFeature2D();
    void testFeature3D();
    void testUpdateGroups();

    Context _context;
};

ListenerTest::ListenerTest(): _context() {
    addTests({&ListenerTest::testFeature2D,
              &ListenerTest::testFeature3D,
              &ListenerTest::testUpdateGroups});
}

void ListenerTest::testFeature2D() {
    Scene2D scene;
    Object2D object{&scene};
    Listener2D listener{object};

    constexpr Vector3 offset{1.0f, 2.0f, 0.0f};
    object.translate(offset.xy());
    listener.update({});

    CORRADE_COMPARE(Renderer::listenerPosition(), offset);
}

void ListenerTest::testFeature3D() {
    Scene3D scene;
    Object3D object{&scene};
    Listener3D listener{object};

    constexpr Vector3 offset{2.0f, 4.0f, 7.0f};
    object.translate(offset);
    listener.update({});

    CORRADE_COMPARE(Renderer::listenerPosition(), offset);
}

void ListenerTest::testUpdateGroups() {
    Scene3D scene;
    Object3D sourceObject{&scene};
    Object3D object{&scene};
    PlayableGroup3D group;
    Playable3D playable{sourceObject, &group};
    Listener3D listener{object};

    constexpr Vector3 offset{6.0f, 2.0f, -2.0f};
    object.rotateY(Deg(90.0f));
    object.translate(offset);
    sourceObject.translate(offset*13.0f);

    listener.update({group});

    CORRADE_COMPARE(Renderer::listenerPosition(), offset);
    constexpr Vector3 rotatedFwd{-1.0f, 0.0f, 0.0f};
    CORRADE_COMPARE(Renderer::listenerOrientation()[0], rotatedFwd);
    CORRADE_COMPARE(playable.source().position(), offset*13.0f);
}

}}}

CORRADE_TEST_MAIN(Magnum::Audio::Test::ListenerTest)
