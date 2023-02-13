#pragma once

namespace engine::offsets
{
	constexpr auto game_object_manager = 0x17FFD28;
}

namespace engine::offsets::object
{
	constexpr auto last_tagged_object = 0x0;
	constexpr auto first_tagged_object = 0x8;

	constexpr auto last_camera_tagged_object = 0x10;
	constexpr auto first_camera_tagged_object = 0x18;

	constexpr auto last_active_object = 0x20;
	constexpr auto first_active_object = 0x28;
}

namespace engine::offsets::base_object
{
	constexpr auto next_base_object = 0x8;
	constexpr auto game_object = 0x10;
}

namespace engine::offsets::game_object
{
	constexpr auto class_object = 0x30;
	constexpr auto name = 0x60;
}

namespace engine::offsets::class_object
{
	constexpr auto transform = 0x8;
	constexpr auto object_reference = 0x18;
}

namespace engine::offsets::game_world
{
	constexpr auto self = 0x28;

    constexpr auto loot_list = 0x70;
	constexpr auto registered_players = 0x90;

    constexpr auto grenade_list = 0x108;

    constexpr auto exfil_class = 0x18;
}

namespace engine::offsets::loot
{
    constexpr auto interactive_class = 0x28;
    constexpr auto item_class = 0xB0;

    constexpr auto item_profile = 0x40;
    constexpr auto item_cached_object = 0x50;

    constexpr auto object_id = 0x14;
}

namespace engine::offsets::exfil_class
{
    constexpr auto exfil_points = 0x20;

    constexpr auto scav_exfil_points = 0x28;
}

namespace engine::offsets::generic_list
{
	constexpr auto elements = 0x10;
	constexpr auto size = 0x18;

	constexpr auto first_element = 0x20;
}

namespace engine::offsets::fps_camera
{
	constexpr auto view_matrix = 0xDC;
}

namespace engine::offsets::player
{
	constexpr auto player_body = 0xA8;
	constexpr auto procedural_weapon_animation = 0x198;
	constexpr auto health_controller = 0x520;
    constexpr auto hands_controller = 0x538;
	constexpr auto physical = 0x4F8;

    constexpr auto profile = 0x4E8;
}

namespace engine::offsets::profile
{
    constexpr auto information = 0x28;
    constexpr auto settings = 0x48;
}

namespace engine::offsets::physical
{
	constexpr auto inertia = 0x98;
    constexpr auto stamina = 0x38;

    constexpr auto current_stamina = 0x48;
}

namespace engine::offsets::skills
{
    constexpr auto load_speed = 0x170;
    constexpr auto unload_speed = 0x178;
    constexpr auto instant_check = 0x190;
}

namespace engine::offsets::shot_effector
{
    constexpr auto recoil_strength_xy = 0x40;
    constexpr auto recoil_strength_z = 0x48;
    constexpr auto recoil_degree = 0x50;
    constexpr auto recoil_direction = 0x58;
    constexpr auto recoil_radian = 0x64;
    constexpr auto shot_vals = 0x10;
    constexpr auto stiffness = 0x6C;
    constexpr auto recoil = 0x18;
    constexpr auto recoil_power = 0x20;
    constexpr auto intensity = 0x70;
    constexpr auto _separate_intensity_factors = 0x74;
    constexpr auto _weapon = 0x28;
    constexpr auto _aiming_config = 0x30;
    constexpr auto _buffs = 0x38;
    constexpr auto _pose = 0x80;
}

namespace engine::offsets::breath_effector
{
    constexpr auto physical = 0x10;
    constexpr auto is_aiming = 0xA0;
    constexpr auto _hands_rotation_spring = 0x18;
    constexpr auto _recoil_rotation_spring = 0x20;
    constexpr auto intensity = 0xA4;
    constexpr auto _shake_intensity = 0xA8;
    constexpr auto x_random = 0x28;
    constexpr auto y_random = 0x30;
    constexpr auto _breath_intensity = 0xAC;
    constexpr auto _breath_frequency = 0xB0;
    constexpr auto tremor_xrandom = 0x38;
    constexpr auto tremor_yrandom = 0x40;
    constexpr auto tremor_zrandom = 0x48;
    constexpr auto hip_xrandom = 0x50;
    constexpr auto hip_zrandom = 0x58;
    constexpr auto hip_penalty = 0xB4;
    constexpr auto curves = 0x60;
    constexpr auto _processors = 0x68;
    constexpr auto breath_params = 0x70;
    constexpr auto tremor_on = 0xB8;
    constexpr auto fracture = 0xB9;
    constexpr auto _camera_sensetivity = 0xBC;
    constexpr auto stiff_untill = 0xC0;
    constexpr auto energy_lower_limit = 0xC4;
    constexpr auto energy_fracture_limit = 0xC8;
    constexpr auto amplitude_gain_per_shot = 0xCC;
    constexpr auto delay = 0xD0;
    constexpr auto amplitude_gain = 0x78;
    constexpr auto hardness = 0x80;
    constexpr auto _base_hip_random_amplitudes = 0xD4;
    constexpr auto _random_between = 0xDC;
    constexpr auto _shot_hardness = 0xE4;
    constexpr auto _lack_of_oxygen_strength = 0x88;
    constexpr auto __overweight_k___backing_field = 0xE8;
    constexpr auto oxygen_level = 0x90;
    constexpr auto stamina_level = 0x98;
}

namespace engine::offsets::procedural_weapon_animation
{
    constexpr auto m_od__ca_me_ra__bo_ne = 0x0;
    constexpr auto l_au_nc_he_r__ca_me_ra__bo_ne = 0x0;
    constexpr auto l_in_e__of__si_gh_t__p0 = 0x0;
    constexpr auto l_in_e__of__si_gh_t__p1 = 0x0;
    constexpr auto c_am_er_a__bo_ne = 0x0;

    constexpr auto hands_container = 0x18;
    constexpr auto camera_container = 0x20;
    constexpr auto mask = 0x100;
    constexpr auto breath = 0x28;
    constexpr auto walk = 0x30;
    constexpr auto motion_react = 0x38;
    constexpr auto force_react = 0x40;
    constexpr auto shootingg = 0x48;
    constexpr auto turn_away = 0x50;
    constexpr auto custom_effector = 0x58;
    constexpr auto offset = 0x104;
    constexpr auto ags_delta_height_range = 0x110;

    constexpr auto _camera_shift_to_line_of_sight = 0x134;

    constexpr auto scope_aim_transforms = 0x78;
    constexpr auto weapon_flip_speed = 0x140;
    constexpr auto camera_smooth_time = 0x144;

    constexpr auto _crank_recoil = 0x178;

    constexpr auto _should_move_weapon_closer = 0x17B;
    constexpr auto _v_camera_target = 0x17C;
    constexpr auto rotation_camera_offset = 0x188;
    constexpr auto _point_of_view = 0x194;

    constexpr auto _current_aiming_plane = 0x88;
    constexpr auto _far_plane = 0x90;

    constexpr auto _fov_compensatory_distance = 0x1AC;

    constexpr auto _ags_delta_height = 0x1B8;

    constexpr auto _shot_direction = 0x1E0;

    constexpr auto tactical_reload_stiffnes = 0x1F0;
    constexpr auto tactical_reload_position = 0x1FC;

    constexpr auto camera_smooth_steady = 0x200;
    constexpr auto camera_smooth_recoil = 0x204;

    constexpr auto camera_smooth_out = 0x208;
    constexpr auto available_scopes_changed = 0xA8;
    constexpr auto camera_smooth_blender = 0xB0;
    constexpr auto aim_sway_max = 0x20C;
    constexpr auto aim_sway_min = 0x218;
    constexpr auto sway_falloff = 0x224;
    constexpr auto aim_sway_starts_threshold = 0x228;
    constexpr auto aim_sway_max_threshold = 0x22C;

    constexpr auto smoothed_tilt = 0x284;
    constexpr auto possible_tilt = 0x288;
    constexpr auto _launcher_zero_z = 0x28C;
    constexpr auto _launcher_rotation_zero_x = 0x290;
    constexpr auto rotation_zero_sum = 0x294;
    constexpr auto position_zero_sum = 0x2A0;
    constexpr auto blindfire_blender = 0xC8;
    constexpr auto tilt_blender = 0xD0;
    constexpr auto blind_fire_offset = 0x2AC;
    constexpr auto blind_fire_rotation = 0x2B8;
    constexpr auto blind_fire_camera = 0x2C4;
    constexpr auto side_fire_offset = 0x2D0;
    constexpr auto side_fire_rotation = 0x2DC;
    constexpr auto side_fire_camera = 0x2E8;
    constexpr auto pitch = 0x2F4;
    constexpr auto aiming_displacement_str = 0x2F8;

    constexpr auto blind_fire_end_position = 0x324;

    constexpr auto active_blends = 0xF8;
}

namespace engine::offsets::motion
{
    constexpr auto intensity = 0xD0;
}