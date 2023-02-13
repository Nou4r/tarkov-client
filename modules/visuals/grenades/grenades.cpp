#include "grenades.hpp"

#include <cmath>
#include <mutex>

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

#include <codecvt>

void modules::visuals::grenades::callback(engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, const std::vector< grenade_structure_t >& grenades_copy, engine::types::player_t local_player_copy, engine::types::player_t closest_player, engine::structures::vector3_t lp_pos)
{
    if (!std::get< bool >(config::config_map[features::display_grenades]))
        return;

    if (!camera_copy.instance || !local_game_world_copy.instance)
        return;

    for (const auto item : grenades_copy)
    {
        const auto root_world_position = item.pos;

        const auto distance = static_cast<std::uint32_t>(root_world_position.distance(lp_pos));

        const auto root_position = camera_copy.world_to_screen(root_world_position);

        const auto draw_list = ImGui::GetBackgroundDrawList();

        if (!root_position.has_value())
            continue;

        const auto root_screen_position = root_position.value();

        if (std::get< bool >(config::config_map[features::display_grenades]))
        {
            std::string to_draw = "Grenade ["; to_draw.append( std::to_string( distance ) ); to_draw.append( "m]" );
            
            {
                const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::grenade_color]);

                auto item_color = ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] };

                draw_list->AddOutlinedText(renderer::imgui::elements::fonts::bender_font_reg, 10.7f, { root_screen_position.x, root_screen_position.y }, item_color, to_draw.c_str());
            }
        }
    }
}