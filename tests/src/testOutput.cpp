#include <iostream>
#include <fstream>
#include <string> 

/* arg[0]: reference output
 * arg[1]: output to test
*/
int main(int argc, char* argv[]) {

    int lineCount = 0;

    std::ifstream referenceOutput(argv[1]);
    std::ifstream testOutput(argv[2]);

    std::string referenceLine;
    std::string testLine;

    if(argc == 3)
    {
        while(std::getline(referenceOutput, referenceLine) && std::getline(testOutput, testLine))
        {
            lineCount++;

            if(testLine.compare(referenceLine) != 0)
            {
                if(testLine.compare("") == 0 || referenceLine.compare("") == 0)
                    std::cout << "ERREUR : Les deux fichiers n'ont pas la même longueur" << lineCount << std::endl;
                else
                {
                    std::cout << "ERREUR : Les résultats ne sont pas cohérents, erreur à la ligne : " << lineCount << std::endl;
                    std::cout << "On attendait : " << referenceLine << std::endl;
                    std::cout << "On a reçu : " << testLine << std::endl;
                    return 0;
                }
            }

        }
    }
    else
    {
        std::cout << "Paramètres d'entrée incorects, il faut donner deux fichiers à comparer" << std::endl;
        return 0;
    }

    if(lineCount == 0)
        std::cout << "Impossible de lire les fichiers donné en entrée" << std::endl;
    else
    {
        std::cout << std::endl << std::endl << "LE RESULTAT EST CORRECT" << std::endl << std::endl;
    }
    return 0;
}