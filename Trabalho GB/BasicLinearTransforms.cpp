#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"

#include <highlevelmonitorconfigurationapi.h>
#include <opencv2\highgui\highgui.hpp>



#include <iostream>


//TRABALHO PROCESSAMENTO GRÁFICO - FILTROS E STICKERS
//Grupo: Arthur Wagner, André Monteiro e Isadora Ghisleni

//Funcionamento do programa:

/* Ao rodar o código, o usuário deve escolher entre 3 opções:

1 - Aplicação dos Stickers na imagem: o usuário deve clicar na ordem correta na tela, no osso cujo nome aparece na tela secundária. Caso queira conferir a resposta, basta clicar em 'r'.
2 - Aplicação de filtros: o usuário pode escolher dentre os filtros da lista para aplicar na imagem do esqueleto. Há a opção de salvar a image.
3 - Abertura da webcam: o usuário pode escolher abrir sua webcam com os filtros da lista, ou sem. Há a opção de salvar um frame da tela.


*/





using namespace cv;
using namespace std;

enum mouseEvents { NONE, LEFTBUTTON_DOWN };

//Declaração das variáveis

int mouseEvent = LEFTBUTTON_DOWN;
int mousex, mousey;
Mat imgSkelAlpha, imgSkelResp, imgStickers, imgOriginal, imgSkelColor, imgFilters; //TESTE
Mat imgFalanges, imgFemur, imgRadio, imgTibia, imgUmero, imgVeias, imgNervoso, imgSkelSistemas;
int stickercounter = 1;

void drawText(Mat& image);


//Função de sobrepor imagens dos Stickers

void overlayImage(Mat* src, Mat* overlay, const Point& location)
{
    for (int y = max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;
        if (fY >= overlay->rows)
            break;
        for (int x = max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;
            if (fX >= overlay->cols)
                break;
            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;
            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}


// Função de clique do mouse

static void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        //cout << "Left click has been made, Position: (" << x << ", " << y << ")" << endl;
        mouseEvent = LEFTBUTTON_DOWN;
        mousex = x;
        mousey = y;
        //cout << "lalla" << endl;
        if (stickercounter == 1)
        {
            overlayImage(&imgSkelAlpha, &imgFalanges, Point(mousex - imgFalanges.cols / 2, mousey - imgFalanges.rows / 2));
            imshow("MainWindow", imgSkelAlpha);
            stickercounter = stickercounter + 1;
        }
        else if (stickercounter == 2)
        {
            overlayImage(&imgSkelAlpha, &imgFemur, Point(mousex - imgFemur.cols / 2, mousey - imgFemur.rows / 2));
            imshow("MainWindow", imgSkelAlpha);
            stickercounter = 3;
        }
        else if (stickercounter == 3)
        {
            overlayImage(&imgSkelAlpha, &imgRadio, Point(mousex - imgRadio.cols / 2, mousey - imgRadio.rows / 2));
            imshow("MainWindow", imgSkelAlpha);
            stickercounter = stickercounter + 1;
        }
        else if (stickercounter == 4)
        {
            overlayImage(&imgSkelAlpha, &imgTibia, Point(mousex - imgTibia.cols / 2, mousey - imgTibia.rows / 2));
            imshow("MainWindow", imgSkelAlpha);
            stickercounter = stickercounter + 1;
        }
        else if (stickercounter == 5)
        {
            overlayImage(&imgSkelAlpha, &imgUmero, Point(mousex - imgUmero.cols / 2, mousey - imgUmero.rows / 2));
            imshow("MainWindow", imgSkelAlpha);
            stickercounter = stickercounter + 1;
        }
        else if (stickercounter == 6)
        {
            //SALVAR - salva na mesma pasta do cpp

            imwrite("Skelnovonomes.jpg", imgSkelAlpha);;

            stickercounter = stickercounter + 1;
        }
    }
    else if (event == EVENT_RBUTTONDOWN) {
        //cout << "Rightclick has been made, Position: (" << x << ", " << y << ")" << endl;
    }
    else if (event == EVENT_MBUTTONDOWN) {
        //cout << "Middleclick has been made, Position: (" << x << ", " << y << ")" << endl;
    }
    else if (event == EVENT_MOUSEMOVE) {
        //cout << "cURRENT MOUSE POSITIONS, Position: (" << x << ", " << y << ")" << endl;
    }
}





int main(int argc, char** argv)
{
    cout << "Built with OpenCV " << CV_VERSION << endl;
   
    //definição das imagens originais e tratadas
    Mat imgSkel;    //Define imagem original                                       
    Mat imgCinza, imgCanny, imgBlur, imgBlurCanny, imgDilate, imgErode; //define filtros da imagem, tratamento de cor
    Mat imgEqual, imgBilat;
    Mat imgCrop, imgReSize, imgScale;   //define cortes e dimensionamentos


    //Importa imagem do esqueleto
    imgSkel = imread("skel.jpg",IMREAD_UNCHANGED);   //Define a imagem capturada
    resize(imgSkel, imgScale, Size(), 0.8, 0.8);
    cvtColor(imgScale, imgSkelAlpha, COLOR_BGR2BGRA); //4canais

    imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
    resize(imgSkel, imgScale, Size(), 0.8, 0.8);
    cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); //4canais

    //Importa respostas
    imgSkelResp = imread("skelresposta.png", IMREAD_UNCHANGED);
    resize(imgSkelResp, imgSkelResp, Size(), 0.8, 0.8);
    cvtColor(imgSkelResp, imgSkelResp, COLOR_BGR2BGRA);

    //Importa Stickers
    imgStickers = imread("stickers.jpg", IMREAD_UNCHANGED);  
    imgFilters = imread("filters.jpg", IMREAD_UNCHANGED);

    //Importa nomes dos ossos - Stickers
    imgFalanges = imread("falanges.png", IMREAD_UNCHANGED);
    resize(imgFalanges, imgFalanges, Size(), 0.2, 0.2);

    imgFemur = imread("femur.png", IMREAD_UNCHANGED);
    resize(imgFemur, imgFemur, Size(), 0.2, 0.2);

    imgRadio = imread("radio.png", IMREAD_UNCHANGED);
    resize(imgRadio, imgRadio, Size(), 0.2, 0.2);

    imgTibia = imread("tibia.png", IMREAD_UNCHANGED);
    resize(imgTibia, imgTibia, Size(), 0.2, 0.2);

    imgUmero = imread("umero.png", IMREAD_UNCHANGED);
    resize(imgUmero, imgUmero, Size(), 0.2, 0.2);

    //Transforma em 4 canais
    cvtColor(imgFalanges, imgFalanges, COLOR_BGR2BGRA);
    cvtColor(imgFemur, imgFemur, COLOR_BGR2BGRA);
    cvtColor(imgRadio, imgRadio, COLOR_BGR2BGRA);
    cvtColor(imgTibia, imgTibia, COLOR_BGR2BGRA);
    cvtColor(imgUmero, imgUmero, COLOR_BGR2BGRA);
   

    //Abre janela principal
    namedWindow("MainWindow", WINDOW_AUTOSIZE);
    imshow("MainWindow", imgSkelAlpha);

    int main;
    main = 0;

    while (main != 'f') {



        //Tratamentos da imagem
        cvtColor(imgScale, imgCinza, COLOR_BGR2GRAY);   //1 filtro escala de cinza
        GaussianBlur(imgScale, imgBlur, Size(5, 5), 5, 0);  //Filtro de blur
        Canny(imgScale, imgCanny, 25, 75);  //Filtro Canny 
        Canny(imgBlur, imgBlurCanny, 25, 75);   //Filtro BlurCanny
        Mat grad_x, grad_y, grad;    //Processamento Sobel
        Sobel(imgScale, grad_x, CV_16S, 1, 0, 1, 1, 0, BORDER_DEFAULT);//Processamento Sobel
        Sobel(imgScale, grad_y, CV_16S, 0, 1, 1, 1, 0, BORDER_DEFAULT);//Processamento Sobel
        Mat abs_grad_x, abs_grad_y;//Processamento Sobel
        convertScaleAbs(grad_x, abs_grad_x);//Processamento Sobel
        convertScaleAbs(grad_y, abs_grad_y);//Processamento Sobel
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);//Processamento Sobel
        equalizeHist(imgCinza, imgEqual);
        bilateralFilter(imgScale, imgBilat, 15, 200, 300);//Filtro Bilateral
        printf("\n\n\n\n");


       for (;;)
        {

           // Primeiro MENU de escolha

            cout << "1 - Escolha o nome dos ossos" << endl;
            cout << "2 - Filtros" << endl;
            cout << "3 - Webcam" << endl;
         
            for (;;)
            {
                char a = (char)waitKey();



                if (a == '1') {

                    cout << "clique na tela na ordem pedida" << endl;
                    cout << "Digite 'r' para ver a resposta\n\n" << endl;

                    //cria janela secundária
                    namedWindow("SecWindow", WINDOW_AUTOSIZE);
                    imshow("SecWindow", imgStickers);


                    //Habilita função do mouse
                    setMouseCallback("MainWindow", mouseCallback, NULL);

                }
                else if (a == 'r') {

                    namedWindow("Respostas", WINDOW_AUTOSIZE);
                    imshow("Respostas", imgSkelResp);

                }

                else if (a == '2')
                {
                    destroyAllWindows();

                    imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                    resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                    imshow("MainWindow", imgScale);
                    imshow("SecWindow", imgFilters);

                    break;
                }

                else if (a == '3')
                {
                    destroyAllWindows();
                   
                    break;
                }

                
            }

            break;
        }

        //Seleção de filtros

        for (;;)
        {
            char c = (char)waitKey();

            if (c == 'm') {

                printf("\n\nMENU \n");
                printf("selecione o filtro: \n");
                printf(" 1 - Cinza\n 2 - Blur\n 3 - Canny\n 4 - Embacada\n 5 - Sobel \n 6 - Equalizado\n 7 - Bilateral\n 8 - Sistemas do corpo\n 9 - Sistema Circulatorio\n 0 - Sistema Nervoso\n s - salvar imagem\n");
            }


            else if (c == '1') {

                imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); 

                overlayImage(&imgOriginal, &imgCinza, Point(0, 0));
                imshow("MainWindow", imgOriginal); //display imagem com filtro escala de cinza


            }

            else if (c == '2') {

                imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); 

                overlayImage(&imgOriginal, &imgBlur, Point(0, 0));
                imshow("MainWindow", imgOriginal); //display imagem com filtro blur

            }

            else if (c == '3') {

                imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); 

                overlayImage(&imgOriginal, &imgCanny, Point(0, 0)); //detecção de bordas
                imshow("MainWindow", imgOriginal); //display imagem canny
            }

            else if (c == '4') {

                imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); 

                overlayImage(&imgOriginal, &imgBlurCanny, Point(0, 0));
                imshow("MainWindow", imgOriginal); //display imagem BlurCanny 
            }

            else if (c == '5') {

                imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); 

                overlayImage(&imgOriginal, &grad, Point(0, 0));
                imshow("MainWindow", imgOriginal); //Display imagem com filtro sobel
            }

            else if (c == '6') {

                imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); 

                overlayImage(&imgOriginal, &imgEqual, Point(0, 0));
                imshow("MainWindow", imgOriginal); //display imagem com filtro equal

            }

            else if (c == '7') {

                imgOriginal = imread("skel.jpg", IMREAD_UNCHANGED);   //Imagem original para restaurar
                resize(imgSkel, imgScale, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgOriginal, COLOR_BGR2BGRA); 

                overlayImage(&imgOriginal, &imgBilat, Point(0, 0));
                imshow("MainWindow", imgOriginal); //display imagem com filtro bilateral

            }
            else if (c == '8') {

                //Sobrepor imagens dos sistemas

                imgNervoso = imread("nervoso.png", IMREAD_UNCHANGED);
                resize(imgNervoso, imgNervoso, Size(), 0.8, 0.8);
                imgVeias = imread("veias.png", IMREAD_UNCHANGED);
                resize(imgVeias, imgVeias, Size(), 0.8, 0.8);
                cvtColor(imgScale, imgSkelSistemas, COLOR_BGR2BGRA);

                cvtColor(imgNervoso, imgNervoso, COLOR_BGR2BGRA);
                cvtColor(imgVeias, imgVeias, COLOR_BGR2BGRA);
                
                overlayImage(&imgSkelSistemas, &imgNervoso, Point(0, 0));
                overlayImage(&imgSkelSistemas, &imgVeias, Point(0, 0));

                imshow("MainWindow", imgSkelSistemas); //Imagem com os sistemas    


            }

            else if (c == '0') {

                //SISTEMA NERVOSO - filtro cor

                imgNervoso = imread("nervoso.png", IMREAD_UNCHANGED);
                resize(imgNervoso, imgNervoso, Size(), 0.9, 0.9);
                cvtColor(imgNervoso, imgNervoso, COLOR_BGR2BGRA);
              
                          
                cv::cvtColor(imgNervoso, imgNervoso, cv::COLOR_BGR2HSV); //Transforma em HSV
                cv::Vec3b hsvPixel(104, 100, 110); //códigos de cor

                Mat3b hsv(hsvPixel);//criar objeto Mat
                int thresh = 100; //alcance de cor

                Scalar minHSV = cv::Scalar(hsvPixel.val[0] - thresh, hsvPixel.val[1] - thresh, hsvPixel.val[2] - thresh);
                Scalar maxHSV = cv::Scalar(hsvPixel.val[0] + thresh, hsvPixel.val[1] + thresh, hsvPixel.val[2] + thresh);

                Mat maskHSV, imgSisNerv;
                inRange(imgNervoso, minHSV, maxHSV, maskHSV);
                bitwise_and(imgNervoso, imgNervoso, imgSisNerv, maskHSV);

                imshow("MainWindow", imgSisNerv); //Mostra imagem filtrando sistema nervoso

            }

            else if (c == '9') {

                //SISTEMA CIRCULATÓRIO - filtro cor

               
                imgVeias = imread("veias.png", IMREAD_UNCHANGED);
                resize(imgVeias, imgVeias, Size(), 0.8, 0.8);
                cvtColor(imgVeias, imgVeias, COLOR_BGR2BGRA);
               

                cv::cvtColor(imgVeias, imgSkelColor, cv::COLOR_BGR2HSV); //Transforma em HSV
                cv::Vec3b hsvPixel(96, 159, 238); //códigos de cor

                Mat3b hsv(hsvPixel);//criar objeto Mat
                int thresh = 100; //alcance de cor

                Scalar minHSV = cv::Scalar(hsvPixel.val[0] - thresh, hsvPixel.val[1] - thresh, hsvPixel.val[2] - thresh);
                Scalar maxHSV = cv::Scalar(hsvPixel.val[0] + thresh, hsvPixel.val[1] + thresh, hsvPixel.val[2] + thresh);

                Mat maskHSV, imgSisCirc;
                inRange(imgSkelColor, minHSV, maxHSV, maskHSV);
                bitwise_and(imgSkelColor, imgSkelColor, imgSisCirc, maskHSV);

                imshow("MainWindow", imgSisCirc); //Mostra imagem filtrando sistema circulatório
            }
            else if (c == 's') {


                imwrite("SkelFiltro.jpg", imgOriginal);  //salvar imagem de filtro escolhido

            }

           

            else

            break;


           
        }

        main = 'f';

    }

    destroyAllWindows();


    //Declarar variáveis de vídeo

    Mat image, imageGray;
    VideoCapture capture;
    Mat imageCinza, imageCanny, imageBlur, imageBlurCanny, imageDilate, imageErode; //def filtros da imagem, tratamento de cor
    Mat imageEqual, imageBilat;
 
    int v = 0;

    capture.open(0);
MENU:
    system("cls");
    cout << "Capture is opened" << endl;
    printf("\n\nMENU \n");
    printf("selecione o filtro: \n");
    printf(" 1 - sem filtro \n 2 - Escala de Cinza\n 3 - Blur\n 4 - Canny\n 5 -Blur Canny \n 6 - Sobel\n 7 - Equal\n 8 - Bilateral\n 9- Sair\n OBS: Para voltar ao menu clicar 'x'\n");
    cout << "Digite a opcao desejada aqui:  ";
    cin >> v;

    if (capture.isOpened())
    {
       
        for (;;)
        {
            {

                capture >> image;

                if (image.empty())
                    break;


                //sem filtro
                if (v == 1) {
                    drawText(image);
                    imshow("Webcam", image);

                    capture.read(image);

                    if(waitKey(30) == 's')
                    {
                       
                        image = image.clone();
                        imwrite("VideoCapture.jpg", image);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }
                // Filtro Gray
                else if (v == 2) {
                    cvtColor(image, imageGray, COLOR_BGR2GRAY);
                    drawText(imageGray);
                    imshow("Webcam", imageGray);

                    capture.read(imageGray);

                    if (waitKey(30) == 's')
                    {

                        imageGray = imageGray.clone();
                        imwrite("VideoCaptureGray.jpg", imageGray);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }
                //Filtro Blur
                else if (v == 3) {
                    GaussianBlur(image, imageBlur, Size(5, 5), 5, 0);
                    drawText(imageBlur);
                    imshow("Webcam", imageBlur);

                    capture.read(imageBlur);

                    if (waitKey(30) == 's')
                    {

                        imageBlur = imageBlur.clone();
                        imwrite("VideoCaptureBlur.jpg", imageBlur);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }

                //Filtro Canny
                else if (v == 4) {
                    Canny(image, imageCanny, 25, 75);  //Filtro Canny
                    drawText(imageCanny);
                    imshow("Webcam", imageCanny);

                    capture.read(imageCanny);

                    if (waitKey(30) == 's')
                    {

                        imageCanny = imageCanny.clone();
                        imwrite("VideoCaptureCanny.jpg", imageCanny);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }
                //Filtro BlurCanny
                else if (v == 5) {
                    Canny(image, imageCanny, 25, 75);
                    GaussianBlur(imageCanny, imageBlurCanny, Size(5, 5), 5, 0);
                    drawText(imageBlurCanny);
                    imshow("Webcam", imageBlurCanny);

                    capture.read(imageBlurCanny);

                    if (waitKey(30) == 's')
                    {

                        imageBlurCanny = imageBlurCanny.clone();
                        imwrite("VideoCaptureBlurCanny.jpg", imageBlurCanny);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }
                //Filtro Sobel
                else if (v == 6) {
                    Mat grad_x, grad_y, grad;
                    Sobel(image, grad_x, CV_16S, 1, 0, 1, 1, 0, BORDER_DEFAULT);//Processamento Sobel
                    Sobel(image, grad_y, CV_16S, 0, 1, 1, 1, 0, BORDER_DEFAULT);//Processamento Sobel
                    Mat abs_grad_x, abs_grad_y;//Processamento Sobel
                    convertScaleAbs(grad_x, abs_grad_x);//Processamento Sobel
                    convertScaleAbs(grad_y, abs_grad_y);//Processamento Sobel
                    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);//Processamento Sobel
                    drawText(grad);
                    imshow("Webcam", grad);

                    capture.read(grad);

                    if (waitKey(30) == 's')
                    {

                        grad = grad.clone();
                        imwrite("VideoCaptureSobel.jpg", grad);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }
                //Filtro equal
                else if (v == 7) {
                    cvtColor(image, imageGray, COLOR_BGR2GRAY);
                    equalizeHist(imageGray, imageEqual);
                    drawText(imageEqual);
                    imshow("Webcam", imageEqual);

                    capture.read(imageEqual);

                    if (waitKey(30) == 's')
                    {

                        imageEqual = imageEqual.clone();
                        imwrite("VideoCaptureEqual.jpg", imageEqual);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }

                //Filtro bilateral
                else if (v == 8) {
                    bilateralFilter(image, imageBilat, 15, 200, 150);//Filtro Bilateral
                    drawText(imageBilat);
                    imshow("Webcam", imageBilat);

                    capture.read(imageBilat);

                    if (waitKey(30) == 's')
                    {

                        imageBilat = imageBilat.clone();
                        imwrite("VideoCaptureBilateral.jpg", imageBilat);

                    }

                    if (waitKey(30) == 'x') {
                        destroyAllWindows();
                        goto MENU;
                    }
                }
                else if (v == 9) {
                    break;
                }

            }

        }
    }
    else
    {
        cout << "No capture" << endl;
        image = Mat::zeros(480, 640, CV_8UC1);
        drawText(image);
        imshow("Webcam", image);
        waitKey(0);
    }
    return 0;


}

void drawText(Mat& image)
{
    putText(image, "Hello OpenCV",
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, // font face and scale
        Scalar(255, 255, 255), // white
        1, LINE_AA); // line thickness and type
}

  
