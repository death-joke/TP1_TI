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
    cv::Mat fresque(fresqueHeight, fresqueWidth, CV_8UC3, cv::Scalar(255, 255, 255)); // empty fresque

    while (fragmentFile >> index >> posx >> posy >> angle)
    {
        if (index < 400)
        {

            // Load the fragment
            std::string fragmentFileName = "../frag_eroded/frag_eroded_" + std::to_string(index) + ".png";
            cv::Mat fragment = cv::imread(fragmentFileName);

            if (fragment.empty())
            {
                std::cerr << "Erreur : Impossible de charger l'image " << fragmentFileName << std::endl;
                return 1;
            }

            // position of the fragment
            std::cout << "Index: " << index << ", Position: (" << posx << ", " << posy << "), Angle: " << angle << std::endl;

            // Rotate the fragment
            cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point(fragment.cols / 2, fragment.rows / 2), angle, 1);
            cv::Mat rotatedFragment;
            cv::warpAffine(fragment, rotatedFragment, rotationMatrix, fragment.size());

            // Placez le fragment dans l'image de fresque
            int x = static_cast<int>(posx - rotatedFragment.cols / 2);
            int y = static_cast<int>(posy - rotatedFragment.rows / 2);

            // copy the fragment to the fresque
            for (int i = 0; i < rotatedFragment.rows; i++)
            {
                for (int j = 0; j < rotatedFragment.cols; j++)
                {
                    if (rotatedFragment.at<cv::Vec3b>(i, j) != cv::Vec3b(0, 0, 0))

                    {
                        fresque.at<cv::Vec3b>(y + i, x + j) = rotatedFragment.at<cv::Vec3b>(i, j);
                    }
                }
            }
        }
    }

    // Enregistrez l'image de la fresque finale
    cv::imwrite("fresque_reconstruite.png", fresque);

    // print the fresque
    // cv::namedWindow("fresque image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Fresque", fresque);
    cv::waitKey(0);

    fragmentFile.close();
    return 0;
}