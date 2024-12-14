#include <freeglut.h>
#include <string>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846
using namespace std;

int mainWindow = -1, secondaryWindow = -1;
bool isSecondaryWindowActive = false;


string inputText = "";

struct Point {
    float x, y;
};

vector<Point> trianglePoints;
bool isDrawingTriangle = false;
bool isTriangleComplete = false;

float transformationMatrix[3][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};

string inputTextScalingX = "";
string inputTextScalingY = "";



bool isScalingInputActive1 = false;
bool isScalingInputActive2 = false;

bool isInputActive1 = false;
bool isInputActive2 = false;

const int MAX_INPUT_LENGTH = 3;


const float TEXT_AREA_LEFT = 40.0f;
const float TEXT_AREA_RIGHT = 90.0f;
const float TEXT_AREA_TOP = 90.0f;
const float TEXT_AREA_BOTTOM = 85.0f;

// Forward declarations
void rotateTransform();
void scaleTransform();  
void translateTransform();
void shearTransform();
void reflectTransform();
void handleMainMenuKeyboard(unsigned char key, int x, int y);
void handleRotateWindowKeyboard(unsigned char key, int x, int y);
void rotateHandleMouse(int button, int state, int x, int y);
void handleShearWindowKeyboard(unsigned char key, int x, int y);
void handleReflectWindowKeyboard(unsigned char key, int x, int y);
void shearHandleMouse(int button, int state, int x, int y);
void reflectHandleMouse(int button, int state, int x, int y);
void displayShear();
void displayReflect();
void displayMainMenu();
void displayRotate();
void reshapeWindow(int w, int h);
void drawTriangle();
void applyTranslation();
void applyRotation();
void displayRotate();


void windowToOpenGL(int x, int y, float& glX, float& glY) {
    glX = (float)x / 500.0f * 100.0f - 100.0f;  
    glY = 100.0f - (float)y / 500.0f * 100.0f;  
}

void drawTextBold(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void displayMainMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawTextBold(-90, 90, "Main Menu");
    drawText(-90, 85, "Press 'r' for rotate window");
    drawText(-90, 80, "Press 's' for scale window");
    drawText(-90, 75, "Press 't' for translate window");
    drawText(-90, 70, "Press 'h' for shear window");
    drawText(-90, 65, "Press 'f' for reflect window");
    drawText(-90, 60, "Press 'q' to quit");
    drawTextBold(45, -90, "Love Dewangan_500109339");
    glutSwapBuffers();
}

void drawTriangle() {
    
    glColor3f(0.0f, 1.0f, 0.0f); 
    glBegin(GL_TRIANGLES);
    for (const auto& point : trianglePoints) {
        glVertex2f(point.x, point.y);
    }
    glEnd();

    
    
}

void displayMatrix(float matrix[3][3], float x, float y) {
    string matrixRows[3];
    char buffer[32];


    for (int i = 0; i < 3; i++) {
        matrixRows[i] = "[ ";

        for (int j = 0; j < 3; j++) {
            snprintf(buffer, sizeof(buffer), "%.2f ", matrix[i][j]);
            matrixRows[i] += buffer;
        }
        matrixRows[i] += "]";
    }


    glColor3f(1.0f, 1.0f, 0.0f);
    drawText(x, y + 10, "Translation Matrix:");


    for (int i = 0; i < 3; i++) {
        drawText(x, y - i * 5, matrixRows[i]);
    }
}


/*TRANSLATE TRANSFORMATION*/

void applyTranslation() {
    
    float tx = stof(inputTextScalingX);
    float ty = stof(inputTextScalingY);

    //UPDATION OF TRANSFORMATION ACCORDING TO TRANSLATION
    transformationMatrix[0][2] = tx;
    transformationMatrix[1][2] = ty;

    
    for (auto& point : trianglePoints) {
        
        float x = point.x + tx;
        float y = point.y + ty;
        point.x = x;
        point.y = y;
    }
}



void displayPositionMatrix(const vector<Point>& points, float x, float y) {
    string matrixRows[3];
    char buffer[32];

    // Draw matrix title
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for matrix
    drawText(x, y + 10, "Position of Triangle Matrix:");

    if (points.size() == 3) {

        // Format first row (x coordinates)
        snprintf(buffer, sizeof(buffer), "[ %.2f  %.2f  %.2f ]",
            points[0].x, points[1].x, points[2].x);
        matrixRows[0] = buffer;

        // Format second row (y coordinates)
        snprintf(buffer, sizeof(buffer), "[ %.2f  %.2f  %.2f ]",
            points[0].y, points[1].y, points[2].y);
        matrixRows[1] = buffer;

        // Format third row (homogeneous coordinates)
        matrixRows[2] = "[  1.00   1.00   1.00 ]";

        
        for (int i = 0; i < 3; i++) {
            drawText(x, y - i * 5, matrixRows[i]);
        }

    }
    else {
        // If triangle is not complete, show empty matrix format
        drawText(x, y, "[  -     -     -  ]");
        drawText(x, y - 5, "[  -     -     -  ]");
        drawText(x, y - 10, "[  1     1     1  ]");
    }
}



void displayTranslate() {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    
    drawTextBold(-90, 90, "Translating Window");
    drawText(-90, 85, "Press ESC to return to main menu");
    drawText(-90, 75, "Click three points to draw a triangle");
    drawText(-90, 70, "Enter translation values and press Enter to translate");
    drawTextBold(-90, -95, "Love Dewangan_500109339");

    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 3, "X Axis:");
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 13, "Y Axis:");

    
    if (isInputActive1) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    //Input Box for X Axis
    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM);
    glEnd();

    //Input Box for Y Axis
    if (isInputActive2) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP - 10);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP - 10);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM - 10);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM - 10);
    glEnd();

   
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 3,
        inputTextScalingX + (isInputActive1 ? "_" : ""));
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 13,
        inputTextScalingY + (isInputActive2 ? "_" : ""));

    
    if (!trianglePoints.empty()) {
        drawTriangle();
    }

    
    displayMatrix(transformationMatrix, 20, -70);
    displayPositionMatrix(trianglePoints, -90, -70);

    
    glColor3f(0.7f, 0.7f, 0.7f); 
    drawText(20, -85, "Where:");
    drawText(20, -90, "tx = translation in x-direction");
    drawText(20, -95, "ty = translation in y-direction");

    glutSwapBuffers();
}


void applyScaling() {
    float sx = stof(inputTextScalingX);
    float sy = stof(inputTextScalingY);

    //UPDATION OF TRANSFORMATION ACCORDING TO SCALING
    transformationMatrix[0][0] = sx;
    transformationMatrix[1][1] = sy;

    
    for (auto& point : trianglePoints) {
        float x = point.x * sx;
        float y = point.y * sy;
        point.x = x;
        point.y = y;
    }
}

/*SCALING TRANSFORMATION*/

void displayScale() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    
    drawTextBold(-90, 90, "Scaling Window");
    drawText(-90, 85, "Press ESC to return to main menu");
    drawText(-90, 75, "Click three points to draw a triangle");
    drawText(-90, 70, "Enter scaling factors and press Enter to scale");
    drawTextBold(-90, -95, "Love Dewangan_500109339");
    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 3, "X Scale:");
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 13, "Y Scale:");

    
    if (isScalingInputActive1) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    
    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM);
    glEnd();

    
    if (isScalingInputActive2) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP - 10);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP - 10);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM - 10);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM - 10);
    glEnd();

    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 3,
        inputTextScalingX + (isScalingInputActive1 ? "_" : ""));
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 13,
        inputTextScalingY + (isScalingInputActive2 ? "_" : ""));

    
    if (!trianglePoints.empty()) {
        drawTriangle();
    }

    
    displayMatrix(transformationMatrix, 20, -70);
    displayPositionMatrix(trianglePoints, -90, -70);

    
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(20, -85, "Where:");
    drawText(20, -90, "sx = scaling factor in x-direction");
    drawText(20, -95, "sy = scaling factor in y-direction");

    glutSwapBuffers();
}

/*ROTATION TRANSFORMATION*/

void applyRotation() {
    if (inputText.empty()) return;


    float angle = stof(inputText) * M_PI / 180.0f;

    //UPDATION OF TRANSFORMATION ACCORDING TO ROTATION
    transformationMatrix[0][0] = cos(angle);
    transformationMatrix[0][1] = -sin(angle);
    transformationMatrix[1][0] = sin(angle);
    transformationMatrix[1][1] = cos(angle);

    
    float centerX = 0, centerY = 0;
    for (const auto& point : trianglePoints) {
        centerX += point.x;
        centerY += point.y;
    }
    centerX /= trianglePoints.size();
    centerY /= trianglePoints.size();

    
    for (auto& point : trianglePoints) {
        
        float x = point.x - centerX;
        float y = point.y - centerY;

        
        float rotatedX = x * cos(angle) - y * sin(angle);
        float rotatedY = x * sin(angle) + y * cos(angle);

        
        point.x = rotatedX + centerX;
        point.y = rotatedY + centerY;
    }
}

void displayRotate() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    
    drawTextBold(-90, 90, "Rotating Window");
    drawText(-90, 85, "Press ESC to return to main menu");
    drawText(-90, 75, "Click three points to draw a triangle");
    drawText(-90, 70, "Enter angle in degrees and press Enter to rotate");
    drawText(-90, 65, "Positive angle rotates Anticlockwise");
    drawText(-90, 60, "Negative angle rotates Clockwise");
    drawTextBold(-90, -95, "Love Dewangan_500109339");
    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 3, "Angle:");

    
    if (isInputActive1) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    
    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM);
    glEnd();

    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 3,
        inputText + (isInputActive1 ? "_" : ""));

    
    if (!trianglePoints.empty()) {
        drawTriangle();
    }

    
    displayMatrix(transformationMatrix, 20, -70);
    displayPositionMatrix(trianglePoints, -90, -70);

    

    glutSwapBuffers();
}

/*REFLECTION TRANSFORMATION*/

void applyReflection() {
    
    if (inputText.empty()) return;

    
    char axis = tolower(inputText[0]);

    //UPDATION OF TRANSFORMATION ACCORDING TO REFLECTION
    if (axis == 'x') {
        
        transformationMatrix[0][0] = 1;   
        transformationMatrix[1][1] = -1;  
    }
    else if (axis == 'y') {
        
        transformationMatrix[0][0] = -1;  
        transformationMatrix[1][1] = 1;   
    }
    else if (axis == 'o') {
        
        transformationMatrix[0][0] = -1;
        transformationMatrix[1][1] = -1;
    }
    else {
        return; 
    }

    
    for (auto& point : trianglePoints) {
        if (axis == 'x') {
            point.y = -point.y;  
        }
        else if (axis == 'y') {
            point.x = -point.x;  
        }
        else if (axis == 'o') {
            point.x = -point.x;  
            point.y = -point.y;
        }
    }
}

void displayReflect() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    
    drawTextBold(-90, 90, "Reflection Window");
    drawText(-90, 85, "Press ESC to return to main menu");
    drawText(-90, 75, "Click three points to draw a triangle");
    drawText(-90, 70, "Enter x for X-axis reflection");
    drawText(-90, 65, "Enter y for Y-axis reflection");
    drawText(-90, 60, "Enter o for Origin reflection");
    drawTextBold(-90, -95, "Love Dewangan_500109339");
    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 3, "Axis:");

    
    if (isInputActive1) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM);
    glEnd();

    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 3,
        inputText + (isInputActive1 ? "_" : ""));

    
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    
    glVertex2f(-90.0f, 0.0f);
    glVertex2f(90.0f, 0.0f);
    
    glVertex2f(0.0f, -90.0f);
    glVertex2f(0.0f, 90.0f);
    glEnd();

    
    if (!trianglePoints.empty()) {
        drawTriangle();
    }

    
    displayMatrix(transformationMatrix, 20, -70);
    displayPositionMatrix(trianglePoints, -90, -70);

    glutSwapBuffers();
}

/*SHEARING TRANSFORMATION*/

void applyShearing() {
    float shx = stof(inputTextScalingX);  
    float shy = stof(inputTextScalingY);  

    //UPDATION OF TRANSFORMATION ACCORDING TO SHEARING
    transformationMatrix[0][1] = shx;  
    transformationMatrix[1][0] = shy;  

    
    float centerX = 0, centerY = 0;
    for (const auto& point : trianglePoints) {

        centerX += point.x;
        centerY += point.y;
    }
    centerX /= trianglePoints.size();
    centerY /= trianglePoints.size();

    
    for (auto& point : trianglePoints) {
        
        float x = point.x - centerX;
        float y = point.y - centerY;

        
        float newX = x + shx * y;
        float newY = shy * x + y;

        
        point.x = newX + centerX;
        point.y = newY + centerY;
    }
}

void displayShear() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    
    drawTextBold(-90, 90, "Shearing Window");
    drawText(-90, 85, "Press ESC to return to main menu");
    drawText(-90, 75, "Click three points to draw a triangle");
    drawText(-90, 70, "Enter shear factors and press Enter to shear");
    drawTextBold(-90, -95, "Love Dewangan_500109339");
    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 3, "X Shear:");
    drawText(TEXT_AREA_LEFT - 35, TEXT_AREA_TOP - 13, "Y Shear:");

    
    if (isInputActive1) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    
    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM);
    glEnd();

    
    if (isInputActive2) {
        glColor3f(0.2f, 0.2f, 0.3f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    glBegin(GL_QUADS);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_TOP - 10);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_TOP - 10);
    glVertex2f(TEXT_AREA_RIGHT, TEXT_AREA_BOTTOM - 10);
    glVertex2f(TEXT_AREA_LEFT, TEXT_AREA_BOTTOM - 10);
    glEnd();

    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 3,
        inputTextScalingX + (isInputActive1 ? "_" : ""));
    drawText(TEXT_AREA_LEFT + 3, TEXT_AREA_TOP - 13,
        inputTextScalingY + (isInputActive2 ? "_" : ""));

    
    if (!trianglePoints.empty()) {
        drawTriangle();
    }

    
    displayMatrix(transformationMatrix, 20, -70);
    displayPositionMatrix(trianglePoints, -90, -70);

    
    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(20, -85, "Where:");
    drawText(20, -90, "shx = shear factor in x-direction");
    drawText(20, -95, "shy = shear factor in y-direction");

    glutSwapBuffers();
}








void reshapeWindow(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0f, 100.0f, -100.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void handleMainMenuKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 't':
    case 'T':
        translateTransform();
        break;
    case 's':
    case 'S':
        scaleTransform();
        break;
    case 'r':
    case 'R':
        rotateTransform();
        break;
    case 'h':
    case 'H':
        shearTransform();
        break;
    case 'f':
    case 'F':
        reflectTransform();
        break;
    case 'q':
    case 'Q':
        exit(0);
        break;
    }
}

void handleTranslateWindowKeyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC key
        glutDestroyWindow(secondaryWindow);
        secondaryWindow = -1;
        mainWindow = glutCreateWindow("Main Menu");
        glutDisplayFunc(displayMainMenu);
        glutReshapeFunc(reshapeWindow);
        glutKeyboardFunc(handleMainMenuKeyboard);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glutReshapeWindow(1000, 1000);
        isInputActive1 = false;
        isInputActive2 = false;
        trianglePoints.clear();
        isTriangleComplete = false;
        return;
    }

    if (key == 13 && isTriangleComplete) { // Enter key
        if (!inputTextScalingX.empty() && !inputTextScalingY.empty()) {
            applyTranslation();
            inputTextScalingX = "";
            inputTextScalingY = "";
            isInputActive1 = false;
            isInputActive2 = false;
            glutPostRedisplay();
        }
        return;
    }

    // Handle text input for X coordinate
    if (isInputActive1) {
        if (key == 8) { // Backspace
            if (!inputTextScalingX.empty()) {
                inputTextScalingX.pop_back();
            }
        }
        else if ((key >= '0' && key <= '9') || (key == '-' && inputTextScalingX.empty()) ||
            (key == '.' && inputTextScalingX.find('.') == string::npos)) {
            if (inputTextScalingX.length() < MAX_INPUT_LENGTH) {
                inputTextScalingX += key;
            }
        }
        glutPostRedisplay();
    }

    // Handle text input for Y coordinate
    if (isInputActive2) {
        if (key == 8) { // Backspace
            if (!inputTextScalingY.empty()) {
                inputTextScalingY.pop_back();
            }
        }
        else if ((key >= '0' && key <= '9') || (key == '-' && inputTextScalingY.empty()) ||
            (key == '.' && inputTextScalingY.find('.') == string::npos)) {
            if (inputTextScalingY.length() < MAX_INPUT_LENGTH) {
                inputTextScalingY += key;
            }
        }
        glutPostRedisplay();
    }
}

void handleScaleWindowKeyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC key
        glutDestroyWindow(secondaryWindow);
        secondaryWindow = -1;
        mainWindow = glutCreateWindow("Main Menu");
        glutDisplayFunc(displayMainMenu);
        glutReshapeFunc(reshapeWindow);
        glutKeyboardFunc(handleMainMenuKeyboard);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glutReshapeWindow(1000, 1000);
        isScalingInputActive1 = false;
        isScalingInputActive2 = false;
        trianglePoints.clear();
        isTriangleComplete = false;
        return;
    }

    if (key == 13 && isTriangleComplete) { // Enter key
        if (!inputTextScalingX.empty() && !inputTextScalingY.empty()) {
            applyScaling();
            inputTextScalingX = "";
            inputTextScalingY = "";
            isScalingInputActive1 = false;
            isScalingInputActive2 = false;
            glutPostRedisplay();
        }
        return;
    }

    // Handle text input for X coordinate
    if (isScalingInputActive1) {
        if (key == 8) { // Backspace
            if (!inputTextScalingX.empty()) {
                inputTextScalingX.pop_back();
            }
        }
        else if ((key >= '0' && key <= '9') ||
            (key == '.' && inputTextScalingX.find('.') == string::npos)) {
            if (inputTextScalingX.length() < MAX_INPUT_LENGTH) {
                inputTextScalingX += key;
            }
        }
        glutPostRedisplay();
    }

    // Handle text input for Y coordinate
    if (isScalingInputActive2) {
        if (key == 8) { // Backspace
            if (!inputTextScalingY.empty()) {
                inputTextScalingY.pop_back();
            }
        }
        else if ((key >= '0' && key <= '9') ||
            (key == '.' && inputTextScalingY.find('.') == string::npos)) {
            if (inputTextScalingY.length() < MAX_INPUT_LENGTH) {
                inputTextScalingY += key;
            }
        }
        glutPostRedisplay();
    }
}

void handleRotateWindowKeyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC key
        glutDestroyWindow(secondaryWindow);
        secondaryWindow = -1;
        mainWindow = glutCreateWindow("Main Menu");
        glutDisplayFunc(displayMainMenu);
        glutReshapeFunc(reshapeWindow);
        glutKeyboardFunc(handleMainMenuKeyboard);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glutReshapeWindow(1000, 1000);
        isInputActive1 = false;
        trianglePoints.clear();
        isTriangleComplete = false;
        return;
    }

    if (key == 13 && isTriangleComplete) { // Enter key
        if (!inputText.empty()) {
            applyRotation();
            inputText = "";
            isInputActive1 = false;
            glutPostRedisplay();
        }
        return;
    }

    // Handle text input for angle
    if (isInputActive1) {
        if (key == 8) { // Backspace
            if (!inputText.empty()) {
                inputText.pop_back();
            }
        }
        else if ((key >= '0' && key <= '9') ||
            (key == '-' && inputText.empty()) ||
            (key == '.' && inputText.find('.') == string::npos)) {
            if (inputText.length() < MAX_INPUT_LENGTH) {
                inputText += key;
            }
        }
        glutPostRedisplay();
    }
}

void handleReflectWindowKeyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC key
        glutDestroyWindow(secondaryWindow);
        secondaryWindow = -1;
        mainWindow = glutCreateWindow("Main Menu");
        glutDisplayFunc(displayMainMenu);
        glutReshapeFunc(reshapeWindow);
        glutKeyboardFunc(handleMainMenuKeyboard);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glutReshapeWindow(1000, 1000);
        isInputActive1 = false;
        trianglePoints.clear();
        isTriangleComplete = false;
        return;
    }

    if (key == 13 && isTriangleComplete) { // Enter key
        if (!inputText.empty()) {
            applyReflection();
            inputText = "";
            isInputActive1 = false;
            glutPostRedisplay();
        }
        return;
    }

    // Handle text input
    if (isInputActive1) {
        if (key == 8) { // Backspace
            if (!inputText.empty()) {
                inputText.pop_back();
            }
        }
        else if (tolower(key) == 'x' || tolower(key) == 'y' || tolower(key) == 'o') {
            if (inputText.empty()) {  // Only allow one character
                inputText += tolower(key);
            }
        }
        glutPostRedisplay();
    }
}

void handleShearWindowKeyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC key
        glutDestroyWindow(secondaryWindow);
        secondaryWindow = -1;
        mainWindow = glutCreateWindow("Main Menu");
        glutDisplayFunc(displayMainMenu);
        glutReshapeFunc(reshapeWindow);
        glutKeyboardFunc(handleMainMenuKeyboard);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glutReshapeWindow(1000, 1000);
        isInputActive1 = false;
        isInputActive2 = false;
        trianglePoints.clear();
        isTriangleComplete = false;
        return;
    }

    if (key == 13 && isTriangleComplete) { // Enter key
        if (!inputTextScalingX.empty() && !inputTextScalingY.empty()) {
            applyShearing();
            inputTextScalingX = "";
            inputTextScalingY = "";
            isInputActive1 = false;
            isInputActive2 = false;
            glutPostRedisplay();
        }
        return;
    }

    // Handle text input for X shear
    if (isInputActive1) {
        if (key == 8) { // Backspace
            if (!inputTextScalingX.empty()) {
                inputTextScalingX.pop_back();
            }
        }
        else if ((key >= '0' && key <= '9') ||
            (key == '-' && inputTextScalingX.empty()) ||
            (key == '.' && inputTextScalingX.find('.') == string::npos)) {
            if (inputTextScalingX.length() < MAX_INPUT_LENGTH) {
                inputTextScalingX += key;
            }
        }
        glutPostRedisplay();
    }

    // Handle text input for Y shear
    if (isInputActive2) {
        if (key == 8) { // Backspace
            if (!inputTextScalingY.empty()) {
                inputTextScalingY.pop_back();
            }
        }
        else if ((key >= '0' && key <= '9') ||
            (key == '-' && inputTextScalingY.empty()) ||
            (key == '.' && inputTextScalingY.find('.') == string::npos)) {
            if (inputTextScalingY.length() < MAX_INPUT_LENGTH) {
                inputTextScalingY += key;
            }
        }
        glutPostRedisplay();
    }
}


void translateHandleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float glX, glY;
        windowToOpenGL(x, y, glX, glY);

      
        if (glX >= TEXT_AREA_LEFT && glX <= TEXT_AREA_RIGHT) {
            if (glY >= TEXT_AREA_BOTTOM && glY <= TEXT_AREA_TOP) {
                isInputActive1 = !isInputActive1;
                isInputActive2 = false;
                glutPostRedisplay();
                return;
            }
            else if (glY >= TEXT_AREA_BOTTOM - 10 && glY <= TEXT_AREA_TOP - 10) {
                isInputActive2 = !isInputActive2;
                isInputActive1 = false;
                glutPostRedisplay();
                return;
            }
        }

        // Handle triangle drawing
        if (!isTriangleComplete) {
            Point newPoint = { glX, glY };
            trianglePoints.push_back(newPoint);

            if (trianglePoints.size() == 3) {
                isTriangleComplete = true;
            }
            glutPostRedisplay();
        }
    }
}



void scaleHandleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float glX, glY;
        windowToOpenGL(x, y, glX, glY);

        if (glX >= TEXT_AREA_LEFT && glX <= TEXT_AREA_RIGHT) {
            if (glY >= TEXT_AREA_BOTTOM && glY <= TEXT_AREA_TOP) {
                isScalingInputActive1 = !isScalingInputActive1;
                isScalingInputActive2 = false;
                glutPostRedisplay();
                return;
            }
            else if (glY >= TEXT_AREA_BOTTOM - 10 && glY <= TEXT_AREA_TOP - 10) {
                isScalingInputActive2 = !isScalingInputActive2;
                isScalingInputActive1 = false;
                glutPostRedisplay();
                return;
            }
        }

        // Handle triangle drawing
        if (!isTriangleComplete) {
            Point newPoint = { glX, glY };
            trianglePoints.push_back(newPoint);

            if (trianglePoints.size() == 3) {
                isTriangleComplete = true;
            }
            glutPostRedisplay();
        }
    }
}

void rotateHandleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float glX, glY;
        windowToOpenGL(x, y, glX, glY);

        // Check if click is within text input area
        if (glX >= TEXT_AREA_LEFT && glX <= TEXT_AREA_RIGHT &&
            glY >= TEXT_AREA_BOTTOM && glY <= TEXT_AREA_TOP) {
            isInputActive1 = !isInputActive1;
            glutPostRedisplay();
            return;
        }

        // Handle triangle drawing
        if (!isTriangleComplete) {
            Point newPoint = { glX, glY };
            trianglePoints.push_back(newPoint);

            if (trianglePoints.size() == 3) {
                isTriangleComplete = true;
            }
            glutPostRedisplay();
        }
    }
}

void reflectHandleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float glX, glY;
        windowToOpenGL(x, y, glX, glY);

        // Check if click is within text input area
        if (glX >= TEXT_AREA_LEFT && glX <= TEXT_AREA_RIGHT &&
            glY >= TEXT_AREA_BOTTOM && glY <= TEXT_AREA_TOP) {
            isInputActive1 = !isInputActive1;
            glutPostRedisplay();
            return;
        }

        // Handle triangle drawing
        if (!isTriangleComplete) {
            Point newPoint = { glX, glY };
            trianglePoints.push_back(newPoint);

            if (trianglePoints.size() == 3) {
                isTriangleComplete = true;
            }
            glutPostRedisplay();
        }
    }
}


void shearHandleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float glX, glY;
        windowToOpenGL(x, y, glX, glY);

        
        if (glX >= TEXT_AREA_LEFT && glX <= TEXT_AREA_RIGHT) {
            if (glY >= TEXT_AREA_BOTTOM && glY <= TEXT_AREA_TOP) {
                isInputActive1 = !isInputActive1;
                isInputActive2 = false;
                glutPostRedisplay();
                return;
            }
            else if (glY >= TEXT_AREA_BOTTOM - 10 && glY <= TEXT_AREA_TOP - 10) {
                isInputActive2 = !isInputActive2;
                isInputActive1 = false;
                glutPostRedisplay();
                return;
            }
        }

        
        if (!isTriangleComplete) {
            Point newPoint = { glX, glY };
            trianglePoints.push_back(newPoint);

            if (trianglePoints.size() == 3) {
                isTriangleComplete = true;
            }
            glutPostRedisplay();
        }
    }
}




void translateTransform() 
{

    glutDestroyWindow(mainWindow);
    mainWindow = -1;
    secondaryWindow = glutCreateWindow("Translating Window");
    glutDisplayFunc(displayTranslate);
    glutReshapeFunc(reshapeWindow);

    glutKeyboardFunc(handleTranslateWindowKeyboard);
    glutMouseFunc(translateHandleMouse);  
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glutReshapeWindow(1000, 1000);

}


void scaleTransform()
{
    glutDestroyWindow(mainWindow);
    mainWindow = -1;
    secondaryWindow = glutCreateWindow("Scaling Window");
    glutDisplayFunc(displayScale);
    glutReshapeFunc(reshapeWindow);

    glutKeyboardFunc(handleScaleWindowKeyboard);
    glutMouseFunc(scaleHandleMouse);  
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glutReshapeWindow(1000, 1000);

}

void rotateTransform()
{

    glutDestroyWindow(mainWindow);
    mainWindow = -1;
    secondaryWindow = glutCreateWindow("Rotating Window");
    glutDisplayFunc(displayRotate);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(handleRotateWindowKeyboard);
    glutMouseFunc(rotateHandleMouse);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glutReshapeWindow(1000, 1000);

}

void reflectTransform() 
{

    glutDestroyWindow(mainWindow);
    mainWindow = -1;
    secondaryWindow = glutCreateWindow("Reflection Window");
    glutDisplayFunc(displayReflect);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(handleReflectWindowKeyboard);
    glutMouseFunc(reflectHandleMouse);
    glClearColor(0.4f, 0.3f, 0.3f, 1.0f);  
    glutReshapeWindow(1000, 1000);

}



void shearTransform() 
{

    glutDestroyWindow(mainWindow);
    mainWindow = -1;
    secondaryWindow = glutCreateWindow("Shearing Window");
    glutDisplayFunc(displayShear);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(handleShearWindowKeyboard);
    glutMouseFunc(shearHandleMouse);
    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
    glutReshapeWindow(1000, 1000);

}



int main(int argc, char** argv) 
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    mainWindow = glutCreateWindow("Main Menu");
    glutDisplayFunc(displayMainMenu);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(handleMainMenuKeyboard);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutMainLoop();
    return 0;

}