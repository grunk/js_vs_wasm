#include <vector>
#include <cmath>
#include <algorithm>
#include <emscripten.h>

/**
 * Crée une table de correspondance (Look-Up Table) à partir d'un histogramme.
 * C'est l'équivalent C++ de la fonction createLut en JS.
 */
std::vector<int> createLut(const std::vector<int>& histogram, int totalPixels) {
    std::vector<int> cdf(256, 0);
    std::vector<int> lut(256, 0);

    // 1. Calculer la CDF (Fonction de Répartition Cumulative)
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // 2. Trouver la première valeur non-nulle de la CDF (cdfMin)
    int cdfMin = 0;
    for (int i = 0; i < 256; ++i) {
        if (cdf[i] > 0) {
            cdfMin = cdf[i];
            break;
        }
    }

    // 3. Calculer le dénominateur pour la normalisation
    float denominator = static_cast<float>(totalPixels - cdfMin);

    // Gérer la division par zéro (si tous les pixels ont la même couleur)
    if (denominator == 0) {
        for (int i = 0; i < 256; i++) {
            lut[i] = i; // Pas de changement
        }
        return lut;
    }

    // 4. Créer la table LUT basée sur la formule d'égalisation
    float scale = 255.0f / denominator;
    for (int i = 0; i < 256; i++) {
        float mappedValue = (static_cast<float>(cdf[i]) - cdfMin) * scale;
        // Arrondir et borner (clamp) entre 0 et 255
        lut[i] = static_cast<int>(std::round(std::max(0.0f, std::min(255.0f, mappedValue))));
    }

    return lut;
}

/**
 * Fonction principale exportée vers WASM.
 * Elle modifie le tableau de pixels "data" sur place.
 * Le format de "data" est [R, G, B, A, R, G, B, A, ...]
 */
extern "C" {
    // Ajout de EMSCRIPTEN_KEEPALIVE pour garantir l'exportation
    EMSCRIPTEN_KEEPALIVE
    void applyHistogramEqualization(unsigned char* data, int width, int height) {
        int totalPixels = width * height;
        // dataLength est le nombre total d'octets (pixels * 4 canaux)
        int dataLength = totalPixels * 4;

        // 1. Calculer les histogrammes pour R, G, B
        std::vector<int> histR(256, 0);
        std::vector<int> histG(256, 0);
        std::vector<int> histB(256, 0);

        for (int i = 0; i < dataLength; i += 4) {
            histR[data[i]]++;   // Rouge
            histG[data[i + 1]]++; // Vert
            histB[data[i + 2]]++; // Bleu
            // data[i+3] (Alpha) est ignoré
        }

        // 2. Créer les tables de correspondance (LUT)
        std::vector<int> lutR = createLut(histR, totalPixels);
        std::vector<int> lutG = createLut(histG, totalPixels);
        std::vector<int> lutB = createLut(histB, totalPixels);

        // 3. Appliquer les LUT aux données de l'image
        // Les valeurs R,G,B sont utilisées comme indices pour les tables LUT,
        // et la sortie est re-castée en unsigned char.
        for (int i = 0; i < dataLength; i += 4) {
            data[i] = static_cast<unsigned char>(lutR[data[i]]);
            data[i + 1] = static_cast<unsigned char>(lutG[data[i + 1]]);
            data[i + 2] = static_cast<unsigned char>(lutB[data[i + 2]]);
        }
        // L'alpha (data[i+3]) n'est pas modifié.
    }
}
