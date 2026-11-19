#ifndef OPENMSE_KERNEL_ABI_H
#define OPENMSE_KERNEL_ABI_H

/*
 * OpenMSE.Core stable C ABI, version 1.
 *
 * - Opaque kernel handle; no C++ types, exceptions, HWND, COM or CLR types cross this boundary.
 * - Every string argument is UTF-8 with an explicit byte length (no terminator required).
 * - Every call that produces data writes a kernel-owned omk_buffer holding one UTF-8 JSON object:
 *     success: {"ok":true,"result":{...}}
 *     failure: {"ok":false,"error":{"code":"conflict","message":"..."}}
 *   The caller releases it with omk_buffer_free, even on failure. The status code mirrors "ok"/"code".
 * - Positions are zero-based lines and UTF-16 code-unit columns over LF-normalised text.
 * - A kernel handle is internally serialised; callers should still use it from one dispatcher thread.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef OPENMSE_CORE_BUILD
#define OMK_API __declspec(dllexport)
#else
#define OMK_API __declspec(dllimport)
#endif
#define OMK_CALL __cdecl

#ifdef __cplusplus
extern "C" {
#endif

/* 2: toolbox and design services. 3: tools, workspaces and tasks. 4: legacy host manager. 5: debugging and capabilities. */
#define OMK_ABI_VERSION 5u

typedef struct omk_kernel omk_kernel;
typedef struct omk_buffer { char* data; size_t size; } omk_buffer;
typedef int32_t omk_status;

/* Status values; 1..14 follow openmse::kernel::ErrorCode order plus one. */
#define OMK_OK 0
#define OMK_E_UNSUPPORTED 1
#define OMK_E_INVALID_ARGUMENT 2
#define OMK_E_INVALID_STATE 3
#define OMK_E_CONFLICT 4
#define OMK_E_CANCELLED 5
#define OMK_E_TIMEOUT 6
#define OMK_E_HOST_UNAVAILABLE 7
#define OMK_E_HOST_BUSY 8
#define OMK_E_HOST_EXITED 9
#define OMK_E_DEPENDENCY_MISSING 10
#define OMK_E_PROTOCOL_ERROR 11
#define OMK_E_UNSAVED_DOCUMENT 12
#define OMK_E_IO_ERROR 13
#define OMK_E_BACKEND_ERROR 14
#define OMK_E_ABI_MISMATCH 100

/* Edit flags. COALESCE lets the kernel merge adjacent short typing edits into one undo step. */
#define OMK_EDIT_COALESCE 0x1u

/* Close policies, matching openmse::kernel::ClosePolicy. */
#define OMK_CLOSE_REFUSE_DIRTY 0u
#define OMK_CLOSE_SAVE 1u
#define OMK_CLOSE_DISCARD 2u

OMK_API uint32_t OMK_CALL omk_abi_version(void);
OMK_API omk_status OMK_CALL omk_create(uint32_t client_abi_version, omk_kernel** out_kernel);
OMK_API void OMK_CALL omk_destroy(omk_kernel* kernel);
OMK_API void OMK_CALL omk_buffer_free(omk_buffer* buffer);

/* Untitled document seeded with text; saved later as UTF-8 with BOM and CRLF unless told otherwise. */
OMK_API omk_status OMK_CALL omk_document_create(omk_kernel*, const char* name, size_t name_size, const char* text, size_t text_size, omk_buffer* out);
/* Opens a file, or returns the already-open document for the same file ("already_open":true). */
OMK_API omk_status OMK_CALL omk_document_open(omk_kernel*, const char* path, size_t path_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_document_snapshot(omk_kernel*, uint64_t document, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_document_replace(omk_kernel*, uint64_t document, uint64_t expected_revision,
    uint32_t begin_line, uint32_t begin_column, uint32_t end_line, uint32_t end_column,
    const char* text, size_t text_size, uint32_t flags, omk_buffer* out);
/* Undo/redo report the applied change against the pre-call text so views can patch in place. */
OMK_API omk_status OMK_CALL omk_document_undo(omk_kernel*, uint64_t document, uint64_t expected_revision, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_document_redo(omk_kernel*, uint64_t document, uint64_t expected_revision, omk_buffer* out);
/* path_size == 0 saves to the current path; otherwise "save as" to path. */
OMK_API omk_status OMK_CALL omk_document_save(omk_kernel*, uint64_t document, uint64_t expected_revision, const char* path, size_t path_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_document_close(omk_kernel*, uint64_t document, uint32_t policy, omk_buffer* out);

/* Command routing. document == 0 means no document context. */
OMK_API omk_status OMK_CALL omk_command_list(omk_kernel*, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_command_query(omk_kernel*, const char* id, size_t id_size, uint64_t document, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_command_execute(omk_kernel*, const char* id, size_t id_size, uint64_t document, uint64_t expected_revision, omk_buffer* out);

/* Toolbox catalog (config/toolbox.json format). Entries get stable IDs such as "html.button" / "winforms.button". */
OMK_API omk_status OMK_CALL omk_toolbox_load(omk_kernel*, const char* path, size_t path_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_toolbox_list(omk_kernel*, omk_buffer* out);

/*
 * Design service over .html/.htm and .omform documents. Node IDs ("n0", "n1", ...) are valid only for the
 * revision the snapshot reports. omk_design_apply takes one operation as UTF-8 JSON:
 *   {"op":"add","toolbox_id":"html.button","x":60,"y":100}
 *   {"op":"set","node":"n3","properties":{"name":..,"text":..,"x":..,"y":..,"width":..,"height":..,"back_color":..,"fore_color":..}}
 *   {"op":"delete","node":"n3"}
 *   {"op":"set_form","properties":{"title":..,"width":..,"height":..}}
 * The kernel computes the source patch and commits it as one edit and one undo step. The result carries the
 * document state, the applied edit in pre-change coordinates ("edit", or null if nothing changed), the node to
 * select ("node") and the new design snapshot ("design").
 */
OMK_API omk_status OMK_CALL omk_design_snapshot(omk_kernel*, uint64_t document, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_design_apply(omk_kernel*, uint64_t document, uint64_t expected_revision, const char* op_json, size_t op_size, omk_buffer* out);
/* language: "csharp" or "vb". Result: {"source":"..."} for .omform documents. */
OMK_API omk_status OMK_CALL omk_design_export(omk_kernel*, uint64_t document, const char* language, size_t language_size, omk_buffer* out);

/*
 * Tools, workspaces and tasks (ABI 3). A workspace is a project graph read from .sln/.csproj/.vbproj.
 * Builds and runs are tasks: they return immediately with a task snapshot, publish "task.started",
 * "task.output" ({"task":N,"line":"..."}) and "task.finished" events, and end as succeeded, failed or
 * cancelled with the real process exit code. project may be empty (whole solution / startup project).
 * omk_task_wait blocks up to timeout_ms without holding the kernel's service lock.
 */
OMK_API omk_status OMK_CALL omk_tools_list(omk_kernel*, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_workspace_open(omk_kernel*, const char* path, size_t path_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_workspace_snapshot(omk_kernel*, uint64_t workspace, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_workspace_close(omk_kernel*, uint64_t workspace, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_build_start(omk_kernel*, uint64_t workspace, const char* configuration, size_t configuration_size, const char* project, size_t project_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_run_start(omk_kernel*, uint64_t workspace, const char* configuration, size_t configuration_size, const char* project, size_t project_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_task_cancel(omk_kernel*, uint64_t task, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_task_snapshot(omk_kernel*, uint64_t task, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_task_wait(omk_kernel*, uint64_t task, uint32_t timeout_ms, omk_buffer* out);

/*
 * Legacy hosts (ABI 4), described by config/hosts.json. The kernel verifies each host executable's version and
 * SHA-256, launches it through the OpenMSE launcher, and binds only that instance (PID + creation time, process
 * handle held). States: starting, connecting, ready, needs_verification, disconnected, stopping, stopped,
 * exited, failed. Requests are asynchronous ("host.response" events, or omk_host_request_wait) and end as
 * ok | error | timeout | not_executed | disconnected | host_exited. A timeout means the outcome is unknown:
 * the host becomes needs_verification and refuses writes until omk_host_verify re-confirms its identity.
 * Requests that expired while queued in the bridge are reported not_executed. Nothing is replayed.
 * omk_host_sync_document sends a kernel document to the host buffer; it is idempotent by content.
 * stop modes: "detach" (leave the IDE running), "shutdown" (graceful), "terminate" (owned instance only).
 */
OMK_API omk_status OMK_CALL omk_hosts_load(omk_kernel*, const char* manifest, size_t manifest_size, const char* root, size_t root_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_start(omk_kernel*, const char* engine, size_t engine_size, const char* workspace, size_t workspace_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_list(omk_kernel*, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_snapshot(omk_kernel*, uint64_t host, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_wait(omk_kernel*, uint64_t host, uint32_t timeout_ms, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_request(omk_kernel*, uint64_t host, const char* method, size_t method_size, const char* params_json, size_t params_size, uint32_t timeout_ms, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_sync_document(omk_kernel*, uint64_t host, uint64_t document, uint32_t timeout_ms, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_request_wait(omk_kernel*, uint64_t host, uint64_t request, uint32_t timeout_ms, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_verify(omk_kernel*, uint64_t host, uint32_t timeout_ms, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_reconnect(omk_kernel*, uint64_t host, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_host_stop(omk_kernel*, uint64_t host, const char* mode, size_t mode_size, omk_buffer* out);

/*
 * Debugging (ABI 5): CLR 2.0 programs through OpenMSE.DebugHost.{x86,x64}.exe (chosen from the target's PE
 * header). breakpoints_json: [{"file":"C:\\...\\Program.cs","line":7}] (one-based lines). Session states:
 * starting, running, stopped, terminating, exited, failed. omk_debug_control actions: continue, step_over,
 * step_into, step_out (stopped only), pause (running only), stop. Variables and evaluation are stopped-only;
 * evaluation reads locals, arguments and fields ("name" or "name.field"), never runs debuggee code.
 * Events: debug.state, debug.stopped, debug.output, debug.breakpoint, debug.exited.
 * omk_capabilities lists providers with evidence levels (behavior_verified / blocked / unsupported).
 */
OMK_API omk_status OMK_CALL omk_debug_start(omk_kernel*, uint64_t workspace, const char* configuration, size_t configuration_size, const char* project, size_t project_size, const char* breakpoints_json, size_t breakpoints_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_debug_snapshot(omk_kernel*, uint64_t session, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_debug_wait(omk_kernel*, uint64_t session, uint32_t timeout_ms, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_debug_control(omk_kernel*, uint64_t session, const char* action, size_t action_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_debug_breakpoint_add(omk_kernel*, uint64_t session, const char* file, size_t file_size, uint32_t line, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_debug_breakpoint_remove(omk_kernel*, uint64_t session, uint32_t id, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_debug_variables(omk_kernel*, uint64_t session, uint32_t frame, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_debug_evaluate(omk_kernel*, uint64_t session, uint32_t frame, const char* expression, size_t expression_size, omk_buffer* out);
OMK_API omk_status OMK_CALL omk_capabilities(omk_kernel*, omk_buffer* out);

/* Ordered event queue. Result is {"event":null} when empty, else {"event":{"sequence":N,"topic":"...","payload":{...}}}. */
OMK_API omk_status OMK_CALL omk_event_poll(omk_kernel*, omk_buffer* out);

#ifdef __cplusplus
}
#endif

#endif
