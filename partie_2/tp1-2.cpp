#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

struct Fragment {
    int index;
    double x, y, alpha;
};

int main() {
    // Charger les données de solution.txt
    std::ifstream solutionFile("../solution.txt");
    std::vector<Fragment> solutionFragments;
    int index;
    double x, y, alpha;
    while (solutionFile >> index >> x >> y >> alpha) {
        solutionFragments.push_back({index, x, y, alpha});
    }
    solutionFile.close();

    // Charger les données de fragments.txt
    std::ifstream fragmentsFile("../fragments.txt");
    std::vector<Fragment> fragments;
    while (fragmentsFile >> index >> x >> y >> alpha) {
        fragments.push_back({index, x, y, alpha});
    }
    fragmentsFile.close();

    // Paramètres de tolérance
    double delta_x = 1.0; // 1 pixel
    double delta_y = 1.0; // 1 pixel
    double delta_alpha = 1.0; // 1 degré

    // Calculer la précision
    double totalWellLocalizedArea = 0.0;
    double totalFragmentNotFromFresque = 0.0;
    double totalFresqueArea = 0.0;

    for (const Fragment& solutionFragment : solutionFragments) {
        bool inThefreque = false;
            std::string fragmentFileName = "../frag_eroded/frag_eroded_" + std::to_string(solutionFragment.index) + ".png";
            cv::Mat fragment = cv::imread(fragmentFileName);

            //print fragment size
            std::cout << "Fragment " << solutionFragment.index << " : " << fragment.cols << "x" << fragment.rows << std::endl;
            totalFresqueArea += fragment.cols * fragment.rows;


        for (const Fragment& actualFragment : fragments) {
            if (solutionFragment.index == actualFragment.index) {
                inThefreque = true;
                // Vérifier la localisation
                bool wellLocalized = (std::abs(solutionFragment.x - actualFragment.x) <= delta_x) &&
                                     (std::abs(solutionFragment.y - actualFragment.y) <= delta_y) &&
                                     (std::abs(solutionFragment.alpha - actualFragment.alpha) <= delta_alpha);
                std::cout << "Fragment " << solutionFragment.index << " : " << (wellLocalized ? "bien localisé" : "mal localisé") << std::endl;

                if (wellLocalized) {
                    // Calculer la surface bien localisée
                    totalWellLocalizedArea += fragment.cols * fragment.rows;
                    
                }
            }
        }
        if(!inThefreque) {
            std::cout << "Fragment " << solutionFragment.index << " : " << "n'appartient pas à la fresque" << std::endl;
            totalFragmentNotFromFresque += fragment.cols * fragment.rows;
        }
    }

    // Calculer la surface totale des fragments de la fresque
    // Vous devez avoir une logique pour calculer la surface totale des fragments ici.
    //print all the surface 
    std::cout << "Surface totale des fragments de la fresque : " << totalFresqueArea << std::endl;
    std::cout << "Surface totale des fragments bien localisés : " << totalWellLocalizedArea << std::endl;
    std::cout << "Surface totale des fragments n'appertenant pas à la fresque : " << totalFragmentNotFromFresque << std::endl;

    // Calculer la précision p
    double p = (totalWellLocalizedArea-totalFragmentNotFromFresque) / totalFresqueArea;

    // Afficher la précision
    std::cout << "Précision de localisation (p) : " << p << std::endl;

    return 0;
}
