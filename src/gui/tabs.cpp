#include "gui/tabs.h"

#include <string>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <SDL3/SDL_dialog.h>

#include "gui/events.h"

namespace GUI {
    void RenderSendingTab() {
        if (ImGui::Button("Enviar Arquivos")) {
            SendEvent(GUIEvents::SEND_FILES);
        }

        if (ImGui::Button("Enviar Pasta")) {
            SendEvent(GUIEvents::SEND_FOLDER);
        }
    }

    void RenderReceivingTab() {
        static std::string inputBuffer;
        ImGui::Text("Insira o ticket do arquivo aqui:");
        ImGui::SameLine();
        ImGui::InputText("##TicketPaste", &inputBuffer);

        // Botão pra começar o processo de recebimento dos arquivos.
        if (ImGui::Button("Confirmar")) {
            SendEvent(GUIEvents::RECEIVE_ARCHIVE, inputBuffer.data());
            inputBuffer.clear();
        }
    }

    void RenderTutorialTab() {
    }
}
