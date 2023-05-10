#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    // Lecture de l'image à partir de la console
    string image_path;
    cout << "Entrez le chemin de l'image : ";
    getline(cin, image_path);
    Mat image = imread(image_path);

    // Conversion de l'image en niveau de gris
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);

    // Application d'un filtre flou pour réduire le bruit
    Mat blurred_image;
    GaussianBlur(gray_image, blurred_image, Size(3, 3), 0);

    // Binarisation de l'image pour segmenter les caractères
    Mat binary_image;
    threshold(blurred_image, binary_image, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    // Détection des contours pour extraire les caractères
    vector<vector<Point>> contours;
    findContours(binary_image, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    // Extraction des régions d'intérêt (ROI) contenant les caractères
    vector<Rect> char_rects;
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        float aspect_ratio = float(rect.width) / float(rect.height);
        if (aspect_ratio >= 0.2 && aspect_ratio <= 1.0 && rect.area() >= 100) {
            char_rects.push_back(rect);
        }
    }

    // Affichage des caractères extraits et écriture dans la console
    for (size_t i = 0; i < char_rects.size(); i++) {
        // Extraction de la région d'intérêt (ROI) contenant le caractère
        Mat char_roi = binary_image(char_rects[i]);

        // Redimensionnement de la ROI pour avoir une taille fixe
        Mat resized_char_roi;
        resize(char_roi, resized_char_roi, Size(20, 20));

        // Ecriture du caractère dans la console
        cout << "Caractere " << i + 1 << " : " << endl;
        for (int row = 0; row < resized_char_roi.rows; row++) {
            for (int col = 0; col < resized_char_roi.cols; col++) {
                if (resized_char_roi.at<uchar>(row, col) == 0) {
                    cout << "*";
                }
                else {
                    cout << " ";
                }
            }
            cout << endl;
        }
        cout << endl;

        // Dessin du rectangle autour du caractère sur l'image originale
        rectangle(image, char_rects[i], Scalar(0, 0, 255), 2);
    }


    namedWindow("Image", WINDOW_NORMAL);
    imshow("Image", image);
    waitKey(0);

    // Effacement de la mémoire allouée par les images
    image.release();
    gray_image.release();
    blurred_image.release();
    binary_image.release();

    return 0;
}
