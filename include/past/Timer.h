#pragma once

#include <ctime>

class Timer
{
public:
    Timer();
    void start();               ///< タイマーの計測を開始します。
    void stop();                ///< タイマーの計測を終了します。
    double elapsedSec()const;   ///< start 関数を呼び出してから stop 関数を呼び出すまでの経過時間を取得します。
private:
    std::clock_t mTimeBegin;    ///< 開始時刻
    std::clock_t mTimeEnd;      ///< 終了時刻
};

