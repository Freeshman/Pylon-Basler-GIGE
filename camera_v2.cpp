/*Camera capture function using "Pylon+opencv"  method to save the captured Image
 *This capture way does support the Image file to story in a given directory
 *                                                          
 *                                                          Tom-Hu
 *                                                          CAUC-SIAE
 *                                                          Mail:wdqqhs@qq.com
 *                                                          2018-03-12
 *                                                          
 */
#include "camera.h"
int Camera_Init(){
    int exitCode = 0;
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
        camera.ExposureTimeAbs.SetValue(EXPTIME);
        camera.Width.SetValue(WIDTH);
        camera.Height.SetValue(HEIGHT);
        camera.BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Once);
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
               cout << "ExposureTimeAbs "<<camera.ExposureTimeAbs.GetValue()<< endl;
               cout << "Width            : " << camera.Width.GetValue() << endl;
               cout << "Height           : " << camera.Height.GetValue() << endl;
               cout << "BalancehiteAuto  : " << camera.BalanceWhiteAuto.GetValue()<<endl;
        // Close the camera.
        camera.BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Off);
               cout << "BalancehiteAuto  : " << camera.BalanceWhiteAuto.GetValue()<<endl;
        camera.Close();
    }
    catch (const GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        exitCode = 1;
    }
    // Releases all pylon resources.
    PylonTerminate();
    return exitCode;
}

int Capture(QString number)
{
    // The exit code of the sample application.
    int exitCode = 0;
    // 创建一个Pylonlmage后续将用来创建OpenCV images
          CPylonImage pylonImage;
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

        camera.BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Once);
        camera.ExposureTimeAbs.SetValue(50000);
        camera.Width.SetValue(camera.Width.GetMax());
        camera.Height.SetValue(camera.Height.GetMax());
       // 新建pylon ImageFormatConverter对象.
       CImageFormatConverter formatConverter;
       //确定输出像素格式
       formatConverter.OutputPixelFormat = PixelType_BGR8packed;
        //Start Grabbing one photo
        camera.StartGrabbing(1);
        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;
        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
                while ( camera.IsGrabbing())
                {
                    // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
                    camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
                    // Image grabbed successfully?
                    if (ptrGrabResult->GrabSucceeded())
                    {
                        QString filename="test/"+number+".png";
                        formatConverter.Convert(pylonImage, ptrGrabResult);
                        Mat image = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3,(uint8_t *) pylonImage.GetBuffer());
                        imwrite(filename.toStdString(),image);
                    }else
                    {
                        cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
                    }
                }
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
    // Releases all pylon resources.
    PylonTerminate();
    return exitCode;
}
