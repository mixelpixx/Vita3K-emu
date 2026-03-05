// Vita3K emulator project
// Copyright (C) 2026 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "private.h"

#include <gui/functions.h>
#include <gui/state.h>

#include <config/state.h>
#include <emuenv/state.h>

#include <algorithm>
#include <cmath>

namespace gui {

// Easing function for smooth animations
static float ease_out_cubic(float t) {
    return 1.0f - std::pow(1.0f - t, 3.0f);
}

static float ease_out_back(float t) {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

} // namespace gui

// BubbleGridState implementation
void BubbleGridState::start_scroll(int to_page) {
    if (to_page == current_page || is_scrolling)
        return;
    target_page = to_page;
    scroll_start_time = std::chrono::steady_clock::now();
    scroll_progress = 0.f;
    is_scrolling = true;
}

void BubbleGridState::update() {
    if (!is_scrolling)
        return;

    float elapsed = std::chrono::duration<float>(
        std::chrono::steady_clock::now() - scroll_start_time
    ).count();

    scroll_progress = std::min(elapsed / scroll_duration, 1.0f);

    if (scroll_progress >= 1.0f) {
        current_page = target_page;
        scroll_progress = 0.f;
        is_scrolling = false;
    }
}

float BubbleGridState::get_scroll_offset() const {
    if (!is_scrolling)
        return 0.f;

    float eased = gui::ease_out_cubic(scroll_progress);
    float direction = (target_page > current_page) ? -1.0f : 1.0f;
    return direction * eased;
}

namespace gui {

// Draw a single bubble icon with rounded corners and optional press effect
static void draw_bubble(
    GuiState &gui,
    EmuEnvState &emuenv,
    const App &app,
    const ImVec2 &pos,
    const ImVec2 &size,
    float scale,
    bool is_pressed,
    bool is_hovered
) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    // Get app icon
    const auto &apps_icon = gui.app_selector.user_apps_icon;
    const auto icon_it = apps_icon.find(app.path);

    // Calculate scaled size for press animation
    float press_scale = is_pressed ? 0.92f : (is_hovered ? 1.02f : 1.0f);
    ImVec2 scaled_size(size.x * press_scale * scale, size.y * press_scale * scale);
    ImVec2 offset((size.x * scale - scaled_size.x) / 2.0f, (size.y * scale - scaled_size.y) / 2.0f);
    ImVec2 bubble_pos(pos.x + offset.x, pos.y + offset.y);

    // Draw shadow
    if (!is_pressed) {
        ImVec2 shadow_pos(bubble_pos.x + 4.0f * scale, bubble_pos.y + 4.0f * scale);
        ImVec2 shadow_max(shadow_pos.x + scaled_size.x, shadow_pos.y + scaled_size.y);
        draw_list->AddRectFilled(shadow_pos, shadow_max, IM_COL32(0, 0, 0, 60), scaled_size.x * 0.15f);
    }

    // Draw bubble background
    ImVec2 bubble_max(bubble_pos.x + scaled_size.x, bubble_pos.y + scaled_size.y);
    draw_list->AddRectFilled(bubble_pos, bubble_max, IM_COL32(255, 255, 255, 255), scaled_size.x * 0.15f);

    // Draw icon
    if (icon_it != apps_icon.end() && icon_it->second) {
        ImVec2 icon_padding(scaled_size.x * 0.08f, scaled_size.y * 0.08f);
        ImVec2 icon_pos(bubble_pos.x + icon_padding.x, bubble_pos.y + icon_padding.y);
        ImVec2 icon_size(scaled_size.x - icon_padding.x * 2, scaled_size.y - icon_padding.y * 2);
        ImVec2 icon_max(icon_pos.x + icon_size.x, icon_pos.y + icon_size.y);

        draw_list->AddImageRounded(
            icon_it->second,
            icon_pos, icon_max,
            ImVec2(0, 0), ImVec2(1, 1),
            IM_COL32_WHITE,
            icon_size.x * 0.1f,
            ImDrawFlags_RoundCornersAll
        );
    } else {
        // Draw placeholder
        ImVec2 icon_padding(scaled_size.x * 0.15f, scaled_size.y * 0.15f);
        ImVec2 icon_pos(bubble_pos.x + icon_padding.x, bubble_pos.y + icon_padding.y);
        ImVec2 icon_size(scaled_size.x - icon_padding.x * 2, scaled_size.y - icon_padding.y * 2);
        ImVec2 icon_max(icon_pos.x + icon_size.x, icon_pos.y + icon_size.y);
        draw_list->AddRectFilled(icon_pos, icon_max, IM_COL32(100, 100, 100, 255), icon_size.x * 0.1f);
    }

    // Draw highlight on hover
    if (is_hovered && !is_pressed) {
        draw_list->AddRect(bubble_pos, bubble_max, IM_COL32(255, 255, 255, 180), scaled_size.x * 0.15f, 0, 3.0f * scale);
    }
}

// Draw app title below bubble
static void draw_bubble_title(
    const App &app,
    const ImVec2 &pos,
    const ImVec2 &bubble_size,
    float scale
) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    // Use short title if available, otherwise truncate title
    std::string display_title = app.stitle.empty() ? app.title : app.stitle;
    if (display_title.length() > 12) {
        display_title = display_title.substr(0, 11) + "...";
    }

    ImVec2 text_size = ImGui::CalcTextSize(display_title.c_str());
    float title_y = pos.y + bubble_size.y * scale + 8.0f * scale;
    float title_x = pos.x + (bubble_size.x * scale - text_size.x) / 2.0f;

    // Draw text shadow
    draw_list->AddText(ImVec2(title_x + 1, title_y + 1), IM_COL32(0, 0, 0, 180), display_title.c_str());
    // Draw text
    draw_list->AddText(ImVec2(title_x, title_y), IM_COL32(255, 255, 255, 255), display_title.c_str());
}

// Draw page indicator dots
static void draw_page_indicators(
    const ImVec2 &viewport_pos,
    const ImVec2 &viewport_size,
    int total_pages,
    int current_page,
    float scale
) {
    if (total_pages <= 1)
        return;

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    const float dot_radius = 6.0f * scale;
    const float dot_spacing = 20.0f * scale;
    const float total_width = (total_pages - 1) * dot_spacing;
    const float start_x = viewport_pos.x + (viewport_size.x - total_width) / 2.0f;
    const float y = viewport_pos.y + viewport_size.y - 30.0f * scale;

    for (int i = 0; i < total_pages; i++) {
        float x = start_x + i * dot_spacing;
        ImU32 color = (i == current_page) ? IM_COL32(255, 255, 255, 255) : IM_COL32(255, 255, 255, 100);
        float radius = (i == current_page) ? dot_radius : dot_radius * 0.7f;
        draw_list->AddCircleFilled(ImVec2(x, y), radius, color);
    }
}

void draw_livearea_bubbles(GuiState &gui, EmuEnvState &emuenv) {
    const ImVec2 VIEWPORT_POS(emuenv.logical_viewport_pos.x, emuenv.logical_viewport_pos.y);
    const ImVec2 VIEWPORT_SIZE(emuenv.logical_viewport_size.x, emuenv.logical_viewport_size.y);
    const ImVec2 VIEWPORT_SCALE(VIEWPORT_SIZE.x / 960.0f, VIEWPORT_SIZE.y / 544.0f);
    const float SCALE = std::min(VIEWPORT_SCALE.x, VIEWPORT_SCALE.y);

    // Update animation state
    gui.bubble_grid.update();

    // Combine system and user apps
    std::vector<App> all_apps;
    all_apps.insert(all_apps.end(), gui.app_selector.sys_apps.begin(), gui.app_selector.sys_apps.end());
    all_apps.insert(all_apps.end(), gui.app_selector.user_apps.begin(), gui.app_selector.user_apps.end());

    if (all_apps.empty())
        return;

    // Calculate layout
    const int apps_per_page = gui.bubble_grid.get_apps_per_page();
    const int total_pages = (static_cast<int>(all_apps.size()) + apps_per_page - 1) / apps_per_page;

    // Bubble dimensions (PS Vita style)
    const ImVec2 BUBBLE_SIZE(100.0f, 100.0f);
    const float BUBBLE_SPACING_X = 130.0f * SCALE;
    const float BUBBLE_SPACING_Y = 150.0f * SCALE;
    const float TITLE_HEIGHT = 30.0f * SCALE;

    // Grid positioning
    const float grid_width = BubbleGridState::bubbles_per_row * BUBBLE_SPACING_X;
    const float grid_height = BubbleGridState::rows_per_page * BUBBLE_SPACING_Y + TITLE_HEIGHT;
    const float grid_start_x = VIEWPORT_POS.x + (VIEWPORT_SIZE.x - grid_width) / 2.0f + BUBBLE_SPACING_X * 0.15f;
    const float grid_start_y = VIEWPORT_POS.y + (VIEWPORT_SIZE.y - grid_height) / 2.0f - 20.0f * SCALE;

    // Draw background
    draw_background(gui, emuenv);

    // Create fullscreen window for bubble grid
    ImGui::SetNextWindowPos(VIEWPORT_POS);
    ImGui::SetNextWindowSize(VIEWPORT_SIZE);
    ImGui::Begin("##livearea_bubbles", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings
    );

    // Calculate scroll offset for page transition animation + drag feedback
    float scroll_offset = gui.bubble_grid.get_scroll_offset() * VIEWPORT_SIZE.x;
    // Add real-time drag offset for swipe feedback
    scroll_offset += gui.bubble_grid.get_drag_offset();
    int display_page = gui.bubble_grid.current_page;

    // Draw bubbles for current page
    int start_index = display_page * apps_per_page;
    int end_index = std::min(start_index + apps_per_page, static_cast<int>(all_apps.size()));

    for (int i = start_index; i < end_index; i++) {
        const auto &app = all_apps[i];
        int page_index = i - start_index;
        int row = page_index / BubbleGridState::bubbles_per_row;
        int col = page_index % BubbleGridState::bubbles_per_row;

        ImVec2 bubble_pos(
            grid_start_x + col * BUBBLE_SPACING_X + scroll_offset,
            grid_start_y + row * BUBBLE_SPACING_Y
        );

        // Create invisible button for interaction
        ImGui::SetCursorScreenPos(bubble_pos);
        std::string button_id = "##bubble_" + app.path;

        bool is_pressed = (gui.bubble_grid.pressed_app_index == i);
        bool is_hovered = false;

        ImGui::InvisibleButton(button_id.c_str(), ImVec2(BUBBLE_SIZE.x * SCALE, BUBBLE_SIZE.y * SCALE));

        if (ImGui::IsItemHovered()) {
            is_hovered = true;
            gui.bubble_grid.selected_app_index = i;

            if (ImGui::IsItemActive()) {
                gui.bubble_grid.pressed_app_index = i;
                is_pressed = true;
            }
        }

        if (ImGui::IsItemClicked()) {
            gui.bubble_grid.pressed_app_index = i;
        }

        // Handle click release - launch app
        if (ImGui::IsItemDeactivated() && gui.bubble_grid.pressed_app_index == i) {
            gui.bubble_grid.pressed_app_index = -1;
            // Open live area or launch app
            emuenv.app_path = app.path;
            if (emuenv.cfg.show_live_area_screen) {
                pre_load_app(gui, emuenv, true, app.path);
            } else {
                pre_load_app(gui, emuenv, false, app.path);
            }
        }

        // Draw the bubble
        draw_bubble(gui, emuenv, app, bubble_pos, BUBBLE_SIZE, SCALE, is_pressed, is_hovered);

        // Draw title below bubble
        draw_bubble_title(app, bubble_pos, BUBBLE_SIZE, SCALE);
    }

    // Draw page indicators
    draw_page_indicators(VIEWPORT_POS, VIEWPORT_SIZE, total_pages, display_page, SCALE);

    // Handle page navigation via swipe, mouse wheel, or keyboard
    if (ImGui::IsWindowHovered()) {
        // Mouse wheel for page navigation
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel > 0 && display_page > 0) {
            gui.bubble_grid.start_scroll(display_page - 1);
        } else if (wheel < 0 && display_page < total_pages - 1) {
            gui.bubble_grid.start_scroll(display_page + 1);
        }

        // Arrow keys for page navigation
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && display_page > 0) {
            gui.bubble_grid.start_scroll(display_page - 1);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow) && display_page < total_pages - 1) {
            gui.bubble_grid.start_scroll(display_page + 1);
        }

        // Touch/mouse swipe gesture handling
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            
            // Start tracking drag if significant horizontal movement
            if (!gui.bubble_grid.is_dragging && std::abs(drag_delta.x) > 10.0f * SCALE) {
                // Only start drag if horizontal movement exceeds vertical
                if (std::abs(drag_delta.x) > std::abs(drag_delta.y) * 1.5f) {
                    gui.bubble_grid.is_dragging = true;
                    gui.bubble_grid.drag_start_x = ImGui::GetIO().MousePos.x;
                    gui.bubble_grid.pressed_app_index = -1;  // Cancel any bubble press
                }
            }

            // Update drag offset for visual feedback
            if (gui.bubble_grid.is_dragging) {
                gui.bubble_grid.drag_offset = drag_delta.x;
                
                // Limit drag at edges (rubber band effect)
                if (display_page == 0 && drag_delta.x > 0) {
                    gui.bubble_grid.drag_offset = drag_delta.x * 0.3f;  // Resistance at left edge
                } else if (display_page >= total_pages - 1 && drag_delta.x < 0) {
                    gui.bubble_grid.drag_offset = drag_delta.x * 0.3f;  // Resistance at right edge
                }
            }
        }
    }

    // Handle swipe release
    if (gui.bubble_grid.is_dragging && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        float swipe_threshold = gui.bubble_grid.swipe_threshold * SCALE;
        
        if (gui.bubble_grid.drag_offset < -swipe_threshold && display_page < total_pages - 1) {
            // Swipe left -> next page
            gui.bubble_grid.start_scroll(display_page + 1);
        } else if (gui.bubble_grid.drag_offset > swipe_threshold && display_page > 0) {
            // Swipe right -> previous page
            gui.bubble_grid.start_scroll(display_page - 1);
        }
        
        gui.bubble_grid.is_dragging = false;
        gui.bubble_grid.drag_offset = 0.f;
    }

    // Reset pressed state if mouse not down and not dragging
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && !gui.bubble_grid.is_dragging) {
        gui.bubble_grid.pressed_app_index = -1;
    }

    ImGui::End();
}

} // namespace gui
