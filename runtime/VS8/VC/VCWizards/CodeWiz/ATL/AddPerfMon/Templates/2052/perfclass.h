// [!output HEADER_FILE]: [!output CLASS_NAME] 类的接口。
//


#pragma once

[!if ATTRIBUTED]
[!if SAMPLE_PERFMON_OBJECT]
[ perf_object(
		namestring="[!output PERFMON_NAME] Sample Object",
		helpstring="A Sample PerfMon Object for [!output PERFMON_NAME]",
		detail=PERF_DETAIL_NOVICE) ]
class [!output CLASS_NAME]SampleObject
{
public:
	[ perf_counter(
			namestring="[!output PERFMON_NAME] Sample Counter",
			helpstring="A Sample PerfMon Counter for [!output PERFMON_NAME]",
			countertype=PERF_COUNTER_RAWCOUNT,
			defscale=0,
			detail=PERF_DETAIL_NOVICE,
			default_counter=true) ]
	ULONG m_nCounter;
};
[!endif]

[!if TODO_COMMENTS]
// TODO: 首先向项目添加性能监视器对象。
// 然后可以向这些对象添加计数器。
// 使用 ClassView 中 [!output CLASS_NAME] 的上下文
// 菜单向项目添加 PerfMon 对象。然后使用新近创建
// 的 PerfMon 对象的上下文菜单添加 PerfMon 计数器。
[!endif]
[ perfmon(name="[!output PERFMON_NAME]", register=true) ]
class [!output CLASS_NAME]
{
public:
};
[!else]
[!if SAMPLE_PERFMON_OBJECT]
class [!output CLASS_NAME]SampleObject :
	public CPerfObject
{
public:
	DECLARE_PERF_CATEGORY([!output CLASS_NAME]SampleObject, 1, _T("[!output PERFMON_NAME] Sample Object"), _T("A Sample PerfMon Object for [!output PERFMON_NAME]"), -1);
	BEGIN_COUNTER_MAP([!output CLASS_NAME]SampleObject)
		DEFINE_COUNTER(m_nCounter, _T("[!output PERFMON_NAME] Sample Counter"), _T("A Sample PerfMon Counter for [!output PERFMON_NAME]"), PERF_COUNTER_RAWCOUNT, 0)
	END_COUNTER_MAP()

	ULONG m_nCounter;
};
[!endif]

[!if TODO_COMMENTS]
// TODO: 首先向项目添加性能监视器对象。
// 然后可以向这些对象添加计数器。
// 使用 ClassView 中 [!output CLASS_NAME] 的上下文
// 菜单向项目添加 PerfMon 对象。然后使用新近创建
// 的 PerfMon 对象的上下文菜单添加 PerfMon 计数器。
[!endif]
class [!output CLASS_NAME] :
	public CPerfMon
{
public:
#define [!output PERFMON_NAME] _T("[!output PERFMON_NAME]")
	BEGIN_PERF_MAP([!output PERFMON_NAME])
[!if SAMPLE_PERFMON_OBJECT]
		CHAIN_PERF_CATEGORY([!output CLASS_NAME]SampleObject)
[!endif]
	END_PERF_MAP()
};

PERFREG_ENTRY([!output CLASS_NAME]);
[!endif]
