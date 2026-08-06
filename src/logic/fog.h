// src/logic/fog.h — Row visibility and fog logic
// Pure game logic layer: no raylib includes
#pragma once

namespace game::logic {

// Returns true if rowId is within the 3 visible rows from currentRow
inline bool rowVisible(int rowId, int currentRow) {
  return rowId >= currentRow && rowId < currentRow + 3;
}

// Returns true if rowId should be fogged (show card backs)
// A row is fogged if it is visible but beyond the visible threshold
inline bool rowFog(int rowId, int currentRow, int nbRowsHidden) {
  if (!rowVisible(rowId, currentRow)) {
    return false;
  }
  return rowId > currentRow + (2 - nbRowsHidden);
}

} // namespace game::logic
