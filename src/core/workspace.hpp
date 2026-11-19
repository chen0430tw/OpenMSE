#pragma once
// WorkspaceService: a real project graph read from .sln / .csproj / .vbproj (VS2005-era MSBuild),
// and ToolService: discovery of the build tools installed on this machine.
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace openmse::core {

struct Tool {
    std::string id, kind, framework, version;   // e.g. "msbuild-2.0", "msbuild", "v2.0.50727", "2.0.50727.9149"
    std::wstring path;
};
std::vector<Tool> discover_tools();

struct ProjectConfiguration {
    std::string solution_configuration;  // "Debug|Any CPU"
    std::string project_configuration;   // "Debug|AnyCPU"
    bool build = true;
    std::string platform_target;
    std::wstring artifact;               // expected primary output
};

struct Project {
    std::string id, name, kind, type_guid; // kind: csharp | vb | other
    std::wstring path, directory;
    std::string output_type, assembly_name, framework, tools_version, load_error;
    std::vector<std::wstring> files;
    std::vector<std::string> project_references;  // project ids inside this workspace
    std::vector<std::wstring> unresolved_references;
    std::vector<std::string> assembly_references;
    std::vector<ProjectConfiguration> configurations;
    bool executable() const { return output_type == "exe" || output_type == "winexe"; }
    const ProjectConfiguration* configuration(const std::string& solution_configuration) const;
};

struct Workspace {
    std::uint64_t id = 0;
    std::wstring path, root;
    bool solution = false;
    std::vector<std::string> configurations;
    std::string startup_project;
    std::vector<Project> projects;
    const Project* project(const std::string& id) const;
};

class WorkspaceService {
public:
    Workspace& open(const std::string& utf8_path);
    Workspace& get(std::uint64_t id);
    void close(std::uint64_t id);
private:
    std::map<std::uint64_t, Workspace> workspaces_;
    std::uint64_t next_ = 1;
};

const Tool* choose_msbuild(const std::vector<Tool>&, const Workspace&);

} // namespace openmse::core
