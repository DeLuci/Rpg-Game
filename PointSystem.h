#pragma once
class PointSystem {
public:
  // returns true if set successfully
  [[nodiscard]] const unsigned int getMax() const noexcept {
    return _max;
  }
  [[nodiscard]] const unsigned int getCurrent() const noexcept {
    return _current;
  }
  [[nodiscard]] const bool isFull() const noexcept { return (_current == _max); }

  void setMax(unsigned int new_max) noexcept {
      if (new_max < 1)
          _max = 1;
      else
          _max = new_max;

      if (_current > _max)
          _current = _max;
  }

  void reduceCurrent(unsigned int damage) noexcept {
      if (damage > _current) {
          _current = 0;
          return;
      }

      _current -= damage;
  }

  void increaseCurrent(unsigned int amount) noexcept {
      if (amount + _current > _max) {
          _current = _max;
          return;
      }

      _current += amount;
  }

  PointSystem() { _current = 1; _max = 1; }

  PointSystem(unsigned int c, unsigned int m) {
      _current = c;
      _max = m;
      if (_current > _max)
          _current = _max;
  } 

private:
  unsigned int _current;
  unsigned int _max;
};
