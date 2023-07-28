#include "geom.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

Point createPoint(double x, double y) {
    Point p = {x, y};
    return p;
}

float distance(Point *p1, Point *p2) {
    return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}



Polyline createPolyline(char wkt[]) {    
    Polyline line = {};
    return line;
}


Polygon createPolygon(char wkt[]) {
    PointArray pointArray = parsePointsStr(wkt);
    Point points[pointArray.size];
    for (int i = 0; i < pointArray.size; i++) {
        points[i] = pointArray.points[i];
    }
    Polygon polygon = {
        10.0, 100.0  
    };
    return polygon;
}

/**
 * 将字符串转换为Point类型
*/
Point parsePointStr(char *tmp) {
    char *coord;
    double x;
    double y;
    int index = 0;
    while ((coord = strsep(&tmp, " ")) != NULL) {
        if (index == 0) {
            x = atof(coord);
        } else {
            y = atof(coord);
        }
        index = index + 1;
    }
    Point p = {x, y};
    return p;
}

/**
 * 将wkt类型的字符串转换为point数组
*/
PointArray parsePointsStr(char wkt[]) {
    char *string = strdup(wkt);
    char *tmp;
    Point (*points)[5] = malloc(sizeof(Point) * 5);  // points为Point类型的指针，指向数组第一个元素
    int point_num = 0;
    while ((tmp = strsep(&string, ",")) != NULL)
    {       
        Point p = parsePointStr(tmp);
        // Point *newPoints = (Point*) malloc((point_num + 1) * sizeof(Point));
        // printf("point num is: %d; points size is: %ld; newPoints size is: %ld\n", 
        //     point_num, sizeof(points), sizeof(newPoints));
        // memcpy(newPoints, points, sizeof(points));
        // points = newPoints;
        *(*points + point_num) = p;
        // points[point_num] = p;
        point_num = point_num + 1;
    }
    PointArray pointArray = {
        points, point_num
    };
    free(points);
    printf("points size is: %ld\n", pointArray.size);
    
    return pointArray;
}





