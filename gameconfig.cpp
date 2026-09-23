#include "gameconfig.h"
#include "addresses.h"
#include "khook.hpp"

CGameConfig::CGameConfig(const std::string& gameDir, const std::string& path)
{
    this->m_szGameDir = gameDir;
    this->m_szPath = path;
    this->m_pKeyValues = new KeyValues("Games");
}

CGameConfig::~CGameConfig() { delete m_pKeyValues; }

bool CGameConfig::Init(IFileSystem* filesystem, char* conf_error, int conf_error_size)
{
    if (!m_pKeyValues->LoadFromFile(filesystem, m_szPath.c_str(), nullptr))
    {
        snprintf(conf_error, conf_error_size, "Failed to load gamedata file");
        return false;
    }

    KeyValues* game = m_pKeyValues->FindKey(m_szGameDir.c_str(), false);
    if (game)
    {
#if defined _LINUX
        const char* platform = "linux";
#else
        const char* platform = "windows";
#endif

        KeyValues* offsets = game->FindKey("Offsets", false);
        if (offsets)
        {
            FOR_EACH_SUBKEY(offsets, it) { m_umOffsets[it->GetName()] = it->GetInt(platform, -1); }
        }

        KeyValues* signatures = game->FindKey("Signatures", false);
        if (signatures)
        {
            FOR_EACH_SUBKEY(signatures, it)
            {
                m_umLibraries[it->GetName()] = std::string(it->GetString("library"));
                m_umSignatures[it->GetName()] = std::string(it->GetString(platform));
            }
        }
    }
    else
    {
        snprintf(conf_error, conf_error_size, "Failed to find game: %s", m_szGameDir.c_str());
        return false;
    }
    return true;
}

const std::string CGameConfig::GetPath() { return m_szPath; }

const char* CGameConfig::GetSignature(const std::string& name)
{
    auto it = m_umSignatures.find(name);
    if (it == m_umSignatures.end())
    {
        return nullptr;
    }
    return it->second.c_str();
}

int CGameConfig::GetOffset(const std::string& name)
{
    auto it = m_umOffsets.find(name);
    if (it == m_umOffsets.end())
    {
        return -1;
    }
    return it->second;
}

const char* CGameConfig::GetLibrary(const std::string& name)
{
    auto it = m_umLibraries.find(name);
    if (it == m_umLibraries.end())
    {
        return nullptr;
    }
    return it->second.c_str();
}

CModule** CGameConfig::GetModule(const char* name)
{
    const char* library = this->GetLibrary(name);
    if (!library) return nullptr;

    if (strcmp(library, "engine") == 0) return &modules::engine;
    else if (strcmp(library, "server") == 0)
        return &modules::server;
    else if (strcmp(library, "client") == 0)
        return &modules::client;
    else if (strcmp(library, "vscript") == 0)
        return &modules::vscript;
    else if (strcmp(library, "tier0") == 0)
        return &modules::tier0;
    else if (strcmp(library, "networksystem") == 0)
        return &modules::networksystem;
#ifdef _WIN32
    else if (strcmp(library, "hammer") == 0)
        return &modules::hammer;
#endif
    return nullptr;
}

bool CGameConfig::IsSymbol(const char* name)
{
    const char* sigOrSymbol = this->GetSignature(name);
    if (!sigOrSymbol || strlen(sigOrSymbol) <= 0)
    {
        Error("Missing signature or symbol", name);
        return false;
    }
    return sigOrSymbol[0] == '@';
}

const char* CGameConfig::GetSymbol(const char* name)
{
    const char* symbol = this->GetSignature(name);

    if (!symbol || strlen(symbol) <= 1)
    {
        Error("Missing symbol", name);
        return nullptr;
    }
    return symbol + 1;
}

void* CGameConfig::ResolveSignature(const char* name)
{
    CModule** module = this->GetModule(name);
    if (!module || !(*module))
    {
        Error("Invalid Module %s", name);
        return nullptr;
    }

    void* address = nullptr;
    if (this->IsSymbol(name))
    {
        const char* symbol = this->GetSymbol(name);
        if (!symbol)
        {
            Error("Invalid symbol for %s", name);
            return nullptr;
        }
        address = dlsym((*module)->m_hModule, symbol);
    }
    else
    {
        const char* signature = this->GetSignature(name);
        if (!signature)
        {
            Error("Failed to find signature for %s", name);
            return nullptr;
        }

        std::string pattern = HexToPattern(signature);

        address = KHook::LookupSignature((*module)->m_base, (*module)->m_size, pattern.c_str());
    }

    if (!address)
    {
        Error("Failed to find address for %s", name);
        return nullptr;
    }
    return address;
}

// Static functions
std::string CGameConfig::GetDirectoryName(const std::string& directoryPathInput)
{
    std::string directoryPath = std::string(directoryPathInput);

    size_t found = std::string(directoryPath).find_last_of("/\\");
    if (found != std::string::npos)
    {
        return std::string(directoryPath, found + 1);
    }
    return "";
}

std::string CGameConfig::HexToPattern(const char* src)
{
    std::string pattern;

    for (const char* p = src; *p;)
    {
        if (p[0] == '\\' && (p[1] == 'x' || p[1] == 'X') && p[2] && p[3])
        {
            char byteStr[3] = { p[2], p[3], '\0' };

            if (!pattern.empty()) pattern += ' ';

            if (strcmp(byteStr, "2A") == 0 || strcmp(byteStr, "2a") == 0) pattern += '?';
            else
                pattern += byteStr;

            p += 4;
        }
        else
        {
            pattern += *p;
            p++;
        }
    }

    return pattern;
}
