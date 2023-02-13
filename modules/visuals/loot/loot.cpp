#include "loot.hpp"

#include <cmath>
#include <mutex>

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

#include <codecvt>

void modules::visuals::loot::callback(engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, const std::vector< loot_structure_t >& items_copy, engine::types::player_t local_player_copy, engine::types::player_t closest_player, engine::structures::vector3_t lp_pos)
{
    if (!std::get< bool >(config::config_map[features::display_items]) && !std::get< bool >(config::config_map[features::display_corpses]))
        return;

    if ( !camera_copy.instance || !local_game_world_copy.instance )
        return;

    auto iter = 0u;
    for ( const auto item : items_copy )
    {
        const auto root_world_position = item.pos;

        const auto distance = static_cast<std::uint32_t>( root_world_position.distance(lp_pos) );

        if ( ( ++iter > 500 && item.price < 25000 ) )//Dont count an item that wouldnt be rendered anyways in the render count
            continue;

       const auto root_position = camera_copy.world_to_screen(root_world_position);

        const auto draw_list = ImGui::GetBackgroundDrawList();

        if (!root_position.has_value())
            continue;

        const auto root_screen_position = root_position.value();

        if ( std::get< bool >( config::config_map[features::display_corpses] ) && item.name == "Corpse" )
        {
            std::string to_draw = "Corpse ["; to_draw.append( std::to_string( distance ) ); to_draw.append( "m]" );
            {
                const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::corpse_color]);

                draw_list->AddOutlinedText( renderer::imgui::elements::fonts::bender_font_reg, 11.4f, { root_screen_position.x, root_screen_position.y }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str( ) );
            }
        }
        
        if ( std::get< bool >( config::config_map[features::display_items] ) && !( item.name == "Corpse" ) )
        {
            if ( item.price <= std::get< int >( config::config_map[features::display_item_max_price] ) && item.price >= std::get< int >( config::config_map[features::display_item_min_price] ) )
            {
                std::string to_draw = item.name + "\n$" + std::to_string( item.price ) + "\n ["; to_draw.append( std::to_string( distance ) ); to_draw.append( "m]" );
                {
                    const auto col = item.price < 25000 ? std::get< engine::structures::vector4_t >(config::config_map[features::item_color]) : std::get< engine::structures::vector4_t >(config::config_map[features::expensive_item_color]);

                    auto item_color = ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] };

                    draw_list->AddOutlinedText( renderer::imgui::elements::fonts::bender_font_reg, 10.8f, { root_screen_position.x, root_screen_position.y }, item_color, to_draw.c_str( ) );
                }
            }
        }
    }
}