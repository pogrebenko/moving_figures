#ifndef CONSTS_H
#define CONSTS_H

#define APP_DATA_VERSION 0

#define APP_VARIANT_FULL 1
#define APP_VARIANT_DEMO 2

#define APP_VARIANT APP_VARIANT_DEMO

#define MAX_NEAR_SIZE       10
#define MIN_FIGURE_SIZE     10
#define MAX_FIGURE_SIZE    500
#define DEFAULT_TOLERANCE   10
#define DELTA_ANGLE         10.
#define DELTA_RESIZE        10.

#define __SMART__

typedef enum {
    ExportTypeNone              = 0,
    ExportTypeBin               = 1,
    ExportTypeJSON              = 2,
} FormatFileType_t;

#define __EXPORT_FORMAT__ ExportTypeJSON

typedef enum {
    ActionTypeNone              = 0,
    ActionTypeAddFigure         = 1,
    ActionTypeAddRelation       = 2,
    ActionTypeMove              = 3,
    ActionTypeDelete            = 4,
} ActionType_t;

typedef enum {
    FigureTypeNone              = 0,
    FigureTypeTriangle          = 1,
    FigureTypeEllipse           = 2,
    FigureTypeRectangle         = 3
} FigureType_t;

typedef enum {
    RelationTypeNone            = 0,
    RelationTypeLineNondirect   = 1,
    RelationTypeLineBidirect    = 2,
    RelationTypeLineDirectLeft  = 3,
    RelationTypeLineDirectRight = 4,
} RelationType_t;

#endif // CONSTS_H
