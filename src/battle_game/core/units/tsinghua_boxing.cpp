#include "tsinghua_boxing.h"

#include "battle_game/core/game_core.h"


namespace battle_game::unit {
namespace {
uint32_t left_fist_model_index = 0xffffffffu;
uint32_t right_fist_model_index = 0xffffffffu;
}  // namespace

TBoxing::TBoxing(GameCore *game_core, uint32_t id, uint32_t player_id)
    : Unit(game_core, id, player_id) {
  if (!~left_fist_model_index) {
    auto mgr = AssetsManager::GetInstance();
    {
      /* Left Fist */
      left_fist_model_index = mgr->RegisterModel(
          {{{-0.5f, 0.8f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.5f, 0.8f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.8f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.8f, -0.4f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.7f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.2f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{-0.1f, -0.2f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.2f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.6f, -0.4f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{0.0f, -0.9f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
           {{-0.8f, -0.7f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}},
          {0, 1, 2, 3, 4, 5, 6, 7, 8, 4, 9, 10, 0});
    }

    {
      /* Right Fist */
      right_fist_model_index = mgr->RegisterModel(
          {
              {{-0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
              {{-0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
              {{0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
              {{0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
          },
          {0, 1, 2, 1, 2, 3});
    }
  }
}

void TBoxing::Render() {
  battle_game::SetTransformation(position_, rotation_);
  battle_game::SetTexture(0);
  battle_game::SetColor(game_core_->GetPlayerColor(player_id_));
  battle_game::DrawModel(left_fist_model_index);
  battle_game::DrawModel(right_fist_model_index);
}

void TBoxing::Update() {
  FistsMove(3.0f, glm::radians(180.0f));
  Attack();
}

bool TBoxing::IsHit(glm::vec2 position) const {
  position = WorldToLocal(position);
  return position.x > -0.8f && position.x < 0.8f && position.y > -1.0f &&
         position.y < 1.0f && position.x + position.y < 1.6f &&
         position.y - position.x < 1.6f;
}

void TBoxing::FistsMove(float move_speed, float rotate_angular_speed) {
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    glm::vec2 move_offset{0.0f, 0.0f};
    if (input_data.key_down[GLFW_KEY_W]) {
      move_offset.y += 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_S]) {
      move_offset.y -= 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_A]) {
      move_offset.x -= 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_D]) {
      move_offset.x += 1.0f;
    }
    move_offset *= kSecondPerTick * move_speed * GetSpeedScale();
    game_core_->PushEventMoveUnit(id_, position_ + move_offset);
  }
}

void TBoxing::Attack() {
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    if (input_data.mouse_button_down[GLFW_MOUSE_BUTTON_LEFT]) {
      auto velocity = Rotate(glm::vec2{0.0f, 20.0f}, turret_rotation_);
      GenerateBullet<bullet::CannonBall>(
          position_ + Rotate({0.0f, 1.2f}, turret_rotation_), turret_rotation_,
          GetDamageScale(), velocity);
      fire_count_down_ = kTickPerSecond;  // Fire interval 1 second.
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