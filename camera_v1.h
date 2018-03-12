/*   A lite capture library for the lab407 basler colorful camera
 *      with the support of parameters changing such as ExposureTimeAbs etc.
 *                                                                         ---Tom
 *                                                             Mail:wdqqhs@qq.com
 *                                                                     2018-03-04
 * Note:Paste the following code into a Qt project file to configure
        # LIBS setting for using pylon5
        LIBS += -L/opt/pylon5/lib64
        LIBS += -L/opt/pylon5/bin/Linux64_x64
        LIBS += -L/opt/pylon5/genicam/bin/Linux64_x64/GenApi/Generic
        LIBS += -L/opt/pylon5/genicam/bin/Linux64_x64
        LIBS += -lpylonbase
        LIBS += -lpylonutility
        LIBS += -lGenApi_gcc_v3_0_Basler_pylon_v5_0
        LIBS += -lGCBase_gcc_v3_0_Basler_pylon_v5_0
        LIBS += -lLog_gcc_v3_0_Basler_pylon_v5_0
        LIBS += -lNodeMapData_gcc_v3_0_Basler_pylon_v5_0
        LIBS += -lXmlParser_gcc_v3_0_Basler_pylon_v5_0
        LIBS += -lMathParser_gcc_v3_0_Basler_pylon_v5_0


        # Include for pylon
        INCLUDEPATH +=  /opt/pylon5/include \
                        /opt/pylon5/genicam/library/CPP/include \
                        /opt/pylon5/Samples/include \
 *
 */
#ifndef CAMERA_H
#define CAMERA_H
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <QString>

#define WIDTH 1028
#define HEIGHT 1028
#define EXPTIME 50000
// Namespace for using pylon objects.
typedef Pylon::CBaslerGigEInstantCamera Camera_t;
using namespace Basler_GigECameraParams;
using namespace Pylon;
// Namespace for using cout.
using namespace std;
/*
 * Capture function to capture a image and save as the given name with type of QString
 */
int Capture(QString);

/*
 * Init the camera to set the ExposureTimeAbs and WhiteBalanceOnce
 */
int Camera_Init();
#endif // CAMERA_H
