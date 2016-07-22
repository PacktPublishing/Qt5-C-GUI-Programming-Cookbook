import QtQuick 2.4
import QtCanvas3D 1.0
import QtQuick.Window 2.2

import "glcode.js" as GLCode

Window {
    title: qsTr("Qt_Canvas_3D")
    width: 480
    height: 320
    visible: true

    Canvas3D {
        id: canvas3d
        anchors.fill: parent
        focus: true

        onInitializeGL: {
            GLCode.initializeGL(canvas3d);
        }

        onPaintGL: {
            GLCode.paintGL(canvas3d);
        }

        onResizeGL: {
            GLCode.resizeGL(canvas3d);
        }
    }
}
