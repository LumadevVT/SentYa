#include "gui/tabs.h"

#include <string>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <SDL3/SDL_clipboard.h>

#include "gui/events.h"
#include "process/manager.h"
#include "utils/log.h"

namespace GUI
{
    void RenderTutorialTab()
    {
        static std::string introText = R"(Olá, este aqui é o SentYa, o app de envio de arquivos baseado no sendme!

Para enviar um arquivo, vá para a aba "Enviar" e aperte "Enviar arquivos" ou "Enviar pasta".
Assim que processado o arquivo/pasta, clique em copiar o ticket e envie para o destinatário!
Lembre-se: você tem que deixar o ticket aberto para funcionar, se cancelar o ticket não funciona mais.

Para receber o arquivo, copie o ticket que você recebeu do remetente e cole na aba "Receber".
Depois disso, basta aguardar que seus arquivos serão instalados na sua pasta escolhida!

Aviso: o app não consegue reconhecer direito arquivos e pastas com acento, tome cuidado com o nome das pastas e arquivos.

O programa ainda está em desenvolvimento, então por favor mande uma mensagem no discord para @lumadevvt se tiver algum problema ou bug!
)";

        ImGui::TextWrapped("%s", introText.c_str());

        // const auto backends = ProcessManager::GetAvailableBackends();
        // if (
        //     ImGui::BeginCombo(
        //         "Escolha o backend",
        //         ProcessManager::BackendToStr(ProcessManager::GetCurrentBackend()))
        // )
        // {
        //     static size_t item_selected_idx = 0;
        //     for (size_t n = 0; n < backends.size(); n++)
        //     {
        //         const bool is_selected = item_selected_idx == n;
        //         Log::Debug(
        //             "Item selected: "s + ProcessManager::BackendToStr(backends.at(item_selected_idx)));
        //         if (ImGui::Selectable(ProcessManager::BackendToStr(backends.at(n)), is_selected))
        //             item_selected_idx = n;
        //
        //         // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        //         if (is_selected) ImGui::SetItemDefaultFocus();
        //     }
        //
        //     if (backends.at(item_selected_idx) != ProcessManager::GetCurrentBackend())
        //         ProcessManager::ChangeBackend(backends.at(item_selected_idx));
        //
        //
        //     ImGui::EndCombo();
        // }
    }

    void RenderSendingTab()
    {
        if (ImGui::Button("Enviar Arquivos"))
        {
            SendEvent(GUIEvents::SEND_FILES);
        }

        ImGui::SameLine();

        if (ImGui::Button("Enviar Pasta"))
        {
            SendEvent(GUIEvents::SEND_FOLDER);
        }

        ImGui::Separator();

        const auto& tasks = ProcessManager::GetActiveProcesses();
        for (const auto& task : tasks)
        {
            if (task.type != Process::Type::SEND) continue;

            ImGui::TextWrapped("%s", task.ticket.c_str());

            const auto& data = std::get<Process::SendData>(task.data);

            static std::string filePathBuffer;
            filePathBuffer = "";
            for (size_t i = 0; i < data.archivePaths.size(); i++)
            {
                filePathBuffer += data.archivePaths[i].string();
                if (i != data.archivePaths.size() - 1)
                    filePathBuffer += ", ";
            }
            ImGui::TextWrapped("%s", filePathBuffer.c_str());

            if (ImGui::Button(("Copiar Ticket##" + task.ticket).c_str()))
                SDL_SetClipboardText(task.ticket.c_str());
            ImGui::SameLine();
            if (ImGui::Button(("Cancelar##" + task.ticket).c_str()))
                SendEvent(GUIEvents::STOP_PROCESS, new std::string(task.ticket));

            ImGui::Separator();
        }
    }

    void RenderReceivingTab()
    {
        static std::string inputBuffer;
        ImGui::Text("Insira o ticket do arquivo aqui:");
        ImGui::SameLine();
        ImGui::InputText("##TicketPaste", &inputBuffer);

        // Botão para começar o processo de recebimento dos arquivos.
        if (ImGui::Button("Confirmar"))
        {
            SendEvent(GUIEvents::RECEIVE_ARCHIVE, new std::string(inputBuffer));
            inputBuffer.clear();
        }

        const auto& tasks = ProcessManager::GetActiveProcesses();
        for (const auto& task : tasks)
        {
            if (task.type != Process::Type::RECEIVE) continue;

            ImGui::TextWrapped("%s", task.ticket.c_str());

            const auto& data = std::get<Process::ReceiveData>(task.data);
            ImGui::TextWrapped("Tempo decorrido: %02d:%02d",
                               static_cast<Uint32>(data.timerSeconds) / 60,
                               static_cast<Uint32>(data.timerSeconds)
            );
            ImGui::Separator();
        }
    }
}
