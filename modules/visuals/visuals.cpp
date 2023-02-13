#include "visuals.hpp"

#include <mutex>
#include <cmath>

#include "../../renderer/renderer.hpp"
#include "../../renderer/menu/config/config.hpp"
#include "../../settings/settings.hpp"

void modules::visuals::callback( engine::types::camera_t camera_copy, engine::types::local_game_world_t local_game_world_copy, const std::vector<engine::types::player_t>& players_copy, engine::types::player_t local_player_copy, engine::types::player_t closest_player, engine::structures::vector3_t lp_pos )
{
	if ( !camera_copy.instance || !local_game_world_copy.instance )
		return;

	const auto draw_list = ImGui::GetBackgroundDrawList( );

	if constexpr ( settings::values::premium )
	{
		if (closest_player.instance)
		{
			const auto fac = local_player_copy.get_procedural_weapon_animation().get_firearm_controller().instance;

			if (fac)
			{

				auto fireport_position = driver.read< engine::types::transform_t >(local_player_copy.instance, { 0x578, 0x148, 0x10 }).get_position(); //PlayerBones

				const auto gun_base = driver.read< engine::types::transform_t >(fac + 0xf8).get_position();

				const auto weapon_ln = driver.read<float>(fac + 0x150);

				auto head_world_position = closest_player.get_bone_transform(engine::structures::bones::head).get_position();

				const auto from_sp = camera_copy.world_to_screen(gun_base + (engine::structures::vector3_t{ fireport_position - gun_base }.normalized() * weapon_ln));

				const auto head_sp = camera_copy.world_to_screen(head_world_position);

				const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::fov_color]);

				if (head_sp && from_sp)
					draw_list->AddLine({ head_sp->x, head_sp->y }, { from_sp->x, from_sp->y }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 2.f);
			}
		}
	}

	for ( const auto player : players_copy )
	{
		if ( !player.instance )
			continue;

		const auto root_world_position = player.get_position( );

		const auto head_world_position = player.get_bone_transform( engine::structures::bones::head ).get_position( );
		const auto root_postion = camera_copy.world_to_screen( root_world_position );

		const auto head_position = camera_copy.world_to_screen( head_world_position );

		if ( !root_postion.has_value( ) || !head_position.has_value( ) )
			continue;

		const auto root_screen_position = root_postion.value();
		const auto head_screen_position = head_position.value();

		const auto distance = static_cast<std::uint32_t>( root_world_position.distance(lp_pos) );

		if (distance > std::get< int >(config::config_map[features::display_max_distance]))
			continue;

		const float width = std::fabs(head_screen_position.y - root_screen_position.y) / 4.0f;
		const float height = std::fabs(head_screen_position.y - root_screen_position.y) * 1.2f;

		const auto line_len_width = width / 2.5f;

		auto [current_health, max_health] = player.get_health_controller().get_health();

		const auto new_width_health = (width) / max_health * current_health;
		const auto new_height_health = (height) / max_health * current_health;

		if (std::get< bool >(config::config_map[features::display_health]))
		{
			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::health_color]);

			{
				draw_list->AddRectFilled({ root_screen_position.x - width - (width / 10.f) / 4.0f - 5, root_screen_position.y - 2 }, { root_screen_position.x - width - (width / 10.f) - 7, root_screen_position.y - new_height_health }, ImColor{ (255.f - 2.55f * current_health) / 255.0f, (2.55f * current_health) / 255.0f, 0.f });
			}

			{
				draw_list->AddRectFilled({ root_screen_position.x - width - (width / 10.f) / 4.0f - 5, root_screen_position.y - 2 }, { root_screen_position.x - width - (width / 10.f) - 7, root_screen_position.y - height - 1 }, ImColor{ 0.f, 0.f, 0.f, 0.2f });
			}

			std::string to_draw = std::to_string(static_cast<int>(current_health)); to_draw.append("HP");

			draw_list->AddOutlinedText(renderer::imgui::elements::fonts::bender_font_reg, 10.6f, { root_screen_position.x + width + 5.f, root_screen_position.y - height + 14.f }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str());
		}

		if (std::get< bool >(config::config_map[features::display_bounding_box]))
		{
			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::bounding_box_color]);

			{
				draw_list->AddRect({ root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x + width, root_screen_position.y - height }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 0, 0, 1.8f);
			}
		}

		if (std::get< bool >(config::config_map[features::display_filled_box]))
		{
			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::filled_box_color]);

			{
				draw_list->AddRectFilled({ root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x + width, root_screen_position.y - height }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] });
			}
		}

		if (std::get< bool >(config::config_map[features::display_players_type]))
		{
			std::string to_draw = player.get_profile_controller().get_profile_information().player_type();

			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::player_types_color]);

			{
				draw_list->AddOutlinedText(renderer::imgui::elements::fonts::bender_font_reg, 10.8f, { root_screen_position.x + width + 5.f, root_screen_position.y - height }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str());
			}
		}

		if (std::get< bool >(config::config_map[features::display_players_name]))
		{
			std::string to_draw = player.get_profile_controller( ).get_profile_information( ).player_name( );

			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::player_names_color]);

			const auto text_width = ImGui::CalcTextSize( to_draw.c_str( ) ).x;

			if ( player.get_profile_controller( ).get_profile_information( ).player_type( ) == "Player" )
			{
				draw_list->AddOutlinedText(renderer::imgui::elements::fonts::bender_font_reg, 10.8f, { ( root_screen_position.x - ( width / 2 ) - ( text_width / 2 ) ), root_screen_position.y - height - 15.f }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str());
			}
		}

		if (std::get< bool >(config::config_map[features::display_corner_box]) && !std::get< bool >(config::config_map[features::display_bounding_box]))
		{
			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::corner_box_color]);

			{
				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y - height }, { root_screen_position.x - width + line_len_width, root_screen_position.y - height }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);
				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y - height }, { root_screen_position.x - width, (root_screen_position.y - height) + line_len_width }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);

				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y - height }, { root_screen_position.x + width - line_len_width, root_screen_position.y - height }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);
				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y - height }, { root_screen_position.x + width, (root_screen_position.y - height) + line_len_width }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);

				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x - width + line_len_width, root_screen_position.y }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);
				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x - width, root_screen_position.y - line_len_width }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);

				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y }, { root_screen_position.x + width - line_len_width, root_screen_position.y }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);
				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y }, { root_screen_position.x + width, root_screen_position.y - line_len_width }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, 1.8f);
			}
		}

		if (std::get< bool >(config::config_map[features::display_head_hitbox]))
		{
			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::hitbox_color]);

			{
				draw_list->AddCircle({ head_screen_position.x, head_screen_position.y }, 5.f, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] });
			}
		}

		if (std::get< bool >(config::config_map[features::display_skeleton]) && distance <= 150)
		{
			engine::types::transform_t bones[134];

			player.get_bone_transforms(bones);

			const auto r_forearm_pos = bones[engine::structures::engine_bones::HumanRForearm1].get_position(); const auto r_forearm_spos = camera_copy.world_to_screen(r_forearm_pos);
			const auto l_forearm_pos = bones[engine::structures::engine_bones::HumanLForearm1].get_position(); const auto l_forearm_spos = camera_copy.world_to_screen(l_forearm_pos);

			const auto r_digit_pos = bones[engine::structures::engine_bones::HumanRDigit11].get_position(); const auto r_digit_spos = camera_copy.world_to_screen(r_digit_pos);
			const auto l_digit_pos = bones[engine::structures::engine_bones::HumanLDigit11].get_position(); const auto l_digit_spos = camera_copy.world_to_screen(l_digit_pos);

			const auto spine1_pos = bones[engine::structures::engine_bones::HumanSpine1].get_position(); const auto spine1_spos = camera_copy.world_to_screen(spine1_pos);
			const auto spine2_pos = bones[engine::structures::engine_bones::HumanPelvis].get_position(); const auto spine2_spos = camera_copy.world_to_screen(spine2_pos);

			const auto r_calf_pos = bones[engine::structures::engine_bones::HumanRCalf].get_position(); const auto r_calf_spos = camera_copy.world_to_screen(r_calf_pos);
			const auto l_calf_pos = bones[engine::structures::engine_bones::HumanLCalf].get_position(); const auto l_calf_spos = camera_copy.world_to_screen(l_calf_pos);

			const auto r_toe_pos = bones[engine::structures::engine_bones::HumanRToe].get_position(); const auto r_toe_spos = camera_copy.world_to_screen(r_toe_pos);
			const auto l_toe_pos = bones[engine::structures::engine_bones::HumanLToe].get_position(); const auto l_toe_spos = camera_copy.world_to_screen(l_toe_pos);

			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::skeleton_color]);

			const auto color = ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] };

			if( /* >:) */
				r_forearm_spos.has_value() &&l_forearm_spos.has_value() &&
				r_digit_spos.has_value() && l_digit_spos.has_value() && 
				spine1_spos.has_value() && spine2_spos.has_value() &&
				r_calf_spos.has_value() && l_calf_spos.has_value() &&
				r_toe_spos.has_value() && l_toe_spos.has_value()
				)
			{
				draw_list->AddLine({ head_screen_position.x, head_screen_position.y }, { spine1_spos.value().x, spine1_spos.value().y }, color);

				draw_list->AddLine({ head_screen_position.x, head_screen_position.y }, { r_forearm_spos.value().x, r_forearm_spos.value().y }, color);

				draw_list->AddLine({ head_screen_position.x, head_screen_position.y }, { l_forearm_spos.value().x, l_forearm_spos.value().y }, color);

				draw_list->AddLine({ r_forearm_spos.value().x, r_forearm_spos.value().y }, { r_digit_spos.value().x, r_digit_spos.value().y }, color);

				draw_list->AddLine({ l_forearm_spos.value().x, l_forearm_spos.value().y }, { l_digit_spos.value().x, l_digit_spos.value().y }, color);

				draw_list->AddLine({ spine1_spos.value().x, spine1_spos.value().y }, { spine2_spos.value().x, spine2_spos.value().y }, color);

				draw_list->AddLine( { spine2_spos.value().x, spine2_spos.value().y }, { r_calf_spos.value().x, r_calf_spos.value().y }, color);

				draw_list->AddLine({ r_calf_spos.value().x, r_calf_spos.value().y }, { r_toe_spos.value().x, r_toe_spos.value().y }, color);

				draw_list->AddLine({ spine2_spos.value().x, spine2_spos.value().y }, { l_calf_spos.value().x, l_calf_spos.value().y }, color);

				draw_list->AddLine({ l_calf_spos.value().x, l_calf_spos.value().y }, { l_toe_spos.value().x, l_toe_spos.value().y }, color);
			}
		}

		if (std::get< bool >(config::config_map[features::display_distance]))
		{
			std::string to_draw; to_draw.append(std::to_string(distance)); to_draw.append("M");

			const auto col = std::get< engine::structures::vector4_t >(config::config_map[features::distance_color]);

			{
				draw_list->AddOutlinedText(renderer::imgui::elements::fonts::bender_font_reg, 10.6f, { root_screen_position.x + width + 5.f, root_screen_position.y - height + 7.f }, ImColor{ col.color[0], col.color[1], col.color[2], col.color[3] }, to_draw.c_str());
			}
		}
	}
}
