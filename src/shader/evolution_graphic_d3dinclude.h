#ifndef __EVOLUTION_D3DINCLUDE_H__
#define __EVOLUTION_D3DINCLUDE_H__

#include "evolution_graphic_shader_chain.h"

namespace EVOLUTION{
    namespace GRAPHIC{
        //-------------------------------------------------------------
        //          D3DInclude
        //-------------------------------------------------------------
        class D3DInclude : public ID3DInclude{
        private:
            std::string m_ShaderDir;
        public:
            D3DInclude(const c8* shderfile){
                //	パス分割
                char	workpath[MAX_PATH];
                memcpy(workpath, shderfile, strlen(shderfile) + 1);
                for (int i = strlen(shderfile); i > 0; i--){
                    if (IsDBCSLeadByte(workpath[i - 2])){
                        i--;
                        continue;
                    }
                    if (workpath[i - 1] == '\\' || workpath[i - 1] == '/'){
                        workpath[i] = '\0';
                        break;
                    }
                }

                m_ShaderDir = workpath;
            }

            HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) {
                try {
                    std::string finalPath;
                    switch (IncludeType) {
                    case D3D_INCLUDE_LOCAL:
                        finalPath = m_ShaderDir + "\\" + pFileName;
                        break;
                    }


                    CORE::FILE::IFileRead* read_file;
                    FUNCTION::CreateFileRead(&read_file, finalPath.c_str());

                    if (read_file != nullptr){
                        u32 fileSize = (u32)read_file->GetFileSize();
                        u8* buffer = nullptr;
                        buffer = new u8[fileSize];
                        read_file->Read(buffer, fileSize);
                        read_file->Release();
                        *ppData = buffer;
                        *pBytes = fileSize;
                    }
                    else {
                        return E_FAIL;
                    }
                    return S_OK;
                }
                catch (...) {
                    return E_FAIL;
                }
            }

            HRESULT __stdcall Close(LPCVOID pData) {
                char* buf = (char*)pData;
                delete[] buf;
                return S_OK;
            }

        };
    }
}

#endif //!__EVOLUTION_D3DINCLUDE_H__