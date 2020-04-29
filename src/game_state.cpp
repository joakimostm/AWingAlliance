#include <functional>

#include "game_state.h"
#include "actor/ship.h"
#include "keyboard.h"

std::unique_ptr<GameState>
GameState::Create(const std::map<std::string, std::unique_ptr<Model>>* models_ptr)
{
    return std::make_unique<GameState>(models_ptr);
}

GameState::GameState(const std::map<std::string, std::unique_ptr<Model>>* models_ptr)
  : models_ptr(models_ptr),
    spline(Spline::Waypoint({ -32.0f, 148.0f, -128.0f }, { 0.0f, 0.0f, 1.0f }, 512.0f),
           Spline::Waypoint({ 0.0f, 112.0f, 0.0f }, { -1.0f, 0.0f, .0f }, 512.0f))
{
}

void GameState::register_ships(const std::map<std::string, ScenarioParser::ActorEntry>& actors)
{
    for (const auto& actor_item : actors)
    {
        register_ship(actor_item.first, actor_item.second);
    }
}

void GameState::register_ship(const std::string& name, const ScenarioParser::ActorEntry& actorentry)
{
    const Model* model_ptr = models_ptr->at(actorentry.type).get();
    Ships.insert(std::make_pair(
        name,
        actor::Ship::Create(
            actorentry.pos,
            actorentry.dir,
            model_ptr,
            std::bind(Laser::RegisterLaser, std::ref(Lasers), std::placeholders::_1))));
}

void GameState::register_laser(const Laser& laser)
{
}

void GameState::register_terrain(const ScenarioParser::TerrainEntry* terrainentry)
{
    terrain = std::make_unique<Terrain>(terrainentry);
}

const Terrain* const GameState::GetTerrain() const
{
    return terrain.get();
}

const std::map<std::string, std::unique_ptr<actor::Ship>>& GameState::GetShips() const
{
    return Ships;
}

const std::list<Laser>& GameState::GetLasers() const
{
    return Lasers;
}

Camera& GameState::GetCamera() const
{
    return camera;
}

void GameState::register_player(const std::string& name)
{
    player_name = name;
}

void GameState::integrate(std::chrono::system_clock::time_point t,
                          std::chrono::duration<float> d_time)
{
    ProcessKeyboardInput(keyboardInfo);
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(d_time).count() / 1000.0f;

    if (!Lasers.empty())
    {
        using namespace std::chrono;
        auto time_until_expiration =
            duration_cast<seconds>(Lasers.front().expire_time - system_clock::now());

        if (time_until_expiration < milliseconds(0))
        {
            Lasers.pop_front();
        }
        for (Laser& laser : Lasers)
        {
            laser.Update(dt);
        }
    }

    if (keyboardInfo.test(KeyboardMapping::SPACEBAR))
        Ships.at("awing1")->Fire();

    Ships.at("awing1")->Update(keyboardInfo, dt);
    Ships.at("tie2")->Follow(*Ships.at("awing1"), dt);

    using namespace std::chrono;
    const auto repeat_time = milliseconds(5000);
    const auto spline_time_d = duration_cast<milliseconds>((t.time_since_epoch() % repeat_time));
    const auto spline_time = spline_time_d.count() / static_cast<float>(repeat_time.count());
    const auto ret = spline(spline_time);
    Ships.at("tie1")->SetPose(ret.first, ret.second);

    camera.Update(dt);
}
