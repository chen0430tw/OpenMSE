#include <openmse/kernel/services.hpp>
#include <type_traits>

using namespace openmse::kernel;
static_assert(contract_version == 1);
static_assert(std::is_abstract_v<IDocuments>);
static_assert(std::is_abstract_v<IWorkspaces>);
static_assert(std::is_abstract_v<ILanguages>);
static_assert(std::is_abstract_v<ICommands>);
static_assert(std::is_abstract_v<IBuilds>);
static_assert(std::is_abstract_v<IDebugger>);
static_assert(!std::is_same_v<DocumentId, WorkspaceId>);
static_assert(!std::is_same_v<TaskId, DebugSessionId>);
static_assert(sizeof(Revision) == 8);

void exercise_public_contract_types()
{
    TransportRequest request{1, "compile-check", "host.identity", "{}", std::chrono::seconds(5)};
    Result<Unit> success{Unit{}};
    Result<Unit> failure{Error{ErrorCode::unsupported, "Capability is not available", "legacy", {}}};
    (void)request; (void)success; (void)failure;
}
