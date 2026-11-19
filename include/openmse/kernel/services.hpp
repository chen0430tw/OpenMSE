#pragma once

// Phase-one contracts. These are interfaces, not an implementation of the IDE kernel.
// No HWND, COM pointer, CLR type, or legacy package identity crosses this boundary.
#include <chrono>
#include <cstdint>
#include <future>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace openmse::kernel {

inline constexpr std::uint32_t contract_version = 1;

template<class Tag> struct Id { std::string value; };
using HostId = Id<struct HostTag>;
using WorkspaceId = Id<struct WorkspaceTag>;
using DocumentId = Id<struct DocumentTag>;
using ProjectId = Id<struct ProjectTag>;
using TaskId = Id<struct TaskTag>;
using DebugSessionId = Id<struct DebugSessionTag>;
using BreakpointId = Id<struct BreakpointTag>;
using Revision = std::uint64_t;
struct Unit {};

enum class ErrorCode {
    unsupported, invalid_argument, invalid_state, conflict, cancelled, timeout,
    host_unavailable, host_busy, host_exited, dependency_missing, protocol_error,
    unsaved_document, io_error, backend_error
};
struct Error {
    ErrorCode code;
    std::string message;
    std::string provider;
    std::optional<std::string> native_code; // e.g. HRESULT; never the primary API error model.
};
template<class T> using Result = std::variant<T, Error>;
template<class T> using Async = std::future<Result<T>>;

struct ICancellation {
    virtual bool requested() const noexcept = 0;
    virtual ~ICancellation() = default;
};
struct RequestContext {
    std::string correlation_id;
    std::chrono::steady_clock::time_point deadline;
    std::shared_ptr<const ICancellation> cancellation;
};

enum class EvidenceLevel { unknown, declared, interface_observed, behavior_verified, blocked };
struct Capability {
    std::string name;
    EvidenceLevel evidence;
    std::string provider;
    std::string evidence_uri;
    std::vector<std::string> limitations;
};
struct HostIdentity {
    HostId id;
    std::string engine;
    std::uint32_t process_id;
    std::string creation_file_time; // Decimal string: no precision loss in JSON consumers.
    std::string executable_uri;
};

// Zero-based lines and UTF-16 code units, regardless of the file's on-disk encoding.
// Core text and wire strings are UTF-8. DTE's one-based positions need adapter conversion.
struct Position { std::uint32_t line; std::uint32_t utf16_column; };
struct Range { Position begin; Position end; };
struct Location { std::string uri; Range range; };
struct TextEdit { Range range; std::string utf8_text; };
enum class Newline { lf, crlf, mixed };
struct DocumentSnapshot {
    DocumentId id;
    std::string uri;
    Revision revision;
    std::string utf8_text;
    std::string disk_encoding;
    Newline newline;
    bool dirty;
    bool read_only;
};
struct SaveResult { Revision persisted_revision; std::string disk_encoding; };
enum class ClosePolicy { refuse_dirty, save, discard };

struct IDocuments {
    virtual Async<DocumentSnapshot> open(WorkspaceId, std::string uri, RequestContext) = 0;
    virtual Async<DocumentSnapshot> snapshot(DocumentId, RequestContext) = 0;
    virtual Async<Revision> apply(DocumentId, Revision expected, std::vector<TextEdit>, RequestContext) = 0;
    virtual Async<SaveResult> save(DocumentId, Revision expected, RequestContext) = 0;
    virtual Async<Unit> close(DocumentId, ClosePolicy, RequestContext) = 0;
    virtual ~IDocuments() = default;
};

struct ProjectSummary { ProjectId id; std::string name; std::string uri; std::string kind; };
struct WorkspaceSnapshot { WorkspaceId id; std::string root_uri; std::vector<ProjectSummary> projects; };
struct IWorkspaces {
    virtual Async<WorkspaceSnapshot> open(std::string root_uri, RequestContext) = 0;
    virtual Async<ProjectSummary> import_project(WorkspaceId, std::string uri, RequestContext) = 0;
    virtual Async<Unit> close(WorkspaceId, RequestContext) = 0;
    virtual ~IWorkspaces() = default;
};

struct Diagnostic { std::string uri; Range range; std::string code; std::string message; bool is_error; };
struct Completion { std::string label; TextEdit edit; };
struct ILanguages {
    virtual Async<std::vector<Diagnostic>> diagnose(DocumentId, Revision, RequestContext) = 0;
    virtual Async<std::vector<Completion>> complete(DocumentId, Revision, Position, RequestContext) = 0;
    virtual Async<std::vector<Location>> definitions(DocumentId, Revision, Position, RequestContext) = 0;
    virtual ~ILanguages() = default;
};

// Stable command names belong to the kernel; legacy command names stay in adapters.
struct CommandContext {
    WorkspaceId workspace;
    std::optional<DocumentId> document;
    std::optional<ProjectId> project;
};
struct CommandDescriptor { std::string id; std::string label; std::string required_capability; };
struct CommandState { bool enabled; bool checked; std::string unavailable_reason; };
struct ICommands {
    virtual std::vector<CommandDescriptor> list() const = 0;
    virtual Async<CommandState> query(std::string id, CommandContext, RequestContext) = 0;
    virtual Async<Unit> execute(std::string id, CommandContext, RequestContext) = 0;
    virtual ~ICommands() = default;
};

struct BuildRequest { WorkspaceId workspace; ProjectId project; std::string configuration; };
struct BuildFinished {
    TaskId task;
    int exit_code;
    std::optional<std::uint32_t> failed_projects;
    std::optional<std::uint32_t> compiler_errors;
    std::optional<std::uint32_t> compiler_warnings;
    std::vector<std::string> artifact_uris;
};
struct IBuilds {
    virtual Async<TaskId> start(BuildRequest, RequestContext) = 0;
    virtual Async<Unit> cancel(TaskId, RequestContext) = 0;
    virtual Async<BuildFinished> wait(TaskId, RequestContext) = 0;
    virtual ~IBuilds() = default;
};

enum class DebugMode { idle, starting, running, stopped, terminating, exited, failed };
struct SourceBreakpoint { std::string uri; Position position; std::optional<std::string> condition; };
struct DebugFrame { std::string function; std::optional<std::string> source_uri; std::optional<Position> position; };
struct DebugSnapshot { DebugSessionId id; DebugMode mode; std::vector<DebugFrame> frames; };
struct DebugValue { bool valid; std::string type; std::string display; };
struct IDebugger {
    virtual Async<DebugSessionId> start(ProjectId, RequestContext) = 0;
    virtual Async<BreakpointId> add_breakpoint(DebugSessionId, SourceBreakpoint, RequestContext) = 0;
    virtual Async<DebugSnapshot> snapshot(DebugSessionId, RequestContext) = 0;
    virtual Async<Unit> continue_execution(DebugSessionId, RequestContext) = 0;
    virtual Async<Unit> step_over(DebugSessionId, RequestContext) = 0;
    virtual Async<DebugValue> evaluate(DebugSessionId, std::string expression, RequestContext) = 0;
    virtual Async<Unit> stop(DebugSessionId, RequestContext) = 0;
    virtual ~IDebugger() = default;
};

struct Event {
    std::string correlation_id;
    std::string topic;
    std::uint64_t sequence;
    std::string json_payload;
};
struct IEvents {
    virtual void publish(Event) = 0; // Implementations queue delivery; no legacy callback runs on the UI thread.
    virtual ~IEvents() = default;
};
struct ICapabilities {
    virtual std::vector<Capability> list() const = 0;
    virtual ~ICapabilities() = default;
};
struct TransportRequest {
    std::uint32_t protocol_version;
    std::string id;
    std::string method;
    std::string json_params;
    std::chrono::milliseconds timeout;
};
struct IHostTransport {
    virtual Async<std::string> invoke(HostId, TransportRequest, RequestContext) = 0;
    virtual ~IHostTransport() = default;
};

} // namespace openmse::kernel
