#include "tsinghua_boxing.h"

#include "battle_game/core/game_core.h"

namespace battle_game::unit {
namespace {
uint32_t fist_model_index = 0xffffffffu;
}  // namespace

TBoxing::TBoxing(GameCore *game_core, uint32_t id, uint32_t player_id)
    : Unit(game_core, id, player_id) {
  if (!~fist_model_index) {
    auto mgr = AssetsManager::GetInstance();
    fist_model_index = mgr->RegisterModel(
        {{{0.5f, -0.8f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.5f, -0.8f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.8f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.8f, 0.4f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.7f, 0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.2f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{0.1f, 0.2f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.2f, 0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.6f, 0.4f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{-0.0f, 0.9f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
         {{0.8f, 0.1f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}},
        {0, 1, 6,  1, 2, 3,  1, 3, 4, 1, 4, 5, 1, 5, 6,
         0, 6, 10, 6, 9, 10, 6, 7, 9, 7, 8, 9, 8, 9, 4});
  }
}

void TBoxing::Render() {
  battle_game::SetTransformation(position_, rotation_);
  battle_game::SetTexture(0);
  battle_game::SetColor(game_core_->GetPlayerColor(player_id_));
  battle_game::DrawModel(fist_model_index);
}

void TBoxing::Update() {
  FistsMove(3.0f, glm::radians(180.0f));
  Attack();
}

bool TBoxing::IsHit(glm::vec2 position) const {
  position = WorldToLocal(position);
  return position.x > -0.8f && position.x < 0.8f && position.y > -0.8f &&
         position.y < 0.8f;
}

void TBoxing::FistsMove(float move_speed, float rotate_angular_speed) {
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    glm::vec2 move_offset{0.0f};
    if (input_data.key_down[GLFW_KEY_W]) {
      move_offset.y += 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_S]) {
      move_offset.y -= 1.0f;
    }
    move_offset *= kSecondPerTick * move_speed * GetSpeedScale();
    auto new_position =
        position_ + glm::vec2{glm::rotate(glm::mat4{1.0f}, rotation_,
                                          glm::vec3{0.0f, 0.0f, 1.0f}) *
                              glm::vec4{move_offset, 0.0f, 0.0f}};
    if (!game_core_->IsBlockedByObstacles(new_position)) {
      game_core_->PushEventMoveUnit(id_, new_position);
    }
    float rotation_offset = 0.0f;
    if (input_data.key_down[GLFW_KEY_A]) {
      rotation_offset += 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_D]) {
      rotation_offset -= 1.0f;
    }
    rotation_offset *= kSecondPerTick * rotate_angular_speed;
    game_core_->PushEventRotateUnit(id_, rotation_ + rotation_offset);
  }
}

void TBoxing::Attack() {
  if (!fire_count_down_) {
    auto player = game_core_->GetPlayer(player_id_);
    if (player) {
      auto &input_data = player->GetInputData();
      if (input_data.mouse_button_down[GLFW_MOUSE_BUTTON_LEFT]) {
        auto velocity = Rotate(glm::vec2{0.0f, 20.0f}, rotation_);
        GenerateBullet<bullet::CannonBall>(position_, rotation_,
                                           GetDamageScale(), velocity);
        fire_count_down_ = kTickPerSecond;  // Fire interval 1 second.
      }
    }
  }
  if (fire_count_down_) {
    fire_count_down_--;
  }
}

const char *TBoxing::UnitName() const {
  return "Tsinghua Boxing";
}

const char *TBoxing::Author() const {
  return "AcetBug";
}
}  // namespace battle_game::unit