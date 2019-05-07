#ifndef BPUYO_TYPES_H_
#define BPUYO_TYPES_H_

#include "rotatetype.h"

using Player = int;
using PutIndex = int;

using Score = int;
using Frame = int;

using Row = int;
using Column = int;
using FieldIndex = int;

using PutIndex = int;

enum Color : int { EMPTY = 0, RED, YELLOW, GREEN, PURPLE, BLUE, OJAMA, WALL };

static const Player MYSELF = 1;
static const Player ENEMY = 0;

static const int ROTATE_NUMBER = 4;
static const int PUTTYPE_PATTERN = 22;

// 単位はフレーム
// インデックスは段数で、最初は0段目(使わない）
// @todo 自然落下以降の反転
/* 横置き */
static constexpr Frame kTimeFlatDrop[] = {1000, 56, 54, 52, 50, 48, 46,  44,
                                          42,   40, 38, 36, 34, 32, 1000};
/* 無回転 */
static constexpr Frame kTimeNoRotateDrop[] = {1000, 59, 57, 55, 53, 51, 49,  47,
                                              45,   43, 41, 39, 37, 35, 1000};
/* 親子反転(ROTATE180) */
static constexpr Frame kTimeReverseDrop[] = {1000, 57, 55, 53, 51, 49, 47,  45,
                                             43,   41, 39, 37, 35, 33, 1000};
/* ちぎりによる遅延 */
static constexpr Frame kTimeChigiriRowDifference[] = {
    0, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};

/// 1連鎖の時間　インデックスは落下段数(0はクイック
static constexpr Frame kTimeOneChainAndDrop[] = {53, 81, 83, 85, 87,  89,  91,
                                                 93, 95, 97, 99, 101, 103, 105};
/* 連鎖時のぷよの設置時間*/
static constexpr Frame kTimePutAfterChain = 25;

/* 以下自然落下 */
/* 横置き */
static constexpr Frame kTimeFlatNaturalDrop[] = {
    0, 39, 55, 71, 87, 103, 119, 135, 151, 167, 183, 199, 215, 231, 247};
/* 無回転 */
static constexpr Frame kTimeNoRotateNaturalDrop[] = {
    0, 39, 55, 71, 87, 103, 119, 135, 151, 167, 183, 199, 215, 231, 247};
/* 親子反転(ROTATE180) */
static constexpr Frame kTimeReverseNaturalDrop[] = {
    0, 38, 54, 70, 86, 102, 118, 134, 150, 166, 182, 198, 214, 230, 246};



/* 連鎖ボーナス */
static constexpr Score kBonusChain[] = {0,   0,   8,   16,  32,  64,  96,
                                        128, 160, 192, 224, 256, 288, 320,
                                        352, 384, 416, 448, 480, 512};
/* 連結ボーナス */
static constexpr Score kBonusLink[] = {0, 0,  0,  0,  0,  2,  3,  4,  5, 6,
                                       7, 10, 10, 10, 10, 10, 10, 10, 10};
/* 色数ボーナス */
static constexpr Score kBonusColor[] = {0, 0, 3, 6, 12, 24};

static const Score UPPER_FATAL_DOSE = 4900;
static const Score LOWER_FATAL_DOSE = 4200;

static const RotateType ALL_INDEXED_ROTATE_TYPE[PUTTYPE_PATTERN] = {
    ROTATE_0,   ROTATE_90,  ROTATE_180, ROTATE_0,   ROTATE_90,  ROTATE_180,
    ROTATE_270, ROTATE_0,   ROTATE_90,  ROTATE_180, ROTATE_270, ROTATE_0,
    ROTATE_90,  ROTATE_180, ROTATE_270, ROTATE_0,   ROTATE_90,  ROTATE_180,
    ROTATE_270, ROTATE_0,   ROTATE_180, ROTATE_270};
#endif
