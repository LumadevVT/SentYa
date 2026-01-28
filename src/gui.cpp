#include "gui.h"

#include "gui/tabs.h"

#include <string>
#include <imgui.h>

namespace GUI {
    void RenderTabs() {
        ImGui::BeginTabBar("Abas da Aplicação");

        if (ImGui::BeginTabItem("Tutorial")) {
            RenderTutorialTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Enviar")) {
            RenderSendingTab();
            ImGui::EndTabItem();
        }

        // Aba de recebimento de ticket
        if (ImGui::BeginTabItem("Receber")) {
            RenderReceivingTab();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}
