// ParametrizeCamera_NativeParameterAccess.cpp
/*
    Note: Before getting started, Basler recommends reading the Programmer's Guide topic
    in the pylon C++ API documentation that gets installed with pylon.
    If you are upgrading to a higher major version of pylon, Basler also
    strongly recommends reading the Migration topic in the pylon C++ API documentation.
    For camera configuration and for accessing other parameters, the pylon API
    uses the technologies defined by the GenICam standard hosted by the
    European Machine Vision Association (EMVA). The GenICam specification
    (http://www.GenICam.org) defines a format for camera description files.
    These files describe the configuration interface of GenICam compliant cameras.
    The description files are written in XML (eXtensible Markup Language) and
    describe camera registers, their interdependencies, and all other
    information needed to access high-level features such as Gain,
    Exposure Time, or Image Format by means of low-level register read and
    write operations.
    The elements of a camera description file are represented as software
    objects called Nodes. For example, a node can represent a single camera
    register, a camera parameter such as Gain, a set of available parameter
    values, etc. Each node implements the GenApi::INode interface.
    Using the code generators provided by GenICam's GenApi module,
    a programming interface is created from a camera description file.
    Thereby, a member is provided for each parameter that is available for the camera device.
    The programming interface is exported by the Device Specific Instant Camera classes.
    This is the easiest way to access parameters.
    This sample shows the 'native' approach for configuring a camera
    using device specific instant camera classes.
    See also the ParametrizeCamera_GenericParameterAccess sample for the 'generic'
    approach for configuring a camera.
*/
// Include files to use the PYLON API.
#define USE_GIGE
#include <pylon/PylonIncludes.h>
#include <QString>
#include <cv.h>
#include <highgui.h>
// Namespace for using pylon objects.
using namespace Pylon;
#include <pylon/gige/BaslerGigEInstantCamera.h>
typedef Pylon::CBaslerGigEInstantCamera Camera_t;
using namespace Basler_GigECameraParams;
// Namespace for using cout.
using namespace std;
using namespace cv;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 3;

int main(int argc, char* argv[])
{
//    Mat image;
    // 创建一个Pylonlmage后续将用来创建OpenCV images
          CPylonImage pylonImage;
    // The exit code of the sample application.
    int exitCode = 0;
    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();
    try
    {
        // Only look for cameras supported by Camera_t.
        CDeviceInfo info;
        info.SetDeviceClass( Camera_t::DeviceClass());

        // Create an instant camera object with the first found camera device matching the specified device class.
        Camera_t camera( CTlFactory::GetInstance().CreateFirstDevice( info));

        // Open the camera for accessing the parameters.
        camera.Open();

        // Get camera device information.
        cout << "Camera Device Information" << endl
             << "=========================" << endl;
        cout << "Vendor           : "
             << camera.DeviceVendorName.GetValue() << endl;
        cout << "Model            : "
             << camera.DeviceModelName.GetValue() << endl;
        cout << "Firmware version : "
             << camera.DeviceFirmwareVersion.GetValue() << endl << endl;
        // Camera settings.
        cout << "Camera Device Settings" << endl
             << "======================" << endl;

        camera.BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Once);
        camera.ExposureTimeAbs.SetValue(200000);
        cout << "ExposureTimeAbs "<<camera.ExposureTimeAbs.GetValue()<< endl;
        cout << "Width            : " << camera.Width.GetValue() << endl;
        cout << "Height           : " << camera.Height.GetValue() << endl;
        camera.Width.SetValue(camera.Width.GetMax());
        camera.Height.SetValue(camera.Height.GetMax());
//设置相机最大缓冲区,默认为10
       camera.MaxNumBuffer = 5;
       // 新建pylon ImageFormatConverter对象.
       CImageFormatConverter formatConverter;
       //确定输出像素格式
       formatConverter.OutputPixelFormat = PixelType_BGR8packed;
        // For demonstration purposes only, set the exposure time to an initial value.
        //camera.ExposureTimeRaw.SetValue( camera.ExposureTimeRaw.GetMin());
/*
        // Set the AOI:
        // On some cameras the Offsets are read-only,
        // so we check whether we can write a value. Otherwise, we would get an exception.
        // GenApi has some convenience predicates to check this easily.
        if (IsWritable(camera.OffsetX))
        {
            camera.OffsetX.SetValue(camera.OffsetX.GetMin());
        }
        if (IsWritable(camera.OffsetY))
        {
            camera.OffsetY.SetValue(camera.OffsetY.GetMin());
        }
*/
//        // Some properties have restrictions. Use GetInc/GetMin/GetMax to make sure you set a valid value.
//        int64_t newWidth = 202;
//        newWidth = Adjust(newWidth, camera.Width.GetMin(), camera.Width.GetMax(), camera.Width.GetInc());
//        int64_t newHeight = 101;
//        newHeight = Adjust(newHeight, camera.Height.GetMin(), camera.Height.GetMax(), camera.Height.GetInc());
//        camera.Width.SetValue(newWidth);
//        camera.Height.SetValue(newHeight);
//        cout << "OffsetX          : " << camera.OffsetX.GetValue() << endl;
//        cout << "OffsetY          : " << camera.OffsetY.GetValue() << endl;
//        cout << "Width            : " << camera.Width.GetValue() << endl;
//        cout << "Height           : " << camera.Height.GetValue() << endl;
//        // Remember the current pixel format.

//        //set the pixelformat
//        PixelFormatEnums oldPixelFormat = camera.PixelFormat.GetValue();
//        cout << "Old PixelFormat  : " << camera.PixelFormat.ToString() << " (" << oldPixelFormat << ")" << endl;
//        // Restore the old pixel format.
//        camera.PixelFormat.SetValue(oldPixelFormat);
        camera.StartGrabbing( c_countOfImagesToGrab);
        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;
        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
                // when c_countOfImagesToGrab images have been retrieved.
                int i=0;
                while ( camera.IsGrabbing())
                {
                    // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
                    camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
                    // Image grabbed successfully?
                    if (ptrGrabResult->GrabSucceeded())
                    {
                        // Access the image data.
                        cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
                        cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
                        const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
                        cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
//                        cout <<  (uint32_t) pImageBuffer[1] << endl << endl;
//                        cout <<  (uint32_t) pImageBuffer[2] << endl << endl;
//                        Mat image = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, pImageBuffer);
//                        Pylon::CImagePersistence::Save(ImageFileFormat_Png,"test.png",ptrGrabResult);
                        formatConverter.Convert(pylonImage, ptrGrabResult);
                        Mat image = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3,(uint8_t *) pylonImage.GetBuffer());
//                        Mat image = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1,(uint8_t *) ptrGrabResult->GetBuffer());
                        QString filename=QString::number(i)+".png";
                        imwrite(filename.toStdString(),image);
                        i++;
                    }else
                    {
                        cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
                    }
                }
//        // Set pixel format to Mono8 if available.
//        if ( GenApi::IsAvailable( camera.PixelFormat.GetEntry(PixelFormat_Mono8)))
//        {
//            camera.PixelFormat.SetValue(PixelFormat_Mono8);
//            cout << "New PixelFormat  : " << camera.PixelFormat.ToString() << " (" << camera.PixelFormat.GetValue() << ")" << endl;
//        }
#ifdef GAIN
        // Set the new gain to 50% ->  Min + ((Max-Min) / 2)
        //
        // Note: Some newer camera models may have auto functions enabled.
        //       To be able to set the gain value to a specific value
        //       the Gain Auto function must be disabled first.
        if (IsWritable(camera.GainAuto))
        {
            camera.GainAuto.FromString("Off");
        }
        int64_t newGainRaw = camera.GainRaw.GetMin() + ((camera.GainRaw.GetMax() - camera.GainRaw.GetMin()) / 2);
        // Make sure the calculated value is valid
        newGainRaw = Adjust(newGainRaw, camera.GainRaw.GetMin(), camera.GainRaw.GetMax(), camera.GainRaw.GetInc());
        camera.GainRaw.SetValue(newGainRaw);
        cout << "Gain (50%)       : " << camera.GainRaw.GetValue() << " (Min: " << camera.GainRaw.GetMin() << "; Max: " << camera.GainRaw.GetMax() << "; Inc: " << camera.GainRaw.GetInc() << ")" << endl;
#endif
        // Close the camera.
        camera.Close();
    }
    catch (const GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        exitCode = 1;
    }
    // Comment the following two lines to disable waiting on exit
    cerr << endl << "Press Enter to exit." << endl;
    while( cin.get() != '\n');
    // Releases all pylon resources.
    PylonTerminate();
    return exitCode;
}

#ifdef GAIN
// Adjust value so it complies with range and increment passed.
//
// The parameter's minimum and maximum are always considered as valid values.
// If the increment is larger than one, the returned value will be: min + (n * inc).
// If the value doesn't meet these criteria, it will be rounded down so that it does.
int64_t Adjust(int64_t val, int64_t minimum, int64_t maximum, int64_t inc)
{
    // Check the input parameters.
    if (inc <= 0)
    {
        // Negative increments are invalid.
        throw LOGICAL_ERROR_EXCEPTION("Unexpected increment %d", inc);
    }
    if (minimum > maximum)
    {
        // Minimum must not be bigger than or equal to the maximum.
        throw LOGICAL_ERROR_EXCEPTION("minimum bigger than maximum.");
    }
    // Check the lower bound.
    if (val < minimum)
    {
        return minimum;
    }
    // Check the upper bound.
    if (val > maximum)
    {
        return maximum;
    }
    // Check the increment.
    if (inc == 1)
    {
        // Special case: all values are valid.
        return val;
    }
    else
    {
        // The value must be min + (n * inc).
        // Due to the integer division, the value will be rounded down.
        return minimum + ( ((val - minimum) / inc) * inc );
    }
}
#endif
