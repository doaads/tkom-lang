#pragma once

#include "position.h"


class PositionManager {
    private:
        Position position;
    public:
      PositionManager() {}

      Position save_position();
      void adjust_position(char ch);
};
