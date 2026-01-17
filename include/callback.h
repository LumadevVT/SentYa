#pragma once

void ChangeDownloadFolderCallback(void *userdata, const char *const*fileList, int filter);

void SendFilesCallback(void *userdata, const char *const *fileList, int filter);
void SendFoldersCallback(void *userdata, const char *const *fileList, int filter);

