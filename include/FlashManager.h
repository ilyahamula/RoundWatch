#ifndef FLAHMANAGER_H
#define FLAHMANAGER_H

using uint8 = unsigned char;

namespace FlashManager
{
    void SaveCurrDivision(const int dial, const uint8 div);
    void SaveColor(const uint8 r, const uint8 g, const uint8 b);
};

#endif