using System;
using System.IO;
using System.Security;
using System.Text;

namespace OpenMSE.Shell
{
    internal sealed class Template
    {
        internal string Name, Description, Extension, Content;
        internal bool Project;
        public override string ToString() { return Name; }
        internal static readonly Template[] All = {
            new Template { Name="文本文件", Description="创建 UTF-8 文本文档。", Extension=".txt", Content="" },
            new Template { Name="HTML 页面", Description="创建包含 HTML5 文档结构的网页。", Extension=".html", Content="<!doctype html>\r\n<html lang=\"zh-CN\">\r\n<head>\r\n    <meta charset=\"utf-8\">\r\n    <title>OpenMSE</title>\r\n</head>\r\n<body>\r\n    <h1>Hello, OpenMSE</h1>\r\n</body>\r\n</html>\r\n" },
            new Template { Name="JavaScript 文件", Description="创建 JavaScript 源文件。", Extension=".js", Content="// OpenMSE\r\nfunction main() {\r\n    return \"Hello, OpenMSE\";\r\n}\r\n" },
            new Template { Name="VBScript 文件", Description="创建 VBScript 源文件。", Extension=".vbs", Content="Option Explicit\r\n' OpenMSE\r\n" },
            new Template { Name="C# 控制台项目", Description="创建可供 Visual Studio 2005 构建的 .NET 2.0 C# 项目。", Extension=".cs", Project=true, Content="using System;\r\n\r\ninternal static class Program\r\n{\r\n    private static void Main()\r\n    {\r\n        Console.WriteLine(\"Hello, OpenMSE\");\r\n    }\r\n}\r\n" },
            new Template { Name="Visual Basic 控制台项目", Description="创建可供 Visual Studio 2005 构建的 .NET 2.0 Visual Basic 项目。", Extension=".vb", Project=true, Content="Imports System\r\n\r\nModule Program\r\n    Sub Main()\r\n        Console.WriteLine(\"Hello, OpenMSE\")\r\n    End Sub\r\nEnd Module\r\n" }
            ,new Template { Name="Windows 窗体布局", Description="可视化设计 Windows 窗体，导出 C# 或 Visual Basic 源码。", Extension=".omform", Content=new FormLayout().Serialize() }
        };

        internal string CreateProject(string parent, string name)
        {
            if (!Project) throw new InvalidOperationException("请选择项目模板。");
            if (String.IsNullOrWhiteSpace(name) || name.IndexOfAny(Path.GetInvalidFileNameChars()) >= 0 || name == "." || name == ".." || name.EndsWith(".") || name.EndsWith(" "))
                throw new ArgumentException("请输入有效的项目名称。");
            string folder = Path.Combine(Path.GetFullPath(parent), name);
            if (Directory.Exists(folder) || File.Exists(folder)) throw new IOException("目标已存在，请选择其他项目名称。");
            Directory.CreateDirectory(folder);
            bool vb = Extension == ".vb";
            string guid = Guid.NewGuid().ToString("B").ToUpperInvariant();
            string projectName = name + (vb ? ".vbproj" : ".csproj");
            string escaped = SecurityElement.Escape(name);
            string props = "<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n  <PropertyGroup>\r\n    <Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>\r\n    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>\r\n    <ProjectGuid>" + guid + "</ProjectGuid>\r\n    <OutputType>Exe</OutputType>\r\n    <AssemblyName>" + escaped + "</AssemblyName>\r\n    <RootNamespace>OpenMSEProject</RootNamespace>\r\n    <TargetFrameworkVersion>v2.0</TargetFrameworkVersion>\r\n    <PlatformTarget>x86</PlatformTarget>\r\n    <OutputPath>bin\\Debug\\</OutputPath>\r\n    <DebugSymbols>true</DebugSymbols>\r\n    <DebugType>full</DebugType>\r\n    <Optimize>false</Optimize>\r\n  </PropertyGroup>\r\n  <ItemGroup>\r\n    <Reference Include=\"System\" />\r\n    <Compile Include=\"Program" + Extension + "\" />\r\n  </ItemGroup>\r\n  <Import Project=\"$(MSBuildBinPath)\\Microsoft." + (vb ? "VisualBasic" : "CSharp") + ".targets\" />\r\n</Project>\r\n";
            File.WriteAllText(Path.Combine(folder, projectName), props, new UTF8Encoding(true));
            File.WriteAllText(Path.Combine(folder, "Program" + Extension), Content, new UTF8Encoding(true));
            string kind = vb ? "{F184B08F-C81C-45F6-A57F-5ABD9991F28F}" : "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}";
            string solution = "Microsoft Visual Studio Solution File, Format Version 9.00\r\n# Visual Studio 2005\r\nProject(\"" + kind + "\") = \"" + name + "\", \"" + projectName + "\", \"" + guid + "\"\r\nEndProject\r\nGlobal\r\n\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n\t\tDebug|Any CPU = Debug|Any CPU\r\n\tEndGlobalSection\r\n\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n\t\t" + guid + ".Debug|Any CPU.ActiveCfg = Debug|Any CPU\r\n\t\t" + guid + ".Debug|Any CPU.Build.0 = Debug|Any CPU\r\n\tEndGlobalSection\r\nEndGlobal\r\n";
            string path = Path.Combine(folder, name + ".sln");
            File.WriteAllText(path, solution, new UTF8Encoding(true));
            return path;
        }
    }

    internal sealed class EditorDocument
    {
        internal string PathName;
        internal Encoding Encoding = new UTF8Encoding(true);
        internal string Original = "";
        internal string DiskText;
        internal string Newline = "\r\n";
        internal static string Normalize(string text) { return text.Replace("\r\n", "\n").Replace("\r", "\n"); }
        internal string Read(string path)
        {
            byte[] data = File.ReadAllBytes(path);
            if (Array.IndexOf(data, (byte)0) >= 0 && !(data.Length >= 2 && ((data[0] == 255 && data[1] == 254) || (data[0] == 254 && data[1] == 255))))
                throw new IOException("该文件似乎是二进制文件，请使用原版兼容工具打开。");
            Encoding fallback;
            try { new UTF8Encoding(false, true).GetString(data); fallback = new UTF8Encoding(false); }
            catch (DecoderFallbackException) { fallback = System.Text.Encoding.Default; }
            using (var reader = new StreamReader(new MemoryStream(data), fallback, true)) {
                DiskText = reader.ReadToEnd(); Encoding = reader.CurrentEncoding;
            }
            if (data.Length >= 3 && data[0] == 239 && data[1] == 187 && data[2] == 191) Encoding = new UTF8Encoding(true);
            PathName = System.IO.Path.GetFullPath(path);
            Newline = DiskText.Contains("\r\n") ? "\r\n" : "\n";
            Original = Normalize(DiskText);
            return Original;
        }
        internal void Save(string path, string text)
        {
            path = System.IO.Path.GetFullPath(path);
            if (String.Equals(path, PathName, StringComparison.OrdinalIgnoreCase) && DiskText != null) {
                using (var reader = new StreamReader(path, Encoding, true))
                    if (reader.ReadToEnd() != DiskText) throw new IOException("文件已被其他程序修改，请先重新打开或另存为，避免覆盖。");
            }
            string value = Normalize(text).Replace("\n", Newline);
            // Fail rather than silently replacing characters in legacy code-page files.
            Encoding strict = (Encoding)Encoding.Clone(); strict.EncoderFallback = EncoderFallback.ExceptionFallback;
            byte[] body = strict.GetBytes(value), preamble = strict.GetPreamble();
            byte[] bytes = new byte[preamble.Length + body.Length];
            Buffer.BlockCopy(preamble, 0, bytes, 0, preamble.Length); Buffer.BlockCopy(body, 0, bytes, preamble.Length, body.Length);
            string temp = path + ".openmse-" + Guid.NewGuid().ToString("N") + ".tmp";
            try {
                File.WriteAllBytes(temp, bytes);
                if (File.Exists(path)) File.Replace(temp, path, null); else File.Move(temp, path);
            } finally { if (File.Exists(temp)) File.Delete(temp); }
            PathName = path; DiskText = value; Original = Normalize(text);
        }
    }
}
