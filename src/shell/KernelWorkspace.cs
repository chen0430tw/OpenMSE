using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace OpenMSE.Shell
{
    // Managed views of the kernel's WorkspaceService and TaskManager results.
    internal sealed class KernelProjectConfiguration
    {
        internal string SolutionConfiguration, ProjectConfiguration, PlatformTarget, Artifact;
        internal bool Build;
    }

    internal sealed class KernelProject
    {
        internal string Id, Name, Kind, Path, OutputType, AssemblyName, Framework, LoadError;
        internal bool Executable;
        internal List<string> Files=new List<string>(), ProjectReferences=new List<string>(), UnresolvedReferences=new List<string>(), AssemblyReferences=new List<string>();
        internal List<KernelProjectConfiguration> Configurations=new List<KernelProjectConfiguration>();
        internal KernelProjectConfiguration Configuration(string name) { return Configurations.FirstOrDefault(c=>c.SolutionConfiguration==name); }
    }

    internal sealed class KernelWorkspace
    {
        internal ulong Id;
        internal string Path, Root, StartupProject, MSBuild;
        internal bool Solution;
        internal List<string> Configurations=new List<string>();
        internal List<KernelProject> Projects=new List<KernelProject>();
        internal KernelProject Project(string id) { return Projects.FirstOrDefault(p=>p.Id==id); }

        static List<string> Strings(object value) { return ((IEnumerable)value).Cast<object>().Select(o=>(string)o).ToList(); }
        internal static KernelWorkspace From(Dictionary<string,object> r)
        {
            var w=new KernelWorkspace { Id=KernelClient.U64(r["id"]), Path=(string)r["path"], Root=(string)r["root"], Solution=(bool)r["solution"], StartupProject=(string)r["startup_project"], Configurations=Strings(r["configurations"]) };
            var tool=r["msbuild"] as Dictionary<string,object>; if(tool!=null) w.MSBuild=(string)tool["id"]+" "+(string)tool["version"];
            foreach(Dictionary<string,object> p in (IEnumerable)r["projects"]) {
                var project=new KernelProject { Id=(string)p["id"], Name=(string)p["name"], Kind=(string)p["kind"], Path=(string)p["path"], OutputType=(string)p["output_type"], AssemblyName=(string)p["assembly_name"], Framework=(string)p["framework"], LoadError=(string)p["load_error"], Executable=(bool)p["executable"],
                    Files=Strings(p["files"]), ProjectReferences=Strings(p["project_references"]), UnresolvedReferences=Strings(p["unresolved_references"]), AssemblyReferences=Strings(p["assembly_references"]) };
                foreach(Dictionary<string,object> c in (IEnumerable)p["configurations"])
                    project.Configurations.Add(new KernelProjectConfiguration { SolutionConfiguration=(string)c["solution_configuration"], ProjectConfiguration=(string)c["project_configuration"], Build=(bool)c["build"], PlatformTarget=(string)c["platform_target"], Artifact=(string)c["artifact"] });
                w.Projects.Add(project);
            }
            return w;
        }
    }

    internal sealed class KernelDiagnostic
    {
        internal string File, Code, Message;
        internal int Line, Column;
        internal bool IsError;
        public override string ToString() { return (File ?? "")+(Line>0 ? "("+Line+","+Column+")" : "")+": "+(IsError ? "error" : "warning")+" "+Code+": "+Message; }
    }

    internal sealed class KernelFrame
    {
        internal int Index, Line, Column;
        internal string Function, File;
    }

    internal sealed class KernelDebugSession
    {
        internal ulong Id;
        internal string State, Reason, Adapter, Program, LastError;
        internal long? ExitCode;
        internal List<KernelFrame> Frames=new List<KernelFrame>();
        internal List<string> Output=new List<string>();
        internal bool Stopped { get { return State=="stopped"; } }
        internal bool Active { get { return State=="starting" || State=="running" || State=="stopped" || State=="terminating"; } }
        internal static KernelDebugSession From(Dictionary<string,object> s)
        {
            var d=new KernelDebugSession { Id=KernelClient.U64(s["id"]), State=(string)s["state"], Reason=(string)s["reason"], Adapter=(string)s["adapter"], Program=(string)s["program"], LastError=(string)s["last_error"],
                ExitCode=s["exit_code"]==null ? (long?)null : Convert.ToInt64(s["exit_code"]) };
            foreach(Dictionary<string,object> f in (IEnumerable)s["frames"])
                d.Frames.Add(new KernelFrame { Index=Convert.ToInt32(f["index"]), Function=(string)f["function"], File=f["file"] as string, Line=Convert.ToInt32(f["line"]), Column=Convert.ToInt32(f["column"]) });
            foreach(object o in (IEnumerable)s["output"]) d.Output.Add((string)o);
            return d;
        }
    }

    internal sealed class KernelTask
    {
        internal ulong Id, Workspace;
        internal string Kind, State, Project, Configuration, Tool, Command;
        internal long? ExitCode;
        internal int Errors, Warnings;
        internal long DurationMs;
        internal List<KernelDiagnostic> Diagnostics=new List<KernelDiagnostic>();
        internal List<string> Artifacts=new List<string>(), Tail=new List<string>();
        internal bool Running { get { return State=="running"; } }
        internal static KernelTask From(Dictionary<string,object> t)
        {
            var task=new KernelTask { Id=KernelClient.U64(t["id"]), Workspace=KernelClient.U64(t["workspace"]), Kind=(string)t["kind"], State=(string)t["state"], Project=(string)t["project"], Configuration=(string)t["configuration"], Tool=(string)t["tool"], Command=(string)t["command"],
                ExitCode=t["exit_code"]==null ? (long?)null : Convert.ToInt64(t["exit_code"]), Errors=Convert.ToInt32(t["errors"]), Warnings=Convert.ToInt32(t["warnings"]), DurationMs=Convert.ToInt64(t["duration_ms"]) };
            foreach(Dictionary<string,object> d in (IEnumerable)t["diagnostics"])
                task.Diagnostics.Add(new KernelDiagnostic { File=(string)d["file"], Line=Convert.ToInt32(d["line"]), Column=Convert.ToInt32(d["column"]), Code=(string)d["code"], Message=(string)d["message"], IsError=(bool)d["is_error"] });
            foreach(object a in (IEnumerable)t["artifacts"]) task.Artifacts.Add((string)a);
            foreach(object l in (IEnumerable)t["tail"]) task.Tail.Add((string)l);
            return task;
        }
    }
}
