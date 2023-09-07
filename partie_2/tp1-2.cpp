#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

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
    double totalFresqueArea = 0.0;

    for (const Fragment& solutionFragment : solutionFragments) {
        for (const Fragment& actualFragment : fragments) {
            if (solutionFragment.index == actualFragment.index) {
                // Vérifier la localisation
                bool wellLocalized = (std::abs(solutionFragment.x - actualFragment.x) <= delta_x) &&
                                     (std::abs(solutionFragment.y - actualFragment.y) <= delta_y) &&
                                     (std::abs(solutionFragment.alpha - actualFragment.alpha) <= delta_alpha);

                if (wellLocalized) {
                    // Ajouter la surface du fragment bien localisé
                    // Vous devez avoir une logique pour calculer la surface d'un fragment ici
                    // Par exemple, vous pouvez utiliser des dimensions spécifiques des fragments.
                    // totalWellLocalizedArea += surfaceFragment(solutionFragment);
                }
            }
        }
    }

    // Calculer la surface totale des fragments de la fresque
    // Vous devez avoir une logique pour calculer la surface totale des fragments ici.

    // Calculer la précision p
    double p = totalWellLocalizedArea / totalFresqueArea;

    // Afficher la précision
    std::cout << "Précision de localisation (p) : " << p << std::endl;

    return 0;
}
