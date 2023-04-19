#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#pragma warning(disable : 4819)
#endif

#include <Exceptions.h>
#include <iostream>
#include <cstring>
#include <tuple>

#include <ImageIO.h>
#include <ImagesCPU.h>
#include <ImagesNPP.h>

#include <npp.h>

#include <filesystem>
#include <vector>
#include <exception>

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define stringify( name ) #name

std::vector<std::string> ListDir(const std::string &dirPath)
{
    DIR *dp;
    struct dirent *ep;
    std::vector<std::string> paths;

    dp = opendir(dirPath.c_str());
    if (dp != NULL)
    {
        while (ep = readdir(dp))
        {
            std::string extension = ".tiff";
            std::string d_name(ep->d_name); // = string(;
            if ( d_name.length() > extension.length() && d_name.compare(d_name.length() - extension.length(), extension.length(), extension)==0) //strcmp(d_name, ".") != 0 && strcmp(d_name, "..") != 0 &&
            {
                // std::cout<< "11"<< std::endl;
                auto tmp = d_name; //dirPath + "/" + 
                paths.push_back(tmp);
                std::cout << tmp << std::endl;
            }
        }
        (void)closedir(dp);
    }
    else
        perror("Couldn't open the directory");

    return paths;
}

std::tuple<std::string, std::string> ParseCommandLine(int argc, char *argv[])
{
    std::string images_dir_path, out_path;

    for (int i = 1; i < argc; i += 2)
    {
        if (std::strcmp(argv[i], "-I") == 0)
        {
            images_dir_path = argv[i + 1];
        }
        else if (std::strcmp(argv[i], "-O") == 0)
        {
            out_path = argv[i + 1];
        }
        else
        {
            throw new std::exception();
        }
    }

    auto rt = std::tuple<std::string, std::string>(images_dir_path, out_path);
    return rt;
}

int main(int argc, char *argv[])
{
    // std::string images_dir_path, xx;
    auto tuple = ParseCommandLine(argc, argv);

    std::string images_dir_path = std::get<0>(tuple); //"/home/raffy/repos/python-playground/cuda-final-project/asd.jpg";
    std::string out_path = std::get<1>(tuple); //"/home/raffy/repos/python-playground/cuda-final-project/asd.jpg";
    
    std::cout << "images_dir_path: " << images_dir_path << std::endl;
    std::cout << "out_path: " << out_path << std::endl;

    std::vector<std::string> images_names = ListDir(images_dir_path);

    npp::ImageCPU_8u_C1 host_image;

    try
    {
        for (auto i = images_names.begin(); i < images_names.end(); i++)
        {
            auto image_name = *i;
            std::string image_path = images_dir_path +"/"+ image_name;
            std::cout << "processing: " << image_path <<" ..."<< std::endl;
            npp::loadImage(image_path, host_image); // loads grayscale image
            std::cout << "loading image to host: done" << std::endl;
            std::cout << "src pitch: " << (int)host_image.pitch() << std::endl;
            npp::ImageNPP_8u_C1 srcDeviceImage(host_image);
            std::cout << "srcDeviceImage: done" << std::endl;

            NppiSize srcROI = {(int)srcDeviceImage.width(), (int)srcDeviceImage.height()};
            npp::ImageNPP_8u_C1 dstDeviceImage(srcROI.width, srcROI.height);
            std::cout << "ROI: done" << std::endl;

            auto nppiError = nppiMirror_8u_C1R(
                srcDeviceImage.data(),
                srcDeviceImage.pitch(),
                dstDeviceImage.data(),
                dstDeviceImage.pitch(),
                srcROI,
                NppiAxis::NPP_HORIZONTAL_AXIS); //mirrors given image horizontally
            std::cout << "status:" << cudaGetErrorName((cudaError_t)nppiError) << std::endl;
            std::cout << "image mirroring: done" << std::endl;

            npp::ImageCPU_8u_C1 dstHostImage(dstDeviceImage.size());
            dstDeviceImage.copyTo(dstHostImage.data(), dstHostImage.pitch()); // copies image from device to host
            std::cout << "dst copying: done" << std::endl;
        
            saveImage(out_path+"/"+image_name.substr(0,image_name.length()-5) + "_new.tiff", dstHostImage); //saves image
            std::cout << "saving: done" << std::endl;
            
            nppiFree(srcDeviceImage.data()); 
            nppiFree(dstDeviceImage.data());
        }
        exit(EXIT_SUCCESS);
    }
    catch (const npp::Exception &e)
    {
        std::cerr << e << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

}