#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

int main()
{
    std::ifstream fragmentFile("../fragments.txt");
    if (!fragmentFile.is_open())
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier fragments.txt" << std::endl;
        return 1;
    }

    int index;
    double posx, posy, angle;

    int fresqueWidth = 1707;
    int fresqueHeight = 775;
    cv::Mat fresque(fresqueHeight, fresqueWidth, CV_8UC4, cv::Scalar(255, 255, 255)); // empty fresque

    while (fragmentFile >> index >> posx >> posy >> angle)
    {

        // Load the fragment
        std::string fragmentFileName = "../frag_eroded/frag_eroded_" + std::to_string(index) + ".png";
        cv::Mat fragment = cv::imread(fragmentFileName, -1);

        if (fragment.empty())
        {
            std::cerr << "Erreur : Impossible de charger l'image " << fragmentFileName << std::endl;
            return 1;
        }

     

        // Rotate the fragment
        cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point(fragment.cols / 2, fragment.rows / 2), angle, 1);
        cv::Mat rotatedFragment;
        cv::warpAffine(fragment, rotatedFragment, rotationMatrix, fragment.size());

        // Coordinate of the top left corner of the fragment
        int x = static_cast<int>(posx - rotatedFragment.cols / 2);
        int y = static_cast<int>(posy - rotatedFragment.rows / 2);

        // copy the fragment to the fresque
        for (int i = 0; i < rotatedFragment.rows; i++)
        {
            for (int j = 0; j < rotatedFragment.cols; j++)
            {
                cv::Vec4b &pixel = rotatedFragment.at<cv::Vec4b>(i, j);
                if (pixel[3] > 0)
                {
                    fresque.at<cv::Vec4b>(y + i, x + j) = pixel;
                }
            }
        }
    
    }
        // Save l'image de la fresque finale
        cv::imwrite("fresque_reconstruite.png", fresque);

        // print the fresque
        cv::imshow("Fresque", fresque);
        cv::waitKey(0);

        fragmentFile.close();
        return 0;
}