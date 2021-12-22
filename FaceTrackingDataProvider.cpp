#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include "SRanipal.h"
#include "SRanipal_Eye.h"
#include "SRanipal_Lip.h"
#include "SRanipal_Enums.h"
#include "SRanipal_NotRelease.h"
#pragma comment (lib, "SRanipal.lib")
using namespace ViveSR;

#include <iostream>
#include <chrono>
#include <thread>

std::string CovertErrorCode(int error);

std::thread* t = nullptr;
bool EnableLip = false, EnableLipV2 = false;
bool looping = false;
void streaming() {
    // Lip
    char lip_image[800 * 400];
    ViveSR::anipal::Lip::LipData lip_data;
    ViveSR::anipal::Lip::LipData_v2 lip_data_v2;
    lip_data.image = lip_image;
    lip_data_v2.image = lip_image;

    int result = ViveSR::Error::WORK;
    while (looping) {
        if (EnableLipV2) {
            result = ViveSR::anipal::Lip::GetLipData_v2(&lip_data_v2);
            if (result == ViveSR::Error::WORK) {
                float* weightings = lip_data_v2.prediction_data.blend_shape_weight;
                printf("Lip Ver2: \n");
                for (int i = 0; i < ViveSR::anipal::Lip::blend_shape_nums; i++) {
                    printf("%.2f ", weightings[i]);
                }
                printf("\n");
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}


int main()
{

    std::cout << "Initializing SRAnipal Lip v2 Engine\n";

    char* version = new char();
    ViveSR::anipal::SRanipal_GetVersion(version);

    char str = 0;
    int error, id = NULL;
    while (true) {

        //init lipv2
        error = ViveSR::anipal::Initial(ViveSR::anipal::Lip::ANIPAL_TYPE_LIP_V2, NULL);
        if (error == ViveSR::Error::WORK) {
            EnableLipV2 = true;
            printf("Successfully initialize version2 Lip engine.\n");
            printf("\nPress any key to stop engine");
        }
        else printf("Fail to initialize version2 Lip engine. please refer the code %d %s.\n", error, CovertErrorCode(error).c_str());

        //launch thread
        if (t == nullptr) {
            t = new std::thread(streaming);
            if (t)   looping = true;
        }

        //user input to stop
        str = getchar();

        //stop thread
        looping = false;
        if (t == nullptr) continue;
        t->join();
        delete t;
        t = nullptr;

        break;
    }
    ViveSR::anipal::Release(ViveSR::anipal::Lip::ANIPAL_TYPE_LIP);
    ViveSR::anipal::Release(ViveSR::anipal::Lip::ANIPAL_TYPE_LIP_V2);
    return 0;
}

std::string CovertErrorCode(int error) {
    std::string result = "";
    switch (error) {
    case(RUNTIME_NOT_FOUND):     result = "RUNTIME_NOT_FOUND"; break;
    case(NOT_INITIAL):           result = "NOT_INITIAL"; break;
    case(FAILED):                result = "FAILED"; break;
    case(WORK):                  result = "WORK"; break;
    case(INVALID_INPUT):         result = "INVALID_INPUT"; break;
    case(FILE_NOT_FOUND):        result = "FILE_NOT_FOUND"; break;
    case(DATA_NOT_FOUND):        result = "DATA_NOT_FOUND"; break;
    case(UNDEFINED):             result = "UNDEFINED"; break;
    case(INITIAL_FAILED):        result = "INITIAL_FAILED"; break;
    case(NOT_IMPLEMENTED):       result = "NOT_IMPLEMENTED"; break;
    case(NULL_POINTER):          result = "NULL_POINTER"; break;
    case(OVER_MAX_LENGTH):       result = "OVER_MAX_LENGTH"; break;
    case(FILE_INVALID):          result = "FILE_INVALID"; break;
    case(UNINSTALL_STEAM):       result = "UNINSTALL_STEAM"; break;
    case(MEMCPY_FAIL):           result = "MEMCPY_FAIL"; break;
    case(NOT_MATCH):             result = "NOT_MATCH"; break;
    case(NODE_NOT_EXIST):        result = "NODE_NOT_EXIST"; break;
    case(UNKONW_MODULE):         result = "UNKONW_MODULE"; break;
    case(MODULE_FULL):           result = "MODULE_FULL"; break;
    case(UNKNOW_TYPE):           result = "UNKNOW_TYPE"; break;
    case(INVALID_MODULE):        result = "INVALID_MODULE"; break;
    case(INVALID_TYPE):          result = "INVALID_TYPE"; break;
    case(MEMORY_NOT_ENOUGH):     result = "MEMORY_NOT_ENOUGH"; break;
    case(BUSY):                  result = "BUSY"; break;
    case(NOT_SUPPORTED):         result = "NOT_SUPPORTED"; break;
    case(INVALID_VALUE):         result = "INVALID_VALUE"; break;
    case(COMING_SOON):           result = "COMING_SOON"; break;
    case(INVALID_CHANGE):        result = "INVALID_CHANGE"; break;
    case(TIMEOUT):               result = "TIMEOUT"; break;
    case(DEVICE_NOT_FOUND):      result = "DEVICE_NOT_FOUND"; break;
    case(INVALID_DEVICE):        result = "INVALID_DEVICE"; break;
    case(NOT_AUTHORIZED):        result = "NOT_AUTHORIZED"; break;
    case(ALREADY):               result = "ALREADY"; break;
    case(INTERNAL):              result = "INTERNAL"; break;
    case(CONNECTION_FAILED):     result = "CONNECTION_FAILED"; break;
    case(ALLOCATION_FAILED):     result = "ALLOCATION_FAILED"; break;
    case(OPERATION_FAILED):      result = "OPERATION_FAILED"; break;
    case(NOT_AVAILABLE):         result = "NOT_AVAILABLE"; break;
    case(CALLBACK_IN_PROGRESS):  result = "CALLBACK_IN_PROGRESS"; break;
    case(SERVICE_NOT_FOUND):     result = "SERVICE_NOT_FOUND"; break;
    case(DISABLED_BY_USER):      result = "DISABLED_BY_USER"; break;
    case(EULA_NOT_ACCEPT):       result = "EULA_NOT_ACCEPT"; break;
    case(RUNTIME_NO_RESPONSE):   result = "RUNTIME_NO_RESPONSE"; break;
    case(OPENCL_NOT_SUPPORT):    result = "OPENCL_NOT_SUPPORT"; break;
    case(NOT_SUPPORT_EYE_TRACKING): result = "NOT_SUPPORT_EYE_TRACKING"; break;
    case(LIP_NOT_SUPPORT):       result = "LIP_NOT_SUPPORT"; break;
    default:
        result = "No such error code";
    }
    return result;
}