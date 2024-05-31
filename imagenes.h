#ifndef IMAGENES_H
#define IMAGENES_H

#include <string>
#include <QImage>
#include <QString>
using namespace std;
#include <opencv2/opencv.hpp>
using namespace cv;

///////////////////////////////////////////////////////////////////
/////////  DEFINICIONES PUBLICAS                     //////////////
///////////////////////////////////////////////////////////////////

#define MAX_VENTANAS 100
// Número máximo de ventanas permitido

#define DESP_X_HIJO 70
#define DESP_Y_HIJO 50
// Desplazamiento en X e Y de las ventanas hijas respecto de la ventana principal


///////////////////////////////////////////////////////////////////
/////////  TIPOS DE DATOS PROPIOS                    //////////////
///////////////////////////////////////////////////////////////////

// STRUCT ventana
//    Representa la información asociada a una imagen que está siendo
//    editada actualmente en la aplicación

struct ventana {
    bool usada;          // Indica si se está usando actualmente esta posición
    bool modificada;     // Ha sido modificada después de abrirla o crearla
    string nombre;       // Nombre de la ventana codificada en UTF8
    string nombref;      // Nombre del fichero de imagen codificado en Latin1
    int orden;           // Número de orden entre las ventanas abiertas
    Rect roi;            // Región de interés seleccionada
    Mat img;             // Imagen almacenada en esta posición
};

// ENUM tipo_herramienta
//    Enumerado con los distintos tipos posibles de herramientas
//    que se pueden usar. Añadir nuevas aquí

enum tipo_herramienta {HER_PUNTO, HER_LINEA, HER_SELECCION, HER_RECTANGULO, HER_ELIPSE, HER_ARCO_IRIS,
                       HER_TRAZO, HER_RELLENADO, HER_SUAVIZADO, HER_COPIA};

// ENUM canales_histograma2D
//    Enumerado con todos los posibles pares de canales (3) para un histograma en 2D
enum canales_histograma2D {R_G, R_B, G_B};


///////////////////////////////////////////////////////////////////
/////////  VARIABLES PÚBLICAS                        //////////////
///////////////////////////////////////////////////////////////////

extern ventana foto[MAX_VENTANAS];
// El array foto guarda la información de todas las imágenes abiertas
// actualmente o que pueden ser abiertas, hasta un máximo de MAX_VENTANAS

extern tipo_herramienta herr_actual;
// Herramienta que se ha seleccionado actualmente

extern int radio_pincel;
// Radio actual para pintar puntos y líneas

extern Scalar color_pincel;
// Color actual del pincel

extern int difum_pincel;
// Difuminado actual del pincel

extern bool preguntar_guardar;
// Al cerrar una imagen modificada, preguntar si se guarda o no

extern bool isCopiado;
// Para saber si se ha copiado una foto o todavia no

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE MANEJO DE VENTANAS           //////////////
///////////////////////////////////////////////////////////////////

void inic_fotos (void);
// Inicializa el array foto, de imágenes abiertas actualmente

void fin_fotos (void);
// Libera las imágenes del array foto

int primera_libre (void);
// Busca la primera entrada libre del array foto

void crear_nueva (int nfoto, int ancho, int alto, Scalar color);
// Crea una nueva entrada, en la posición nfoto, con el ancho y el alto dados,
// de profundidad CV_8U y 3 canales. color indica el color de fondo

void crear_nueva (int nfoto, Mat img);
// Crea una nueva entrada, en la posición nfoto, usando la imagen dada en img

void crear_nueva (int nfoto, string nombre);
// Crea una nueva entrada, en la posición nfoto, leyéndola del archivo nombre.
// Si no se puede leer, no hace nada

void mostrar (int nfoto);
// Muestra la imagen de la entrada nfoto en la ventana correspondiente

void guardar_foto (int nfoto, string nombre= "");
// Guarda en disco la imagen de nfoto, con el nombre dado. Si el nombre
// es "", usa el nombre de la imagen en el array foto

void cerrar_foto (int nfoto, bool destruir_ventana= true);
// Elimina la entrada en la posición nfoto. destruir_ventana indica si debe
// destruirse la ventana de HighGUI (ya que si lo ha hecho el usuario, no
// debe volver a hacerse)

int nombre_a_numero (string nombre);
// Dado un nombre de ventana, busca su número en el array foto.
// Si no lo encuentra devuelve -1

int foto_activa (void);
// Devuelve el número de la ventana de imagen activa (la que esté en primer
// lugar). Si no hay ninguna devuelve -1

int num_fotos (int &usadas, int &modificadas);
// Devuelve el número de fotos que hay abiertas y modificadas actualmente

void set_callback_foto (int nfoto, bool activo);
// Activa y desactiva el callback por defecto de una ventana,
// según el valor del parámetro activo

void nueva_desde_portapaples(int nfoto, QImage& imagen);
// Abrir la imagen 'imagen' guardada en el portapapeles -> Convertir QImage a Mat

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE IMAGENES    //////////////
///////////////////////////////////////////////////////////////////

void invertir (int nfoto, int nres);
// Invierte la imagen contenida en nfoto y la almacena en nres, que será una
// nueva ventana

void rotar_angulo (Mat imagen, Mat &imgRes, double angulo, double escala= 1.0, int modo= 0);
// Rota la imagen contenida en imagen, en la cantidad grado (grados
// de 0 a 360) y almacena el resultado en imgRes. La imagen de salida
// siempre está centrada.
// El parámetro modo indica cómo se calcula el tamaño de la imagen resultante:
//    0 = El mismo que la original
//    1 = Tamaño máximo (caber todo), no se pierden los picos de la imagen original
//    2 = Tamaño mínimo (sin bordes), no aparece fondo negro en el resultado

void rotar_exacto (int nfoto, int nres, int grado);
// Rota la imagen contenida en nfoto, en la cantidad 90*grado
// y la almacena en nres. Se trata siempre de una rotación exacta

void ver_brillo_contraste_gama (int nfoto, double suma, double prod, double gama,
                           bool guardar= false);
// Modifica la imagen en nfoto, tomando para cada píxel A(x,y) el valor:
// A(x,y)*prod + suma. Si guardar==true se guarda la imagen, y en otro caso
// solo se visualiza el resultado, pero sin guardar

void escala_color(int nfoto, int nres);
// Transformacion de escala de color usando el color_pincel

Mat op_pinchar_estirar(Mat img, int cx, int cy, double radio, double grado);

void ver_pinchar_estirar(int nfoto, int cx, int cy, double radio, double grado, bool guardar = false);
// Transformacion de pinchar/estirar

void ver_suavizado (int nfoto, int ntipo, int tamx, int tamy,
                    bool guardar= false);
// Modifica la imagen en nfoto, aplicando un suavizado de tamaño dado. ntipo
// puede ser: 1=GAUSIANO; 2=MEDIA. Si guardar==true se guarda la imagen, y
// en otro caso solo se visualiza el resultado, pero sin guardar

void ver_perspectiva(int nfoto_origen, int nfoto_destino, Point2f pt_origen[], Point2f pt_destino[], bool guardar = false);
// Aplicar una transformacion perspectiva

void ver_bajorrelieve(int nfoto, int nres, double angulo, double grado, int nfondo, bool guardar = false);


void ver_histograma(int nfoto, int canal, int nres);
// Mostrar el histograma de la imagen nfoto del canal 'canal',
// almacenando el resulado en nres
// canal 0 -> Azul; canal 1 -> Verde, canal 2 -> Rojo, canal 3 -> gris

void copiar_a_nueva(int nfoto, int nres);
// Copia el ROI de la imagen nfoto, creando una nueva
// en nres

QImage copiar_al_portapapeles(int nfoto);
// Copia el ROI de la imagen nfoto, convirtiendola en QImage
// para poder copiar en el portapapeles

QVector<QString> ver_informacion(int nfoto);
// Devuelve informacion de la foto activa (ancho, alto, profundidad, canales, memoria, media RGB,...)

void ver_ajuste_lineal(int nfoto, double pmin, double pmax, bool guardar = false);
// Operacion de ajuste lineal del histograma con percentiles
// pmin y pmax

Mat op_mat_sat_lum(Mat img, double sat, double lum, int matiz);

void ver_mat_sat_lum(int nfoto, double sat, double lum, int matiz, bool guardar = false);
// Aplica la transformacion de matiz/saturacion/luminosidad

void ajustar_rojo_verde_azul(int nfoto, double* producto, int* suma, int num_canales, bool guardar = false);
// A cada uno de los tres canales de la foto (RGB), lo multiplica por una cierta cantidad y le suma otra
// de forma independiente

void rotar_y_reescalar(int nfoto, double angulo, double escala, bool guardar = false);
// Rotar la imagen 'nfoto' una cierta cantidad de grados y reescalar la imagen en relacion
// a la escala dada

void convertir_a_falso(int nfoto, int colorMap, bool guardar = false);
// Convertir la imagen actual a una paleta de color falso seleccionada por el usuario

void cambiar_modelo_color(int nfoto, int nres, int modelo);
// Cambia el modelo de color de la foto original (BGR) a un nuevo modelo de color indicado por el parametro 'modelo'
// Se crea una nueva foto con ese modelo

void ver_espectro(int nfoto, int nres);
// Calcular el espectro de intensidad de la foto activa (magnitud de la transformada de Fourier)

void ver_perfilado(int nfoto, int radio, double porcentaje, bool guardar = false);
// Aplica la convulución de perfilado sobre la imagen activa. El radio indica el tamaño del kernel (matriz de convolucion)
// y el porcentaje sirve como ponderacion entre la imagen actual y la laplaciana

void ponerTextoEnImagen(int nfoto, string texto, int tam, Scalar color, int x, int y, bool guardar = false);
// Añade a la foto actual un texto con un color dado, y aplicando un efecto de sobra y semitransparencia.
// El texto se coloca en la posicion (x, y)

Mat aplicarMorfologia(Mat &entrada, int nres, int operacion, int tam, int iteraciones, bool guardar = false);
// Aplicar la operacion morfologica dada a la imagen de entrada usando un kernel con un tamaño dado
// y con las iteraciones indicadas. Devuelve la imagen modificada

void balanceBlancos(int nfoto, int nres);
// Aplica el algoritmo Gray World a la foto activa para ajustar el balance de blancos
// Se crea una nueva foto con el ajusta hecho

void aplicarInpaint(int nfoto, int nres, Mat &mascara, int radio);
// Aplicar el algoritmo de inpaint a la foto activa, usando una mascara con los cambios indicados
// El resultado se guarda en una nueva foto

void ver_histograma2D(int nfoto, int nres, int celdas, canales_histograma2D canales, bool guardar = false);
// Mostrar el histograma de dos canales (R-G, R-B, G-B) indicando el numero de celdas (bins).
// Guardar el resultado cuando guardar sea igual a true

void ecualizar_histograma_independiente(int nfoto, int nres);
// Ecualiza el histograma ecualizando cada canal de la foto de forma independiente

void ecualizar_histograma_conjunta(int nfoto, int nres);
// Ecualiza el histograma de la foto de forma conjunta. Para ello, se convierta la imagen a grises, se calcula
// su histograma y se normaliza y, por ultimo, se utiliza una tabla LUT

void ecualizar_histograma_local(int nfoto, int nres, int radio, bool guardar = false);
// Por cada pixel de la foto se obtiene una region con un radio determinado. Se calcula su histograma y se obtiene
// el percentil del valor del pixel A(x,y). El valor del nuevo pixel es 255*percentil.
// Importante: Trabajar en el espacio de color Lab con el canal L (luminosidad)

void ecualizar_histograma_local_CLAHE(int nfoto, int nres, int radio, bool guardar = false);
// Aplicar el algoritmo CLAHE para ecualizar el histograma de forma local

void ver_convolucion(int nfoto, int nres, Mat M, double mult, double suma, bool guardar = false);
// Transformacion de convolucion

void media_ponderada (int nf1, int nf2, int nueva, double peso);
// Calcula la media ponderada entre la imagen nf1 y la nf2, según el peso dado,
// y almacena el resultado en la posición nueva. Se aplica la fórmula:
// nueva(x,y):= peso*nf1(x,y)+(1-peso)*nf2(x,y). La imagen nf2 se reescala al
// tamaño de nf1

string Lt1(string cadena);
// Convertir una cadena de UTF8 a Latin1

// FUNCIONES AUXILIARES

void centrarFourier(Mat &fourier);
// Centrar la transformadad de Fourier. Consiste en dividir la imagen en 4 cuadrantes (rectangulos), e intercambiar los
// cuadrantes diagonalmente

double getEscalaTexto(string texto, int tam);
// Obtener la escala del texto a partir del tamaño en pixeles que se quiere que ocupe

void calcularMediaLAB(Mat &imagen, double &media_a, double &media_b);
// Calcula la media de los canales a y b de una imagen en el espacio de color Lab

void desplazarCanalesLAB(Mat &imagen, double des_a, double des_b);
// Desplaza los valores del canal a y b una determinada cantidad

#endif // IMAGENES_H
