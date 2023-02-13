#include "extracts.hpp"

#include <cmath>
#include <mutex>

#include "../../../renderer/renderer.hpp"
#include "../../../renderer/menu/config/config.hpp"

engine::types::local_game_world_t cached_game_world { };

std::vector< std::pair<engine::types::exfil_t, std::string>> cached_exfils;

void modules::visuals::extracts::callback(engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, engine::types::player_t local_player_copy, engine::structures::vector3_t lp_pos)
{
    if ( !camera_copy.instance || !local_game_world_copy.instance )
        return;

    if ( std::get< bool >( config::config_map[features::display_extractions] ) )
    {
        if ( cached_game_world.instance != local_game_world_copy.instance )
        {
            const auto exit_controller = local_game_world_copy.get_exfil_controller( );

            cached_exfils = exit_controller.get_exfils( );

            cached_game_world = local_game_world_copy;
        }

        for ( const auto exfil : cached_exfils ) //exfils dont move soooooooooo can cache pos eventually
        {
            const auto& [ inst, name ] = exfil;

            auto transform_ptr = driver.read<engine::types::transform_t>( exfil.first.instance, { 0x10, 0x30, 0x30, 0x8, 0x28 } );

            if ( !transform_ptr.instance )
                continue;

            const auto draw_list = ImGui::GetBackgroundDrawList( );

            const auto position = transform_ptr.get_position( );

            const auto screen_pos = camera_copy.world_to_screen( position );

            if ( !screen_pos.has_value( ) )
                continue;

            const auto root_screen_position = screen_pos.value( );

            const auto distance = static_cast<std::uint32_t>( position.distance(lp_pos) );

            {
                const auto [ x, y ] = *screen_pos;

                std::string to_draw = exfil.second + " ["; to_draw.append( std::to_string( distance ) ); to_draw.append( "m]" );
                {
                    const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::extraction_color]);

                    draw_list->AddOutlinedText( renderer::imgui::elements::fonts::bender_font_reg, 11.8f, { x, y }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str( ) );
                }
            }
        }
    }
    else
    {
        const auto exit_controller = local_game_world_copy.get_exfil_controller( );

        cached_exfils = exit_controller.get_exfils( );

        cached_game_world = local_game_world_copy;
          
        return;
    }
}