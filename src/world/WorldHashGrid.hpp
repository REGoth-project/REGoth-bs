#pragma once
#include <map>
#include <Math/BsVector3.h>

namespace REGoth
{
  /**
   * Stores a grid of cells, where each cell can contain `bs::SceneObject`s or
   * something else.
   *
   * By supplying a position, the `WorldHashGrid` is able to quickly figure
   * out what other objects are inside the cell the given position lays in.
   * Additionally, it can search the surrounding cells.
   *
   * Use Case - Show object names
   * ============================
   *
   * While the player is walking through the world, they can take objects into
   * focus by looking at them. A focused object will also have its name drawn
   * on top of it. This means, for each frame, the game needs to find a close
   * object to the player, which is also being looked at. Since the number of
   * *focusable* objects can get rather large, we don't want to iterate through
   * all of them every frame.
   *
   * By searching the cell around the player, we can get the possibly focused
   * items much quicker.
   */
  template <typename T>
  class WorldHashGrid
  {
  public:
    WorldHashGrid(bs::INT32 cellSize)
        : mCellSize(cellSize)
    {
    }

    /**
     * Adds an object at the given position. Note that this takes a 3D vector
     * but it will project it down to the XZ-plane since the Grid is 2D.
     */
    void add(const bs::Vector3& position, const T& value)
    {
      auto cell = positionToCell(position);

      mCells.insert(std::make_pair(cell, value));
    }

    /*void remove(const bs::Vector3& position, const T& value)
    {
        mCells.erase();
    }*/

  private:
    struct Cell
    {
      bs::INT32 x;
      bs::INT32 y;

      bool operator<(const Cell& rhs) const
      {
        if (x != v.x) return x < v.x;

        return y < v.y;
      }
    };

    Cell positionToCell(const bs::Vector3& position) const
    {
      Cell c{};

      c.x = static_cast<bs::INT32>(position.x / mCellSize);
      c.y = static_cast<bs::INT32>(position.y / mCellSize);

      return c;
    }

    bs::INT32 mCellSize;
    std::multimap<Cell, T> mCells;
  }
}  // namespace REGoth