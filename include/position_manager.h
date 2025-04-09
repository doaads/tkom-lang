#pragma once

#include "position.h"


class PositionManager {
    private:
        Position position;
    public:
      PositionManager() {}

      Position save_position() const;
      void adjust_position(char ch);
};
