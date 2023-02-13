#include "menu.hpp"

#include <array>
#include <functional>

#include "../renderer.hpp"

#include "../../engine/engine.hpp"

#include "../../dependencies/rawdata/person.hpp"
#include "../../dependencies/rawdata/skeleton.hpp"
#include "../../dependencies/libs/imgui_tricks.hpp"

#include "../../dependencies/xorstring/xorstring.hpp"

#include "image/image.hpp"
#include "config/config.hpp"

#include "../../modules/visuals/visuals.hpp"

#include "../../modules/visuals/visuals.hpp"
#include "../../modules/visuals/loot/loot.hpp"

#include "../../modules/visuals/extracts/extracts.hpp"
#include "../../modules/visuals/grenades/grenades.hpp"

#include "../../modules/aimbot/aimbot.hpp"

#include "../../settings/settings.hpp"

static auto current_tab = 0;

void Draw_Main( float, float )
{
    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 0,0 } );

    ImGui::SetCursorPos( { 24, 235 } );
    ImGui::BeginChild( xs( "Main" ), { 337, 530 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove );
    {
        switch ( current_tab )
        {
        case 0:
            renderer::imgui::elements::functions::checkbox( xs( "Aimbot" ), xs( "enables aimbot" ), std::get< bool >( config::config_map[features::aimbot_enable ] ) );

            renderer::imgui::elements::functions::hotkey( "##1", &std::get< int >( config::config_map[features::aimbot_hotkey ] ) );

            renderer::imgui::elements::functions::separator( );
                
            renderer::imgui::elements::functions::information( xs( "Smoothness" ), xs( "sets the smoothness of the aimbot" ), std::get< int >( config::config_map[features::aimbot_smoothness ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##2", &std::get< int >( config::config_map[features::aimbot_smoothness ] ), 0, 10, "" );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::information( xs( "Bone" ), xs( "assigns the aiming bone" ), std::get< int >( config::config_map[features::aimbot_bone ] ), "%i" );

            renderer::imgui::elements::functions::combo( "##3", &std::get< int >( config::config_map[features::aimbot_bone ] ), "head\0\rneck\0\rpelvis\0\rleft_thigh\0\rright_thigh\0\0", 1 );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::information( xs( "Max Distance" ), xs( "sets the distance interval to aim at" ), std::get< int >( config::config_map[features::aimbot_max_distance ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##4", &std::get< int >( config::config_map[features::aimbot_max_distance ] ), 0, 500, "" );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Ignore AI" ), xs( "skips over AI players" ), std::get< bool >( config::config_map[features::aimbot_ignore_ai ] ) );

            renderer::imgui::elements::functions::checkbox( xs( "Show FOV" ), xs( "shows circular FOV" ), std::get< bool >( config::config_map[features::aimbot_show_fov ] ) );

            renderer::imgui::elements::functions::information( xs( "FOV" ), xs( "sets the aimbot fov" ), std::get< int >( config::config_map[features::aimbot_fov ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##5", &std::get< int >( config::config_map[features::aimbot_fov] ), 0, 100, "" );

            renderer::imgui::elements::functions::color_picker( "##6", std::get< engine::structures::vector4_t >( config::config_map[features::fov_color ] ).color );

            renderer::imgui::elements::functions::information( xs( "FOV Segments" ), xs( "adjusts the aimbot fov shape" ), std::get< int >( config::config_map[features::aimbot_fov_segments ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##7", &std::get< int >( config::config_map[features::aimbot_fov_segments ] ), 0, 10, "" );
            break;
        case 1:
            renderer::imgui::elements::functions::information( xs( "Rage Features" ), xs( "this tab is only available to premium members" ) );
            break;
        case 2:   
            if constexpr (settings::values::premium)
            {
                renderer::imgui::elements::functions::checkbox(xs("Loot Through Walls"), xs("extends interaction range"), std::get< bool >(config::config_map[features::loot_through_walls]));
            }

            if constexpr (settings::values::premium)
            {
                renderer::imgui::elements::functions::checkbox(xs("No Inertia"), xs("removes inertia"), std::get< bool >(config::config_map[features::no_inertia]));
            }
            renderer::imgui::elements::functions::checkbox(xs("No Recoil"), xs("removes recoil"), std::get< bool >(config::config_map[features::enable_no_recoil]));

            if ( std::get< bool >( config::config_map[features::enable_no_recoil ] ) )
            {
                renderer::imgui::elements::functions::information( xs( "Recoil X" ), xs( "sets the vertical recoil value" ), std::get< int >( config::config_map[features::recoil_x ] ), "%i" );
                renderer::imgui::elements::functions::slider_int( "##3", &std::get< int >( config::config_map[features::recoil_x ] ), 0, 10, "" );

                renderer::imgui::elements::functions::information( xs( "Recoil Y" ), xs( "sets the vertical horizontal value" ), std::get< int >( config::config_map[features::recoil_y ] ), "%i" );
                renderer::imgui::elements::functions::slider_int( "##4", &std::get< int >( config::config_map[features::recoil_y ] ), 0, 10, "" );

                renderer::imgui::elements::functions::information( xs( "Recoil Depth X" ), xs( "sets the verrtical depth recoil value" ), std::get< int >( config::config_map[features::recoil_depth_x ] ), "%i" );
                renderer::imgui::elements::functions::slider_int( "##5", &std::get< int >( config::config_map[features::recoil_depth_x ] ), 0, 10, "" );

                renderer::imgui::elements::functions::information( xs( "Recoil Depth Y" ), xs( "sets the horizontal depth recoil value" ), std::get< int >( config::config_map[features::recoil_depth_y ] ), "%i" );
                renderer::imgui::elements::functions::slider_int( "##6", &std::get< int >( config::config_map[features::recoil_depth_y ] ), 0, 10, "" );

                renderer::imgui::elements::functions::information( xs( "Recoil Random X" ), xs( "sets the random x recoil value" ), std::get< int >( config::config_map[features::recoil_random_x ] ), "%i" );
                renderer::imgui::elements::functions::slider_int( "##7", &std::get< int >( config::config_map[features::recoil_random_x ] ), 0, 10, "" );

                renderer::imgui::elements::functions::information( xs( "Recoil Random Y" ), xs( "sets the random y recoil value" ), std::get< int >( config::config_map[features::recoil_random_y ] ), "%i" );
                renderer::imgui::elements::functions::slider_int( "##8", &std::get< int >( config::config_map[features::recoil_random_y ] ), 0, 10, "" );
            }

            renderer::imgui::elements::functions::checkbox( xs( "No Sway" ), xs( "removes sway" ), std::get< bool >( config::config_map[features::enable_no_sway ] ) );

            if ( std::get< bool >( config::config_map[features::enable_no_sway ] ) )
            {
                renderer::imgui::elements::functions::information( xs( "Sway Intensity" ), xs( "sets the sway intensity" ), std::get< int >( config::config_map[features::sway_intensity ] ), "%i" );
                renderer::imgui::elements::functions::slider_int( "##2", &std::get< int >( config::config_map[features::sway_intensity ] ), 0, 10, "" );
            }

            renderer::imgui::elements::functions::checkbox( xs( "No Stamina" ), xs( "removes stamina" ), std::get< bool >( config::config_map[features::enable_no_stamina ] ) );

            renderer::imgui::elements::functions::checkbox( xs( "Night Vision" ), xs( "enables player night vision" ), std::get< bool >( config::config_map[features::enable_night_vision ] ) );

            renderer::imgui::elements::functions::checkbox( xs( "Thermal Vision" ), xs( "enables player thermal vision" ), std::get< bool >( config::config_map[features::enable_thermal_vision] ) );

            renderer::imgui::elements::functions::checkbox( xs( "Remove Visor" ), xs( "removes player visor only visually" ), std::get< bool >( config::config_map[features::enable_remove_visor ] ) );

            if ( std::get< bool >( config::config_map[features::enable_remove_visor ] ) )
            {
                renderer::imgui::elements::functions::information( xs( "Visor Intensity" ), xs( "sets the Visor intensity" ), std::get< int >( config::config_map[features::visor_intensity ] ), "%i" );
                renderer::imgui::elements::functions::slider_int("##1", &std::get< int >(config::config_map[features::visor_intensity]), 0, 1, "");
            }
            break;
        case 3:
            renderer::imgui::elements::functions::information( xs( "Max Distance" ), xs( "sets the max visual distance" ), std::get< int >( config::config_map[features::display_max_distance ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##1", &std::get< int >( config::config_map[features::display_max_distance ] ), 0, 1000, "" );

            if ( std::get< bool >( config::config_map[features::display_items] ) )
            {
                renderer::imgui::elements::functions::separator( );

                renderer::imgui::elements::functions::information( xs( "Max Item Distance" ), xs( "sets the max item visual distance" ), std::get< int >( config::config_map[features::display_item_max_distance] ), "%i" );

                renderer::imgui::elements::functions::slider_int( "##2", &std::get< int >( config::config_map[features::display_item_max_distance] ), 0, 1000, "" );

                renderer::imgui::elements::functions::separator( );

                renderer::imgui::elements::functions::information( xs( "Max Price" ), xs( "sets the max item visual price to render" ), std::get< int >( config::config_map[features::display_item_max_price] ), "%i" );

                renderer::imgui::elements::functions::slider_int( "##4", &std::get< int >( config::config_map[features::display_item_max_price] ), 0, 10000000, "" );

                renderer::imgui::elements::functions::separator( );

                renderer::imgui::elements::functions::information( xs( "Min Price" ), xs( "sets the min item visual price to render" ), std::get< int >( config::config_map[features::display_item_min_price] ), "%i" );

                renderer::imgui::elements::functions::slider_int( "##5", &std::get< int >( config::config_map[features::display_item_min_price] ), 0, 100000, "" );
            }

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Extractions" ), xs( "draws extractions" ), std::get< bool >( config::config_map[features::display_extractions ] ) );

            renderer::imgui::elements::functions::color_picker( "##6", std::get< engine::structures::vector4_t >( config::config_map[features::extraction_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Grenades" ), xs( "draws grenades" ), std::get< bool >( config::config_map[features::display_grenades ] ) );

            renderer::imgui::elements::functions::color_picker( "##7", std::get< engine::structures::vector4_t >( config::config_map[features::grenade_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Items" ), xs( "draws items" ), std::get< bool >( config::config_map[features::display_items ] ) );

            renderer::imgui::elements::functions::color_picker( "##8", std::get< engine::structures::vector4_t >( config::config_map[features::item_color ] ).color );

            if ( std::get< bool >( config::config_map[features::display_items] ) )
            {
                renderer::imgui::elements::functions::information( xs( "Expensive Item Color" ), xs( "sets the color of the expensive items" ) );

                renderer::imgui::elements::functions::color_picker( "##9", std::get< engine::structures::vector4_t >( config::config_map[features::expensive_item_color] ).color );
            }

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Corpses" ), xs( "draws corpse position" ), std::get< bool >( config::config_map[features::display_corpses] ) );

            renderer::imgui::elements::functions::color_picker( "##10", std::get< engine::structures::vector4_t >( config::config_map[features::corpse_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Bounding Box" ), xs( "draws a rectangular box on entities" ), std::get< bool >( config::config_map[features::display_bounding_box] ) );

            renderer::imgui::elements::functions::color_picker( "##11", std::get< engine::structures::vector4_t >( config::config_map[features::bounding_box_color] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Filled Box" ), xs( "draws a filled rectangular box on entities" ), std::get< bool >( config::config_map[features::display_filled_box ] ) );

            renderer::imgui::elements::functions::color_picker( "##12", std::get< engine::structures::vector4_t >( config::config_map[features::filled_box_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Corner Box" ), xs( "draws a cornered box on entities" ), std::get< bool >( config::config_map[features::display_corner_box ] ) );

            renderer::imgui::elements::functions::color_picker( "##13", std::get< engine::structures::vector4_t >( config::config_map[features::corner_box_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Head Hitbox" ), xs( "draws a head square on entities" ), std::get< bool >( config::config_map[features::display_head_hitbox ] ) );

            renderer::imgui::elements::functions::color_picker( "##14", std::get< engine::structures::vector4_t >( config::config_map[features::hitbox_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Skeleton" ), xs( "draws skeletons on entities" ), std::get< bool >( config::config_map[features::display_skeleton ] ) );

            renderer::imgui::elements::functions::color_picker( "##15", std::get< engine::structures::vector4_t >( config::config_map[features::skeleton_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Players Name" ), xs( "draws players name" ), std::get< bool >( config::config_map[features::display_players_name] ) );

            renderer::imgui::elements::functions::color_picker( "##16", std::get< engine::structures::vector4_t >( config::config_map[features::player_names_color] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Players Type" ), xs( "draws player type name" ), std::get< bool >( config::config_map[features::display_players_type] ) );

            renderer::imgui::elements::functions::color_picker( "##17", std::get< engine::structures::vector4_t >( config::config_map[features::player_types_color ] ).color );

            renderer::imgui::elements::functions::separator( );    

            renderer::imgui::elements::functions::checkbox( xs( "Distance" ), xs( "draws distance of entity" ), std::get< bool >( config::config_map[features::display_distance ] ) );

            renderer::imgui::elements::functions::color_picker( "##18", std::get< engine::structures::vector4_t >( config::config_map[features::distance_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Health" ), xs ( "draws health of entity" ), std::get< bool >( config::config_map[features::display_health ] ) );

            renderer::imgui::elements::functions::color_picker( "##19", std::get< engine::structures::vector4_t >( config::config_map[features::health_color ] ).color );
            break;
        case 4:
            renderer::imgui::elements::functions::checkbox( xs( "Watermark" ), xs( "show/hide the watermark" ), renderer::values::renderer->is_watermark_shown );

            renderer::imgui::elements::functions::checkbox( xs( "Hide Preview" ), xs( "show/hide the visual preview" ), std::get< bool >( config::config_map[features::hide_preview ] ) );

            renderer::imgui::elements::functions::separator( );

            if ( renderer::imgui::elements::functions::button( xs( "Save Config" ), { 328, 35 }, renderer::imgui::elements::button_flags::BUTTON_GRAY ) )
                config::save( );

            ImGui::NewLine( );

            if ( renderer::imgui::elements::functions::button( xs( "Load Config" ), { 328, 35 }, renderer::imgui::elements::button_flags::BUTTON_BLUE ) )
                config::load( );
            break;
        default:
            break;
        }
    }
    ImGui::EndChild( );

    ImGui::PopStyleVar( );
}

void renderer::menu::draw( )
{
    {
        modules::aimbot::visual::draw_fov( );
    }

    std::unique_lock<std::mutex> initiate_thread_locker(engine::values::global_locker);

    const auto camera_copy = engine::values::camera;

    const auto local_game_world_copy = engine::values::local_game_world;

    initiate_thread_locker.unlock();

    std::unique_lock<std::mutex> update_players_locker(engine::values::player_vector_locker);

    const auto players_copy = engine::values::players;

    const auto local_player_copy = local_game_world_copy.get_local_player();

    update_players_locker.unlock();

    std::unique_lock<std::mutex> update_closest_player_locker(engine::values::closest_player_locker);
    const auto closest_player = engine::values::closest_player;
    update_closest_player_locker.unlock();

    const auto root_world_position_lsp = local_player_copy.get_position();

    modules::visuals::callback( camera_copy, local_game_world_copy, players_copy, local_player_copy, closest_player, root_world_position_lsp );

    std::unique_lock<std::mutex> update_items_locker(engine::values::loot_vector_locker);

    const auto items_copy = engine::values::loot;

    update_items_locker.unlock();

    std::unique_lock<std::mutex> update_grenades_locker(engine::values::grenade_vector_locker);

    const auto grenades_copy = engine::values::grenades;

    update_grenades_locker.unlock();

    modules::visuals::loot::callback(camera_copy, local_game_world_copy, items_copy, local_player_copy, closest_player, root_world_position_lsp);

    modules::visuals::extracts::callback(camera_copy, local_game_world_copy, local_player_copy, root_world_position_lsp);

    modules::visuals::grenades::callback(camera_copy, local_game_world_copy, grenades_copy, local_player_copy, closest_player, root_world_position_lsp);

    if ( renderer::values::renderer->is_menu_open )
    {
        ImGui::SetNextWindowSize( { 375, 770 }, ImGuiCond_Once );

        ImVec2 MainPosition = { 0, 0 };

        ImGui::Begin( xs( "EscapeFromTarkov" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove );
        {
            auto DrawList = ImGui::GetWindowDrawList( );
            MainPosition = ImGui::GetWindowPos( );

            DrawList->AddText(renderer::imgui::elements::fonts::druk_wide_bold, 30, { MainPosition.x + 24, MainPosition.y + 36 }, ImColor{ 255, 255, 255 }, settings::values::premium ? xs("EFT PREMIUM") : xs("EFT LITE"));

            DrawList->AddRectFilled( { MainPosition.x + 24, MainPosition.y + 73 }, { MainPosition.x + 24 + 187, MainPosition.y + 73 + 2 }, ImColor{ 255, 255, 255, 10 } );
            DrawList->AddRectFilled( { MainPosition.x + 24, MainPosition.y + 73 }, { MainPosition.x + 24 + 88, MainPosition.y + 73 + 2 }, ImColor{ 138, 194, 227 } );

            ImGui::SetCursorPos( { 24, 107 } );

            ImGui::BeginGroup( );
            {
                renderer::imgui::elements::functions::tab( xs( "LEGIT" ), (const char*)ICON_FA_SHIELD_CHECK, current_tab, 0 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "RAGE" ), (const char*)ICON_FA_SHIELD, current_tab, 1 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "MISC" ), (const char*)ICON_FA_FOLDER, current_tab, 2 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "VISUALS" ), (const char*)ICON_FA_BOLT, current_tab, 3 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "CFG" ), (const char*)ICON_FA_ARCHIVE, current_tab, 4 );

            }
            ImGui::EndGroup( );

            Draw_Main( 0.f, 0.f );
        } ImGui::End( );

        if ( !std::get< bool >( config::config_map[features::hide_preview] ) )
        {
            ImGui::SetNextWindowSize( { 365, 540 }, ImGuiCond_Once );
            auto PreviewPosition = ImVec2{ MainPosition.x + 385, MainPosition.y };
            ImGui::SetNextWindowPos( PreviewPosition );

            ImGui::Begin( xs( "Preview" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove );
            {
                auto DrawList = ImGui::GetWindowDrawList( );
                auto CurrentWindowDrawList = ImGui::GetCurrentWindow( )->DrawList;

                DrawList->AddText( renderer::imgui::elements::fonts::druk_wide_bold, 24, { PreviewPosition.x + 24, PreviewPosition.y + 26 }, ImColor{ 255, 255, 255 }, xs( "Visual Preview" ) );

                DrawList->AddRectFilled( { PreviewPosition.x + 24, PreviewPosition.y + 57 }, { PreviewPosition.x + 24 + 187, PreviewPosition.y + 57 + 2 }, ImColor{ 255, 255, 255, 10 } );
                DrawList->AddRectFilled( { PreviewPosition.x + 24, PreviewPosition.y + 57 }, { PreviewPosition.x + 24 + 88, PreviewPosition.y + 57 + 2 }, ImColor{ 138, 194, 227 } );

                auto RelativeX = ( PreviewPosition.x / 2 ) - 85;
                const engine::structures::vector2_t head_screen_position{ PreviewPosition.x + RelativeX - 85, PreviewPosition.y + 95 };
                const engine::structures::vector2_t root_screen_position{ PreviewPosition.x + RelativeX + 190, PreviewPosition.y + 510 };

                const float width = std::fabs( head_screen_position.y - root_screen_position.y ) / 4.0f;
                const float height = std::fabs( head_screen_position.y - root_screen_position.y ) * 1.2f;

                // To move the object right, subtract from the RelativeX and add the same value to the max value on the right
                // To move the object left, do the opposite

                CurrentWindowDrawList->AddImageRounded( image::d3dx::player_image, PreviewPosition + ImVec2( RelativeX - 85, 95 ), PreviewPosition + ImVec2( RelativeX + 190, 510 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), ImColor( 255, 255, 255 ), 17, 15 );

                if ( std::get< bool >( config::config_map[features::display_bounding_box] ) )
                {
                    auto color = std::get< engine::structures::vector4_t >( config::config_map[features::bounding_box_color] ).color;

                    CurrentWindowDrawList->AddRect( PreviewPosition + ImVec2( RelativeX - 85, 95 ), PreviewPosition + ImVec2( RelativeX + 175, 515 ), ImColor{ color[0], color[1], color[2], color[3] } );
                }

                if ( std::get< bool >( config::config_map[features::display_filled_box] ) )
                {
                    const auto col = std::get< engine::structures::vector4_t >( config::config_map[features::filled_box_color] );

                    {
                        CurrentWindowDrawList->AddRectFilled( PreviewPosition + ImVec2( RelativeX - 85, 95 ), PreviewPosition + ImVec2( RelativeX + 175, 515 ), ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] } );
                    }
                }

                if ( std::get< bool >( config::config_map[features::display_head_hitbox] ) )
                {
                    const auto col = std::get< engine::structures::vector4_t >( config::config_map[features::hitbox_color] );

                    {
                        CurrentWindowDrawList->AddCircle( { root_screen_position.x - 152, root_screen_position.y - 375 }, 8.f, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] } );
                    }
                }

                if ( std::get< bool >( config::config_map[features::display_corner_box] ) && !std::get< bool >( config::config_map[features::display_bounding_box] ) )
                {
                    const auto col = std::get< engine::structures::vector4_t >( config::config_map[features::corner_box_color] );

                    {
                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 275, root_screen_position.y - 415 }, { root_screen_position.x - 255, root_screen_position.y - 415 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );
                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 275, root_screen_position.y - 415 }, { root_screen_position.x - 275, root_screen_position.y - 395 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );

                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 16, root_screen_position.y - 415 }, { root_screen_position.x - 41, root_screen_position.y - 415 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );
                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 16, root_screen_position.y - 415 }, { root_screen_position.x - 16, root_screen_position.y - 395 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );

                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 275, root_screen_position.y + 5 }, { root_screen_position.x - 255, root_screen_position.y + 5 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );
                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 275, root_screen_position.y + 5 }, { root_screen_position.x - 275, root_screen_position.y - 20 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );

                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 16, root_screen_position.y + 5 }, { root_screen_position.x - 41, root_screen_position.y + 5 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );
                        CurrentWindowDrawList->AddLine( { root_screen_position.x - 16, root_screen_position.y + 5 }, { root_screen_position.x - 16, root_screen_position.y - 20 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f );
                    }
                }

                if ( std::get< bool >( config::config_map[features::display_skeleton] ) )
                {
                    auto col = std::get< engine::structures::vector4_t >( config::config_map[features::skeleton_color] );

                    CurrentWindowDrawList->AddImageRounded( image::d3dx::skeleton_image, PreviewPosition + ImVec2( RelativeX - 85, 95 ), PreviewPosition + ImVec2( RelativeX + 190, 510 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 17, 15 );
                }

                if ( std::get< bool >( config::config_map[features::display_players_name] ) )
                {
                    std::string to_draw = "PLAYER123";

                    const auto col = std::get< engine::structures::vector4_t >( config::config_map[features::player_names_color] );

                    {
                        CurrentWindowDrawList->AddOutlinedText( renderer::imgui::elements::fonts::bender_font_reg, 12.5f, { root_screen_position.x - 275, root_screen_position.y - 430 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str( ) );
                    }
                }

                if ( std::get< bool >( config::config_map[features::display_players_type] ) )
                {
                    std::string to_draw = "PLAYER";

                    const auto col = std::get< engine::structures::vector4_t >( config::config_map[features::player_types_color] );

                    {
                        CurrentWindowDrawList->AddOutlinedText( renderer::imgui::elements::fonts::bender_font_reg, 12.5f, { root_screen_position.x - 10, root_screen_position.y - 412 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str( ) );
                    }
                }

                if ( std::get< bool >( config::config_map[features::display_health] ) )
                {
                    std::string to_draw = "440HP";

                    const auto col = std::get< engine::structures::vector4_t >( config::config_map[features::health_color] );

                    {
                        CurrentWindowDrawList->AddRectFilled( PreviewPosition + ImVec2( RelativeX - 105, 95 ), PreviewPosition + ImVec2( RelativeX - 95, 515 ), ImColor { 0.f, 255.f, 0.f } );
                    }

                    {
                        CurrentWindowDrawList->AddRectFilled( PreviewPosition + ImVec2( RelativeX - 108, 94 ), PreviewPosition + ImVec2( RelativeX - 94, 516 ), ImColor{ 0.f, 0.f, 0.f, 0.2f } );
                    }

                    {
                        CurrentWindowDrawList->AddOutlinedText( renderer::imgui::elements::fonts::bender_font_reg, 12.5f, { root_screen_position.x - 10, root_screen_position.y - 400 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str( ) );
                    }
                }

                if ( std::get< bool >( config::config_map[features::display_distance] ) )
                {
                    std::string to_draw = "100M";

                    const auto col = std::get< engine::structures::vector4_t >( config::config_map[features::distance_color] );

                    {
                        CurrentWindowDrawList->AddOutlinedText( renderer::imgui::elements::fonts::bender_font_reg, 12.5f, { root_screen_position.x - 10, root_screen_position.y - 388 }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str( ) );
                    }
                }
            }
            ImGui::End( );
        }
    }
    else if ( renderer::values::renderer->is_watermark_shown )
    {
        ImGui::Begin( xs( "UnityWndClass" ), nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove );
        { 
            auto DrawList = ImGui::GetWindowDrawList( );

            ImGui::SetWindowSize( { 220, 40 } );

            ImGui::SetWindowPos( { 0, 0 } );

            //DrawList->AddText( renderer::imgui::elements::fonts::druk_wide_bold, 25, { 15, 5 }, ImColor{ 255, 255, 255, 70 }, xs( "loaded" ) );

            DrawList->AddImageRounded( image::d3dx::intellisense_icon, 
                ImVec2 { 15, 10 },
                ImVec2 { 40, 35 }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 },
                ImColor { 1.f, 1.f, 1.f, 0.5f }, 0, 15 );
        }
        ImGui::End( );
    }
}