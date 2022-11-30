/**
 * 几何点
*/
typedef struct point {
    float x;
    float y;
} Point;

typedef struct polyline {
    double length;
    Point points[];
} Polyline;

typedef struct polygon {
    double perimeter;
    double area;
    Point points[];
} Polygon;

typedef struct pointArray {
    Point *points;
    int size;
} PointArray;

/**
 * 创建点
*/
Point createPoint(double x, double y);

/**
 * 计算两点的距离
*/
float distance(Point *p1, Point *p2);


Polyline createPolyline(char wkt[]);


Polygon createPolygon(char wkt[]);

PointArray parsePointsStr(char wkt[]);