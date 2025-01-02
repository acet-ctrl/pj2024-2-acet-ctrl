#pragma once

#include "battle_game/core/unit.h"

namespace battle_game::unit {
class TBoxing : public Unit {
 public:
  TBoxing(GameCore *game_core, uint32_t id, uint32_t player_id);
  void Render() override;
  void Update() override;
  [[nodiscard]] bool IsHit(glm::vec2 position) const override;

 protected:
  void FistsMove(float move_speed, float rotate_angular_speed);
  void Attack();
  [[nodiscard]] const char *UnitName() const override;
  [[nodiscard]] const char *Author() const override;

  bool has_gf_{true};
  uint32_t fire_count_down_{0};
};
}  // namespace battle_game::unit