// Oleg Kotov

#include <Arduino_GFX_Library.h>
#include <Arduino.h>

#include <float.h> // FLT_EPSILON
#include <stdlib.h>

#include "matrix.h"
#include "matrix3.h"
#include "vector.h"
#include "vector4.h"
#include "utils.h"
#include "clock.h"
#include "mesh.h"
#include "button.h"

bool backfaceCullingEnabled = false;

enum class DrawMode : uint8_t
{
    Lines,
    Fill,
    Normals,
    Depth,
    Count
};

DrawMode drawMode = DrawMode::Lines;

constexpr int button_x_pin = 12;
Button button_x( button_x_pin );

String framerateText = "30 fps";
String drawModeText = "Lines";

Mesh mesh = createCubeMesh();
// Mesh mesh = createTriangleMesh();

#define NUM_ELEMENTS( x )  ( sizeof( x ) / sizeof( ( x )[0]) )

#define PRINT_MATRIX( x ) printMatrix( #x, x )

void printMatrix( const char* matrixName, const Matrix& matrix )
{
	printf( "> %s\n", matrixName );

	printf( "    %f %f %f %f \n", matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3] );
	printf( "    %f %f %f %f \n", matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3] );
	printf( "    %f %f %f %f \n", matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3] );
	printf( "    %f %f %f %f \n", matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3] );
}

Clock gameClock;

uint16_t backgroundColor = rgb565( 30, 30, 30 );

// yellow
// uint16_t lineColor = rgb565( 255, 227, 158 );
// uint16_t fillColor = rgb565( 53, 50, 43 );

// blue
uint16_t lineColor = rgb565( 85, 170, 255 );
uint16_t fillColor = rgb565( 37, 45, 53 );

Vector4 viewport;

Matrix viewMatrix;
Matrix projectionMatrix;
Matrix viewProjectionMatrix;

const float degToRad = 0.0174532f;

const int16_t displayWidth = 240;
const int16_t displayHeight = 240;
const int16_t canvasWidth = 165;  // 120 - 60fps
const int16_t canvasHeight = 165; // 165 - 30fps
const uint16_t canvasSize = canvasWidth * canvasHeight;

int16_t canvasX = ( displayWidth - canvasWidth ) / 2;
int16_t canvasY = ( displayHeight - canvasHeight ) / 2;

Vector cameraPosition = Vector( 0.0f, 0.0f, 0.0f );
Vector cameraTarget = Vector( 0.0f, 1.0f, 0.0f );
Vector cameraDirection = ( cameraTarget - cameraPosition ).normalize();
Vector lightDirection = cameraDirection;
Vector worldUp = Vector( 0.0f, 0.0f, 1.0f );

float fov = 70.0f * degToRad;
float aspectRatio = (float)canvasWidth / (float)canvasHeight;
float nearClip = 0.1f;
float farClip = 50.0f;

float angle = 0.0f;

Arduino_DataBus* bus = new Arduino_ESP32SPI( DF_GFX_DC, DF_GFX_CS, DF_GFX_SCK, DF_GFX_MOSI, DF_GFX_MISO, VSPI );
Arduino_GFX* display = new Arduino_GC9A01( bus, DF_GFX_RST, /* rotation = */ 1, /* IPS = */ true );
Arduino_GFX* canvas = new Arduino_Canvas( canvasWidth, canvasHeight, display, canvasX, canvasY );

void drawCenteredText( const String& text, int ypos )
{
    int16_t x, y;
    uint16_t w, h;
    
    display->getTextBounds( text, 0, 0, &x, &y, &w, &h );
    
    x = ( canvasWidth - w ) * 0.5f;

    canvas->setCursor( x, ypos );
    canvas->print( text );
}

float* depthBuffer = nullptr;

void initDepthBuffer()
{
    printf( "canvasSize: %i\n", canvasSize );
    depthBuffer = (float*)calloc( canvasSize, sizeof( float ) );

    if ( depthBuffer == nullptr )
    {
        printf( "try hack\n" );

        // hack for allocate memory over 63,360 pixels
        uint16_t canvasHalfSize = canvasSize * 0.5f;
        depthBuffer = (float*)calloc( canvasHalfSize, sizeof( float ) );
        float* tmp = (float*)calloc( canvasHalfSize, sizeof( float ) );

        if ( tmp == nullptr )
        {
            printf( "tmp allocation failed\n" );
        }

        UNUSED( tmp );
    }

    if ( depthBuffer == nullptr )
    {
        printf( "depthBuffer allocation failed\n" );
    }
}

void barycentric( int x0, int y0, int x1, int y1, int x2, int y2, int x, int y, float& alpha, float& beta, float& gamma )
{
    float det = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
    alpha = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / det;
    beta = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / det;
    gamma = 1.0f - alpha - beta;
}

void clearDepthBuffer()
{
    if ( !depthBuffer ) return;

    // for ( int i = 0; i < canvasSize; ++i )
    // {
    //     depthBuffer[i] = 0.0f;
    // }

    memset( depthBuffer, 0, canvasSize * sizeof( float ) );
}

void drawDepthBuffer()
{
    for ( int i = 0; i < canvasSize; ++i )
    {
        float depth = depthBuffer[i];
        uint16_t color = rgb565( depth * 255, depth * 255, depth * 255 );
        // uint16_t color = rgb565( 0.5f * 255, 0.5f * 255, 0.5f * 255 );

        int x = i % canvasWidth;
        int y = i / canvasWidth;

        canvas->drawPixel( x, y, color );
    }
}

void printFreeHeap()
{
    printf( "- free heap: %u bytes\n", ESP.getFreeHeap() );
}

uint16_t getRandomColor()
{
    return rgb565( rand() % 255, rand() % 255, rand() % 255 );
}

Vector project( const Vector4& pointClipSpace )
{
    Vector pointScreenSpace;

    // to NDC

    // float inv = 1.0f / ( position.w + FLT_EPSILON );
    // position *= inv;

    float x_ndc = pointClipSpace.x / pointClipSpace.w;
    float y_ndc = pointClipSpace.y / pointClipSpace.w;
    float z_ndc = pointClipSpace.z / pointClipSpace.w;

    // printf( "x_ndc: %.2f\n", x_ndc );
    // printf( "y_ndc: %.2f\n", y_ndc );
    // printf( "z_ndc: %.2f\n", z_ndc );

    // to Screen Space

    pointScreenSpace.x = ( x_ndc * 0.5f + 0.5f ) * viewport.z + viewport.x;
    pointScreenSpace.y = ( y_ndc * 0.5f + 0.5f ) * viewport.w + viewport.y;

    pointScreenSpace.y = canvasHeight - pointScreenSpace.y;

    // in directx z_ndc already in 0-1
    // pointScreenSpace.z = ( z_ndc * 0.5f + 0.5f ); // 0-1f
    pointScreenSpace.z = z_ndc;

    // the same
    // -1 - 1 to 0 - 1
    // ndc = ndc * 0.5 + 0.5;
    // ndc = ( 1.0 + ndc ) * 0.5;

    return pointScreenSpace;
}

bool isPointInsideCanvas( uint16_t x, uint16_t y )
{
    return ( ( x >= 0 && x < canvasWidth ) && ( y >= 0 && y < canvasHeight ) );
}

void drawPoint( uint16_t x, uint16_t y, uint16_t color )
{
    if ( isPointInsideCanvas( x, y ) )
	{
		canvas->drawPixel( x, y, color );
	}
}

void drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color )
{
    int dx = abs( x1 - x0 );
    int dy = abs( y1 - y0 );
    int sx = ( x0 < x1 ) ? 1 : -1;
    int sy = ( y0 < y1 ) ? 1 : -1;
    int err = dx - dy;

    while ( true )
    {
        drawPoint( x0, y0, color );

        if ( ( x0 == x1 ) && ( y0 == y1 ) ) break;

        int e2 = 2 * err;

        if ( e2 > -dy )
        {
            err -= dy;
            x0 += sx;
        }

        if ( e2 < dx )
        {
            err += dx;
            y0 += sy;
        }
    }
}

void drawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color )
{
    // Если все три вершины имеют одинаковую y-координату,
    // то треугольник не имеет площади
    if ( y0 == y1 && y0 == y2 ) return;

    // Сортировка вершин по y-координате
    // y0 <= y1 <= y2
    if ( y0 > y1 )
    {
        std::swap( x0, x1 );
        std::swap( y0, y1 );
    }

    if ( y1 > y2 )
    {
        std::swap( x1, x2 );
        std::swap( y1, y2 );
    }

    if ( y0 > y1 )
    {
        std::swap( x0, x1 );
        std::swap( y0, y1 );
    }

    // Вычисление высоты треугольника
    int total_height = y2 - y0;

    // Если высота равна 0, то треугольник не имеет площади
    if ( total_height == 0 ) return;

    for ( int i = 0; i < total_height; ++i )
    {
        // Переменная second_half равна true, если i больше или
        // равно y1 - y0, что означает, что мы прошли половину
        // треугольника и теперь находимся во второй половине.
        // Если y1 == y0, то second_half также равен true,
        // чтобы избежать деления на ноль в вычислении параметра beta.
        bool second_half = i > y1 - y0 || y1 == y0;

        // Переменная segment_height определяет высоту текущего
        // сегмента треугольника, то есть высоту отрезка y0-y1
        // или y1-y2, в зависимости от того, в какой половине
        // треугольника мы находимся.
        int segment_height = second_half ? y2 - y1 : y1 - y0;

        // Значение параметра alpha находится как отношение текущей
        // строки i к общей высоте треугольника total_height.
        float alpha = (float)i / total_height;

        // Значение параметра beta находится как отношение текущей
        // строки i минус высота предыдущей половины y1-y0 (если
        // находимся во второй половине) к высоте текущего сегмента
        // segment_height.
        float beta = (float)( i - ( second_half ? y1 - y0 : 0 ) ) / segment_height;
        
        int A_x = x0 + ( x2 - x0 ) * alpha;


        int B_x = second_half ? x1 + ( x2 - x1 ) * beta : x0 + ( x1 - x0 ) * beta;

        if ( A_x > B_x ) std::swap( A_x, B_x );

        // draw lines
        for ( int x = A_x; x <= B_x; ++x )
        {
            drawPoint( x, y0 + i, color );
        }
    }
}

void drawTriangleDepth( int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, uint16_t color )
{
    // Если все три вершины имеют одинаковую y-координату,
    // то треугольник не имеет площади
    if ( y0 == y1 && y0 == y2 ) return;

    // Сортировка вершин по y-координате
    // y0 <= y1 <= y2
    if ( y0 > y1 )
    {
        std::swap( x0, x1 );
        std::swap( y0, y1 );
        std::swap( z0, z1 );
    }

    if ( y1 > y2 )
    {
        std::swap( x1, x2 );
        std::swap( y1, y2 );
        std::swap( z1, z2 );
    }

    if ( y0 > y1 )
    {
        std::swap( x0, x1 );
        std::swap( y0, y1 );
        std::swap( z0, z1 );
    }

    // Вычисление высоты треугольника
    int total_height = y2 - y0;

    // Если высота равна 0, то треугольник не имеет площади
    if ( total_height == 0 ) return;

    for ( int i = 0; i < total_height; ++i )
    {
        // Переменная second_half равна true, если i больше или
        // равно y1 - y0, что означает, что мы прошли половину
        // треугольника и теперь находимся во второй половине.
        // Если y1 == y0, то second_half также равен true,
        // чтобы избежать деления на ноль в вычислении параметра beta.
        bool second_half = i > y1 - y0 || y1 == y0;

        // Переменная segment_height определяет высоту текущего
        // сегмента треугольника, то есть высоту отрезка y0-y1
        // или y1-y2, в зависимости от того, в какой половине
        // треугольника мы находимся.
        int segment_height = second_half ? y2 - y1 : y1 - y0;

        // Значение параметра alpha находится как отношение текущей
        // строки i к общей высоте треугольника total_height.
        float alpha = (float)i / total_height;

        // Значение параметра beta находится как отношение текущей
        // строки i минус высота предыдущей половины y1-y0 (если
        // находимся во второй половине) к высоте текущего сегмента
        // segment_height.
        float beta = (float)( i - ( second_half ? y1 - y0 : 0 ) ) / segment_height;
        
        int sx = x0 + ( x2 - x0 ) * alpha;
        int ex = second_half ? x1 + ( x2 - x1 ) * beta : x0 + ( x1 - x0 ) * beta;

        float z01 = z0 + ( z2 - z0 ) * alpha;
        float z02 = second_half ? z1 + ( z2 - z1 ) * beta : z0 + ( z1 - z0 ) * beta;

        if ( sx > ex )
        {
            std::swap( sx, ex );
            std::swap( z01, z02 );
        }

        // draw lines
        for ( int x = sx; x <= ex; ++x )
        {
            int y = y0 + i;

            // printf( "x: %.2f\n", x );
            // printf( "y: %.2f\n", y );

            float phi = ( ex == sx ) ? 1.0f : (float)( x - sx ) / (float)( ex - sx );
            float depthValue = z01 + ( z02 - z01 ) * phi;

            int depthBufferIndex = x + y * canvasWidth;

            // float alpha;
            // float beta;
            // float gamma;

            // barycentric( x0, y0, x1, y1, x2, y2, x, y, alpha, beta, gamma );

            // float depth0 = z0;
            // float depth1 = z1;
            // float depth2 = z2;

            // float depth = gamma * depth0 + alpha * depth1 + beta * depth2;
            // // float depthValue = barycentric.z * v0.pos.z + barycentric.x * v1.pos.z + barycentric.y * v2.pos.z;

            // printf( "z0: %.2f\n", z0 );
            // printf( "z1: %.2f\n", z1 );
            // printf( "z2: %.2f\n", z2 );

            // printf( "z: %.2f\n", z );
            // printf( "depth: %.2f\n", depth );

            // printf( "bufferDepthIndex: %i\n", idx );
            // printf( "bufferDepthValue: %.2f\n", depthBuffer[idx] );

            // delay( 3000 );

            float depthBufferValue = depthBuffer[depthBufferIndex];

            if ( depthValue >= depthBufferValue )
            {
                depthBuffer[depthBufferIndex] = depthValue;

                if ( drawMode == DrawMode::Depth )
                {
                    continue;
                }

                drawPoint( x, y, color );
            }
        }
    }
}

void drawNormal( const Vector& position, const Vector& normal )
{
    Vector4 startClipSpace = viewProjectionMatrix * Vector4( position, 1.0f );
    Vector4 endClipSpace = viewProjectionMatrix * Vector4( position + normal, 1.0f );

    Vector start = project( startClipSpace );
    Vector end = project( endClipSpace );

    canvas->drawLine( start.x, start.y, end.x, end.y, WHITE );
}

// float animationTime = 0.0f;

void drawMesh( float deltaTime )
{
    angle += 45.0f * deltaTime; // 90

    // animationTime += deltaTime;

    // -45 - 45
    // float rotation = cosf( animationTime ) * 45.0f;

    float angleRadians = angle * degToRad;
    // angleRadians = 0.0f;

    Matrix translation = Matrix().setTranslation( 0.0f, 1.7f, 0.0f );

    Matrix rotationX = Matrix().setFromAxisAngle( Vector( 1.0f, 0.0f, 0.0f ), angleRadians );
    Matrix rotationY = Matrix().setFromAxisAngle( Vector( 0.0f, 1.0f, 0.0f ), angleRadians );
    Matrix rotationZ = Matrix().setFromAxisAngle( Vector( 0.0f, 0.0f, 1.0f ), angleRadians );

    // Matrix scaleMatrix = Matrix().setScaling( 0.5f, 0.5f, 0.5f );

    Matrix modelMatrix = translation * rotationX * rotationY * rotationZ;

    // Matrix rotationZ = Matrix().setFromAxisAngle( Vector( 0.0f, 0.0f, 1.0f ), rotation * degToRad );
    // Matrix modelMatrix = translation * rotationZ;

    // Matrix modelViewMatrix = viewMatrix * modelMatrix;

    // Matrix3 normalMatrix = Matrix3( modelMatrix ).inverse().transposed();
    // Matrix3 normalMatrix = Matrix3( modelMatrix.transposed().inverse() );
    // Matrix3 normalMatrix = Matrix3( modelViewMatrix.transposed().inverse() );

    Matrix normalMatrix = modelMatrix.transposed().inverse();
    normalMatrix[0][3] = 0.0f;
    normalMatrix[1][3] = 0.0f;
    normalMatrix[2][3] = 0.0f;
    normalMatrix[3][0] = 0.0f;
    normalMatrix[3][1] = 0.0f;
    normalMatrix[3][2] = 0.0f;
    normalMatrix[3][3] = 1.0f;

    Matrix modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;

    // drawElements
    // drawNormal( position, direction )

    for ( uint16_t i = 0; i < mesh.vertices.size(); ++i )
    {
        Vertex& vertex = mesh.vertices[i];

        // vertex shader

        // Преобразование вершины в пространство отсечения
        Vector4 position = modelViewProjectionMatrix * Vector4( vertex.position, 1.0f );

        if ( drawMode == DrawMode::Fill || drawMode == DrawMode::Normals )
        {
            // Вычисление нормали вершины в мировом пространстве
            Vector normal = normalMatrix.transformPoint( vertex.normal ).normalize();

            if ( drawMode == DrawMode::Normals )
            {
                vertex.positionWS = modelMatrix.transformPoint( vertex.position );
                drawNormal( vertex.positionWS, normal * 0.1f );
            }

            // Вычисление интенсивности света в направлении источника света
            float intensity = Vector::dotProduct( normal, -lightDirection );
            if ( intensity < 0.0f ) intensity = 0.0f;

            // Умножение интенсивности на цвет источника света    
            // Запись цвета в выходную переменную
            vertex.outColor = rgb565( intensity * vertex.color.x * 255, intensity * vertex.color.y * 255, intensity * vertex.color.z * 255 );
            // vertex.outColor = rgb565( vertex.color.x * 255, vertex.color.y * 255, vertex.color.z * 255 );

            // 53, 50, 43

            // vertex.outColor = rgb565( intensity * 150, intensity * 150, intensity * 150 );
            // vertex.outColor = rgb565( 150, 150, 150 );
        }
        else
        {
            vertex.outColor = rgb565( 157, 148, 254 );
        }

        // Преобразование вершины в мировое пространство
        // vertex.positionWS = modelMatrix.transformPoint( vertex.position );

        // to screen space
        vertex.outPosition = project( position );
    }

    for ( uint8_t idx = 0; idx < mesh.triangles.size(); ++idx )
    {
        const Triangle& triangle = mesh.triangles[idx];

        const Vertex& vertex0 = mesh.vertices[triangle.vertexIndex0];
        const Vertex& vertex1 = mesh.vertices[triangle.vertexIndex1];
        const Vertex& vertex2 = mesh.vertices[triangle.vertexIndex2];

        // if ( backfaceCullingEnabled )
        // {
        //     // todo: this is no world space?
        //     // direction to triangle?

        //     Vector normal = calcTriangleNormal( vertex0.positionWS, vertex1.positionWS, vertex2.positionWS );
        //     float dot = Vector::dotProduct( normal, cameraDirection );

        //     // Vector normal = calcTriangleNormal( vertex0.outPosition, vertex1.outPosition, vertex2.outPosition );
        //     // float dot = Vector::dotProduct( normal, Vector( 0.0f, 0.0f, -1.0f ) );

        //     if ( dot > 0 ) continue;
        // }

        if ( drawMode == DrawMode::Lines )
        {
            drawLine( vertex0.outPosition.x, vertex0.outPosition.y, vertex1.outPosition.x, vertex1.outPosition.y, vertex0.outColor );
            drawLine( vertex1.outPosition.x, vertex1.outPosition.y, vertex2.outPosition.x, vertex2.outPosition.y, vertex0.outColor );
            drawLine( vertex2.outPosition.x, vertex2.outPosition.y, vertex0.outPosition.x, vertex0.outPosition.y, vertex0.outColor );
            continue;
        }

        // drawTriangle( 
        //     vertex0.outPosition.x, vertex0.outPosition.y,
        //     vertex1.outPosition.x, vertex1.outPosition.y, 
        //     vertex2.outPosition.x, vertex2.outPosition.y,
        //     vertex0.outColor 
        // );

        drawTriangleDepth( 
            vertex0.outPosition.x, vertex0.outPosition.y, vertex0.outPosition.z,
            vertex1.outPosition.x, vertex1.outPosition.y, vertex1.outPosition.z,
            vertex2.outPosition.x, vertex2.outPosition.y, vertex2.outPosition.z,
            vertex0.outColor
        );
    }
}

void toggleDrawMode()
{
    uint8_t currentDrawMode = (uint8_t)drawMode;
    uint8_t drawModeCount = (uint8_t)DrawMode::Count;
    uint8_t nextDrawMode = ( currentDrawMode + 1 ) % drawModeCount;

    drawMode = (DrawMode)nextDrawMode;

    switch ( drawMode )
    {
        case DrawMode::Lines:
            drawModeText = "Lines";
            display->fillScreen( backgroundColor );
            break;
        case DrawMode::Fill:
            drawModeText = "Fill";
            break;
        case DrawMode::Normals:
            drawModeText = "Normals";
            break;
        case DrawMode::Depth:
            drawModeText = "Depth";
            display->fillScreen( BLACK );
            break;
    }
}

void setup()
{
    Serial.begin( 115200 );
    Serial.println();

    printf( "app started\n" );

    printFreeHeap();

    display->begin();
    display->fillScreen( backgroundColor );

    canvas->begin();
    // canvas->fillScreen( backgroundColor );
    // canvas->flush();

    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.z = canvasWidth;
    viewport.w = canvasHeight;

    viewMatrix = Matrix::makeLookAtRH( cameraPosition, cameraTarget, worldUp );
    projectionMatrix = Matrix::makePerspectiveReversedRH( fov, aspectRatio, nearClip, farClip );
    viewProjectionMatrix = projectionMatrix * viewMatrix;

    // ---

    // 150 * 150 * 4 = 90,000 bytes
    printFreeHeap();
    initDepthBuffer();
    printFreeHeap();

    display->println( drawModeText );
}

void loop()
{
    float deltaTime = gameClock.calculateDeltaTime();

    if ( button_x.click() ) toggleDrawMode();

    int framerate = 1.0f / deltaTime;

    framerateText.clear();
    framerateText.concat( framerate );
    framerateText.concat( " fps" );

    canvas->fillScreen( backgroundColor );

    clearDepthBuffer();
    drawMesh( deltaTime );

    if ( drawMode == DrawMode::Depth )
    {
        drawDepthBuffer();
    }

    drawCenteredText( framerateText, 0 );
    drawCenteredText( drawModeText, canvasHeight - 8 );

    canvas->flush();
}

