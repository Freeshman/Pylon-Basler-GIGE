/*Camera capture function using Pylon CImagePersistence Save method to save the captured Image
 *However this capture way does not support the Image file to story in a given directory
 *To achieve the goal described above, using "Pylon+opencv" capture function
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
                        QString filename=number+".png";
/*Attention! Pylon::CImagePersistence::Save does not support the directory in the filename!!!!
*/
                       Pylon::CImagePersistence::Save(ImageFileFormat_Png,filename.toStdString().c_str(),ptrGrabResult);
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
