#ifndef TARGETCALC_HPP
#define TARGETCALC_HPP

#include <SFML/Graphics.hpp>
#include "Pac.hpp"
#include "controlGame.hpp"

using namespace std;
using namespace sf;

class TargetCalc{
    public:
        static GhostDirection getToTargetDir(int ghId, Vector2f ghostPos, GhostDirection ghostDir, GhostStatus ghostStatus, Vector2f** targetPos, Vector2f edgePos, PacDirection pacDir, const char* crossCh);
    private:
        static void getTargetOfs(int* targetOfsX, int* targetOfsY, int ghId, Vector2f targetPos, PacDirection pacDir);
        static bool noCondition(const char* ch);
};

#endif