Namespace My

    '下列事件可用于 MyApplication
    '
    'Startup: 应用程序启动时在创建启动窗体之前引发。
    'Shutdown: 在关闭所有应用程序窗体后引发。如果应用程序异常终止，则不引发此事件。
    'UnhandledException: 在应用程序遇到未处理的异常时引发。
    'StartupNextInstance: 在启动单实例应用程序且应用程序已处于活动状态时引发。
    'NetworkAvailabilityChanged: 在连接或断开网络连接时引发。

    Class MyApplication

#If _MyType = "WindowsForms" Then
        'OnInitialize 用于对“我的应用程序模型”(MyApplication)进行高级自定义。
        '应将特定应用程序的启动代码放在 Startup 事件处理程序中。
        <Global.System.Diagnostics.DebuggerStepThrough()> _
        Protected Overrides Function OnInitialize(ByVal commandLineArgs As System.Collections.ObjectModel.ReadOnlyCollection(Of String)) As Boolean
           '设置初始屏幕的超时时间。
           Me.MinimumSplashScreenDisplayTime = 2000
           Return MyBase.OnInitialize(commandLineArgs)
        End Function
#End If

    End Class

End Namespace
