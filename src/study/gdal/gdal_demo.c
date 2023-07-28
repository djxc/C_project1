#include "gdal_demo.h"
#include "cpl_conv.h" /* for CPLMalloc() */
#include "gdal.h"
#include <errno.h>


void test_gdal() {
    // 打开tif文件
    const char* pszFilename = "/home/djxc/data/pred_train_0.tif";
    GDALDatasetH  hDataset;
    GDALAllRegister();
    const GDALAccess eAccess = GA_ReadOnly;
    hDataset = GDALOpen(pszFilename, eAccess );
    if( hDataset == NULL )
    {
        printf("error");
        return;
    }

    // 读取dataset的信息
    GDALDriverH   hDriver;
    double        adfGeoTransform[6];
    hDriver = GDALGetDatasetDriver( hDataset );
    printf( "Driver: %s/%s\n",
            GDALGetDriverShortName( hDriver ),
            GDALGetDriverLongName( hDriver ) );
    printf( "Size is %dx%dx%d\n",
            GDALGetRasterXSize( hDataset ),
            GDALGetRasterYSize( hDataset ),
            GDALGetRasterCount( hDataset ) );
    if( GDALGetProjectionRef( hDataset ) != NULL )
        printf( "Projection is `%s'\n", GDALGetProjectionRef( hDataset ) );
    if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
    {
        printf( "Origin = (%.6f,%.6f)\n",
                adfGeoTransform[0], adfGeoTransform[3] );
        printf( "Pixel Size = (%.6f,%.6f)\n",
                adfGeoTransform[1], adfGeoTransform[5] );
    }

    // 获取波段数据。获取最大值最小值，colortable等
    GDALRasterBandH hBand;
    int             nBlockXSize, nBlockYSize;
    int             bGotMin, bGotMax;
    double          adfMinMax[2];
    hBand = GDALGetRasterBand( hDataset, 1 );
    GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );
    printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
            nBlockXSize, nBlockYSize,
            GDALGetDataTypeName(GDALGetRasterDataType(hBand)),
            GDALGetColorInterpretationName(
                GDALGetRasterColorInterpretation(hBand)) );
    adfMinMax[0] = GDALGetRasterMinimum( hBand, &bGotMin );
    adfMinMax[1] = GDALGetRasterMaximum( hBand, &bGotMax );
    if( ! (bGotMin && bGotMax) )
        GDALComputeRasterMinMax( hBand, TRUE, adfMinMax );
    printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
    if( GDALGetOverviewCount(hBand) > 0 )
        printf( "Band has %d overviews.\n", GDALGetOverviewCount(hBand));
    if( GDALGetRasterColorTable( hBand ) != NULL )
        printf( "Band has a color table with %d entries.\n",
                GDALGetColorEntryCount(
                    GDALGetRasterColorTable( hBand ) ) );

    // 读取像素值
    float *pafScanline;
    int   nXSize = GDALGetRasterBandXSize( hBand );
    pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
    CPLErr cpl_err = GDALRasterIO( hBand, GF_Read, 0, 0, nXSize, 1,
                pafScanline, nXSize, 1, GDT_Float32,
                0, 0 );
    if (cpl_err) {
        printf("get data error");
    }
    GDALClose(hDataset);
}